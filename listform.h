#ifndef LISTFORM_H
#define LISTFORM_H

#include <QWidget>
#include "ui_ResWInElem.h"
#include <QVBoxLayout>
#include <types.h>

namespace Ui {
    class ResElemForm;
}

class ListForm : public QWidget
{
    Q_OBJECT
public:
    explicit ListForm(Criterions crt, QWidget * parent = nullptr);
    ~ListForm();
    void addElem(listElemetData led);
    void delElem(int index);
    void changeStateLable(ProccesThreadState ptc);
    void setDataPth(QString path);
    void showCurrentSet(int percent);
private:
    Ui::ResElemForm * ui;
    QVBoxLayout * main_layout;
    QWidget * scroll_w;
    QString path;
signals:

private slots:
    void openXlsx(listElemetData led);
};

#endif // LISTFORM_H
