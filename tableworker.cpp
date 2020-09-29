#include "tableworker.h"

#include <QStringList>
#include <QDebug>
#include <QVariant>

TableWorker::TableWorker() {
    dateConstr = QDate(1900, 1, 1);
}

TableWorker::~TableWorker() {
    if (doc != nullptr)
        delete doc;

    doc = nullptr;
}

void TableWorker::open(QString file_path) {
    commands.clear();
    doc = new QXlsx::Document(file_path);
    if (!doc->load()) exit(1);
    QStringList sl = doc->sheetNames();
    sl.removeOne("Анализ");
    int sheetIndexNumber = 1;
    foreach(QString name, sl) {
        Command cmd = {sheetIndexNumber, name};
        commands.push_back(cmd);
        sheetIndexNumber++;
    }

}

void TableWorker::visibleOpen(QString file_path, QString sheet_name) {
    QXlsx::Document _doc(file_path);
    QXlsx::AbstractSheet * _sheet = _doc.sheet(sheet_name);
    _sheet->setVisible(true);
}

void TableWorker::closeXlsx() {
    if (doc != nullptr)
        delete doc;

    doc = nullptr;
}

QVector<Command> TableWorker::getCommands() const { return commands; }

QList<DataAnalize> TableWorker::genLDA(Command cmd, QDate dateConstr) {

    QXlsx::Worksheet * wsheet;
    QXlsx::AbstractSheet * currentSheet;

    int lastRow = -1;
    int lastColumn = -1;
    currentSheet = doc->sheet( cmd.name );
    currentSheet->workbook()->setActiveSheet( cmd.id );
    wsheet = (QXlsx::Worksheet *) currentSheet->workbook()->activeSheet();
    QVector<QXlsx::CellLocation> clList = wsheet->getFullCells( &lastRow, &lastColumn );

    QList<DataAnalize> LDA;
    for (int i = 4; i <= lastRow; ++i) {
        DataAnalize da = parseToDA(cmd.name, i);
        if (da.date >= dateConstr)
            LDA.push_back(da);
    }

    return LDA;
}

void TableWorker::setDateConstr(QDate dConstr) { dateConstr = dConstr; }

DataAnalize TableWorker::parseToDA(QString ews, int row_id) {
    DataAnalize da;

    doc->selectSheet(ews);
    QString yach;

    QVariant var = doc->read( row_id, 2 );
    da.id = var.toInt();

    var = doc->read( row_id, 3 );
    yach = var.toString();
    QStringList yachs = yach.split('.');
    da.date = QDate(yachs[2].toInt(), yachs[1].toInt(), yachs[0].toInt());

    var = doc->read( row_id, 4 );
    da.enemy_name = var.toString();

    var = doc->read( row_id, 5 );
    yach = var.toString();
    if (yach.size() != 0) {
        yachs = yach.split(';');
        foreach (QString word, yachs)
            if (word.size() != 0)
                da.goal_time1.push_back(word.toInt());
    }


    var = doc->read( row_id, 6 );
    yach = var.toString();
    if (yach.size() != 0) {
        yachs = yach.split(';');
        foreach (QString word, yachs)
            if (word.size() != 0)
                da.goal_time2.push_back(word.toInt());
    }

    var = doc->read( row_id, 7 );
    yach = var.toString();
    if (yach.size() != 0) {
        yachs = yach.split(';');
        foreach (QString word, yachs)
            if (word.size() != 0)
                da.goal_time3.push_back(word.toInt());
    }

    var = doc->read( row_id, 9 );
    da.place = var.toString();

    var = doc->read( row_id, 18);
    yach = var.toString();
    if (yach.size() != 0) {
        yachs = yach.split(';');
        foreach (QString word, yachs)
            if (word.size() != 0)
                da.y_card_time1.push_back(word.toInt());
    }

    var = doc->read( row_id, 19);
    yach = var.toString();
    if (yach.size() != 0) {
        yachs = yach.split(';');
        foreach (QString word, yachs)
            if (word.size() != 0)
                da.y_card_time2.push_back(word.toInt());
    }

    var = doc->read( row_id, 21);
    da.corner_count_time1 = var.toInt();

    var = doc->read( row_id, 22);
    da.corner_count_time2 = var.toInt();

    return da;
}

