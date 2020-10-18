#include "configworker.h"
#include <QString>
#include <QFile>
#include <QDateTime>
#include <QDebug>
#include <stdio.h>
#include <iostream>
#include "tableworker.h"
#include "utils.h"

#if _WIN32 || _WIN64
    #define _CRT_SECURE_NO_WARNINGS
#endif

#define MAX_COUNT_CMD_IN_TABLE 1000

ConfigWorker::ConfigWorker() {
    sdb = QSqlDatabase::addDatabase("QSQLITE");
    QDir dir(ANALIZE_RESULT_DIR);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}

ConfigWorker::~ConfigWorker() {}

int ConfigWorker::setConfigFile() {
    FILE * f = nullptr;
    if (!(f = fopen(CONGIF_FILENAME, "r")))
            return -1;

    char str[1024];

    while (!feof(f)) {
        fgets(str, 1023, f);
        QStringList strl = QString(str).split("=");
        switch (StringToCFP(strl[0])) {
        case ConfigFileParametrs::Xlsx_path : xlsx_path = strl[1].remove(strl[1].size() - 1, 1);
            break;
        case ConfigFileParametrs::DB_path : db_path = strl[1];//.remove(strl[1].size() - 1, 1);
            break;
        }
    }

    qDebug() << xlsx_path;
    qDebug() << db_path;

    /*
    fgets(path, 1023, f);
    xlsx_path = QString(path);
    xlsx_path = xlsx_path.remove(xlsx_path.size() - 1, 1);
    fgets(path, 1023, f);
    db_path = QString(path);
    */

    fclose(f);

    return 0;
}

QString ConfigWorker::getXlsxPath() const { return xlsx_path; }
QString ConfigWorker::getDBPath() const { return db_path; }

void ConfigWorker::setXlsxPath(QString path) {
    xlsx_path = path;
}

void ConfigWorker::setDBPath(QString path) {
    db_path = path;
}

int ConfigWorker::openDB() {
    sdb.setDatabaseName(db_path);
    if (!sdb.open())
           return -1;

    return 0;
}

void ConfigWorker::saveConfigFile() {
    FILE * f = nullptr;

    f = fopen(CONGIF_FILENAME, "w");
    fprintf(f, "Xlsx_path=%s\n", xlsx_path.toUtf8().toStdString().c_str());
    fprintf(f, "DB_path=%s\0", db_path.toUtf8().toStdString().c_str());

    fclose(f);
}

void ConfigWorker::initDatabase() {
    if (db_path.size() == 0)
        exit(0);

    QDir dir(db_path);
    if (dir.exists())
        dir.remove(db_path);

    openDB();

    QSqlQuery query(sdb);
    /*
    query.exec("CREATE TABLE if not exists Countries (id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(256) NOT NULL UNIQUE)");

    QDir * main_dir = new QDir(xlsx_path);

    main_dir->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    QStringList filters = {"*.xlsx"};

    QFileInfoList list = main_dir->entryInfoList();

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo countryInfo = list[i];

        query.prepare("INSERT OR IGNORE INTO Countries (name) VALUES (:name)");
        query.bindValue(":name", countryInfo.fileName());
        query.exec();

        query.exec(QString("CREATE TABLE if not exists \"%1\" (id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(256) NOT NULL UNIQUE)")
                      .arg(DB_LEAGUES_TABLE(countryInfo.fileName())));

        QDir * dir = new QDir(xlsx_path + "/" + countryInfo.fileName() + '/');

        dir->setFilter(QDir::Files | QDir::NoDotDot);
        dir->setNameFilters(filters);

        QFileInfoList list = dir->entryInfoList();
        for (int i = 0; i < list.size(); ++i) {
            QFileInfo fileInfo = list[i];

            query.prepare(QString("INSERT OR IGNORE INTO \"%1\" (name) VALUES (:name)").arg(DB_LEAGUES_TABLE(countryInfo.fileName())));
            query.bindValue(":name", fileInfo.fileName());
            query.exec();
        }

        delete dir;

    }

    delete main_dir;
    */

    query.exec("CREATE TABLE if not exists Countries (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                      "name VARCHAR(256) NOT NULL UNIQUE)");

    query.exec("CREATE TABLE if not exists CmdTable (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                      "cid INTEGER NOT NULL, "
                                                      "cmd VARCHAR(256) NOT NULL,"
                                                      "league VARCHAR(256) NOT NULL, "
                                                      "country VARCHAR(256) NOT NULL, "
                                                      "checked INTEGER NOT NULL)");
}

