#include "clickableellipse.h"

ClickableEllipse::ClickableEllipse(int _action_id, bool _complex)
    : action_id(_action_id)
    , complex(_complex)
{
    setAcceptHoverEvents(true);
}

int ClickableEllipse::get_action()
{
    return action_id;
}

void ClickableEllipse::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug() << "Press Ellipse: " << event->pos();
    if (complex)
        emit complex_action(get_action());
    else
        emit simple_action(get_action());

    Q_UNUSED(event)
}

void ClickableEllipse::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverEnterEvent(event);
    QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
    Q_UNUSED(event)
}

void ClickableEllipse::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverLeaveEvent(event);
    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
    Q_UNUSED(event)
}
