#include "treescene.h"
#include <iostream>

TreeScene::TreeScene()
{
    scene = new QGraphicsScene();
    scene->setSceneRect(-900, -600, 900, 600);
    make_grid();
//    make_tree();
}

TreeScene::~TreeScene()
{
    delete scene;
    delete grid;
    delete trunk;
    delete m_repr;
}

QGraphicsScene* TreeScene::get_scene()
{
    return scene;
}

void TreeScene::view_scaled(int w, int h)
{
    std::cout << "Scale event! : " << w << h;
}

void TreeScene::make_grid()
{
    // n main sections for each matrix
    int n = 3;
    qreal w = scene->width(),
          h = scene->height();

    qreal container_w = w / n,
          container_h = h;

    std::cout << "w: " << w << std::endl;
    std::cout << "h: " << h << std::endl;
    std::cout << "cw: " << container_w << std::endl;
    std::cout << "ch: " << container_h << std::endl;


    for (int i = 1; i < n; ++i)
    {
        QGraphicsLineItem *ll = make_line(*_pens.dim_p,
                                          -w + container_w * i,
                                          -w + container_w * i,
                                          -h, h, 0);
        place_item(*ll);
    }

//    QLineF ll(scene->sceneRect().topLeft(), scene->sceneRect().bottomRight());

//    scene->addLine(ll, *_pens.dim_p);

//    for (int i = 0; i < n; ++i)
//    {
//        place_item(*make_line(*_pens.dim_p, 0, 100, 0, 100, 0) ,0, 0);
//    }

}

void TreeScene::make_tree()
{
    trunk = make_line(*_pens.white_p, 0, 0, 0, 100, 0);
    place_item(*trunk);
}

QGraphicsLineItem* TreeScene::make_line(QPen& p,
                                        qreal lx0, qreal ly0,
                                        qreal lx1, qreal ly1,
                                        qreal angle)
{
    // Standard line 1px wide
    QGraphicsLineItem *line = new QGraphicsLineItem();
    line->setPen(p);
    line->setLine(lx0, lx1, ly0, ly1);
    line->setRotation(angle);
    return line;
}

void TreeScene::set_line_settings(QGraphicsLineItem& line,
                                  QPen& p, Qt::PenStyle& p_style,
                                  int line_w)
{

}

void TreeScene::place_item(QGraphicsItem& item)
{
    scene->addItem(&item);
}

void TreeScene::remove_item(QGraphicsItem& item)
{
    scene->removeItem(&item);
}
