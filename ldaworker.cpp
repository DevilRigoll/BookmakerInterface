#include "ldaworker.h"

LDAWorker::LDAWorker(QList<DataAnalize> LDA) : m_LDA(LDA) {};

float LDAWorker::getAvgNoGoals(MatchPeriod period) {
    float avg = 0;
    int noPerCount = 0;

    switch(period) {
    case FIRST_TIME: {
        for (int i = 0; i < m_LDA.size(); ++i) {
            int count;
            for(count = 0; i < m_LDA.size() && m_LDA[i].goal_time1.size() == 0; ++count, ++i);
            if (count > 1) {
                noPerCount++;
                avg += count;
            }
        }
    }
        break;
    case SECOND_TIME: {
        for (int i = 0; i < m_LDA.size(); ++i) {
            int count;
            for(count = 0; i < m_LDA.size() && m_LDA[i].goal_time2.size() == 0
                            && m_LDA[i].goal_time3.size() == 0; ++count, ++i);
            if (count > 1) {
                noPerCount++;
                avg += count;
            }
        }
    }
        break;
    case AFTER_80MIN: {
        for (int i = 0; i < m_LDA.size(); ++i) {
            int count;
            for(count = 0; i < m_LDA.size() && m_LDA[i].goal_time3.size() == 0; ++count, ++i);
            if (count > 1) {
                noPerCount++;
                avg += count;
            }
        }
        break;
    }
    case WHOLE_MATCH: {
        return getAvgNoGAllTimes();
    }
    }

    return avg / (float)noPerCount;
}

float LDAWorker::getAvgNoYellow(MatchPeriod period) {
    float avg = 0;
    int noPerCount = 0;

    switch(period) {
    case FIRST_TIME: {
        for (int i = 0; i < m_LDA.size(); ++i) {
            int count;
            for(count = 0; i < m_LDA.size() && m_LDA[i].y_card_time1.size() == 0; ++count, ++i);
            if (count > 1) {
                noPerCount++;
                avg += count;
            }
        }
    }
        break;
    case SECOND_TIME: {
        for (int i = 0; i < m_LDA.size(); ++i) {
            int count;
            for(count = 0; i < m_LDA.size() && m_LDA[i].y_card_time2.size() == 0; ++count, ++i);
            if (count > 1) {
                noPerCount++;
                avg += count;
            }
        }
        break;
    }
    case WHOLE_MATCH: {
        return getAvgNoYAllTimes();
    }

    }

    return avg / (float)noPerCount;
}

float LDAWorker::getAvgNoCorner(MatchPeriod period) {
    float avg = 0;
    int noPerCount = 0;

    switch(period) {
    case FIRST_TIME: {
        for (int i = 0; i < m_LDA.size(); ++i) {
            int count;
            for(count = 0; i < m_LDA.size() && m_LDA[i].corner_count_time1 == 0; ++count, ++i);
            if (count > 1) {
                noPerCount++;
                avg += count;
            }
        }
    }
        break;
    case SECOND_TIME: {
        for (int i = 0; i < m_LDA.size(); ++i) {
            int count;
            for(count = 0; i < m_LDA.size() && m_LDA[i].corner_count_time2 == 0; ++count, ++i);
            if (count > 1) {
                noPerCount++;
                avg += count;
            }
        }
        break;
    }
    case WHOLE_MATCH: {
        return getAvgNoCAllTimes();
    }
    }

    return avg / (float)noPerCount;
}

float LDAWorker::getAvgNoGAllTimes() {
    float avg = 0;
    int noPerCount = 0;

    for (int i = 0; i < m_LDA.size(); ++i) {
        int count;
        for(count = 0;
            i < m_LDA.size() && m_LDA[i].goal_time1.size() == 0
            && m_LDA[i].goal_time2.size() == 0
            && m_LDA[i].goal_time3.size() == 0;
            ++count, ++i);
        if (count > 1) {
            noPerCount++;
            avg += count;
        }
    }

    return avg / (float)noPerCount;
}

float LDAWorker::getAvgNoYAllTimes() {
    float avg = 0;
    int noPerCount = 0;

    for (int i = 0; i < m_LDA.size(); ++i) {
        int count;
        for(count = 0;
            i < m_LDA.size() &&
            m_LDA[i].y_card_time1.size() == 0
            && m_LDA[i].y_card_time2.size() == 0;
            ++count, ++i);
        if (count > 1) {
            noPerCount++;
            avg += count;
        }
    }

    return avg / (float)noPerCount;
}

float LDAWorker::getAvgNoCAllTimes() {
    float avg = 0;
    int noPerCount = 0;

    for (int i = 0; i < m_LDA.size(); ++i) {
        int count;
        for(count = 0;
            i < m_LDA.size()
            && m_LDA[i].corner_count_time1 == 0
            && m_LDA[i].corner_count_time2 == 0;
            ++count, ++i);
        if (count > 1) {
            noPerCount++;
            avg += count;
        }
    }

    return avg / (float)noPerCount;
}

float LDAWorker::getLastNoGAllTimes() {
    int count = 0;

    for (int i = m_LDA.size() - 1;
         i > 0
         && m_LDA[i].goal_time1.size() == 0
         && m_LDA[i].goal_time2.size() == 0
         && m_LDA[i].goal_time3.size() == 0;
         --i, ++count);

    return count;
}

float LDAWorker::getLastNoYAllTimes() {
    int count = 0;

    for (int i = m_LDA.size() - 1;
         i > 0
         && m_LDA[i].y_card_time1.size() == 0
         && m_LDA[i].y_card_time2.size() == 0;
         --i, ++count);

    return count;
}

