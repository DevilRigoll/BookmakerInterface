#include "gen3tablewindow.h"
#include "ui_gen3tablewindow.h"


#include <QDir>
#include <QFile>
#include "tableworker.h"


Gen3TableWindow::Gen3TableWindow(ConfigWorker * cfg, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Gen3TableWindow),
    m_cfg(cfg),
    msg(nullptr)
{
    ui->setupUi(this);

    connect(ui->GenBtn, SIGNAL(clicked()), this, SLOT(clickGenTables()));
    connect(ui->CountryBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeCountryList(int)));
    connect(ui->LeagueBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLeaguesList(int)));
    connect(ui->Cmd1Box, SIGNAL(currentIndexChanged(int)), this, SLOT(changeCmdList(int)));
    connect(ui->Cmd2Box, SIGNAL(currentIndexChanged(int)), this, SLOT(changeCmdList(int)));

    ui->GenBtn->setEnabled(false);
    ui->LeagueBox->setEnabled(false);
    ui->Cmd1Box->setEnabled(false);
    ui->Cmd2Box->setEnabled(false);
}

Gen3TableWindow::~Gen3TableWindow() {

    if (msg != nullptr)
        delete msg;

    delete ui;
}

void Gen3TableWindow::init() {
    ui->CountryBox->addItem(" ");
    ui->CountryBox->addItems(m_cfg->getContries());
}

void Gen3TableWindow::changeCountryList(int id) {
    if (ui->CountryBox->currentText() == " ") {
        ui->GenBtn->setEnabled(false);
        ui->LeagueBox->setEnabled(false);
        ui->Cmd1Box->setEnabled(false);
        ui->Cmd2Box->setEnabled(false);
        return;
    }

    ui->LeagueBox->setEnabled(true);
    ui->LeagueBox->clear();
    ui->LeagueBox->addItem(" ");
    ui->LeagueBox->addItems(m_cfg->getLeagues(ui->CountryBox->currentText()));
}

void Gen3TableWindow::changeLeaguesList(int id) {
    if ( ui->LeagueBox->currentText() == " ") {
        ui->GenBtn->setEnabled(false);
        ui->Cmd1Box->setEnabled(false);
        ui->Cmd2Box->setEnabled(false);
        return;
    }


    ui->Cmd1Box->clear();
    ui->Cmd2Box->clear();
    ui->GenBtn->setEnabled(true);
    ui->Cmd1Box->setEnabled(true);
    ui->Cmd2Box->setEnabled(true);

    QVector<CMDState> cmds = m_cfg->getCmds(ui->CountryBox->currentText(), ui->LeagueBox->currentText());

    foreach (CMDState cmd, cmds) {
        ui->Cmd1Box->addItem(cmd.cmd.name);
        ui->Cmd2Box->addItem(cmd.cmd.name);
    }
}

void Gen3TableWindow::changeCmdList(int id) {
    if (ui->Cmd1Box->currentText() != ui->Cmd2Box->currentText())
        ui->GenBtn->setEnabled(true);
    else
        ui->GenBtn->setEnabled(false);
}

void Gen3TableWindow::clickGenTables() {
    TableWorker tw;
    QString path = m_cfg->getXlsxPath() + "/" + ui->CountryBox->currentText() + "/" + ui->LeagueBox->currentText();

    tw.open(path);

    QVector<Command> cmds = tw.getCommands();

    Command cmd1;
    Command cmd2;

    for (int i = 0; i < cmds.size(); ++i) {
        if (cmds[i].name == ui->Cmd1Box->currentText())
            cmd1 = cmds[i];
        if (cmds[i].name == ui->Cmd2Box->currentText())
            cmd2 = cmds[i];
    }

    int flag = 0;
    flag |= ui->GoalsCB->checkState() == Qt::Checked ? TablesFlags::GOALS : 0;
    flag |= ui->YCardsCB->checkState() == Qt::Checked ? TablesFlags::YELLOW_CARDS : 0;
    flag |= ui->CornersCB->checkState() == Qt::Checked ? TablesFlags::CORNERS : 0;

    QDir dir("tables");

    if (!dir.exists())
        dir.mkpath(".");


    path = "./tables/" + ui->CountryBox->currentText() + "_" + ui->LeagueBox->currentText() + "_"
            + ui->Cmd1Box->currentText() + "_vs_" + ui->Cmd1Box->currentText() + ".xlsx";

    if (QFile::exists(path))
        QFile::remove(path);

    qDebug() << cmd1.id << ' ' << cmd2.id;

    int ret = tw.makeTable1vs1(cmd1, cmd2, flag, path);

    if (msg != nullptr)
        delete msg;

    msg = new QMessageBox();
    msg->setIcon(QMessageBox::Warning);
    msg->setWindowTitle("Error!!!");

    if (ret == -1) {
        msg->setText("Enemy command wasn't found!");
        msg->show();
    }
}
