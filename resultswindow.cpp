#include "resultswindow.h"
#include "ui_resultswindow.h"

#include "threadanalizeprocess.h"
#include "listelem.h"

#include <QDebug>

ResultsWindow::ResultsWindow(ConfigWorker * cfg, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultsWindow),
    cfg(cfg)
{
    ui->setupUi(this);
    tp = nullptr;
    scroll_widget = nullptr;
    main_layout = nullptr;

    connect(ui->percentSpin, SIGNAL(valueChanged(int)), this, SLOT(showCurrentSet(int)));
    connect(ui->SearchLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(showSearchResults(const QString)));
    connect(ui->SaveResultsBtn, SIGNAL(clicked()), this, SLOT(safeResults()));
}

ResultsWindow::~ResultsWindow() {
    delete ui;
    if (tp != nullptr)
        delete tp;
}

void ResultsWindow::runAnalize(QVector<Criterions> vcrt) {    
    if (tp != nullptr)
        delete tp;
    tp = new QThreadPool(this);

    if (scroll_widget != nullptr) {
        delete scroll_widget;
        scroll_widget = nullptr;
    }

    scroll_widget = new QWidget(ui->scrollArea);
    main_layout = new QHBoxLayout(scroll_widget);
    main_layout->setAlignment(Qt::AlignLeft);
    scroll_widget->setLayout(main_layout);
    ui->scrollArea->setWidget(scroll_widget);

    for (int i = 0; i < vcrt.size(); ++i) {

        //qDebug() << "cicle - " << vcrt[i];

        ListForm * lf = new ListForm(vcrt[i], scroll_widget);
        lf->setDataPth(cfg->getXlsxPath());
        lf->changeStateLable(STARTING);
        main_layout->addWidget(lf);

        qDebug() << "-1 " << i;

        ThreadAnalizeProcess tap(lf, cfg, vcrt[i]);
        qDebug() << "-1.1 " << i;
        /*
        while (1) {
            qDebug() << "-2 " << i;
            if (tp->tryStart(&tap)) {
                break;
            }
            qDebug() << "-3 " << i;
            lf->changeStateLable(WAITING);
            qDebug() << "-4 " << i;
            QThread::sleep(2);
        }
        */
        tap.run();
        //delete tap;
    }
}

void ResultsWindow::startAnalize(QVector<Criterions> vcrt) {
    this->show();
    qDebug() << "start analize";
    m_vcrt = vcrt;
    runAnalize(vcrt);
}

void ResultsWindow::showCurrentSet(int value) {
    int i = 0;
    while (i < main_layout->count()) {
        ListForm * clf = (ListForm *)main_layout->itemAt(i)->widget();
        clf->showCurrentSet(value);
        i++;
    }
}

void ResultsWindow::showSearchResults(const QString str) {

}

void ResultsWindow::safeResults() {

    QList<ResAnalize> lresa;
    for (int i = 0; i < main_layout->count(); ++i) {
        ListForm * clf = (ListForm *)main_layout->itemAt(i)->widget();
        ResAnalize resa;
        resa.crt = clf->getCriterion();
        QVBoxLayout * l = clf->getMainLayout();

        for (int j = 0; j < l->count(); ++j) {
            ListElem * le = (ListElem *)l->itemAt(j)->widget();
            resa.lled.push_back(le->getData());
        }
        lresa.push_back(resa);
    }

    cfg->saveAnalizeResults(lresa);
}

void ResultsWindow::sshowSavedResults(QList<ResAnalize> lra) {
    if (scroll_widget != nullptr) {
        delete scroll_widget;
        scroll_widget = nullptr;
    }

    scroll_widget = new QWidget(ui->scrollArea);
    main_layout = new QHBoxLayout(scroll_widget);
    main_layout->setAlignment(Qt::AlignLeft);
    scroll_widget->setLayout(main_layout);
    ui->scrollArea->setWidget(scroll_widget);

    this->show();

    for (int i = 0; i < lra.size(); ++i) {
        ListForm * lf = new ListForm(lra[i].crt, scroll_widget);
        lf->changeStateLable(FINISH);
        main_layout->addWidget(lf);
        for (int j = 0; j < lra[i].lled.size(); ++j) {
            lf->addElem(lra[i].lled[j]);
        }
    }

}