float LDAWorker::getLastNoCAllTimes() {
    int count = 0;

    for (int i = m_LDA.size() - 1;
         i > 0
         && m_LDA[i].corner_count_time1 == 0
         && m_LDA[i].corner_count_time2 == 0;
         --i, ++count);

    return count;
}

float LDAWorker::getLastCountNoGoals(MatchPeriod period) {
    int count = 0;

    switch(period) {
        case FIRST_TIME:      for (int i = m_LDA.size() - 1; i > 0 && m_LDA[i].goal_time1.size() == 0; --i, ++count); break;
        case SECOND_TIME:     for (int i = m_LDA.size() - 1; i > 0 && m_LDA[i].goal_time2.size() == 0 && m_LDA[i].goal_time3.size() == 0; --i, ++count); break;
        case AFTER_80MIN:     for (int i = m_LDA.size() - 1; i > 0 && m_LDA[i].goal_time3.size() == 0; --i, ++count); break;
        case WHOLE_MATCH:     return getLastNoGAllTimes();
    }

    return count;
}

float LDAWorker::getLastCountNoYellow(MatchPeriod period) {
    int count = 0;

    switch(period) {
        case FIRST_TIME:      for (int i = m_LDA.size() - 1; i > 0 && m_LDA[i].y_card_time1.size() == 0; --i, ++count); break;
        case SECOND_TIME:     for (int i = m_LDA.size() - 1; i > 0 && m_LDA[i].y_card_time2.size() == 0; --i, ++count); break;
        case WHOLE_MATCH:     return getLastNoYAllTimes();
    }

    return count;
}

float LDAWorker::getLastCountNoCorner(MatchPeriod period) {
    int count = 0;

    switch(period) {
        case FIRST_TIME:      for (int i = m_LDA.size() - 1; i > 0 && m_LDA[i].corner_count_time1 == 0; --i, ++count); break;
        case SECOND_TIME:     for (int i = m_LDA.size() - 1; i > 0 && m_LDA[i].corner_count_time2 == 0; --i, ++count); break;
        case WHOLE_MATCH:     return getLastNoCAllTimes();
    }

    return count;
}

int LDAWorker::getMaxNoGoals(MatchPeriod period) {
    int max = 0;

    switch(period) {
    case FIRST_TIME: {
        for (int i = 0; i < m_LDA.size(); ++i) {
            int count;
            for(count = 0; i < m_LDA.size() && m_LDA[i].goal_time1.size() == 0; ++count, ++i);
            if (max < count)
                max = count;

        }
    }
        break;
    case SECOND_TIME: {
        for (int i = 0; i < m_LDA.size(); ++i) {
            int count;
            for(count = 0; i < m_LDA.size() && m_LDA[i].goal_time2.size() == 0
                            && m_LDA[i].goal_time3.size() == 0; ++count, ++i);
            if (max < count)
                max = count;
        }
    }
        break;
    case AFTER_80MIN: {
        for (int i = 0; i < m_LDA.size(); ++i) {
            int count;
            for(count = 0; i < m_LDA.size() && m_LDA[i].goal_time3.size() == 0; ++count, ++i);
            if (max < count)
                max = count;
        }
        break;
    }
    case WHOLE_MATCH: {
        return getMaxNoGAllTimes();
    }
    }

    return max;
}

int LDAWorker::getMaxNoGAllTimes() {
    int max = 0;

    for (int i = 0; i < m_LDA.size(); ++i) {
        int count;
        for(count = 0;
            i < m_LDA.size() && m_LDA[i].goal_time1.size() == 0
            && m_LDA[i].goal_time2.size() == 0
            && m_LDA[i].goal_time3.size() == 0;
            ++count, ++i);
        if (max < count)
            max = count;
    }

    return max;
}

QVector<int> LDAWorker::getNoGDiapasons(MatchPeriod period) {
    int N = getMaxNoGoals(period);
    QVector<int> res(N, 0);

    if (N == 0)
        return res;

    switch(period) {
    case FIRST_TIME: {
        for (int i = 0; i < m_LDA.size(); ++i) {
            int count;
            for(count = 0; i < m_LDA.size() && m_LDA[i].goal_time1.size() == 0; ++count, ++i);
            if (count == 0)
                continue;
            res[count - 1]++;
        }
        break;
    }
    case SECOND_TIME: {
        for (int i = 0; i < m_LDA.size(); ++i) {
            int count;
            for(count = 0; i < m_LDA.size() && m_LDA[i].goal_time2.size() == 0
                            && m_LDA[i].goal_time3.size() == 0; ++count, ++i);
            if (count == 0)
                continue;
            res[count - 1]++;
        }
        break;
    }
    case AFTER_80MIN: {
        for (int i = 0; i < m_LDA.size(); ++i) {
            int count;
            for(count = 0; i < m_LDA.size() && m_LDA[i].goal_time1.size() == 0; ++count, ++i);
            if (count == 0)
                continue;
            res[count - 1]++;
        }
        break;
    }
    case WHOLE_MATCH: {
        for (int i = 0; i < m_LDA.size(); ++i) {
            int count;
            for(count = 0;
                i < m_LDA.size() && m_LDA[i].goal_time1.size() == 0
                && m_LDA[i].goal_time2.size() == 0
                && m_LDA[i].goal_time3.size() == 0;
                ++count, ++i);
            if (count == 0)
                continue;
            res[count - 1]++;
        }
        break;
    }
    }

    return res;
}
