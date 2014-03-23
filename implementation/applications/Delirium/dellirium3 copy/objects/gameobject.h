#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

// qt headers
 #include <QtGui/QGraphicsItem>

class GameObject : public QGraphicsItem
{

public:
	GameObject();
	virtual ~GameObject();


public:
	enum ObjectType
	{
		ObjectNone,
		
		ObjectWall, ObjectBush,
		
		ObjectBall, ObjectTarget,
		
		ObjectEnemyBall, ObjectEnemyTarget
	};
	

public:
	ObjectType getObjectType(){ return _objectType; }

public: // overrides to have a default behaviour for our objects
	QRectF boundingRect() const;


protected:
	int _width;
	int _height;
	
	ObjectType _objectType;

};

#endif
