#ifndef SOCCERSCENE_H
#define SOCCERSCENE_H

#include <iostream>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QVector>
#include "field_default_constants.h"
#include "robocup_ssl_client.h"
#include "timer.h"


class SoccerScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit SoccerScene(QObject *parent = 0);

    int UpdateBalls ( QVector<QPointF> &_balls, int cameraID );

    void LoadFieldGeometry();
    void LoadFieldGeometry ( SSL_GeometryFieldSize &fieldSize );

protected:

    float drawScale;
    bool scalingRequested;

    QMutex* drawMutex;
    
signals:
    
public slots:
    void setSizeScene(int);

private:

    //Robots
//    QVector<Robot*> blueRobots,yellowRobots, robots;
    //balls
    QVector < QVector<QGraphicsEllipseItem*> > ballItems;
    //field
    QPainterPath *field;
    QGraphicsPathItem *fieldItem;
    //brushes and pens
    QBrush *fieldBrush, *ballBrush;
    QPen *fieldPen, *fieldLinePen, *ballPen;
    bool shutdownSoccerView;

    void ConstructField();

    //gain size
    double ksize;

    //Field dimensions and geometry
    double line_width;
    double field_length;
    double field_width;
    double boundary_width;
    double referee_width;
    double goal_width;
    double goal_depth;
    double goal_wall_width;
    double center_circle_radius;
    double defense_radius;
    double defense_stretch;
    double free_kick_from_defense_dist;
    double penalty_spot_from_field_line_dist;
    double penalty_line_from_spot_dist;
    
};

#endif // SOCCERSCENE_H