void ConfigWorker::setData(QString country, QString league, QVector<Command> cmds) {
    QSqlQuery query(sdb);
    QString request;// = "CREATE TABLE if not exists \"%1\" (id INTEGER NOT NULL, name VARCHAR(256) NOT NULL UNIQUE, checked INTEGER NOT NULL)";
    //query.exec(request.arg(DB_CMDS_TABLE(country, liga)));

    //request = "INSERT OR IGNORE INTO \"%1\" (id, name, checked) VALUES(:id, :name, :checked)";
    request = "INSERT OR IGNORE INTO CmdTable (cid, cmd, league, country, checked) VALUES(?, ?, ?, ?, ?)";
    /*
    for (int i = 0; i < cmds.size(); ++i) {
        query.prepare(request.arg(DB_CMDS_TABLE(country, liga)));
        query.bindValue(":cid", cmds[i].id);
        query.bindValue(":cmd", cmds[i].name);
        query.bindValue(":checked", 0);
        query.exec();
    }
    */

    query.prepare(request);

    QVariantList cids;
    QVariantList commands;
    QVariantList leagues;
    QVariantList countries;
    QVariantList checkeds;

    for (int i = 0; i < cmds.size(); ++i) {
          cids << cmds[i].id;
          commands << cmds[i].name;
          leagues << league;
          countries << country;
          checkeds << 0;
    }

    query.addBindValue(cids);
    query.addBindValue(commands);
    query.addBindValue(leagues);
    query.addBindValue(countries);
    query.addBindValue(checkeds);

    if (!query.execBatch())
        qDebug() << query.lastError();
}

