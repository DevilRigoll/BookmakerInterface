#include "threadanalizeprocess.h"
#include <QStringList>
#include "ldaworker.h"
#include <tableworker.h>

#include <math.h>
#include "utils.h"

static QMutex analizeMutex;

ThreadAnalizeProcess::ThreadAnalizeProcess(ListForm * w, ConfigWorker * cfg, Criterions crt)
    : m_w(w),
      m_crt(crt)
{
    m_cfg = new ConfigWorker(*cfg);
    qDebug() << "-1m " ;
}

ThreadAnalizeProcess::~ThreadAnalizeProcess() {
    delete m_cfg;
}

bool ThreadAnalizeProcess::isOkByAverageValue(QList<DataAnalize> LDA) {
    LDAWorker lda(LDA);

    float lastNo = 0;
    float avgNo = 0;

    switch (m_crt) {
        case GOAL_FIRTS_TIME:
        case GOAL_SECOND_TIME: {
            avgNo = lda.getAvgNoGoals(criterionToMatchPeriod(m_crt));
            lastNo = lda.getLastCountNoGoals(criterionToMatchPeriod(m_crt));
        }
            break;
        case GOAL_IN_3_TIMES: {
            avgNo = lda.getAvgNoGAllTimes();
            lastNo = lda.getLastNoGAllTimes();
        }
            break;
    }

    float r = 0;
    avgNo = modf(avgNo, &r);
    avgNo = avgNo > 0.5f ? r : r - 1;

    if (lastNo >= avgNo)
        return true;

    return false;
}

float ThreadAnalizeProcess::calcPercent(QList<DataAnalize> LDA, int avgValue) {
    float res;
    LDAWorker ldaw(LDA);
    QVector<int> vec = ldaw.getNoGDiapasons(criterionToMatchPeriod(m_crt));

    int lastNoGoals = ldaw.getLastCountNoGoals(criterionToMatchPeriod(m_crt));


    if (lastNoGoals > avgValue) {
        res = 0;
        int count = 0;
        int count2 = 0;
        for (int i = avgValue; i < vec.size(); ++i) {
            count += vec[i];
            //count2++;
        }
        for (int i = avgValue; i < lastNoGoals && i < vec.size(); ++i)
            res += 1 - ((count - vec[i]) / float(count));
            //count2 += vec[i];

        //res += count2 * 50 / float(count) + 50;
        res *= 50;
        res += 50;

    }
    else if (lastNoGoals < avgValue) {
        res = 0;
        int count = 0;
        int count2 = 0;
        for (int i = 0; i < avgValue && i < vec.size(); ++i)
            //res += 1 - (1 - vec[i] / float(count));
            count += vec[i];

        if (count == 0)
          return 0;

        for (int i = 0; i < lastNoGoals && i < vec.size(); ++i)
            res += 1 - ((count - vec[i]) / float(count));
            //count2 += vec[i];

        //res += count2 * 50 / float(count);
        res *= 50;
        //res += 50;

    }
    else
        res = 50;

    return res;
}

float ThreadAnalizeProcess::getPercent(LDAWorker ldaw, int curr_no_v) {
    if (curr_no_v == 0)
        return 0;

    QVector<int> vec = ldaw.getNoGDiapasons(criterionToMatchPeriod(m_crt));
    float avg = ldaw.getAvgNoGoals(criterionToMatchPeriod(m_crt));
    float r = 0;
    avg = modf(avg, &r);
    avg = avg > 0.5f ? r : r - 1;


    int count = 0;
    for (int i = 0; i < vec.size(); ++i)
        count += (i + 1) * vec[i];

    int count2 = 0;
    for (int i = curr_no_v; i < vec.size(); ++i)
        count2 += (i + 1) * vec[i];

    if (curr_no_v >= vec.size())
        return 100;

    qDebug() << (count - count2) << " / " << float(count) << " " << (count - count2) / float(count);

    return (count - count2) * 100 / float(count);

    //int max = 0;
    //for (int i = curr_no_v; i < vec.size(); ++i)
        //if (vec[curr_no_v] > max)
            //max = vec[curr_no_v];
    int sum = 0;
    for (int i = curr_no_v; i < vec.size(); ++i)
        sum += vec[i];

    if (avg <= curr_no_v) {
        if ((vec[curr_no_v - 1] - 1) >= vec[curr_no_v]) {
            //float denom = vec[curr_no_v - 1] + vec[curr_no_v] - 1;
            //float denom = vec[curr_no_v - 1] + max - 1;
            /*
            float denom = vec[curr_no_v - 1] + sum - 1;
            float numer = vec[curr_no_v - 1] - 1;
            if (denom == 0) {
                if (numer > 0)
                    return 100;
                else
                    return 0;
            }
            return numer * 100 / denom;
            */
        }
        else {
            //float denom = vec[curr_no_v - 1] + max - 1;
            /*
            float numer = vec[curr_no_v - 1] - 1;
            if (denom == 0) {
                if (numer > 0)
                    return 100;
                else
                    return 0;
            }
            return numer * 100 / denom;
            */
        }


    }
    else {
        float res = 0;
        int count = 0;
        for (int i = 0; i < avg && i < vec.size(); ++i)
            count += vec[i];

        if (count == 0)
          return 0;

        for (int i = 0; i < curr_no_v && i < vec.size(); ++i)
            res += 1 - ((count - vec[i]) / float(count));
            //count2 += vec[i];

        //res += count2 * 50 / float(count);
        res *= 50;
        return res;
    }

    return 0;
}

void ThreadAnalizeProcess::run() {
    TableWorker tw;
    qDebug() << "TAP 1";
    //m_w->changeStateLable(ProccesThreadState::WORKING);
    qDebug() << "TAP 2";
    QStringList countries = m_cfg->getContries();
    qDebug() << "TAP 2";
    for (int i = 0; i < countries.size(); ++i) {
        QStringList leagues = m_cfg->useCountry(countries[i]);
        qDebug() << "TAP 3";
        for (int j = 0; j < leagues.size(); ++j) {
            QVector<CMDState> cmds = m_cfg->useLiga(leagues[j]);
            for (int k = 0 ; k < cmds.size(); ++k) {
                if (cmds[k].checked) {
                    QString path = m_cfg->getXlsxPath() + "/" + countries[i] + "/" + leagues[j];
                    qDebug() << "TAP 4";
                    tw.open(path);
                    QList<DataAnalize> LDA = tw.genLDA(cmds[k].cmd);
                    tw.closeXlsx();
                    LDAWorker ldaw(LDA);
                    qDebug() << "TAP 5";
                    int noGoals = ldaw.getLastCountNoGoals(criterionToMatchPeriod(m_crt));
                    qDebug() << countries[i] << " " << leagues[j] << " " << cmds[k].cmd.name;
                    float _percent = getPercent(ldaw, noGoals);
                    qDebug() << "end";
                    listElemetData led = {cmds[k].cmd.id, countries[i], leagues[j], cmds[k].cmd.name, static_cast<int>(_percent), 0, 0};
                    qDebug() << "TAP 6";
                    analizeMutex.lock();
                    m_w->addElem(led);
                    analizeMutex.unlock();
                    qDebug() << "TAP 7";
                }
            }

        }

    }

    m_w->changeStateLable(ProccesThreadState::FINISH);
}
