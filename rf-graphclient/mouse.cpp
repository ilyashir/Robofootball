 #include "mouse.h"

 #include <QGraphicsScene>
 #include <QPainter>
 #include <QStyleOption>

 #include <math.h>

 static const double Pi = 3.14159265358979323846264338327950288419717;
 static double TwoPi = 2.0 * Pi;

 static qreal normalizeAngle(qreal angle)
 {
     while (angle < 0)
         angle += TwoPi;
     while (angle > TwoPi)
         angle -= TwoPi;
     return angle;
 }

 Mouse::Mouse(short _idTeam = 0, float *_visibleRobot = NULL, float *_xRobot = NULL, float*_yRobot = NULL, float*_angleRobot = NULL)
     : angle(0), speed(0), mouseEyeDirection(0), color(0,0,0) //(_idTeam-'b')*(_idTeam-'y')%256
 {
     double position;
	 visibleRobot=_visibleRobot;
	 idTeam=_idTeam;
	 xRobot=_xRobot;
	 yRobot=_yRobot;
	 angleRobot=_angleRobot;
     //rotate(qrand() % (360 * 16));
     startTimer(1000 / 33);
 }


 QRectF Mouse::boundingRect() const
 {
     qreal adjust = 0.5;
     return QRectF(-25 - adjust/2, -25 - adjust/2,
                   50 + adjust, 50 + adjust);
 }

 QPainterPath Mouse::shape() const
 {
     QPainterPath path;
     path.addRect(-20, -20, 20, 20);
     return path;
 }



 void Mouse::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
 {
     // Круг команды
	 (idTeam=='r')?(painter->setBrush(Qt::red)):(painter->setBrush(color));
     painter->drawEllipse(-5, -5, 10, 10);

     // Индентификация
     //painter->setBrush(scene()->collidingItems(this).isEmpty() ? Qt::darkYellow : Qt::red);
	 painter->setBrush((idTeam=='y')?Qt::yellow:((idTeam=='b')?Qt::blue:Qt::red));
	 if(idTeam=='b' || idTeam=='y'){
		painter->drawEllipse(-12, -12, 12, 12);
		//painter->setBrush(Qt::white);
		painter->drawEllipse(0, -14, 12, 12);
		//painter->setBrush(Qt::green);
		painter->drawEllipse(-12, 0, 12, 12);
		//painter->setBrush(Qt::red);
		painter->drawEllipse(0, 2, 12, 12);
	 }
 }

 void Mouse::timerEvent(QTimerEvent *)
 {
	 //printf("My visible is %f \n", *visibleRobot);
	 if(*visibleRobot){
	    show();
		setPos((*xRobot)/3*0.4+400,(*yRobot)*0.15+300);
		//printf("%f %f %f\n", *xRobot, *yRobot, *angleRobot);
		if(idTeam=='b' || idTeam=='y')
			setTransform(QTransform(cos(*angleRobot),sin(*angleRobot),-sin(*angleRobot),cos(*angleRobot),0,0));
		//rotate(10);
	 }
	 else
		 hide();
	 //printf("%d\n",Yellow[1]);
	 /*
	 // Не перемещайте слишком далеко
     QLineF lineToCenter(QPointF(0, 0), mapFromScene(400, 300));
     if (lineToCenter.length() > 150) {
         qreal angleToCenter = ::acos(lineToCenter.dx() / lineToCenter.length());
         if (lineToCenter.dy() < 0)
             angleToCenter = TwoPi - angleToCenter;
         angleToCenter = normalizeAngle((Pi - angleToCenter) + Pi / 2);

         if (angleToCenter < Pi && angleToCenter > Pi / 4) {
             // Повернуть налево
             angle += (angle < -Pi / 2) ? 0.25 : -0.25;
         } else if (angleToCenter >= Pi && angleToCenter < (Pi + Pi / 2 + Pi / 4)) {
             // Повернуть вправо
             angle += (angle < Pi / 2) ? 0.25 : -0.25;
         }
     } else if (::sin(angle) < 0) {
         angle += 0.25;
     } else if (::sin(angle) > 0) {
         angle -= 0.25;
     }

     // Попытаться не сталкиваться с другими мышами
     QList<QGraphicsItem *> dangerMice = scene()->items(QPolygonF()
                                                        << mapToScene(0, 0)
                                                        << mapToScene(-30, -50)
                                                        << mapToScene(30, -50));
     foreach (QGraphicsItem *item, dangerMice) {
         if (item == this)
             continue;

         QLineF lineToMouse(QPointF(0, 0), mapFromItem(item, 0, 0));
         qreal angleToMouse = ::acos(lineToMouse.dx() / lineToMouse.length());
         if (lineToMouse.dy() < 0)
             angleToMouse = TwoPi - angleToMouse;
         angleToMouse = normalizeAngle((Pi - angleToMouse) + Pi / 2);

         if (angleToMouse >= 0 && angleToMouse < Pi / 2) {
             // Повернуть вправо
             angle += 0.5;
         } else if (angleToMouse <= TwoPi && angleToMouse > (TwoPi - Pi / 2)) {
             // Повернуть налево
             angle -= 0.5;
         }
     }

    // Добавим немного случайных движений
     if (dangerMice.size() > 1 && (qrand() % 10) == 0) {
         if (qrand() % 1)
             angle += (qrand() % 100) / 2000.0;
         else
             angle -= (qrand() % 100) / 2000.0;
     }

     speed += (-50 + qrand() % 100) / 100000.0;

     qreal dx = ::sin(angle) * 10;
     mouseEyeDirection = (qAbs(dx / 5) < 1) ? 0 : dx / 5;

     rotate(dx);
     setPos(mapToParent(0, -(3 + sin(speed) * 3)));*/
 }
