#ifndef LDAWORKER_H
#define LDAWORKER_H

#include "types.h"
#include <QList>
#include <QVector>

class LDAWorker{
public:
    LDAWorker(QList<DataAnalize> LDA);

    float getAvgNoGoals(MatchPeriod period);
    float getAvgNoYellow(MatchPeriod period);
    float getAvgNoCorner(MatchPeriod period);

    float getLastCountNoGoals(MatchPeriod period);
    float getLastCountNoYellow(MatchPeriod period);
    float getLastCountNoCorner(MatchPeriod period);

    float getAvgNoGAllTimes();
    float getAvgNoYAllTimes();
    float getAvgNoCAllTimes();

    float getLastNoGAllTimes();
    float getLastNoYAllTimes();
    float getLastNoCAllTimes();

    int getMaxNoGoals(MatchPeriod period);
    int getMaxNoGAllTimes();

    QVector<int> getNoGDiapasons(MatchPeriod period);


private:
    QList<DataAnalize> m_LDA;
};

#endif // LDAWORKER_H
