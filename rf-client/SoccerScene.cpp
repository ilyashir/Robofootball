#include "SoccerScene.h"

SoccerScene::SoccerScene(QObject *parent) :
    QGraphicsScene(parent)
{    
    setBackgroundBrush ( QBrush ( QColor ( 0,0x91,0x19,255 ),Qt::SolidPattern ) );
    //setSceneRect ( -3700,-2700,7400,5400 );
    //drawMutex = _drawMutex;
    shutdownSoccerView = false;
//    glWidget = new QGLWidget ( QGLFormat ( QGL::DoubleBuffer ) );
//    setViewport ( glWidget );
    ksize = 10;
    LoadFieldGeometry();

    ConstructField();
    fieldBrush = new QBrush ( Qt::NoBrush );
    fieldLinePen = new QPen();
    fieldLinePen->setColor ( Qt::white );
    fieldLinePen->setWidth ( 2 );
    fieldLinePen->setJoinStyle ( Qt::MiterJoin );
    fieldItem = this->addPath ( *field,*fieldLinePen,*fieldBrush );
    fieldItem->setZValue(0);
    drawScale = 0.15;



    scalingRequested = true;
}

int SoccerScene::UpdateBalls ( QVector<QPointF> &_balls, int cameraID )
{
  QVector<QGraphicsEllipseItem*> tmp;
  while(cameraID+1>ballItems.size())
    ballItems.append(tmp);

  if ( ballItems[cameraID].size() < _balls.size() ){
    //need to allocate some space for the new balls
    QPen pen ( QColor ( 0xcd,0x59,0x00,0xff ) );
    pen.setWidth ( 2 );
    QBrush brush ( QColor ( 0xff,0x81,0x00,0xff ),Qt::SolidPattern );
    while(_balls.size()>ballItems[cameraID].size()){
      ballItems[cameraID].append ( this->addEllipse ( 0,0,12,12,pen,brush ) );
      ballItems[cameraID][ballItems[cameraID].size()-1]->setZValue(2);
    }
  }
  else if ( ballItems[cameraID].size() >_balls.size() ){
    //need to delete some balls
    while(ballItems[cameraID].size()>_balls.size()){
      this->removeItem ( ballItems[cameraID][0] );
      ballItems[cameraID].remove(0);
    }
  }

  for ( int i=0;i<_balls.size();i++ ){
    //Let's update the ball positions now
//      cout << _balls[i].x()/ksize << " " << _balls[i].y()/ksize << endl;
    ballItems[cameraID][i]->setPos ( _balls[i].x()/ksize-6,-_balls[i].y()/ksize-6 );
  }

  int balls = ballItems[cameraID].size();
  return balls;
}

void SoccerScene::ConstructField()
{

   //scene->removeItem(fieldItem);
  field = new QPainterPath();

  field->moveTo ( 0,-field_width/(2*ksize) );
  field->lineTo ( 0,field_width/(2*ksize) );

  field->addEllipse ( -2*center_circle_radius/(2*ksize),-center_circle_radius/ksize,
                      4*center_circle_radius/(2*ksize),2*center_circle_radius/ksize );

  field->moveTo ( field_length/(2*ksize),-field_width/(2*ksize) );
  field->lineTo ( field_length/(2*ksize),field_width/(2*ksize) );

  field->moveTo ( -field_length/(2*ksize),-field_width/(2*ksize) );
  field->lineTo ( -field_length/(2*ksize),field_width/(2*ksize) );

  field->moveTo ( -field_length/(2*ksize),-field_width/(2*ksize) );
  field->lineTo ( field_length/(2*ksize),-field_width/(2*ksize) );

  field->moveTo ( -field_length/(2*ksize),field_width/(2*ksize) );
  field->lineTo ( field_length/(2*ksize),field_width/(2*ksize) );

  field->moveTo ( field_length/(2*ksize),goal_width/(2*ksize) );
  field->lineTo ( ( field_length/(2*ksize)+goal_depth/ksize ),goal_width/(2*ksize) );
  field->lineTo ( ( field_length/(2*ksize)+goal_depth/ksize ),-goal_width/(2*ksize) );
  field->lineTo ( field_length/(2*ksize),-goal_width/(2*ksize) );
  field->moveTo ( ( field_length/(2*ksize)-defense_radius/ksize ),defense_stretch/(2*ksize) );
  field->lineTo ( ( field_length/(2*ksize)-defense_radius/ksize ),-defense_stretch/(2*ksize) );
  field->moveTo ( ( field_length/(2*ksize)-defense_radius/ksize ),defense_stretch/(2*ksize) );
  field->arcTo ( ( field_length/(2*ksize)-defense_radius/ksize ),- ( defense_radius/ksize-defense_stretch/(2*ksize) ),2*defense_radius/ksize,2*defense_radius/ksize,180,90 );
  field->moveTo ( ( field_length/(2*ksize)-defense_radius/ksize ),-defense_stretch/(2*ksize) );
  field->arcTo ( ( field_length/(2*ksize)-defense_radius/ksize ),- ( defense_radius/ksize+defense_stretch/(2*ksize) ),2*defense_radius/ksize,2*defense_radius/ksize,180,-90 );

  field->moveTo ( -field_length/(2*ksize),goal_width/(2*ksize) );
  field->lineTo ( - ( field_length/(2*ksize)+goal_depth/ksize ),goal_width/(2*ksize) );
  field->lineTo ( - ( field_length/(2*ksize)+goal_depth/ksize ),-goal_width/(2*ksize) );
  field->lineTo ( -field_length/(2*ksize),-goal_width/(2*ksize) );
  field->moveTo ( - ( field_length/(2*ksize)-defense_radius/ksize ),defense_stretch/(2*ksize) );
  field->lineTo ( - ( field_length/(2*ksize)-defense_radius/ksize ),-defense_stretch/(2*ksize) );
  field->moveTo ( - ( field_length/(2*ksize)-defense_radius/ksize ),defense_stretch/(2*ksize) );
  field->arcTo ( - ( field_length/(2*ksize)+defense_radius/ksize ),- (defense_radius/ksize-defense_stretch/(2*ksize) ),2*defense_radius/ksize,2*defense_radius/ksize,0,-90 );
  field->moveTo ( - ( field_length/(2*ksize)-defense_radius/ksize ),-defense_stretch/(2*ksize) );
  field->arcTo ( - ( field_length/(2*ksize)+defense_radius/ksize ),- ( defense_radius/ksize+defense_stretch/(2*ksize) ),2*defense_radius/ksize,2*defense_radius/ksize,0,90 );
}

