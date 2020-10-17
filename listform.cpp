#include "listform.h"
#include "listelem.h"

#include "utils.h"
#include "tableworker.h"
#include <QString>
#include <QDebug>
#include <QAxObject>
#include <string.h>

ListForm::ListForm(Criterions crt, QWidget * parent)
    : QWidget(parent),
    ui(new Ui::ResElemForm),
    crt(crt)
{
    ui->setupUi(this);
    //ui->CmdsList->setLayout(main_layout);
    //ui->CmdsList->setAlignment(Qt::AlignTop);
    ui->ParamName->setText(crtiterionsToString(crt));
    scroll_w = new QWidget(ui->CmdsList);
    main_layout = new QVBoxLayout(scroll_w);
    main_layout->setAlignment(Qt::AlignTop);
    scroll_w->setLayout(main_layout);
    ui->CmdsList->setWidget(scroll_w);
}

ListForm::~ListForm() {
    delete ui;
}

void ListForm::setDataPth(QString path) {
    this->path = path;
}

Criterions ListForm::getCriterion() const {
    return crt;
}

QVBoxLayout * ListForm::getMainLayout() const {
    return main_layout;
}

void ListForm::changeStateLable(ProccesThreadState ptc) {
    ui->ProccesState->setText(stringProcessState(ptc));
}

void ListForm::addElem(listElemetData led) {
    ListElem * le = new ListElem(led, scroll_w);
    //connect(le, SIGNAL(rightMBClicked(QString, QString, QString)), this, SLOT(elementPressed(QString, QString, QString)));
    connect(le, SIGNAL(leftMBDClicked(listElemetData)), this, SLOT(openXlsx(listElemetData)));
    int i = 0;
    while (i < main_layout->count()) {
        ListElem * cle = (ListElem *)main_layout->itemAt(i)->widget();
        if ((cle->getPercent() == led.percent && QString::compare(cle->getCmdName(), led.command, Qt::CaseInsensitive) > 0)
        || (cle->getPercent() < led.percent))
            break;
        i++;
    }

    main_layout->insertWidget(i, le);
}

void ListForm::delElem(int index) {
    QLayoutItem * witem = main_layout->takeAt(index);
    main_layout->removeWidget(witem->widget());
    delete witem->widget();
}

void ListForm::showCurrentSet(int percent) {
    int i = 0;
    while (i < main_layout->count()) {
        ListElem * cle = (ListElem *)main_layout->itemAt(i)->widget();
        if (cle->getPercent() < percent)
            cle->setVisible(false);
        else
            cle->setVisible(true);
        i++;
    }

    main_layout->update();
}

void ListForm::showCurrentSet(QString value) {
    int i = 0;
    while (i < main_layout->count()) {
        ListElem * cle = (ListElem *)main_layout->itemAt(i)->widget();
        if (!strncmp(cle->getCmdName().toStdString().c_str(), value.toStdString().c_str(), value.size()))
            cle->setVisible(true);
        else
            cle->setVisible(false);
        i++;
    }

    main_layout->update();
}

void ListForm::openXlsx(listElemetData led) {
    qDebug() << led.country << " " << led.league << " " << led.command;
    QString xlsx_path = path + "/" + led.country + "/" + led.league;

    QAxObject * excel = new QAxObject("Excel.Application", 0);
    excel->dynamicCall("SetVisible(bool)", true);
    QAxObject * workbooks = excel->querySubObject("Workbooks");
    QAxObject * workbook = workbooks->querySubObject("Open(const QString&)", xlsx_path);
    QAxObject * worksheet = workbook->querySubObject("Worksheets(int)", led.id + 1);
    worksheet->dynamicCall("Select()");
    delete excel;
}