int TableWorker::makeTable1vs1(Command first_command, Command second_command, int flags, QString file_path) {

    tb_doc = new QXlsx::Document();

    main_format = new QXlsx::Format();
    green_format = new QXlsx::Format();
    red_format = new QXlsx::Format();
    yellow_format = new QXlsx::Format();
    brown_format = new QXlsx::Format();
    header_format = new QXlsx::Format();


    main_format->setBorderStyle(QXlsx::Format::BorderHair);
    main_format->setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    main_format->setPatternBackgroundColor(QColor(40,100,200));
    main_format->setFontSize(15);

    header_format->mergeFormat(*main_format);
    green_format->mergeFormat(*main_format);
    red_format->mergeFormat(*main_format);
    yellow_format->mergeFormat(*main_format);
    brown_format->mergeFormat(*main_format);

    header_format->setPatternBackgroundColor(QColor(30,100,170));
    header_format->setFontBold(true);

    green_format->setPatternBackgroundColor(QColor(0,150,0));
    red_format->setPatternBackgroundColor(QColor(150,0,0));
    yellow_format->setPatternBackgroundColor(QColor(250, 240, 70));
    brown_format->setPatternBackgroundColor(QColor(100,50,20));

    QVector<QXlsx::CellLocation> clList;
    QXlsx::Worksheet * wsheet;
    QXlsx::AbstractSheet * currentSheet;

    int lastRow1 = -1; // подсчет количества строк на листе id
    int lastColumn1 = -1; // подсчет количества столбцов на листе id

    int lastRow2 = -1; // подсчет количества строк на листе id
    int lastColumn2 = -1; // подсчет количества столбцов на листе id

    currentSheet = doc->sheet( first_command.name );
    currentSheet->workbook()->setActiveSheet( first_command.id );
    wsheet = (QXlsx::Worksheet *) currentSheet->workbook()->activeSheet();
    clList = wsheet->getFullCells( &lastRow1, &lastColumn1 );

    currentSheet = doc->sheet( second_command.name );
    currentSheet->workbook()->setActiveSheet( second_command.id );
    wsheet = (QXlsx::Worksheet *) currentSheet->workbook()->activeSheet();
    clList = wsheet->getFullCells( &lastRow2, &lastColumn2 );

    //qDebug() << "LR1 = " << lastRow1 << " LC1 = " << lastColumn1 << "\n";
    //qDebug() << "LR2 = " << lastRow2 << " LC1 = " << lastColumn2 << "\n";

    QList<DataAnalize> LDA1;
    QList<DataAnalize> LDA2;

    DataAnalize da;

    //qDebug() << first_command.name;
    //qDebug() << second_command.name;

    for (int i = 4; i <= lastRow1; ++i) {
        da = parseToDA(first_command.name, i);
        if (da.date >= dateConstr)
            if (da.enemy_name == second_command.name)
                LDA1.push_back(da);
    }

    for (int i = 4; i <= lastRow2; ++i) {
        da = parseToDA(second_command.name, i);
        if (da.date >= dateConstr) {
            if (da.enemy_name == first_command.name)
                LDA2.push_back(da);
        }
    }

    if (LDA1.size() != LDA2.size())
        return -1;

    TableSize pos;
    pos.w = 1;
    pos.h = 1;

    if ((flags & TablesFlags::GOALS) == TablesFlags::GOALS) {
        TableSize ts = makeTable1(pos.h, pos.w, LDA1, LDA2);
        pos.w += ts.w + 1;
    }

    //qDebug() << "third";
    if ((flags & TablesFlags::YELLOW_CARDS) == TablesFlags::YELLOW_CARDS) {
        TableSize ts = makeTable2(pos.h, pos.w, LDA1, LDA2);
        pos.w += ts.w + 1;
    }

    //qDebug() << "fourth";
    if ((flags & TablesFlags::CORNERS) == TablesFlags::CORNERS) {
        TableSize ts = makeTable3(pos.h, pos.w, LDA1, LDA2);
        pos.w += ts.w + 1;
    }

    //qDebug() << "fiveth";
    tb_doc->saveAs(file_path);

    delete main_format;
    delete green_format;
    delete red_format;
    delete yellow_format;
    delete brown_format;
    delete header_format;

    delete tb_doc;

    return 0;
}

