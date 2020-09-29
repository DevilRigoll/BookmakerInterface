#ifndef TABLEWORKER_H
#define TABLEWORKER_H

#include "types.h"
#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"

const char GREEN_COLOR = 43;
const char RED_COLOR = 22;
const char YELLOW_COLOR = 36;
const char BROWN_COLOR = 53;
const char MAIN_COLOR = 42;
const char HEADER_COLOR = 8;

#include <QList>

class TableWorker {
public:
    TableWorker();
    ~TableWorker();
    void open(QString file_path);
    void closeXlsx();
    QVector<Command> getCommands() const;
    QList<DataAnalize> genLDA(Command cmd, QDate dateCosntr = QDate(1900, 1, 1));
    void setDateConstr(QDate dConstr);
    int makeTable1vs1(Command first_command, Command second_command, int flags, QString file_path);

    static void visibleOpen(QString file_path, QString sheet_name);
private:
    DataAnalize parseToDA(QString ews, int row_id);
    TableSize makeTable1(int start_row, int start_col, QList<DataAnalize> first_cmd, QList<DataAnalize> second_cmd);
    TableSize makeTable2(int start_row, int start_col, QList<DataAnalize> first_cmd, QList<DataAnalize> second_cmd);
    TableSize makeTable3(int start_row, int start_col, QList<DataAnalize> first_cmd, QList<DataAnalize> second_cmd);

    QXlsx::Document * doc;
    QVector<Command> commands;
    QDate dateConstr;
    QXlsx::Document * tb_doc;

    QXlsx::Format * main_format;
    QXlsx::Format * header_format;
    QXlsx::Format * green_format;
    QXlsx::Format * red_format;
    QXlsx::Format * yellow_format;
    QXlsx::Format * brown_format;
};

#endif // TABLEWORKER_H