void SoccerScene::LoadFieldGeometry()
{
  this->line_width = FieldConstantsRoboCup2009::line_width;
  this->field_length = FieldConstantsRoboCup2009::field_length;
  this->field_width = FieldConstantsRoboCup2009::field_width;
  this->boundary_width = FieldConstantsRoboCup2009::boundary_width;
  this->referee_width = FieldConstantsRoboCup2009::referee_width;
  this->goal_width = FieldConstantsRoboCup2009::goal_width;
  this->goal_depth = FieldConstantsRoboCup2009::goal_depth;
  this->goal_wall_width = FieldConstantsRoboCup2009::goal_wall_width;
  this->center_circle_radius = FieldConstantsRoboCup2009::center_circle_radius;
  this->defense_radius = FieldConstantsRoboCup2009::defense_radius;
  this->defense_stretch = FieldConstantsRoboCup2009::defense_stretch;
  this->free_kick_from_defense_dist = FieldConstantsRoboCup2009::free_kick_from_defense_dist;
  this->penalty_spot_from_field_line_dist = FieldConstantsRoboCup2009::penalty_spot_from_field_line_dist;
  this->penalty_line_from_spot_dist = FieldConstantsRoboCup2009::penalty_line_from_spot_dist;
}

void SoccerScene::LoadFieldGeometry ( SSL_GeometryFieldSize &fieldSize )
{
  this->line_width = fieldSize.line_width();
   // cout << line_width << endln;
  this->field_length = fieldSize.field_length();
     cout << field_length << endl;
  this->field_width = fieldSize.field_width();
     cout << field_width << endl;
  this->boundary_width = fieldSize.boundary_width();
  this->referee_width = fieldSize.referee_width();
  this->goal_width = fieldSize.goal_width();
  this->goal_depth = fieldSize.goal_depth();
  this->goal_wall_width = fieldSize.goal_wall_width();
  this->center_circle_radius = fieldSize.center_circle_radius();
  this->defense_radius = fieldSize.defense_radius();
  this->defense_stretch = fieldSize.defense_stretch();
  this->free_kick_from_defense_dist = fieldSize.free_kick_from_defense_dist();
  this->penalty_spot_from_field_line_dist = fieldSize.penalty_spot_from_field_line_dist();
  this->penalty_line_from_spot_dist = fieldSize.penalty_line_from_spot_dist();

  this->removeItem ( fieldItem );
  ConstructField();
  fieldItem = this->addPath ( *field,*fieldLinePen,*fieldBrush );
}

void SoccerScene::setSizeScene(int _size)
{
    ksize = _size;
    this->removeItem( fieldItem );
//    drawMutex->lock();
    ConstructField();
    fieldItem = this->addPath ( *field,*fieldLinePen,*fieldBrush );
//    drawMutex->unlock();
}
