#include "WDContacts.h"
#include "ui_WDContacts.h"


WDContacts::WDContacts(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WDContacts)
{
    ui->setupUi(this);

    st = new MTStorage();
    mgr = new MTNetwork();
    trayIcon = new WDSystemTrayIcon();
    listModel = new QStandardItemModel();
    //trayIcon->playMedia();

    //ui->lvContacts->setSelectionMode(QAbstractItemView::NoSelection);

    //ui->lvContacts->setModel(listModel);

    connect(ui->btnAddContact,&QPushButton::clicked,this,[=](){
        WDAddContacts* w =new WDAddContacts();
        w->show();
    });

    connect(ui->acPreference,&QAction::triggered,this,[=](){
        qDebug() << "Go Preferences";
        WDPreferences* p = new WDPreferences();
        p->show();
    });
    connect(ui->acUserSettings,&QAction::triggered,this,[=](){
        qDebug() << "Go User Settings";
        WDUserSettings* s = new WDUserSettings();
        s->show();
    });
    connect(ui->acHistory,&QAction::triggered,this,[=](){
        WDMessageHistory* h = new WDMessageHistory();
        h->show();
    });
    connect(mgr,&MTNetwork::MTSUserLookReceive,this,[=](MTUserInfo i){
        ui->lNickName->setText(i.uname);
        ui->lInfo->setText(i.umail);
        MTGUserName = i.uname;
        MTGUserMail = i.umail;
        this->my_Name = i.uname;
        qDebug() << "User Mail "<<MTGUserMail;
    });

    connect(mgr,&MTNetwork::MTSGetContactsReceive,this,[=](QList<MTUserInfo> i){
        QList<MTUserInfo> right_list;
        for(int k = 0 ; k<i.count() ; k++){
            //qDebug() << i.at(k).uname;
            QString display = "";
            display+=i.at(k).uname;
            if(i.at(k).ustatus=="0"){
                display.append(" [等待接受请求]");
            }
            if(i.at(k).ustatus=="1"){
                display.append("");
            }
            if(i.at(k).ustatus=="2"){
                display.append(" [好友请求]");
            }
            if(i.at(k).ustatus=="3"){
                // display.append(" ");
                continue;
            }
            if(i.at(k).ustatus=="4"){
                display.append(" [对方删除好友]");
            }
            QStandardItem *item = new QStandardItem(display);
            addContactToList(i.at(k));
            listModel->appendRow(item);
            right_list.append(i.at(k));
        }
        userInfoList = right_list;
    });

    connect(ui->lvContacts,&QListView::clicked,this,[=](QModelIndex i){
        list_select_count = i.row();
        qDebug() << list_select_count ;
    });

    connect(ui->btnRemoveContact,&QPushButton::clicked,this,[=](){
        if(list_select_count==-1){
            QMessageBox::warning(this,"好友删除","要删除好友，请先选择需要删除的好友。");
        }

        MTUserInfo in = userInfoList.at(list_select_count);
        QMessageBox::StandardButton btn;

        btn = QMessageBox::critical(this,"确定要删除好友？","好友 "+in.uname+" 将被删除，此操作将无法撤销。",QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
        if(btn == QMessageBox::Yes){
            mgr->MTSDeleteContacts(MTGUserID,in.uid);
            QMessageBox::information(this,"操作完成","操作完成，对方的联系人列表中也会删除。",QMessageBox::Ok);
            listModel->clear();
            ui->lvContacts->clear();
            userInfoList.clear();
            mgr->MTSGetContacts(this->my_UID);
        } else {
            return;
        }
    });

    connect(ui->lvContacts,&QListView::doubleClicked,this,[=](QModelIndex i){
        int select_index= i.row();
        MTUserInfo in ;
        in= userInfoList.at(select_index);
        QString uid = in.uid;
        QString status = in.ustatus;
        qDebug() << "Clicked User > "<<in.uname;
        if(status == "0"){
            QMessageBox::information(this,"提示","你和对方暂时还不是好友，请等待对方确认。");
            return;
        }
        if(status == "1"){
            qDebug() << "> Go Message Window rUserID = "<<uid;
            WDMessage* mg = new WDMessage(uid);
            mg->show();
            return;
        }
        if(status == "2"){
            QMessageBox::StandardButton btn;
            btn= QMessageBox::question(this,"提示","是否要添加对方为好友？",QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
            if(btn == QMessageBox::Yes)
            {
                // 要
                mgr->MTSConfirmContacts(this->my_UID,in.uid);
                refreshContactsList();

            }
            else if(btn == QMessageBox::No)
            {
                // 不要
                mgr->MTSDeleteContacts(this->my_UID,in.uid);
                refreshContactsList();
            }
            return;
        }
        if(status == "3"){
            QMessageBox::information(this,"提示","你和对方暂时还不是好友，请等待对方确认。");
            refreshContactsList();
            return;
        }
        if(status == "4"){
            QMessageBox::information(this,"提示","对方已经将你的好友关系删除。");
            refreshContactsList();
            return;
        }

    });

    connect(mgr,&MTNetwork::MTSServerMessageGetReceive,this,[=](QString i){
        qDebug() << "Server Message > " <<i;
        QLabel *per3 = new QLabel(i, this);
        ui->statusBar->addPermanentWidget(per3);
    });

    this->my_UID = MTGUserID;
    mgr->MTSUserLook(this->my_UID);
    mgr->MTSGetContacts(this->my_UID);
    // 切换至在线
    mgr->MTSUpdateUserStatus(this->my_UID,"1");
    mgr->MTSServerMessageGet();
    connect(ui->btnRefresh,&QPushButton::clicked,this,[=](){
        refreshContactsList();
    });
    msgCountTimer = new QTimer(this);
    msgCountTimer->setInterval(5000);
    msgCountTimer->start();

    connect(msgCountTimer,&QTimer::timeout,this,[=](){
        qDebug() << "Fetch Message Count !!";
        mgr->MTSMessageCountGet(this->my_UID);
    });

    connect(mgr,&MTNetwork::MTSMessageCountReceive,this,[=](int c){
        qDebug() << c;
        if(currentcount==-1){
            currentcount = c;
            return;
        }
        if(currentcount == c){
            return;
        }
        if(currentcount < c){
            trayIcon->setNewMessageCount(c-currentcount);
            currentcount = c;
            return;
        }
    });

    QDateTime d;
    ui->lNickName->setToolTip("成功登录于 "+d.currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

}

void WDContacts::getContactsList()
{

}

void WDContacts::closeEvent(QCloseEvent *event)
{
        QMessageBox::StandardButton btn;
        mgr->MTSUpdateUserStatus(this->my_UID,"0");
       btn= QMessageBox::information(this,"退出 Mailto","是否退出 Mailto ？",QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);

       if(btn == QMessageBox::Yes)
       {
            event->accept();
       }
       else if(btn == QMessageBox::No)
       {
           mgr->MTSUpdateUserStatus(this->my_UID,"1");
           event->ignore();
       }

}

void WDContacts::addContactToList(MTUserInfo i)
{
    WGContactItem* item = new WGContactItem();
    item->setTitleContent(i.uname);

    if(i.ustatus=="0"){
        // Waiting to be frient
        item->setAvatar(WGContactItemAvatarType::waiting);
        item->setDescriptionContent("等待对方同意好友请求");
    }
    if(i.ustatus=="1"){
        if(i.ustate=="0"){
            // Offline
            item->setAvatar(WGContactItemAvatarType::offline);
            item->setDescriptionContent("离线");
        }
        if(i.ustate=="1"){
            // Online
            item->setAvatar(WGContactItemAvatarType::online);
            item->setDescriptionContent("在线");
        }
    }
    if(i.ustatus=="2"){
        item->setDescriptionContent("对方请求添加为好友");
        if(i.ustate=="0"){
            // Offline
            item->setAvatar(WGContactItemAvatarType::offline);
        }
        if(i.ustate=="1"){
            // Online
            item->setAvatar(WGContactItemAvatarType::online);
        }
    }
    if(i.ustatus=="3"){
        // Delete contact
    }
    if(i.ustatus=="4"){
        item->setAvatar(WGContactItemAvatarType::error);
        item->setDescriptionContent("好友已被删除");
    }

    QListWidgetItem *listItem1 = new QListWidgetItem();
    listItem1->setSizeHint(QSize(0, 50));

    ui->lvContacts->addItem(listItem1);
    ui->lvContacts->setItemWidget(listItem1, item);
}

void WDContacts::refreshContactsList()
{
    listModel->clear();
    ui->lvContacts->clear();
    userInfoList.clear();
    mgr->MTSGetContacts(this->my_UID);
}

WDContacts::~WDContacts()
{
    delete ui;
}
