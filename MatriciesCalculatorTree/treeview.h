#ifndef TREEVIEW_H
#define TREEVIEW_H

#pragma once
#include <QtCore>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDebug>
#include <iostream>
#include "treescene.h"

class TreeView : public QGraphicsView
{
    Q_OBJECT

private:
    QRectF pos_rect;
    double w, h;
    QGraphicsScene *scene;

    QShortcut *zoom_in;
    QShortcut *zoom_out;
    void init_shortcuts();

private slots:
    void scale_in();
    void scale_out();

public:
    TreeView(QWidget *parent, QGraphicsScene *scene);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // TREEVIEW_H
