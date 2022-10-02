#ifndef MTSTORAGE_H
#define MTSTORAGE_H

#include <QObject>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "MTMessage.h"

class MTStorage : public QObject
{
    Q_OBJECT
public:

    explicit MTStorage(QObject *parent = nullptr);

    const QString domainName = "tristudio";
    const QString softwareName = "mailto";

    void writeSetting(QString key, QString value);
    QString readSetting(QString key);

    void saveMessageData();
    void getMessageData();

    void addMessageData();
    void removeMessageData();

    QSettings* settings;


private:
    QSqlDatabase initSQLConnection();



signals:

};

#endif // MTSTORAGE_H
