#include "WDSystemTrayIcon.h"

WDSystemTrayIcon::WDSystemTrayIcon(QObject *parent)
    : QObject{parent}
{
    QSystemTrayIcon *tc = new QSystemTrayIcon(this);
    tc->setToolTip(QStringLiteral("Mailto"));

#ifdef Q_OS_MACOS
        tc->setIcon(QIcon("://software/pic/iconTooltip.png"));
#endif

#ifdef Q_OS_WIN
        tc->setIcon(QIcon("://software/pic/trayicon_platform_windows.png"));
#endif

    //tc->setIcon(style()->standardIcon(QStyle::SP_TitleBarNormalButton));
    flash_timer = new QTimer();
    trayMenu = new QMenu;
    // QAction * aboutAct = trayMenu->addAction(QStringLiteral("关于"));
    // trayMenu->addAction(aboutAct);

    tc->setContextMenu(trayMenu);

    tc->show();

    emptyMenuAction = new QAction("没有新消息");
    emptyMenuAction->setEnabled(false);

    trayMenu->addAction(emptyMenuAction);

    messageNumberMenuAction = new QAction();
    connect(messageNumberMenuAction,&QAction::triggered,this,[=](){

        setNewMessageCount(0);
    });
    connect(flash_timer,&QTimer::timeout,this,[=](){
        this->flash_count ++;
        if(flash_count == 1){
#ifdef Q_OS_MACOS
        tc->setIcon(QIcon("://software/pic/iconTooltip.png"));
#endif

#ifdef Q_OS_WIN
        tc->setIcon(QIcon("://software/pic/trayicon_platform_windows.png"));
#endif

        } else {
            tc->setIcon(QIcon("://software/pic/iconTooltipEmpty.png"));
            flash_count = 0;
        }
    });


}

void WDSystemTrayIcon::startflash()
{
    flash_timer->start(800);
}

void WDSystemTrayIcon::stopflash()
{
    flash_timer->stop();
}

void WDSystemTrayIcon::setNewMessageCount(int number)
{

    if(number == 0){
        trayMenu->removeAction(messageNumberMenuAction);
        trayMenu->addAction(emptyMenuAction);
        stopflash();
    } else {
        trayMenu->removeAction(emptyMenuAction);
        playMedia();
        //messageNumberMenuAction->setText("未读消息 "+QString::number(number)+" 条");
        messageNumberMenuAction->setText("收到新消息");
        trayMenu->addAction(messageNumberMenuAction);
        startflash();
    }
}

void WDSystemTrayIcon::setUserName(QString uname)
{

}

void WDSystemTrayIcon::playMedia()
{
    // 收音机音质
    // MP3 Windows 上不放 MP3。。。

    QApplication::beep();

    /*
    QMediaPlayer * player = new QMediaPlayer;
    QString pwd = QCoreApplication::applicationDirPath() + "/msg.mp3";

    player->setMedia(QUrl::fromLocalFile(pwd));
    player->setVolume(50);
    player->play();
    qDebug() << "Sound URL " << pwd;
    */
}

void WDSystemTrayIcon::setEmptyMessageBox()
{



}