void ConfigWorker::fillDB() {
    initDatabase();
    /*
    qDebug() << "initDB";

    TableWorker * tw = new TableWorker();
    QStringList countries = getContries();

    for (int i = 0; i < countries.size(); ++i) {
        QStringList leagues = useCountry(countries[i]);
        for (int j = 0; j < leagues.size(); ++j) {
            tw->open(xlsx_path + "/" + countries[i] + "/" + leagues[j]);
            QVector<Command> cmds = tw->getCommands();
            setData(countries[i], leagues[j], cmds);
            tw->closeXlsx();
        }
    }

    qDebug() << "initDB 2";

    delete tw;
    */

    TableWorker * tw = new TableWorker();
    QSqlQuery query(sdb);
    QString request = "INSERT OR IGNORE INTO CmdTable (cid, cmd, league, country, checked) VALUES(?, ?, ?, ?, ?)";

    query.prepare(request);

    QVariantList cids;
    QVariantList commands;
    QVariantList leagues;
    QVariantList countries;
    QVariantList checkeds;

    QVariantList countries_table;

    int count = 0;
    QDir * main_dir = new QDir(xlsx_path);
    main_dir->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    QStringList filters = {"*.xlsx"};

    QFileInfoList list = main_dir->entryInfoList();


    for (int i = 0; i < list.size(); ++i) {
        QFileInfo countryInfo = list[i];
        QDir * dir = new QDir(xlsx_path + "/" + countryInfo.fileName() + '/');
        dir->setFilter(QDir::Files | QDir::NoDotDot);
        dir->setNameFilters(filters);
        countries_table << countryInfo.fileName();
        QFileInfoList list_f = dir->entryInfoList();
        QVariantList leagues_table;
        query.exec(QString("CREATE TABLE if not exists \"%1\" (id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(256) NOT NULL UNIQUE)")
                   .arg(DB_LEAGUES_TABLE(countryInfo.fileName())));

        for (int j = 0; j < list_f.size(); ++j) {
            QFileInfo leaguesInfo = list_f[j];
            leagues_table << leaguesInfo.fileName();

            tw->open(xlsx_path + "/" + countryInfo.fileName() + '/' + leaguesInfo.fileName());
            QVector<Command> cmds = tw->getCommands();
            tw->closeXlsx();

            if (count < MAX_COUNT_CMD_IN_TABLE) {
               for (int k = 0; k < cmds.size(); ++k) {
                   cids << cmds[k].id;
                   commands << cmds[k].name;
                   leagues << leaguesInfo.fileName();
                   countries << countryInfo.fileName();
                   checkeds << 0;
               }
               count += cmds.size();
            }
            else {
                count = 0;
                j--;

                query.prepare("INSERT OR IGNORE INTO CmdTable (cid, cmd, league, country, checked) VALUES(?, ?, ?, ?, ?)");
                query.addBindValue(cids);
                query.addBindValue(commands);
                query.addBindValue(leagues);
                query.addBindValue(countries);
                query.addBindValue(checkeds);

                if (!query.execBatch())
                    qDebug() << query.lastError();

                cids.clear();
                commands.clear();
                leagues.clear();
                countries.clear();
                checkeds.clear();
            }
        }

        delete dir;
        query.prepare(QString("INSERT OR IGNORE INTO \"%1\" (name) VALUES(?)")
                      .arg(DB_LEAGUES_TABLE(countryInfo.fileName())));
        query.addBindValue(leagues_table);
        if (!query.execBatch())
            qDebug() << query.lastError();
    }

    if (cids.size() != 0) {
        query.prepare("INSERT OR IGNORE INTO CmdTable (cid, cmd, league, country, checked) VALUES(?, ?, ?, ?, ?)");
        query.addBindValue(cids);
        query.addBindValue(commands);
        query.addBindValue(leagues);
        query.addBindValue(countries);
        query.addBindValue(checkeds);
    }

    if (!query.execBatch())
        qDebug() << query.lastError();

    int res = query.prepare("INSERT OR IGNORE INTO Countries (name) VALUES(?)");
    query.addBindValue(countries_table);
    if (!query.execBatch())
        qDebug() << query.lastError() << res;

    delete main_dir;

}

void ConfigWorker::changeCmdState(QString country, QString league, QString cmd_name, bool isChecked) {
    QSqlQuery query(sdb);
    QString request = "UPDATE CmdTable SET checked = :checked WHERE cmd = :cmd AND country = :country AND league = :league";
    query.prepare(request);
    query.bindValue(":cmd", cmd_name);
    query.bindValue(":country", country);
    query.bindValue(":league", league);
    query.bindValue(":checked", isChecked);

    int res = query.exec();
    qDebug() << "changeCmdStates = " << res;
}

QStringList ConfigWorker::getContries() {
    QStringList res;

    QSqlQuery query(sdb);
    query.exec("SELECT * FROM Countries");
    while(query.next())
        res << query.value(1).toString();

    return res;
}

QStringList ConfigWorker::getLeagues(QString country) {
    //country_use = country;
    QStringList res;
    QSqlQuery query(sdb);
    query.exec("SELECT * FROM \"" + DB_LEAGUES_TABLE(country) + "\"");

    while(query.next())
        res << query.value(1).toString();

    return res;
}

QVector<CMDState> ConfigWorker::getCmds(QString country, QString league) {
   // liga_use = liga;

    QVector<CMDState> res;

    QSqlQuery query(sdb);
    int res1 = query.exec(QString("SELECT * FROM CmdTable WHERE country = \"%1\" AND league = \"%2\"").arg(country).arg(league));

    qDebug() << "getCmds res = " << res1;

    qDebug() << country << " " << league;

    while(query.next()) {
        CMDState cmds;
        cmds.cmd.id = query.value(1).toInt();
        cmds.cmd.name = query.value(2).toString();
        cmds.checked = query.value(5).toBool();
        qDebug() << cmds.cmd.id << " " << cmds.cmd.name << " " << cmds.checked;
        res.push_back(cmds);
    }

    return res;
}