TableSize TableWorker::makeTable1(int start_row, int start_col, QList<DataAnalize> first_cmd, QList<DataAnalize> second_cmd) {
    TableSize ts;
    ts.w = 11;
    ts.h = first_cmd.size();

    for (int i = 0; i < ts.w; ++i) {
        for (int j = 0; j <= ts.h; ++j) {
            tb_doc->write(start_row + j, start_col + i, "", *main_format);
        }
    }

    //qDebug() << "first 1";

    //qDebug() << first_cmd.size() << " " << second_cmd.size() << " " << ts.h;

    QXlsx::CellRange cl1(start_row, start_col + 2, start_row, start_col + 4);
    tb_doc->write(start_row, start_col + 2, second_cmd[0].enemy_name);
    tb_doc->mergeCells(cl1, *header_format);
    QXlsx::CellRange cl2(start_row, start_col + 7, start_row, start_col + 9);
    tb_doc->write(start_row, start_col + 7, first_cmd[0].enemy_name);
    tb_doc->mergeCells(cl2, *header_format);

    for (int i = 0; i < ts.h; ++i) {
        tb_doc->write(start_row + 1 + i, start_col, QVariant(i + 1));
        tb_doc->write(start_row + 1 + i, start_col + 1, first_cmd[i].date.toString("dd.MM.yyyy"));


        //goals first cmd
        if (first_cmd[i].goal_time1.size() != 0) {
            QString str = "";
            for (int j = 0; j < first_cmd[i].goal_time1.size() - 1; ++j)
                str += QString::number(first_cmd[i].goal_time1[j]) + ";";
            str += QString::number(first_cmd[i].goal_time1[first_cmd[i].goal_time1.size() - 1]);
            tb_doc->write(start_row + 1 + i, start_col + 2, str, *green_format);
        }
        else
            tb_doc->write(start_row + 1 + i, start_col + 2, "", *red_format);


        if (first_cmd[i].goal_time3.size() != 0) {
            QString str = "";
            for (int j = 0; j < first_cmd[i].goal_time3.size() - 1; ++j)
                str += QString::number(first_cmd[i].goal_time3[j]) + ";";
            str += QString::number(first_cmd[i].goal_time3[first_cmd[i].goal_time3.size() - 1]);
            tb_doc->write(start_row + 1 + i, start_col + 4, str, *green_format);
            tb_doc->write(start_row + 1 + i, start_col + 3, str, *green_format);
        }
        else {
            tb_doc->write(start_row + 1 + i, start_col + 4, "", *red_format);
            tb_doc->write(start_row + 1 + i, start_col + 3, "", *red_format);
        }

        if (first_cmd[i].goal_time2.size() != 0) {
            QString str = "";
            for (int j = 0; j < first_cmd[i].goal_time2.size() - 1; ++j)
                str += QString::number(first_cmd[i].goal_time2[j]) + ";";
            str += QString::number(first_cmd[i].goal_time2[first_cmd[i].goal_time2.size() - 1]);
            tb_doc->write(start_row + 1 + i, start_col + 3, str, *green_format);
        }

        tb_doc->write(start_row + 1 + i, start_col + 6, first_cmd[i].place, *header_format);
        QString goals = "%1:%2";
        if (first_cmd[i].place == "Г")
            goals = goals.arg(second_cmd[i].goal_time1.size() + second_cmd[i].goal_time2.size() + second_cmd[i].goal_time3.size())
                 .arg(first_cmd[i].goal_time1.size() + first_cmd[i].goal_time2.size() + first_cmd[i].goal_time3.size());
        else
            goals = goals.arg(first_cmd[i].goal_time1.size() + first_cmd[i].goal_time2.size() + first_cmd[i].goal_time3.size())
                 .arg(second_cmd[i].goal_time1.size() + second_cmd[i].goal_time2.size() + second_cmd[i].goal_time3.size());

        tb_doc->write(start_row + 1 + i, start_col + 5, goals, *header_format);

        //Second command
        if (second_cmd[i].goal_time1.size() != 0) {
            QString str = "";
            for (int j = 0; j < second_cmd[i].goal_time1.size() - 1; ++j)
                str += QString::number(second_cmd[i].goal_time1[j]) + ";";
            str += QString::number(second_cmd[i].goal_time1[second_cmd[i].goal_time1.size() - 1]);
            tb_doc->write(start_row + 1 + i, start_col + 7, str, *green_format);
        }
        else
            tb_doc->write(start_row + 1 + i, start_col + 7, "", *red_format);

        if (second_cmd[i].goal_time3.size() != 0) {
            QString str = "";
            for (int j = 0; j < second_cmd[i].goal_time3.size() - 1; ++j)
                str += QString::number(second_cmd[i].goal_time3[j]) + ";";
            str += QString::number(second_cmd[i].goal_time3[second_cmd[i].goal_time3.size() - 1]);
            tb_doc->write(start_row + 1 + i, start_col + 9, str, *green_format);
            tb_doc->write(start_row + 1 + i, start_col + 8, str, *green_format);
        }
        else {
            tb_doc->write(start_row + 1 + i, start_col + 9, "", *red_format);
            tb_doc->write(start_row + 1 + i, start_col + 8, "", *red_format);
        }

        if (second_cmd[i].goal_time2.size() != 0) {
            QString str = "";
            for (int j = 0; j < second_cmd[i].goal_time2.size() - 1; ++j)
                str += QString::number(second_cmd[i].goal_time2[j]) + ";";
            str += QString::number(second_cmd[i].goal_time2[second_cmd[i].goal_time2.size() - 1]);
            tb_doc->write(start_row + 1 + i, start_col + 8, str, *green_format);
        }
    }

    return ts;
}

