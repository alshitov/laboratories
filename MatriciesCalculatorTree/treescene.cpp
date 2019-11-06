#include "treescene.h"

#define GRID_VERT 7
#define OPERATORS 6
#define TRUNK_H 50
#define BRANCH_H 60
#define MATRIX_CIRLCE_D 35
#define OPERATOR_CIRCLE_D 30

TreeScene::TreeScene()
{

}

TreeScene::~TreeScene()
{

}

void TreeScene::set_msinfo(std::vector<minfo*> _msinfo)
{
    msinfo = _msinfo;
    counter = int(msinfo.size());
    draw();
}

void TreeScene::draw()
{
    clear();
    if (counter > 0)
    {
        make_grid(GRID_VERT, counter);
        make_tree(msinfo);
    }
}

void TreeScene::view_scaled(int _w, int _h)
{
    w = _w; h = _h;
    setSceneRect(0, 0, w, h);
    draw();
}

void TreeScene::make_grid(int rows, int cols)
{
    qreal container_w = w / cols,   // 128.571
          container_h = h / rows;   // 200

    // Vertical lines
    for (int i = 1; i < cols; ++i)
    {
        QGraphicsLineItem *vl = make_line(
            *_pens.dim_p,
            container_w * i, -h,
            container_w * i, h
        );
        place_item(*vl);
    }

    // Horizontal lines
    for (int j = 1; j < rows; ++j)
    {
        QGraphicsLineItem *hl = make_line(
            *_pens.dim_p,
            -w, container_h * j,
            w, container_h * j
        );
        place_item(*hl);
    }
}

void TreeScene::make_tree(std::vector<minfo*> _msinfo)
{
    if (_msinfo.size() == 0) return;

    int i, j, k;
    qreal matrix_container_w = (w / counter),
          matrix_container_h = h / GRID_VERT,
          branch_start_x,
          branch_start_y,
          branch_end_x,
          branch_end_y;
    // Trunk
    trunk = make_line(*_pens.trunk_bold_p, w/2, h, w/2, h - TRUNK_H);
    place_item(*trunk);

    for (i = 0; i < counter; ++i)
    {
        // Full scene on center
        if (counter == 1)
            branch_end_x = w / 2;
        // Two branches spaced by half screen
        else if (counter == 2)
            branch_end_x = (w / 4) + (i * (w / 2));
        else
            branch_end_x = (w / 2) + (i - (counter / 2.0f - 0.5f)) * (w / counter);

        branch_start_x = w / 2;
        branch_start_y = h - TRUNK_H;
        branch_end_y = h - (TRUNK_H + BRANCH_H);

        QGraphicsLineItem *branch = make_line(
            *_pens.branches_brown_p,
            branch_start_x, branch_start_y, branch_end_x, branch_end_y
        );
        place_item(*branch);

        // Ellipses
        qreal ellipse_center_x = branch_end_x - (MATRIX_CIRLCE_D / 2),
              elliplse_center_y = branch_end_y - (MATRIX_CIRLCE_D / 2);

        QGraphicsEllipseItem *ellipse = make_ellipse(
            *_pens.green_p, *_brushes.green_b,
            MATRIX_CIRLCE_D, ellipse_center_x, elliplse_center_y
        );
        place_item(*ellipse);

        // Matrix name annotation
        QGraphicsTextItem *text = new QGraphicsTextItem("A");
        text->setX(ellipse_center_x);
        text->setY(elliplse_center_y);
        place_item(*text);

        // Secondary branches
        qreal operator_container_w = matrix_container_w / 6,
              operator_container_h = matrix_container_h;
        for (j = 0; j < OPERATORS; ++j)
        {
            qreal x2 = (j * operator_container_w)
                     + (operator_container_w / 2)
                     + (i * matrix_container_w),
                  y2 = (j * operator_container_h) + (operator_container_h / 2);

            QGraphicsLineItem *sec_branch = make_line(
                *_pens.yellow_p,
                branch_end_x, branch_end_y, x2, y2
            );
            place_item(*sec_branch);

            // Ellipses
            qreal operator_ellipse_center_x = x2 - (OPERATOR_CIRCLE_D / 2),
                  operator_elliplse_center_y = y2 - (OPERATOR_CIRCLE_D / 2);

            QGraphicsEllipseItem *operator_ellipse = make_ellipse(
                *_pens.green_p, *_brushes.green_b,
                OPERATOR_CIRCLE_D, operator_ellipse_center_x, operator_elliplse_center_y
            );
            place_item(*operator_ellipse);
        }
    }
}

QGraphicsLineItem* TreeScene::make_line(QPen& p,
                                        qreal x1, qreal y1,
                                        qreal x2, qreal y2)
{
    QGraphicsLineItem *line = new QGraphicsLineItem();
    line->setPen(p);
    line->setLine(x1, y1, x2, y2);
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
