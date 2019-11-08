#include "treescene.h"

#define GRID_VERT 7
#define OPERATORS 6
#define TRUNK_H 50
#define BRANCH_H 60
#define MATRIX_CIRLCE_D 35
#define OPERATOR_CIRCLE_D 30

TreeScene::TreeScene()
{
    operators = new QStringList { "Sum", "Sub", "Div", "Mul", "Tns", "Rev" };
    operators_map = new QMap<QString, int>();
    for (int i = 0; i < operators->size(); ++i)
        operators_map->insert(operators->at(i), i);
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

    if (counter < 1) return;

    make_grid(GRID_VERT, counter);
    make_tree();
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

void TreeScene::make_tree()
{
    qreal matrix_container_w = (w / counter),
          matrix_container_h = h / GRID_VERT,
          branch_start_x,
          branch_start_y,
          branch_end_x,
          branch_end_y;

    trunk = make_line(*_pens.trunk_bold_p, w/2, h, w/2, h - TRUNK_H);
    place_item(*trunk);

    for (int i = 0; i < counter; ++i)
    {
        if (counter == 1)
            branch_end_x = w / 2;
        else if (counter == 2)
            branch_end_x = (w / 4) + (i * (w / 2));
        else
            branch_end_x = (w / 2) + (i - (counter / 2.0f - 0.5f)) * (w / counter);

        branch_start_x = w / 2;
        branch_start_y = h - TRUNK_H;
        branch_end_y = h - (TRUNK_H + BRANCH_H);

        make_matrix_branch(
            msinfo.at(static_cast<unsigned long>(i)), i,
            matrix_container_w, matrix_container_h,
            branch_start_x, branch_start_y,
            branch_end_x, branch_end_y
        );
    }
}

void TreeScene::make_matrix_branch(minfo *_minfo, int _counter,
                                   qreal matrix_container_w, qreal matrix_container_h,
                                   qreal branch_start_x, qreal branch_start_y,
                                   qreal branch_end_x, qreal branch_end_y)
{
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
    ellipse->setZValue(999);

    // Matrix name annotation
    QGraphicsTextItem *text = new QGraphicsTextItem(
        QString::fromUtf8(_minfo->get_m_name().c_str())
    );
    text->setX(ellipse_center_x + 10);
    text->setY(elliplse_center_y + 3);
    place_item(*text);
    text->setZValue(1000);

    // Secondary branches
    qreal operator_container_w = matrix_container_w / 6,
          operator_container_h = matrix_container_h;

    // Operators
    for (int j = 0; j < OPERATORS; ++j)
    {
        QString operator_name = operators->at(j);

        qreal x2 = (j * operator_container_w)
                 + (operator_container_w / 2)
                 + (_counter * matrix_container_w),
              y2 = (j * operator_container_h) + (operator_container_h / 2);

        QGraphicsLineItem *sec_branch = make_line(
            *_pens.yellow_p,
            branch_end_x, branch_end_y, x2, y2
        );
        place_item(*sec_branch);

        qreal operator_ellipse_center_x = x2 - (OPERATOR_CIRCLE_D / 2),
              operator_elliplse_center_y = y2 - (OPERATOR_CIRCLE_D / 2);

        int operator_id = operators_map->value(operator_name);

        // Clickable
        if ((4 <= operator_id) && (operator_id <= 5))
        {
            ClickableEllipse *scene_operator_ellipse_clickable = make_clickable_ellipse(
                *_pens.green_p, *_brushes.green_b,
                OPERATOR_CIRCLE_D, operator_ellipse_center_x, operator_elliplse_center_y,
                operator_id, false
            );

            connect(scene_operator_ellipse_clickable, &ClickableEllipse::simple_action,
                [=] {
                    emit this->show_result(operator_id);
                }
            );
            place_item(*scene_operator_ellipse_clickable);
        }
        // Not clickable
        else
        {
            QGraphicsEllipseItem *scene_operator_ellipse = make_ellipse(
                *_pens.green_p, *_brushes.green_b,
                OPERATOR_CIRCLE_D, operator_ellipse_center_x, operator_elliplse_center_y
            );
            place_item(*scene_operator_ellipse);
        }

        QGraphicsTextItem *operator_annotation = new QGraphicsTextItem(operator_name);
        operator_annotation->setX(operator_ellipse_center_x);
        operator_annotation->setY(operator_elliplse_center_y);
        place_item(*operator_annotation);

        // Secondary matrices
        QList<mlist*> *by_operator = new QList<mlist*> {
            _minfo->get_sum_ms(),
            _minfo->get_sub_ms(),
            _minfo->get_mul_ms(),
            _minfo->get_div_ms()
        };

        for (int it = 0; it < by_operator->size(); ++it)
        {
            make_secondary_matrix_branches(
                by_operator->at(it),
                _minfo->get_m(),
                j,
                operator_id,
                matrix_container_w,
                operator_ellipse_center_x, operator_elliplse_center_y
            );
        }
    }
}

void TreeScene::make_secondary_matrix_branches(mlist *_mlist,
                                               NumericMatrix* m,
                                               int counter,
                                               int operator_id,
                                               qreal matrix_container_w,
                                               qreal branch_start_x, qreal branch_start_y)
{
    unsigned long count = _mlist->ms.size();
    if (count == 0) return;

    QGraphicsLineItem *to_left_hor_branch;
    QGraphicsLineItem *to_right_hor_branch;
    QPen *pen = _pens.blue_p;

    qreal to_others_y  = branch_start_y + OPERATOR_CIRCLE_D / 2;

    to_right_hor_branch = make_line(
        *pen,
        branch_start_x + OPERATOR_CIRCLE_D,
        to_others_y,
        branch_start_x + (matrix_container_w * (1.0 - (counter / 6.0) - 0.1)),
        to_others_y
    );

    QList<QPair<qreal, qreal>> *matrices_points = new QList<QPair<qreal, qreal>>();

    if ((counter == 0) || (counter == 1))
    {
        place_item(*to_right_hor_branch);
        qreal between_others_right_padding = (matrix_container_w * (1.0 - (counter / 6.0))
                                           - matrix_container_w * 0.1
                                           - OPERATOR_CIRCLE_D
                                             ) / count;

        for (unsigned long i = 0; i < count; ++i)
        {
            QPair<qreal, qreal> *point = new QPair<qreal, qreal> (
                branch_start_x + ((i + 1) * between_others_right_padding),
                to_others_y - (OPERATOR_CIRCLE_D / 2)
            );
            matrices_points->push_back(*point);
        }

        place_other_matrices(m, _mlist, 0, operator_id, matrices_points);
    }
    else if ((counter == 2) || (counter == 3))
    {
        place_item(*to_right_hor_branch);

        to_left_hor_branch = make_line(
            *pen,
            branch_start_x,
            to_others_y,
            branch_start_x - ((matrix_container_w / 6.0) * (counter + 0.5)) + OPERATOR_CIRCLE_D,
            to_others_y
        );
        place_item(*to_left_hor_branch);

        qreal to_right_branch_length = (matrix_container_w * (1.0 - (counter / 6.0))
                                     - matrix_container_w * 0.1
                                     - OPERATOR_CIRCLE_D);
        qreal to_left_branch_length = ((matrix_container_w / 6.0) * (counter + 0.5))
                                    + OPERATOR_CIRCLE_D;

        qreal proportion = to_right_branch_length / to_left_branch_length;

        int amount_to_right = int(count * proportion),
            amount_to_left  = int(count) - amount_to_right;

        qreal between_others_right_padding = to_right_branch_length / amount_to_right,
              between_others_left_padding  = to_left_branch_length / amount_to_left - OPERATOR_CIRCLE_D / 2;

        // Place others to the left
        for (int i = 0; i < amount_to_left; ++i)
        {
            QPair<qreal, qreal> *point = new QPair<qreal, qreal> (
                branch_start_x + 2 * OPERATOR_CIRCLE_D - to_left_branch_length + (i * between_others_left_padding),
                to_others_y - (OPERATOR_CIRCLE_D / 2)
            );
            matrices_points->push_back(*point);
        }
        place_other_matrices(
            m,
            _mlist,
            0,
            operator_id,
            matrices_points
        );

        // Place others to the right
        for (int i = 0; i < amount_to_right; ++i)
        {
            QPair<qreal, qreal> *point = new QPair<qreal, qreal> (
                branch_start_x + ((i + 1) * between_others_right_padding),
                to_others_y - (OPERATOR_CIRCLE_D / 2)
            );
            matrices_points->push_back(*point);
        }
        place_other_matrices(
            m, _mlist,
            amount_to_left + 1,
            operator_id,
            matrices_points
        );
    }
}

void TreeScene::place_other_matrices(NumericMatrix *m,
                                     mlist *_mlist,
                                     int from,
                                     int operator_id,
                                     QList<QPair<qreal, qreal>> *matrices_points)
{
    for (int i = from; i < matrices_points->size(); ++i)
    {
        QPair<qreal, qreal> point = matrices_points->at(int(i));

        ClickableEllipse *matrix_ellipse_clickable = make_clickable_ellipse(
            *_pens.green_p, *_brushes.green_b,
            OPERATOR_CIRCLE_D, point.first, point.second,
            operator_id, true
        );

        connect(matrix_ellipse_clickable, &ClickableEllipse::complex_action,
            [=] {
                emit this->calc_result(operator_id, m, &_mlist->ms.at(static_cast<unsigned long>(i)));
            }
        );
        place_item(*matrix_ellipse_clickable);

        QGraphicsTextItem *matrix_annotation = new QGraphicsTextItem("A");
        matrix_annotation->setX(point.first + 7);
        matrix_annotation->setY(point.second);
        place_item(*matrix_annotation);
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

ClickableEllipse* TreeScene::make_clickable_ellipse(QPen& p,
                                                    QBrush& b,
                                                    qreal diameter,
                                                    qreal lx0, qreal ly0,
                                                    int _action_id,
                                                    bool _complex)
{
    ClickableEllipse *ellipse = new ClickableEllipse(_action_id, _complex);
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
