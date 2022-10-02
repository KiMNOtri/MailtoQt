#include "WDPreferences.h"
#include "ui_WDPreferences.h"

WDPreferences::WDPreferences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WDPreferences)
{
    ui->setupUi(this);
}

WDPreferences::~WDPreferences()
{
    delete ui;
}
