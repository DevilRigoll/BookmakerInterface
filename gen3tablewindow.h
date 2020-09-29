#ifndef GEN3TABLEWINDOW_H
#define GEN3TABLEWINDOW_H

#include <QWidget>
#include "configworker.h"
#include <QMessageBox>

namespace Ui {
class Gen3TableWindow;
}

class Gen3TableWindow : public QWidget {
    Q_OBJECT
public:
    explicit Gen3TableWindow(ConfigWorker * cfg, QWidget * parent = nullptr);
    ~Gen3TableWindow();
public slots:
    void init();
private:
    Ui::Gen3TableWindow * ui;
    ConfigWorker * m_cfg;
    QMessageBox * msg;
private slots:
    void clickGenTables();
    void changeCountryList(int);
    void changeLeaguesList(int);
    void changeCmdList(int);
};

#endif // GEN3TABLEWINDOW_H