TableSize TableWorker::makeTable2(int start_row, int start_col, QList<DataAnalize> first_cmd, QList<DataAnalize> second_cmd) {
    TableSize ts;
    ts.w = 10;
    ts.h = first_cmd.size();

    for (int i = 0; i < ts.w; ++i) {
        for (int j = 0; j <= ts.h; ++j) {
            tb_doc->write(start_row + j, start_col + i, "", *main_format);
        }
    }

    QXlsx::CellRange cl1(start_row, start_col + 2, start_row, start_col + 4);
    tb_doc->write(start_row, start_col + 2, second_cmd[0].enemy_name);
    tb_doc->mergeCells(cl1, *header_format);
    QXlsx::CellRange cl2(start_row, start_col + 5, start_row, start_col + 7);
    tb_doc->write(start_row, start_col + 5, first_cmd[0].enemy_name);
    tb_doc->mergeCells(cl2, *header_format);

     for (int i = 0; i < ts.h; ++i) {
        tb_doc->write(start_row + 1 + i, start_col, QVariant(i + 1));
        tb_doc->write(start_row + 1 + i, start_col + 1, first_cmd[i].date.toString("dd.MM.yyyy"));

        if (first_cmd[i].y_card_time1.size() == 0)
            tb_doc->write(start_row + 1 + i, start_col + 2, "", *red_format);
        else {
            QString str;
            for (int k = 0; k < first_cmd[i].y_card_time1.size() - 1; ++k)
                str += QString::number(first_cmd[i].y_card_time1[k]) + ";";
            str += QString::number(first_cmd[i].y_card_time1[first_cmd[i].y_card_time1.size() - 1]);
            tb_doc->write(start_row + 1 + i, start_col + 2, str, *yellow_format);
        }

        if (first_cmd[i].y_card_time2.size() == 0)
            tb_doc->write(start_row + 1 + i, start_col + 3, "", *red_format);
        else {
            QString str;
            for (int k = 0; k < first_cmd[i].y_card_time2.size() - 1; ++k)
                str += QString::number(first_cmd[i].y_card_time2[k]) + ";";
            str += QString::number(first_cmd[i].y_card_time2[first_cmd[i].y_card_time2.size() - 1]);
            tb_doc->write(start_row + 1 + i, start_col + 3, str, *yellow_format);
        }

        tb_doc->write(start_row + 1 + i, start_col + 4, QString::number(first_cmd[i].y_card_time2.size() + first_cmd[i].y_card_time1.size()), *green_format);

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        if (second_cmd[i].y_card_time1.size() == 0)
            tb_doc->write(start_row + 1 + i, start_col + 5, "", *red_format);
        else {
            QString str;
            for (int k = 0; k < second_cmd[i].y_card_time1.size() - 1; ++k)
                str += QString::number(second_cmd[i].y_card_time1[k]) + ";";
            str += QString::number(second_cmd[i].y_card_time1[second_cmd[i].y_card_time1.size() - 1]);
            tb_doc->write(start_row + 1 + i, start_col + 5, str, *yellow_format);
        }

        if (second_cmd[i].y_card_time2.size() == 0)
            tb_doc->write(start_row + 1 + i, start_col + 6, "", *red_format);
        else {
            QString str;
            for (int k = 0; k < second_cmd[i].y_card_time2.size() - 1; ++k)
                str += QString::number(second_cmd[i].y_card_time2[k]) + ";";
            str += QString::number(second_cmd[i].y_card_time2[second_cmd[i].y_card_time2.size() - 1]);
            tb_doc->write(start_row + 1 + i, start_col + 6, str, *yellow_format);
        }

        tb_doc->write(start_row + 1 + i, start_col + 7, QString::number(second_cmd[i].y_card_time2.size() + second_cmd[i].y_card_time1.size()), *green_format);

        //////////////////////////////////////////////////////////////////////////

        tb_doc->write(start_row + 1 + i, start_col + 8, QString::number(second_cmd[i].y_card_time2.size() + second_cmd[i].y_card_time1.size()
                                                               + first_cmd[i].y_card_time2.size() + first_cmd[i].y_card_time1.size()), *green_format);
     }

    return ts;
}

