#ifndef TYPES_H
#define TYPES_H

#include <QString>
#include <QVector>
#include <QDateTime>
#include <QList>

struct Command {
      int id;
      QString name;
};

struct ResAnalizeExpectedValue {
      double max;
      double avg;
};

struct TableSize {
      int w;
      int h;
};

struct DataAnalize {
      int id;
      int corner_count_time1;
      int corner_count_time2;
      QVector<int> goal_time1;
      QVector<int> goal_time2;
      QVector<int> goal_time3;
      QVector<int> y_card_time1;
      QVector<int> y_card_time2;
      QString place;
      QString enemy_name;
      QDate date;
};

template <typename T>
struct Vec2T {
    T x;
    T y;
};

typedef
struct listElemetData {
    int id;
    QString country;
    QString league;
    QString command;
    int percent;
    float avgValue;
    float maxValue;
} listElemetData;

enum Criterions {
    GOAL_FIRTS_TIME = 1,
    GOAL_SECOND_TIME = 2,
    GOAL_IN_3_TIMES = 3
};

typedef
struct ResAnalize {
    Criterions crt;
    QList<listElemetData> lled;
} ResAnalize;

enum ProccesThreadState {
    STARTING,
    WAITING,
    WORKING,
    FINISH
};

struct CMDState {
    Command cmd;
    bool checked;
};

enum TablesFlags {
    GOALS = 1,
    YELLOW_CARDS = 3,
    CORNERS = 4
};

enum ConfigFileParametrs {
    Xlsx_path = 1,
    DB_path = 2,
};

enum MatchPeriod {
    FIRST_TIME = 1,
    SECOND_TIME,
    AFTER_80MIN,
    WHOLE_MATCH,
};

#endif // TYPES_H
