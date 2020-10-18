#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QDebug>
#include <QVector>
#include <QString>
#include <QFileDialog>
#include "tableworker.h"
#include "utils.h"

MainWindow::MainWindow(ConfigWorker * cfg, QWidget * parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , cfgWorker(cfg)
{
    ui->setupUi(this);

    connect(ui->countryList, SIGNAL(currentIndexChanged(int)), this, SLOT(changeCountryList(int)));
    connect(ui->leaguesList, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLeaguesList(int)));
    connect(ui->cmdList, SIGNAL(clicked(QModelIndex)), this, SLOT(changeStateCBoxCmd(QModelIndex)));
    connect(ui->AnalizeBtn, SIGNAL(clicked()), this, SLOT(clickedAnalize()));
    connect(ui->genTableBtn, SIGNAL(clicked()), this, SIGNAL(showGenTableWindow()));
    connect(ui->menuSettings, SIGNAL(aboutToShow()), this, SIGNAL(showSettingsWindow()));
    connect(ui->chooseAllCBox, SIGNAL(stateChanged(int)), this, SLOT(chooseAllCBoxCmd()));
    connect(ui->LoadOldResultBtn, SIGNAL(clicked()), this, SLOT(clickLoadOldResults()));
    connect(ui->UncheckAllBtn, SIGNAL(clicked()), this, SLOT(uncheckAll()));

    ui->goalsAnalizeCBox->setCheckState(Qt::Checked);

    ui->leaguesList->setEnabled(false);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::init() {
    ui->countryList->addItem(" ");
    ui->countryList->addItems(cfgWorker->getContries());
}

void MainWindow::clickedAnalize() {
    QVector<Criterions> vcrt;

    if (ui->goalsAnalizeCBox->checkState() == Qt::Checked) {
        vcrt.push_back(Criterions::GOAL_FIRTS_TIME);
        vcrt.push_back(Criterions::GOAL_SECOND_TIME);
        vcrt.push_back(Criterions::GOAL_IN_3_TIMES);
    }

    //ui->percentEdit->setEnabled(false);

    emit startAnalize(vcrt);
}

void MainWindow::changeCountryList(int index) {
    if (ui->countryList->currentText() == " ") {
        //ui->AnalizeBtn->setEnabled(false);
        return;
    }

    ui->leaguesList->setEnabled(true);
    ui->leaguesList->clear();
    ui->leaguesList->addItem(" ");
    ui->leaguesList->addItems(cfgWorker->getLeagues(ui->countryList->currentText()));
}

void MainWindow::changeLeaguesList(int index) {
    if ( ui->leaguesList->currentText() == " ") {
        //ui->AnalizeBtn->setEnabled(false);
        return;
    }

    ui->cmdList->clear();

    ui->AnalizeBtn->setEnabled(true);

    QListWidgetItem * lwi = nullptr;
    QVector<CMDState> cmds = cfgWorker->getCmds(ui->countryList->currentText(), ui->leaguesList->currentText());
    int flag = 0;
    foreach (CMDState cmd, cmds) {
        lwi = new QListWidgetItem(cmd.cmd.name, ui->cmdList);
        lwi->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
        lwi->setCheckState(cmd.checked ? Qt::Checked : Qt::Unchecked);
        if (cmd.checked)
            flag++;
    }

    if (flag == cmds.size())
        ui->chooseAllCBox->setCheckState(Qt::Checked);
    else
        ui->chooseAllCBox->setCheckState(Qt::Unchecked);

}

void MainWindow::changeStateCBoxCmd(QModelIndex index) {
    QListWidgetItem * lwi = ui->cmdList->item(index.row());
    lwi->setCheckState(lwi->checkState() ? Qt::Unchecked : Qt::Checked);
    cfgWorker->changeCmdState(ui->countryList->currentText(), ui->leaguesList->currentText(), lwi->text(), lwi->checkState());
}

void MainWindow::chooseAllCBoxCmd() {
    for (int i = 0; i < ui->cmdList->count(); ++i) {
        QListWidgetItem * lwi = ui->cmdList->item(i);
        lwi->setCheckState(ui->chooseAllCBox->checkState() ? Qt::Checked : Qt::Unchecked);
    }

    cfgWorker->changeLigaState(ui->countryList->currentText(), ui->leaguesList->currentText(), ui->chooseAllCBox->checkState() ? 1 : 0);
}

void MainWindow::clickLoadOldResults() {
    QString path = QFileDialog::getOpenFileName(0,QObject::tr("Укажите файл базы данных"),ANALIZE_RESULT_DIR, QObject::tr("Файл SQLite (*." ANALIZE_RESULT_FILE_FORMAT ")"));
    QList<ResAnalize> res = cfgWorker->loadAnalizeResults(path);
    emit showSavedResults(res);
}

void MainWindow::uncheckAll() {
    for (int i = 0; i < ui->cmdList->count(); ++i) {
        QListWidgetItem * lwi = ui->cmdList->item(i);
        lwi->setCheckState(Qt::Unchecked);
    }
    ui->chooseAllCBox->setCheckState(Qt::Unchecked);
    cfgWorker->changeAllCmdsState(0);

}
