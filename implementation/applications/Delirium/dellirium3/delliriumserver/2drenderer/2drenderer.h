#ifndef TWO_D_RENDERER_H
#define TWO_D_DRENDERER_H

// qt headers
#include <QtGui/QGraphicsView>
#include <QtCore/QPointer>
#include <QtCore/QTime>
#include <QtCore/QMutex>

class QGraphicsScene;
#include "objects/meball/meball.h"
#include "objects/enemyball/enemyball.h"

class TwoDRenderer : public QGraphicsView
{
	Q_OBJECT

public:
	TwoDRenderer();
	~TwoDRenderer();

public:
	enum RunningMode{ Play, Edit };
	void setRuningMode( RunningMode runningMode );
	
	void saveMap( QString filePath );
	void loadMap( QString filePath );


protected: // overrides to implement our job
	void mousePressEvent( QMouseEvent* event );
	void mouseReleaseEvent( QMouseEvent* event );
	void mouseMoveEvent( QMouseEvent* event );
	void wheelEvent( QWheelEvent* event );
	void timerEvent( QTimerEvent* event );

signals:
	void mousePress( QMouseEvent* event );
	void mouseRelease( QMouseEvent* event );
	void mouseMove( QMouseEvent* event );
	
	void userDidMoved( QByteArray moveContent );
	

private: // edit related
	void initEditMode();

public slots:
	void objectTypeToAddChangedToWall();
	void objectTypeToAddChangedToBush();
	void objectTypeToAddChangedToTarget();
	void objectTypeToAddChangedToBall();
	void objectTypeToAddChangedToEnemyTarget();
	void objectTypeToAddChangedToEnemyBall();
	
	void editMouseWasPressed( QMouseEvent* event );
	void editMouseWasReleased( QMouseEvent* event );
	void editMouseWasMoved( QMouseEvent* event );



private: // play related
	void initPlayMode();

public slots:
	void playMouseWasPressed( QMouseEvent* event );
	void playMouseWasReleased( QMouseEvent* event );
	void playMouseWasMoved( QMouseEvent* event );

public:
	void addMove( QByteArray moveContent );

private:
	struct Move
	{
		int _ballID;
		int _power;
		float _startPointX, _startPointY;
		float _angle;
	};
	QList<Move*> _moves;
	QMutex _mutex;
	
	#define MAX_POWER 200
	#define MY_BALLS_START_ID 0
	#define ENEMY_BALLS_START_ID 500
	QMap<int,MeBall*> _myBalls;
	QMap<int,EnemyBall*> _enemyBalls;



private: // general data
	QPointer<QGraphicsScene> _scene;
	RunningMode _runningMode;
	QTime _timeStartMousePress;
	QTime _timeEndMousePress;

};

#endif
