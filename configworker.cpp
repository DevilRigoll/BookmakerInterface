#include "configworker.h"
#include <QString>
#include <QFile>
#include <QDebug>
#include <stdio.h>
#include <iostream>
#include "tableworker.h"
#include "utils.h"

#if _WIN32 || _WIN64
    //#define _CRT_SECURE_NO_WARNINGS
#endif

ConfigWorker::ConfigWorker() {
    sdb = QSqlDatabase::addDatabase("QSQLITE");
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
    QSqlQuery query(sdb);
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

            query.prepare(QString("INSERT OR IGNORE INTO %1 (name) VALUES (:name)").arg(DB_LEAGUES_TABLE(countryInfo.fileName())));
            query.bindValue(":name", fileInfo.fileName());
            query.exec();
        }

        delete dir;

    }

    delete main_dir;
}

void ConfigWorker::setData(QString country, QString liga, QVector<Command> cmds) {
    QSqlQuery query(sdb);
    QString request = "CREATE TABLE if not exists \"%1\" (id INTEGER NOT NULL, name VARCHAR(256) NOT NULL UNIQUE, checked INTEGER NOT NULL)";
    query.exec(request.arg(DB_CMDS_TABLE(country, liga)));

    request = "INSERT OR IGNORE INTO \"%1\" (id, name, checked) VALUES(:id, :name, :checked)";
    for (int i = 0; i < cmds.size(); ++i) {
        query.prepare(request.arg(DB_CMDS_TABLE(country, liga)));
        query.bindValue(":id", cmds[i].id);
        query.bindValue(":name", cmds[i].name);
        query.bindValue(":checked", 0);
        query.exec();
    }
}

void ConfigWorker::fillDB() {
    initDatabase();

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
}

void ConfigWorker::changeCmdState(QString cmd_name, bool isChecked) {
    QSqlQuery query(sdb);
    QString request = "UPDATE \"%1\" SET checked = :checked WHERE name = :name";
    query.prepare(request.arg(DB_CMDS_TABLE(country_use, liga_use)));
    query.bindValue(":name", cmd_name);
    query.bindValue(":checked", isChecked);

    query.exec();
}

void ConfigWorker::saveAnalizeResult(ResAnalize res) {
    QSqlQuery query(sdb);
    QString request = "DROP TABLE if exists \"%1\"";
    query.exec(request.arg("Criterion" + QString(res.criterion)));
    request = "CREATE TABLE if not exists \"%1\" (id INTEGER NOT NULL, name VARCHAR(256) NOT NULL)";
    query.exec(request.arg("Criterion" + QString(res.criterion)));
    request = "INSERT INTO \"%1\" (id, name) VALUES(%2, \"%3\")";
    for (int i = 0; i < res.cmds.size(); ++i)
        query.exec(request
                   .arg(DB_TABLE_RESULTS(res.criterion))
                   .arg(res.cmds[i].id)
                   .arg(res.cmds[i].name));
}

ResAnalize ConfigWorker::loadAnalizeResult(Criterions criterion) {
    ResAnalize res;
    res.criterion = criterion;
    QString req = "SELECT * FROM \"%1\"";
    QSqlQuery query(req.arg(DB_TABLE_RESULTS(criterion)));
    while(query.next()) {
        Command cmd = {query.value(1).toInt(), query.value(2).toString()};
        res.cmds.push_back(cmd);
    }

    return res;
}

QStringList ConfigWorker::getContries() {
    QStringList res;

    QSqlQuery query(sdb);
    query.exec("SELECT * FROM Countries");
    while(query.next())
        res << query.value(1).toString();

    return res;
}

QStringList ConfigWorker::useCountry(QString country) {
    country_use = country;
    QStringList res;
    QSqlQuery query(sdb);
    query.exec("SELECT * FROM \"" + DB_LEAGUES_TABLE(country) + "\"");

    while(query.next())
        res << query.value(1).toString();

    return res;
}

QVector<CMDState> ConfigWorker::useLiga(QString liga) {
    liga_use = liga;

    QVector<CMDState> res;

    QSqlQuery query("SELECT * FROM \"" + DB_CMDS_TABLE(country_use, liga) + "\"");

    while(query.next()) {
        CMDState cmds;
        cmds.cmd.id = query.value(0).toInt();
        cmds.cmd.name = query.value(1).toString();
        cmds.checked = query.value(2).toBool();
        res.push_back(cmds);
    }

    return res;
}
