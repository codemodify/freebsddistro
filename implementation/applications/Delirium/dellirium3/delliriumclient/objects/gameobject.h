#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

// qt headers
#include <QtGui/QGraphicsItem>
#include <QtCore/QObject> 

class GameObject : public QObject, public QGraphicsItem
{

	Q_OBJECT

public:
	GameObject();
	virtual ~GameObject();

	enum ObjectType
	{
		ObjectNone,
		
		ObjectWall, ObjectBush,
		
		ObjectBall, ObjectTarget,
		
		ObjectEnemyBall, ObjectEnemyTarget
	};
	
	ObjectType getObjectType(){ return _objectType; }


public: // overrides to have a default behaviour for our objects
	QRectF boundingRect() const;

protected:
	void mousePressEvent( QGraphicsSceneMouseEvent *event );
	void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
	void timerEvent( QTimerEvent* event );

protected:
	int _width;
	int _height;
	
	ObjectType _objectType;

private:
	QLineF _pathToTraverse;
	int _timerID;

};

#endif
