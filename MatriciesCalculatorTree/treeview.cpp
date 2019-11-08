#include "treeview.h"

TreeView::TreeView(QWidget *parent, QGraphicsScene *scene)
{
    setParent(parent);
    setScene(scene);
    setStyleSheet("border: 1px solid #cccccc;"\
                  "background-color: #191919;");
}

void TreeView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
}

void TreeView::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "Press Scene: " << event->pos();
    QGraphicsView::mousePressEvent(event);
}

void TreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QGraphicsView::mouseDoubleClickEvent(event);
}

void TreeView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
}
