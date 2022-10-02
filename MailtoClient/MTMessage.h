#ifndef MTMESSAGE_H
#define MTMESSAGE_H

#include <QObject>
#include <QDateTime>
#include <QApplication>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QString>
#include <QJsonObject>
#include <QList>
#include <QFile>
#include <QDir>
#include <QTextCodec>


struct MTMessageStruct {
    QString content;
    QString receiveID;
    QString sendID;
    QString receiveName;
    QString sendName;
    QDateTime date;
};

class MTMessage : public QObject
{
    Q_OBJECT
public:
    explicit MTMessage(QObject *parent = nullptr);

    static QString parseJson(MTMessageStruct);
    static MTMessageStruct parseMessage(QString);
    static QString generateDisplayData(QList<MTMessageStruct>*);
    static void messageRecordWriter(QString my_id,QString other_id,QList<MTMessageStruct>* data);
    static QString messageRecordReader(QString my_id,QString other_id);
    static QList<QString> getRecordList(QString my_id);
    static void removeRecord(QString my_id);

signals:

};

#endif // MTMESSAGE_H
