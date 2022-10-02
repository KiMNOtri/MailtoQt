#include "MTMessage.h"

QString MTMessage::parseJson(MTMessageStruct s)
{
    QJsonObject obj;
    obj.insert("content",s.content);
    obj.insert("date",s.date.toString("yyyy-MM-dd-hh-mm-ss"));
    obj.insert("receiveID",s.receiveID);
    obj.insert("receiveName",s.receiveName);
    obj.insert("sendID",s.sendID);
    obj.insert("sendName",s.sendName);
    QJsonDocument doc;
    doc.setObject(obj);

    QString result = doc.toJson(QJsonDocument::Compact);
    qDebug() << "MTMessageParser / Struct to String > " << result;
    return result;
}


MTMessageStruct MTMessage::parseMessage(QString s)
{
    QDateTime d;
    MTMessageStruct re;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(s.toUtf8(),&err);
    if(err.error!=QJsonParseError::NoError){
        qDebug() << "MTMessageParser / String to Struct > PARSE ERROR";
        return re;
    }
    QJsonObject obj = doc.object();

    re.content = obj["content"].toString();
    re.date = d.fromString(obj["date"].toString(),"yyyy-MM-dd-hh-mm-ss");
    re.receiveID = obj["receiveID"].toString();
    re.receiveName = obj["receiveName"].toString();
    re.sendID = obj["sendID"].toString();
    re.sendName = obj["sendName"].toString();

    qDebug() << "MTMessageParser / String to Struct > " << re.content;

    return re;
}

QString MTMessage::generateDisplayData(QList<MTMessageStruct>* list)
{
    QString displayResult = "";

    for(int k =0; k<list->count() ;k++){
        MTMessageStruct s = list->at(k);
        displayResult += s.sendName;
        displayResult += " > ";
        displayResult += s.receiveName;
        displayResult += "   ";
        displayResult += s.date.toString("yyyy-MM-dd hh:mm");
        displayResult += "\n";
        displayResult += s.content;
        displayResult += "\n";
    }

    return displayResult;
}

void MTMessage::messageRecordWriter(QString my_id,QString other_id,QList<MTMessageStruct>* data)
{
    qDebug() << "MTMessage > Save Record " << my_id << " - "<<other_id;
    QDateTime dateTime;
    QString path = QApplication::applicationDirPath() + "/history";
    QDir dir(path);
    if(!dir.exists()){
        bool ismkdir = dir.mkdir(path);
        if(!ismkdir)
            qDebug() << "Create Folder Fail";
        else
            qDebug() << "Create Folder Success";
    }
    else{
        qDebug() << "Skip Create Folder";
    }

    path += "/"+my_id;
    QDir dir2(path);
    if(!dir2.exists()){
        bool ismkdir = dir2.mkdir(path);
        if(!ismkdir)
            qDebug() << "Create Folder2 Fail";
        else
            qDebug() << "Create Folder2 Success";
    }
    else{
        qDebug() << "Skip Create Folder2";
    }

    QFile file(path+"/"+other_id+"-"+dateTime.currentDateTime().toString("yy-MM-dd-hh-mm-ss")+".archive");
    file.open( QIODevice::ReadWrite | QIODevice::Append);
    QTextStream out(&file);
    out << generateDisplayData(data);
    file.close();

    qDebug() << "Write Finished!";
}

QString MTMessage::messageRecordReader(QString my_id,QString archive_name)
{

    QString path = QApplication::applicationDirPath() + "/history/"+my_id+"/"+archive_name;
    QFile file(path);
    QByteArray bytearr;
    if(file.open(QIODevice::ReadOnly)){
        bytearr = file.readAll();
    }
    file.close();


    QString str = QString(bytearr);
#ifdef Q_OS_WIN
    QTextCodec* pTextCodec = QTextCodec::codecForName("System");

    return pTextCodec->toUnicode(bytearr);
#endif
    return str;
}

QList<QString> MTMessage::getRecordList(QString my_id)
{
    qDebug() << "MTMessage > Get List " << my_id;
    QString path = QApplication::applicationDirPath() + "/history/"+my_id;
    QDir dir(path);
    QList<QString> repo;
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        qDebug()<<fileInfo.fileName();
        repo.append(fileInfo.fileName());
    }
    return repo;

}

void MTMessage::removeRecord(QString my_id)
{
    QString del_file = QString(QApplication::applicationDirPath() + "/history/"+my_id);
    QDir dir;
    dir.setPath(del_file);
    dir.removeRecursively();
    qDebug() << "MTMessage > Remove Folder";
}

MTMessage::MTMessage(QObject *parent)
    : QObject{parent}
{

}

