#include "listelem.h"
#include "ui_ElemList.h"
#include <QString>
#include <QMouseEvent>
#include <QMenu>
#include <QString>
#include<QDebug>


ListElem::ListElem(listElemetData led, QWidget *parent)
    : QWidget(parent), index(led.id)
{
    ui = new Ui::ResListElem();
    ui->setupUi(this);
    ui->Country->setText(led.country);
    ui->League->setText(led.league);
    ui->CmdName->setText(led.command);

    ui->Percent->setText(QString::number(led.percent) + '%');

    QPalette p;
    p.setColor(QPalette::Normal, QPalette::WindowText, QColor(int(2.55 * led.percent), int(2.55 * (100 - led.percent)), 0));
    ui->Percent->setPalette(p);

    createActions();
}

ListElem::~ListElem() {
    delete ui;
}

int ListElem::getPercent() const {
    QString str = ui->Percent->text();
    //str.replace("<", "");
    str.replace("%", "");
    return str.toInt();
    //return percent;
}

QString ListElem::getCmdName() const {
    return ui->CmdName->text();
}

listElemetData ListElem::getData() const {
    listElemetData led;
    led.id = index;
    led.league = ui->League->text();
    led.country = ui->Country->text();
    led.command = getCmdName();
    led.percent = getPercent();
    return led;
}

void ListElem::mousePressEvent(QMouseEvent * event) {
    //f(event->button() == Qt::RightButton)
        //emit rightMBClicked(index, ui->CmdName->text(), ui->Liga->text(), ui->Country->text());
   // else if (event->button() == Qt::LeftButton) {

   // }
}

void ListElem::mouseDoubleClickEvent(QMouseEvent * event) {
    if (event->button() == Qt::LeftButton) {
        listElemetData led = {index, ui->Country->text(), ui->League->text(), ui->CmdName->text(), ui->Percent->text().toFloat(), 0 ,0};
        emit leftMBDClicked(led);
    }
}

void ListElem::createActions() {
    moreInfoAct = new QAction(tr("More..."), this);
    connect(moreInfoAct, SIGNAL(triggered()), this, SLOT(useMenuAct()));
}

void ListElem::useMenuAct() {
    listElemetData led = {index, ui->Country->text(), ui->League->text(), ui->CmdName->text(), ui->Percent->text().toFloat(), 0 ,0};
    emit menuActionClick(led);
}

void ListElem::contextMenuEvent(QContextMenuEvent *event) {
     QMenu menu(this);
     menu.addAction(moreInfoAct);
     menu.exec(event->globalPos());
}
