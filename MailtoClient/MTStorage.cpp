#include "MTStorage.h"

MTStorage::MTStorage(QObject *parent)
    : QObject{parent}
{
    settings = new QSettings(domainName,softwareName);
}


void MTStorage::writeSetting(QString key, QString value)
{

    settings->setValue(key,value);

}


QString MTStorage::readSetting(QString key)
{
   return settings->value(key).toString();
}


QSqlDatabase MTStorage::initSQLConnection()
{
    QSqlDatabase database;


    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("MTStorage.db");
    }

    return database;

}
