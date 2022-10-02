#include "MTNetwork.h"

MTNetwork::MTNetwork(QObject *parent)
    : QObject{parent}
{
    mgr = new QNetworkAccessManager(this);

    connect(mgr,&QNetworkAccessManager::finished,this,[=](QNetworkReply* re){
        QTextCodec *codec = QTextCodec::codecForName("utf8");
        QString str = codec->toUnicode(re->readAll());

        HTTPReplyHandler(str);
        re->deleteLater();
    });

}

void MTNetwork::MTSServerStatusRequest()
{
    qDebug() << "[MTS] Request Server Status ...";

    HTTPGetData(MTSServerUrl + "/status");
}


void MTNetwork::MTSUserRegister(QString name,QString mail,QString pwd)
{
    HTTPGetData(MTSServerUrl + "/register?uname="+name+"&umail="+mail+"&upasswd="+pwd);
}


void MTNetwork::MTSUserLogin(QString uid,QString pwd)
{
    ///login?uid=10220&upasswd=1
    HTTPGetData(MTSServerUrl + "/login?uid="+uid+"&upasswd="+pwd);
}


void MTNetwork::MTSUserLook(QString uid)
{
    ///login?uid=10220&upasswd=1
    HTTPGetData(MTSServerUrl + "/look?uid="+uid);
}

void MTNetwork::MTSAddContacts(QString userl,QString userr)
{
    HTTPGetData(MTSServerUrl + "/addcontacts?cuser_l="+userl+"&cuser_r="+userr);
}

void MTNetwork::MTSUpdateUserStatus(QString uid,QString ustatus)
{
    HTTPGetData(MTSServerUrl + "/updatestatus?uid="+uid+"&ustatus="+ustatus);
}

void MTNetwork::MTSUpdateUser(QString uid,QString uname,QString umail,QString upasswd)
{

    HTTPGetData(MTSServerUrl + "/update?uid="+uid+"&uname="+uname+"&umail="+umail+"&upasswd="+upasswd);
}

void MTNetwork::MTSGetContacts(QString uid)
{
    HTTPGetData(MTSServerUrl + "/contacts?uid="+uid);
}

// MTS 联系人 确定建立好友
void MTNetwork::MTSConfirmContacts(QString userl,QString userr)
{
    HTTPGetData(MTSServerUrl + "/confirmcontacts?cuser_l="+userl+"&cuser_r="+userr);
}

// MTS 联系人 删除好友
void MTNetwork::MTSDeleteContacts(QString userl,QString userr)
{
    HTTPGetData(MTSServerUrl + "/deletecontacts?cuser_l="+userl+"&cuser_r="+userr);
}

void MTNetwork::MTSServerMessageGet()
{
    HTTPGetData(MTSServerUrl + "/servermessage");
}

// MTS 投递消息
void MTNetwork::MTSMessageSend(QString content,QString sender,QString receiver)
{
    QJsonObject obj;
    obj.insert("mcontent",content);
    obj.insert("msender",sender);
    obj.insert("mreceiver",receiver);
    QJsonDocument document;
    document.setObject(obj);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);
    HTTPPostData(MTSServerUrl + "/messagesend",strJson);
}

// MTS 获取消息（发送者）
void MTNetwork::MTSMessageGetSender(QString uid)
{
    HTTPGetData(MTSServerUrl + "/messageget/send?uid="+uid);
}
// MTS 获取消息（接受者）
void MTNetwork::MTSMessageGetReceiver(QString uid)
{
    HTTPGetData(MTSServerUrl + "/messageget/receive?uid="+uid);
}

void MTNetwork::MTSMessageCountGet(QString uid)
{
    HTTPGetData(MTSServerUrl + "/messagecount?uid="+uid);
}



void MTNetwork::HTTPPostData(QString url, QString data)
{
    //qDebug() << "Post Data "<<data;
    QNetworkRequest requestInfo;
    requestInfo.setUrl(QUrl(url));
    QByteArray qByteHttpData = data.toUtf8();
    requestInfo.setUrl(url);
    requestInfo.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json;charset=UTF-8"));
    requestInfo.setRawHeader("Connection","Keep-Alive");
    mgr->post(requestInfo, qByteHttpData);
}


void MTNetwork::HTTPGetData(QString url)
{
    qDebug() << "[Network] Get > " << url;
    QNetworkRequest requestInfo;
    requestInfo.setUrl(QUrl(url));
    mgr->get(requestInfo);
}

