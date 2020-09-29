#ifndef THREADANALIZEPROCESS_H
#define THREADANALIZEPROCESS_H

#include <QRunnable>
#include <QMutex>
#include "configworker.h"
#include "listform.h"
#include "ldaworker.h"

class ThreadAnalizeProcess : public QRunnable {
public:
    ThreadAnalizeProcess(ListForm * w, ConfigWorker * cfg, Criterions crt);
    ~ThreadAnalizeProcess();
    void run();// override;
private:
    ConfigWorker * m_cfg;
    ListForm * m_w;
    Criterions m_crt;

    bool isOkByAverageValue(QList<DataAnalize> LDA);
    float calcPercent(QList<DataAnalize> LDA, int avgValue);
    float getPercent(LDAWorker ldaw, int curr_no_v);
};

#endif // THREADANALIZEPROCESS_H
