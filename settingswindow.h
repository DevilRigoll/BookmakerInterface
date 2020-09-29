#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include "configworker.h"

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWindow(ConfigWorker * cfg, QWidget *parent = nullptr);
    ~SettingsWindow();
signals:
    void applyClicked();
public slots:
    void init();
private:
    Ui::SettingsWindow *ui;
    ConfigWorker * m_cfg;
private slots:
    void applyClick();
    void initDataClicked();
    void textChanged(QString );
};

#endif // SETTINGSWINDOW_H
