#ifndef MOUSE_H
#define MOUSE_H

#include <QGraphicsItem>
#include <QObject>

class Mouse : public QObject, public QGraphicsItem
{
    //Q_OBJECT

public:
    Mouse(short, float*, float*, float*, float*);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

protected:
    void timerEvent(QTimerEvent *event);

private:
	short idTeam;
	float *visibleRobot;
	float *xRobot;
	float *yRobot;
	float *angleRobot;
    qreal angle;
    qreal speed;
    qreal mouseEyeDirection;
    QColor color;
};

#endif