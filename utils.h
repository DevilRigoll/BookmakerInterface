#ifndef UTILS_H
#define UTILS_H

#include "types.h"

#define ANALIZE_RESULT_DIR "./results"
#define ANALIZE_RESULT_FILE_FORMAT "ares"

QString stringProcessState(ProccesThreadState pts);

QString crtiterionsToString(Criterions crt);

int criterionsToTime(Criterions crt);

ConfigFileParametrs StringToCFP(QString param);

MatchPeriod criterionToMatchPeriod(Criterions crt);

Criterions intToCrt(int i);

#endif // UTILS_H
