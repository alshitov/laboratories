#include "treescene.h"
#include <iostream>

TreeScene::TreeScene()
{
    w = 900; h = 600;
    setSceneRect(-w, -h, w, h);
    make_grid(7, 3);
    make_tree();
}

TreeScene::~TreeScene()
{
    delete grid;
    delete trunk;
    delete m_repr;
}

void TreeScene::view_scaled(int _w, int _h)
{
    w = _w; h = _h;
    setSceneRect(-w, -h, w, h);
    clear();
    make_grid(7, 3);
    make_tree();
}

void TreeScene::make_grid(int rows, int cols)
{
    qreal w = width(),       // 900
          h = height();      // 600

    qreal container_w = w / cols,   // 128.571
          container_h = h / rows;   // 200

    // Vertical lines
    for (int i = 1; i < cols; ++i)
    {
        QGraphicsLineItem *vl = make_line(*_pens.dim_p,
                                          -w + container_w * i,
                                          -w + container_w * i,
                                          -h, h, 0);
        place_item(*vl);
    }

    // Horizontal lines
    for (int j = 1; j < rows; ++j)
    {
        QGraphicsLineItem *hl = make_line(*_pens.dim_p,
                                          -w, w,
                                          -h + container_h * j,
                                          -h + container_h * j,
                                          0);
        place_item(*hl);
    }
}

void TreeScene::make_tree()
{
    trunk = make_line(*_pens.white_p, 200, 200, 100, 100, 0);
    place_item(*trunk);
    QGraphicsEllipseItem *el = make_ellipse(*_pens.red_p, *_brushes.white_b, 10, 100, 100);
    place_item(*el);
}

QGraphicsLineItem* TreeScene::make_line(QPen& p,
                                        qreal lx0, qreal ly0,
                                        qreal lx1, qreal ly1,
                                        qreal angle)
{
    QGraphicsLineItem *line = new QGraphicsLineItem();
    line->setPen(p);
    line->setLine(lx0, lx1, ly0, ly1);
    line->setRotation(angle);
    return line;
}

QGraphicsEllipseItem* TreeScene::make_ellipse(QPen& p,
                                              QBrush& b,
                                              qreal diameter,
                                              qreal lx0, qreal ly0)
{
    QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem();
    ellipse->setPen(p);
    ellipse->setBrush(b);
    ellipse->setRect(QRectF(lx0, ly0, diameter, diameter));
    ellipse->setStartAngle(0);
    ellipse->setStartAngle(360);
    return ellipse;
}

void TreeScene::place_item(QGraphicsItem& item)
{
    addItem(&item);
}
