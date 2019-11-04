#ifndef TREESCENE_H
#define TREESCENE_H

#include <QtCore>
#include <QtWidgets>
#include <QtGui>

#include <matrixrepr.h>

struct pens
{
    QPen *red_p    = new QPen(Qt::red, 2);
    QPen *white_p  = new QPen(Qt::white, 2);
    QPen *blue_p   = new QPen(Qt::blue, 2);
    QPen *yellow_p = new QPen(Qt::yellow, 2);
    QPen *dim_p    = new QPen(QColor(204, 204, 204), 0.5, Qt::DashLine);
};

struct brushes
{
    QBrush *red_b    = new QBrush(Qt::red);
    QBrush *white_b  = new QBrush(Qt::white);
    QBrush *blue_b   = new QBrush(Qt::blue);
    QBrush *yellow_b = new QBrush(Qt::yellow);
    QBrush *dim_b    = new QBrush(QColor(204, 204, 204));
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
     *
     * QGraphicsEllipseItem    *ellipse;
     * QGraphicsSimpleTextItem *text;
     * QGraphicsPathItem       *path;
     * QGraphicsPolygonItem    *polygon;
     * QGraphicsRectItem       *rectangle;
     *
     */
    Q_OBJECT

private:
    qreal w, h;
    QGraphicsLineItem *trunk;
    QList<QGraphicsLineItem*> *grid;
    MatrixRepr *m_repr;
    pens _pens;
    brushes _brushes;

public:
    TreeScene();
    ~TreeScene();
    QGraphicsScene* get_scene();

    void make_grid(int rows, int cols);
    void make_tree();
    QGraphicsLineItem* make_line(QPen& p,
                                 qreal lx0, qreal lx1,
                                 qreal ly0, qreal ly1,
                                 qreal angle);
    QGraphicsEllipseItem *make_ellipse(QPen& p,
                                       QBrush& b,
                                       qreal diameter,
                                       qreal lx0, qreal ly0);
    void place_item(QGraphicsItem& item);

public slots:
    void view_scaled(int, int);

signals:
    void scene_scaled(int);
};

#endif // TREESCENE_H
