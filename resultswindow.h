#ifndef RESULTSWINDOW_H
#define RESULTSWINDOW_H

#include <QWidget>
#include <QList>
#include <QThreadPool>
#include "types.h"
#include "configworker.h"

#include <QHBoxLayout>

namespace Ui {
    class ResultsWindow;
    class ResElemForm;
}


class ResultsWindow : public QWidget {
    Q_OBJECT

public:
    explicit ResultsWindow(ConfigWorker * cfg, QWidget * parent = nullptr);
    ~ResultsWindow();
private:
    Ui::ResultsWindow * ui;
    ConfigWorker * cfg;
    QThreadPool * tp;
    QHBoxLayout * main_layout;
    QWidget * scroll_widget;
    QVector<Criterions> m_vcrt;
    void addCWThread(Criterions criterion);
    void runAnalize(QVector<Criterions> vcrt);
public slots:
    void startAnalize(QVector<Criterions> vcrt);
    void sshowSavedResults(QList<ResAnalize>);
private slots:
    void showCurrentSet(int value);
    void showSearchResults(const QString str);
    void safeResults();
    //void elementClicked();
};

#endif // RESULTSWINDOW_H
