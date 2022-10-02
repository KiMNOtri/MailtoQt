#ifndef WGCONTACTITEM_H
#define WGCONTACTITEM_H

#include <QWidget>

namespace Ui {
class WGContactItem;
}


enum WGContactItemAvatarType { online,offline,error,waiting };

class WGContactItem : public QWidget
{
    Q_OBJECT

public:
    explicit WGContactItem(QWidget *parent = nullptr);
    ~WGContactItem();

    void setTitleContent(QString);
    void setDescriptionContent(QString);
    void setAvatar(WGContactItemAvatarType type);

private:
    Ui::WGContactItem *ui;
};

#endif // WGCONTACTITEM_H