void ConfigWorker::changeLigaState(QString country, QString league, bool state) {
    QSqlQuery query(sdb);
    int res = query.exec(QString("UPDATE CmdTable SET checked = ") + (state ? "1 ": "0 ") + QString("WHERE country = \"%1\" AND league = \"%2\"").arg(country).arg(league));
    qDebug() << "changeLigaState = " << res;
    qDebug() << query.lastError();
}

void ConfigWorker::changeAllCmdsState(bool state) {
    QSqlQuery query(sdb);
    int res = query.exec(QString("UPDATE CmdTable SET checked = ") + (state ? "1": "0"));
    qDebug() << "changeAllCmdsState = " << res;
}

int ConfigWorker::saveAnalizeResults(QList<ResAnalize> vled) {
    if (vled.size() == 0)
        return 1;

    sdb.close();
    QDateTime dt = QDateTime::currentDateTime();
    QString string_dt = dt.toString("dd_MM_yyyy_hh_mm_ss");
    string_dt = ANALIZE_RESULT_DIR "/" + string_dt + "." ANALIZE_RESULT_FILE_FORMAT;

    int res = 1;
    qDebug() << "FILEPAHT = " << string_dt;

    sdb.setDatabaseName(string_dt);
    if (!sdb.open())
           return 0;
    qDebug() << "CreateFile";
    QSqlQuery query(sdb);
    QString request = "CREATE TABLE if not exists \"AnalizeResults\" (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                                      "lid INTEGER NOT NULL, "
                                                                      "Criterion INTEGAR NOT NULL,"
                                                                      "CmdName VARCHAR(256) NOT NULL, "
                                                                      "League VARCHAR(256) NOT NULL, "
                                                                      "Country VARCHAR(256) NOT NULL, "
                                                                      "Percent INTEGER NOT NULL)";

    res = query.exec(request);
    qDebug() << "CreateTable";
    request = "INSERT INTO \"AnalizeResults\" (lid, Criterion, CmdName, League, Country, Percent) VALUES (?, ?, ?, ?, ?, ?)";
    query.prepare(request);

    QVariantList lid;
    QVariantList crts;
    QVariantList commands;
    QVariantList leagues;
    QVariantList countries;
    QVariantList percents;

    for (int i = 0; i < vled.size(); ++i) {
        for (int j = 0; j < vled[i].lled.size(); ++j) {
            lid << vled[i].lled[j].id;
            crts << vled[i].crt;
            commands << vled[i].lled[j].command;
            leagues << vled[i].lled[j].league;
            countries << vled[i].lled[j].country;
            percents << vled[i].lled[j].percent;
        }
    }
    qDebug() << "FINISH 1";

    query.addBindValue(lid);
    query.addBindValue(crts);
    query.addBindValue(commands);
    query.addBindValue(leagues);
    query.addBindValue(countries);
    query.addBindValue(percents);

    //request[request.size() - 1] = ';';
    qDebug() << "FINISH 2";
    //res = query.exec(request);
    if (!query.execBatch())
        qDebug() << query.lastError();
    qDebug() << "FINISH 3";
    sdb.close();
    qDebug() << "FINISH 4";
    openDB();
    qDebug() << "FINISH 5";

    return res;
}

QList<ResAnalize> ConfigWorker::loadAnalizeResults(QString path) {
    sdb.close();

    QList<ResAnalize> res;
    ResAnalize resa;

    sdb.setDatabaseName(path);
    if (!sdb.open())
           return res;

    QSqlQuery query("SELECT * FROM \"AnalizeResults\"");

    Criterions criterion;
    while(query.next()) {
        listElemetData led;
        led.id = query.value(1).toInt();
        criterion = intToCrt(query.value(2).toInt());
        led.command = query.value(3).toString();
        led.league = query.value(4).toString();
        led.country = query.value(5).toString();
        led.percent = query.value(6).toInt();

        if (resa.lled.size() == 0)
            resa.crt = criterion;
        if (resa.crt != criterion) {
            res.push_back(resa);
            resa.lled.clear();
            resa.crt = criterion;
        }

        resa.lled.push_back(led);

    }

    res.push_back(resa);
    sdb.close();
    openDB();

    return res;
}
