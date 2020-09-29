#include "mainwindow.h"

#include <QApplication>

#include "configworker.h"
#include "resultswindow.h"
#include "gen3tablewindow.h"
#include "settingswindow.h"

#include <QDebug>

#include <QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

    ConfigWorker * cw = new ConfigWorker();

    MainWindow w(cw);
    SettingsWindow sw(cw);
    ResultsWindow rw(cw);
    Gen3TableWindow gtw(cw);

    cw->setConfigFile();
    if (cw->openDB() == 0) {
        qDebug() << "Norm";
    }
    else {
        qDebug() << "Bad";
    }

    w.init();
    gtw.init();
    sw.init();

    w.show();
    QObject::connect(&w, SIGNAL(startAnalize(QVector<Criterions>)), &rw, SLOT(startAnalize(QVector<Criterions>)));
    //QObject::connect(&rw, SIGNAL(destroyed(QObject *)), &w, SLOT(enablePercentEdit(QObject*)));
    QObject::connect(&w, SIGNAL(showGenTableWindow()), &gtw, SLOT(show()));
    QObject::connect(&w, SIGNAL(showSettingsWindow()), &sw, SLOT(show()));

    a.exec();
    cw->saveConfigFile();
    delete cw;
    return 0;
}
