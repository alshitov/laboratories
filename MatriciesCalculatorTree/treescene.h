#ifndef TREESCENE_H
#define TREESCENE_H

#pragma once
#include <QtCore>
#include <QtWidgets>
#include <QtGui>
#include <iostream>
#include "matricescalculator.h"

struct pens
{
    QPen *red_p = new QPen(Qt::red, 2);
    QPen *white_p = new QPen(Qt::white, 2);
    QPen *blue_p = new QPen(Qt::blue, 2);
    QPen *yellow_p = new QPen(Qt::yellow, 2);
    QPen *green_p = new QPen(QColor(0, 128, 0), 3);
    QPen *branches_brown_p = new QPen(QColor(139, 69, 19), 3);
    QPen *trunk_bold_p = new QPen(QColor(139, 69, 19), 7);
    QPen *dim_p = new QPen(QColor(204, 204, 204), 0.5, Qt::DashLine);
};

struct brushes
{
    QBrush *red_b = new QBrush(Qt::red);
    QBrush *white_b = new QBrush(Qt::white);
    QBrush *blue_b = new QBrush(Qt::blue);
    QBrush *yellow_b = new QBrush(Qt::yellow);
    QBrush *green_b = new QBrush(QColor(130, 255, 0));
    QBrush *dim_b = new QBrush(QColor(204, 204, 204));
};


class TreeScene : public QGraphicsScene
{
    /* TreeScene Levels & Objects:
     * +---+------------------------+---------------------------------------------+----------------+
     * | # |         Name           |                    Objects                  |      Count     |
     * +---+------------------------+---------------------------------------------+----------------+
     * | 1 | Trunk                  | 1 Line                                      | 1              |
     * +---+------------------------+---------------------------------------------+----------------+
     * | 2 | First operands         | n Lines + n Circles                         | 2*n            |
     * +---+------------------------+---------------------------------------------+----------------+
     * | 3 | First operators        | max 6 Lines + max 6 Circles                 | max 12         |
     * +---+------------------------+---------------------------------------------+----------------+
     * | 4 | Second operands        | max (n-1) Lines + max (n-1) Circles         | max 2*(n-1)    |
     * +---+------------------------+---------------------------------------------+----------------+
     * | 5 | Second operators       | max 6*(n-1) Lines + max 6*(n-1) Circles     | max 12*(n-1)   |
     * +---+------------------------+---------------------------------------------+----------------+
     * | 6 | Nth operands           | max (n-N+1) Lines + max (n-N+1) Circles     | max 2(n-N+1)   |
     * +---+------------------------+---------------------------------------------+----------------+
     * | 5 | Nth operators          | max 6*(n-N+1) Lines + max 6*(n-N+1) Circles | max 12*(n-N+1) |
     * +---+------------------------+---------------------------------------------+----------------+
     * | 8 | Last operands          | max 1 Line + max 1 Circle                   | max 2          |
     * +---+------------------------+---------------------------------------------+----------------+
     *
     * Note: max means matrix may not have operators such as reverse or transpose
     * because of matrix arithmetic limitations, BUT: those objects will be
     * places onto the tree anyway and marked with red color as impossible to perform.
     */

    Q_OBJECT

private:
    qreal w, h;
    std::vector<minfo*> msinfo;
    int counter = 0;
    QGraphicsLineItem *trunk;
    QList<QGraphicsLineItem*> *grid;
    pens _pens;
    brushes _brushes;

    void make_grid(int rows, int cols);
    void make_tree(std::vector<minfo*> _msinfo);
    void make_branch(qreal x1, qreal y1, qreal x2, qreal y2);
    QGraphicsLineItem* make_line(QPen& p,
                                 qreal lx0, qreal lx1,
                                 qreal ly0, qreal ly1);
    QGraphicsEllipseItem *make_ellipse(QPen& p,
                                       QBrush& b,
                                       qreal diameter,
                                       qreal lx0, qreal ly0);
    void place_item(QGraphicsItem& item);

public:
    TreeScene();
    virtual ~TreeScene() {  }

    void set_msinfo(std::vector<minfo*> _msinfo);
    QGraphicsScene* get_scene();
    void set_counter(int _counter);
    void draw();

public slots:
    void view_scaled(int, int);

signals:
    void scene_scaled(int);
};

#endif // TREESCENE_H
