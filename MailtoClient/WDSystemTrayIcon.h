#ifndef WDSYSTEMTRAYICON_H
#define WDSYSTEMTRAYICON_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QStyle>
#include <QTimer>
#include <QMenu>
#include <QList>
#include <QDebug>
#include <QCoreApplication>
#include <QtMultimedia/QMediaPlayer>
#include <QApplication>
class WDSystemTrayIcon : public QObject
{
    Q_OBJECT
public:
    explicit WDSystemTrayIcon(QObject *parent = nullptr);

    void startflash();
    void stopflash();

    void setNewMessageCount(int);
    void setUserName(QString);
    void playMedia();

private:
    int flash_count = 0;
    QTimer* flash_timer;
    int message_count = 0;
    QString userName;

    QAction* emptyMenuAction;
    QAction* messageNumberMenuAction;
    bool messageBoxEmpty = true;

    QMenu* trayMenu;
    void setEmptyMessageBox();


signals:

};

#endif // WDSYSTEMTRAYICON_H
