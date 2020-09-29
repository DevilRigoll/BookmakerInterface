#include "settingswindow.h"
#include "ui_settingswindow.h"


SettingsWindow::SettingsWindow(ConfigWorker * cfg, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWindow),
    m_cfg(cfg)
{
    ui->setupUi(this);

    connect(ui->ApplyBtn, SIGNAL(clicked()), this, SLOT(applyClick()));
    connect(ui->InitDataBtn, SIGNAL(clicked()), this, SLOT(initDataClicked()));
    connect(ui->Xlsx_path, SIGNAL(textChanged(QString )), this, SLOT(textChanged(QString)));
    connect(ui->DB_path, SIGNAL(textChanged(QString )), this, SLOT(textChanged(QString)));


    ui->ApplyBtn->setEnabled(false);
    ui->InitDataBtn->setEnabled(false);
}

SettingsWindow::~SettingsWindow() {
    delete ui;
}

void SettingsWindow::init() {
    ui->DB_path->setText(m_cfg->getDBPath());
    ui->Xlsx_path->setText(m_cfg->getXlsxPath());
}

void SettingsWindow::applyClick() {
    m_cfg->setXlsxPath(ui->Xlsx_path->text());
    m_cfg->setDBPath(ui->DB_path->text());
    m_cfg->saveConfigFile();

    emit applyClicked();
}

void SettingsWindow::initDataClicked() {
    m_cfg->fillDB();
}

void SettingsWindow::textChanged(QString str) {
    if (ui->Xlsx_path->text() != "" && ui->DB_path->text() != "") {
        ui->ApplyBtn->setEnabled(true);
        ui->InitDataBtn->setEnabled(true);
    }
    else {
        ui->ApplyBtn->setEnabled(false);
        ui->InitDataBtn->setEnabled(false);
    }
}
