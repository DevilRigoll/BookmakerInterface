#include "utils.h"

QString stringProcessState(ProccesThreadState pts) {
    switch(pts) {
        case WAITING: return "Waiting";
        case FINISH: return "Finish";
        case WORKING: return "Working";
        case STARTING: return "Starting";
    }

    return "";
}

QString crtiterionsToString(Criterions crt) {
    switch(crt) {
        case GOAL_FIRTS_TIME: return "Goals in 1st time";
        case GOAL_SECOND_TIME: return "Goals in 2ed time";
        case GOAL_IN_3_TIMES: return "Goal in all times";
    }

    return "";
}

int criterionsToTime(Criterions crt) {
    switch (crt) {
        case GOAL_FIRTS_TIME: return 1;
        case GOAL_SECOND_TIME: return 2;
        case GOAL_IN_3_TIMES: return 999;
    }

    return -1;
}

ConfigFileParametrs StringToCFP(QString param) {
    if (param == "Xlsx_path")
        return ConfigFileParametrs::Xlsx_path;
    else if (param == "DB_path")
        return ConfigFileParametrs::DB_path;
}

MatchPeriod criterionToMatchPeriod(Criterions crt) {
    MatchPeriod p;
    switch(crt) {
        case GOAL_FIRTS_TIME: p = FIRST_TIME; break;
        case GOAL_SECOND_TIME: p = SECOND_TIME; break;
        case GOAL_IN_3_TIMES: p = WHOLE_MATCH; break;
    }

    return p;
}
