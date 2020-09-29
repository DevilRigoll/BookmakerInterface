#ifndef UTILS_H
#define UTILS_H

#include "types.h"


QString stringProcessState(ProccesThreadState pts);

QString crtiterionsToString(Criterions crt);

int criterionsToTime(Criterions crt);

ConfigFileParametrs StringToCFP(QString param);

MatchPeriod criterionToMatchPeriod(Criterions crt);

#endif // UTILS_H
