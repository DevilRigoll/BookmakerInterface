#ifndef CONFIGWORKER_H
#define CONFIGWORKER_H

#include <QObject>
#include <QtSql/QtSql>
#include "types.h"
#include <QString>
#include <QVector>
#include <QStringList>
#include <QMap>

#define CONGIF_FILENAME "./config.txt"
#define DB_LEAGUES_TABLE(x) ("leaguesOf" + (x))
#define DB_CMDS_TABLE(x, y) (x) + "_" + (y)
#define STR_MACRO(x) #x
#define DB_TABLE_RESULTS(x) "RES_TABLE_" STR_MACRO(x)


class ConfigWorker {
public:
    ConfigWorker();
    ~ConfigWorker();
    void setData(QString, QString, QVector<Command>);

    QStringList getLeagues(QString);
    QVector<CMDState> getCmds(QString, QString);

    int openDB();
    void fillDB();

    QStringList getContries();
    QString getXlsxPath() const;
    QString getDBPath() const;
    void setXlsxPath(QString);
    void setDBPath(QString);

    int setConfigFile();
    void saveConfigFile();

    void changeCmdState(QString, QString, QString, bool);
    void changeAllCmdsState(bool);
    void changeLigaState(QString country, QString league, bool state);

    int saveAnalizeResults(QList<ResAnalize> vled);
    QList<ResAnalize> loadAnalizeResults(QString path);
private:
    void initDatabase();

    QSqlDatabase sdb;
    QString db_path;
    QString xlsx_path;
};

#endif // CONFIGWORKER_H
