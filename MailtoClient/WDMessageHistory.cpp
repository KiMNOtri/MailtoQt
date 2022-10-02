#include "WDMessageHistory.h"
#include "ui_WDMessageHistory.h"

WDMessageHistory::WDMessageHistory(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WDMessageHistory)
{
    ui->setupUi(this);


    QList<QString> data = MTMessage::getRecordList(MTGUserID);
    QStringList l;
    for(QString s : data){
        s.replace(QRegExp(".archive$"), "");
        l.append(s);
    }
    QStringListModel *model;
    model = new QStringListModel(this);

    model->setStringList(l);
    ui->lSelection->setModel(model);

    connect(ui->lSelection,&QListView::clicked,this,[=](QModelIndex i){
        qDebug() << "Selected > " <<i.row();
        QString repo = MTMessage::messageRecordReader(MTGUserID,i.data().toString()+".archive");
        ui->lMain->setText(repo);
    });
    connect(ui->btnClear,&QPushButton::clicked,this,[=](){
        QMessageBox::StandardButton btn;

       btn= QMessageBox::information(this,"清除聊天历史记录","是否要清空所有的聊天历史记录？\n此操作无法撤销。",QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);

       if(btn == QMessageBox::Yes)
       {
            MTMessage::removeRecord(MTGUserID);
            this->close();
       }
       else if(btn == QMessageBox::No)
       {
            return;
       }
    });
}

WDMessageHistory::~WDMessageHistory()
{
    delete ui;
}
