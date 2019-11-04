#include "treeview.h"
#include <iostream>
#include <QGraphicsScene>
#include <treescene.h>
#include <QDebug>

TreeView::TreeView(QWidget *parent, QGraphicsScene *scene)
{
    setParent(parent);
    setScene(scene);
//    setGeometry(0, 0, 900, 600);
    setStyleSheet("border: 1px solid #cccccc;"\
                  "background-color: #191919;");
}

void TreeView::window_resized(int w, int h)
{
//    emit(to_scene_signal(w, h));
}

void TreeView::mouseMoveEvent(QMouseEvent *event)
{
    QString s = QString("x: %1; y: %2")
            .arg(event->pos().x(), event->pos().y());
    qDebug() << s.data();
    QGraphicsView::mousePressEvent(event);
}

void TreeView::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "Press: " << event->pos();
}

void TreeView::mouseDoubleClickEvent(QMouseEvent *event)
{

}

void TreeView::mouseReleaseEvent(QMouseEvent *event)
{

}