void MTNetwork::HTTPReplyHandler(QString data)
{
    qDebug() << "[Network] Receive Get Feedback > " << data;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8(),&err);
    if(err.error!=QJsonParseError::NoError){
        qDebug() << "[Network] > JSON PARSE ERROR";
        emit MTSServerError();
        return;
    }
    QJsonObject obj = doc.object();

    QString reply_type = obj.value("mts").toString();
    qDebug() << "[Network] MTS Message Type : "<<reply_type;

    if(reply_type==""){
        qDebug() << "[MTS] Server Error";
        emit MTSServerError();
        return;
    }

    if(reply_type=="status"){
        QString repo = obj.value("content").toString();
        qDebug() << "[MTS] Server Status : "<<repo;
        emit MTSServerStatusReceive(0);
        return;
    }

    if(reply_type=="register"){
        QString repo = obj.value("content").toString();
        qDebug() << "[MTS] Register UID Reply : "<<repo;
        emit MTSUserRegisterReceive(repo);
        return;
    }

    if(reply_type=="login"){
        QString repo = obj.value("content").toString();
        qDebug() << "[MTS] Login Result : "<<repo;
        emit MTSUserLoginReceive(repo);
        return;
    }

    if(reply_type=="look"){
        QString repo = obj.value("content").toString();
        if(repo=="success"){
            MTUserInfo info;
            info.uid = obj.value("uid").toString();
            info.uname = obj.value("uname").toString();
            info.umail = obj.value("umail").toString();
            info.ustatus = QString::number(obj.value("ustatus").toInt(0));
            emit MTSUserLookReceive(info);
        } else {
            MTUserInfo info;
            info.uid = "";
            info.uname = "";
            info.umail = "";
            info.ustatus = "";
            emit MTSUserLookReceive(info);
        }
        return;
    }

    if(reply_type=="addcontacts"){
        QString repo = obj.value("content").toString();
        if(repo=="success"){
            emit MTSAddContactsReceive("success");
        }
        if(repo=="failure"){
            emit MTSAddContactsReceive("failure");
        }
        if(repo=="already"){
            emit MTSAddContactsReceive("already");
        }
        if(repo=="wait"){
            emit MTSAddContactsReceive("wait");
        }
        return;
    }

    if(reply_type=="contacts"){
        QList<MTUserInfo> re;
        QJsonArray a = obj.value("content").toArray();
        for(int i = 0 ; i < a.count() ; i++){
            QJsonValue v = a.at(i);
            QJsonObject obj = v.toObject();
            MTUserInfo in;
            in.uname = obj.value("uname").toString();
            in.uid = QString::number( obj.value("uid").toInt(0));
            in.umail = "";
            in.ustatus = QString::number(obj.value("ustatus").toInt(0));
            in.ustate = QString::number(obj.value("ustate").toInt(0));
            re.append(in);
        }
        emit MTSGetContactsReceive(re);
        return;
    }

    if(reply_type=="updatestatus"){
        QString repo = obj.value("content").toString();
        qDebug() << "[MTS] Update Status Result : "<<repo;
        emit MTSUpdateUserStatusReceive(repo);
        return;
    }

    if(reply_type=="confirmcontacts"){
        QString repo = obj.value("content").toString();
        qDebug() << "[MTS] Confirm Contacts Result : "<<repo;
        emit MTSConfirmContactsReceive(repo);
        return;
    }

    if(reply_type=="deletecontacts"){
        QString repo = obj.value("content").toString();
        qDebug() << "[MTS] Delete Contacts Result : "<<repo;
        emit MTSDeleteContactsReceive(repo);
        return;
    }

    if(reply_type=="messagesend"){
        QString repo = obj.value("content").toString();
        qDebug() << "[MTS] Delete Contacts Result : "<<repo;
        emit MTSMessageSendReceive(repo);
        return;
    }

    if(reply_type=="messagegetsend"){
        QJsonArray a = obj.value("content").toArray();
        emit MTSMessageReceiveSenderReceive(a);
        return;

    }

    if(reply_type=="messagegetreceive"){
        QJsonArray a = obj.value("content").toArray();
        emit MTSMessageReceiveReceiverReceive(a);
        return;
    }

    if(reply_type=="messagecount"){
        int a = obj.value("content").toInt(-1);
        emit MTSMessageCountReceive(a);
    }

    if(reply_type=="servermessage"){
        QString a = obj.value("content").toString();
        emit MTSServerMessageGetReceive(a);
    }


}




