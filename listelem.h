#ifndef LISTELEM_H
#define LISTELEM_H

#include <QWidget>
#include <QAction>
#include "types.h"

namespace Ui {
    class ResListElem;
}

class ListElem : public QWidget
{
    Q_OBJECT
friend class ListForm;
public:
    explicit ListElem(listElemetData led, QWidget * parent = nullptr);
    ~ListElem() override;

    int getPercent() const;
private:
    void createActions();
    QAction * moreInfoAct;

    Ui::ResListElem * ui;
    int index;
    //int percent;
signals:
    void menuActionClick(listElemetData led);
    void leftMBDClicked(listElemetData led);
private slots:
    void useMenuAct();
protected:
    void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
};

#endif // LISTELEM_H