TableSize TableWorker::makeTable3(int start_row, int start_col, QList<DataAnalize> first_cmd, QList<DataAnalize> second_cmd) {
    TableSize ts;
    ts.w = 10;
    ts.h = first_cmd.size();

    for (int i = 0; i < ts.w; ++i) {
        for (int j = 0; j <= ts.h; ++j) {
            tb_doc->write(start_row + j, start_col + i, "", *main_format);
        }
    }

    QXlsx::CellRange cl1(start_row, start_col + 2, start_row, start_col + 4);
    tb_doc->write(start_row, start_col + 2, second_cmd[0].enemy_name);
    tb_doc->mergeCells(cl1, *header_format);
    QXlsx::CellRange cl2(start_row, start_col + 5, start_row, start_col + 7);
    tb_doc->write(start_row, start_col + 5, first_cmd[0].enemy_name);
    tb_doc->mergeCells(cl2, *header_format);

    for (int i = 0; i < ts.h; ++i) {
        tb_doc->write(start_row + 1 + i, start_col, QVariant(i + 1));
        tb_doc->write(start_row + 1 + i, start_col + 1, first_cmd[i].date.toString("dd.MM.yyyy"));

        if (first_cmd[i].corner_count_time1 == 0)
            tb_doc->write(start_row + 1 + i, start_col + 2, "", *red_format);
        else
            tb_doc->write(start_row + 1 + i, start_col + 2, first_cmd[i].corner_count_time1, *brown_format);

        if (first_cmd[i].corner_count_time2 == 0)
            tb_doc->write(start_row + 1 + i, start_col + 3, "", *red_format);
        else
            tb_doc->write(start_row + 1 + i, start_col + 3, first_cmd[i].corner_count_time2, *brown_format);

        tb_doc->write(start_row + 1 + i, start_col + 4, first_cmd[i].corner_count_time2 + first_cmd[i].corner_count_time1, *green_format);

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        if (second_cmd[i].corner_count_time1 == 0)
             tb_doc->write(start_row + 1 + i, start_col + 5, "", *red_format);
        else
            tb_doc->write(start_row + 1 + i, start_col + 5, second_cmd[i].corner_count_time1, *brown_format);

        if (second_cmd[i].corner_count_time2 == 0)
            tb_doc->write(start_row + 1 + i, start_col + 6, "", *red_format);
        else
            tb_doc->write(start_row + 1 + i, start_col + 6, second_cmd[i].corner_count_time2, *brown_format);

        tb_doc->write(start_row + 1 + i, start_col + 7, second_cmd[i].corner_count_time2 + second_cmd[i].corner_count_time1, *green_format);

        /////////////////////////////////////////////////////////////////////////////////////////////

        tb_doc->write(start_row + 1 + i, start_col + 7, second_cmd[i].corner_count_time2 + second_cmd[i].corner_count_time1
                     + first_cmd[i].corner_count_time2 + first_cmd[i].corner_count_time1, *green_format);
    }

    return ts;
}





















