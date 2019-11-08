#include "clickableellipse.h"

ClickableEllipse::ClickableEllipse(int _action_id)
    : action_id(_action_id)
{
    setAcceptHoverEvents(true);
}

int ClickableEllipse::get_action()
{
    return action_id;
}

void ClickableEllipse::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Press Ellipse: " << event->pos();
    emit click(get_action());
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
