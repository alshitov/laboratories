#ifndef CLICKABLEELLIPSE_H
#define CLICKABLEELLIPSE_H
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QtCore>
#include <QCursor>
#include <QApplication>

class ClickableEllipse : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT

public:
    ClickableEllipse(int _action_id, bool _complex);
    int get_action();

signals:
    void simple_action(int);
    void complex_action(int);

private:
    int action_id;
    bool complex;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

};

#endif // CLICKABLEELLIPSE_H
