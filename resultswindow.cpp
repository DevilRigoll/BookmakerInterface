#include "resultswindow.h"
#include "ui_resultswindow.h"

#include "threadanalizeprocess.h"

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
