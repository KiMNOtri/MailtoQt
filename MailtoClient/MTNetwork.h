#ifndef MTNETWORK_H
#define MTNETWORK_H

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTextCodec>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

struct MTUserInfo {
    QString uid;
    QString uname;
    QString umail;
    QString ustatus;
    QString ustate;
};
// 这里的 UserInfo 设计的其实是有问题的但是重构工作量略大就将就写了

class MTNetwork : public QObject
{
    Q_OBJECT
public:

    explicit MTNetwork(QObject *parent = nullptr);

    QString MTSServerUrl = "http://124.220.189.244:1145";

    // MTS 服务器状态请求
    void MTSServerStatusRequest();
    // MTS 用户注册
    void MTSUserRegister(QString name,QString mail,QString pwd);
    // MTS 用户登陆
    void MTSUserLogin(QString uid,QString pwd);
    // MTS 查询用户信息
    void MTSUserLook(QString uid);
    // MTS 更新用户信息
    void MTSUpdateUser(QString uid,QString uname,QString umail,QString upasswd);
    // MTS 联系人 添加好友
    void MTSAddContacts(QString userl,QString userr);
    // MTS 查找好友信息
    void MTSGetContacts(QString uid);
    // MTS 更新用户状态
    void MTSUpdateUserStatus(QString uid,QString ustatus);
    // MTS 联系人 确定建立好友
    void MTSConfirmContacts(QString userl,QString userr);
    // MTS 联系人 删除好友
    void MTSDeleteContacts(QString userl,QString userr);
    // MTS 投递消息
    void MTSMessageSend(QString content,QString sender,QString receiver);
    // MTS 获取消息（发送者）
    void MTSMessageGetSender(QString uid);
    // MTS 获取消息（接受者）
    void MTSMessageGetReceiver(QString uid);
    // MTS 获取消息数量
    void MTSMessageCountGet(QString uid);

    void MTSServerMessageGet();

private:
    QNetworkAccessManager* mgr;
    void HTTPPostData(QString url, QString data);
    void HTTPGetData(QString url);
    void HTTPReplyHandler(QString data);

signals:

    void HTTPPostDataReply(QString data);
    void MTSServerError();

    // MTS 收到服务器状态
    void MTSServerStatusReceive(int);
    void MTSUserLoginReceive(QString status);
    void MTSUserRegisterReceive(QString data);
    void MTSUserLookReceive(MTUserInfo info);
    void MTSAddContactsReceive(QString re);
    void MTSUpdateUserStatusReceive(QString re);
    void MTSGetContactsReceive(QList<MTUserInfo>);
    void MTSConfirmContactsReceive(QString);
    void MTSDeleteContactsReceive(QString);
    void MTSMessageSendReceive(QString);
    void MTSMessageReceiveSenderReceive(QJsonArray);
    void MTSMessageReceiveReceiverReceive(QJsonArray);
    void MTSMessageCountReceive(int);
    void MTSServerMessageGetReceive(QString);

};

#endif // MTNETWORK_H
