#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

#include "configworker.h"

#define DIR_MAIN_PATH "C:/Users/DevilRigoll/Desktop/анализ2/"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(ConfigWorker * cfg, QWidget * parent = nullptr);
    ~MainWindow();
signals:
    void startAnalize(QVector<Criterions> vcrts);
    void showGenTableWindow();
    void showSettingsWindow();
    void showSavedResults(QList<ResAnalize>);
public slots:
    void init();
    void clickLoadOldResults();
private:
    Ui::MainWindow *ui;
    int checkedCmdCount;
    ConfigWorker * cfgWorker;
private slots:
    void changeCountryList(int index);
    void changeLeaguesList(int index);
    void changeStateCBoxCmd(QModelIndex index);
    void clickedAnalize();
    void chooseAllCBoxCmd();
    void uncheckAll();
};
#endif // MAINWINDOW_H
