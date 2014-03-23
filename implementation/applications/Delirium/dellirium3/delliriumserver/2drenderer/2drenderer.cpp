
// qt headers
#include <QtGui/QGraphicsScene>
#include <QtGui/QMouseEvent>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNamedNodeMap>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtCore/QMutexLocker>

#include <math.h>

// local headers
#include "2drenderer.h"

#include "objects/gameobject.h"

#include "objects/neutralbush/neutralbush.h"
#include "objects/neutralwall/neutralwall.h"

//#include "objects/meball/meball.h"
#include "objects/metarget/metarget.h"

//#include "objects/enemyball/enemyball.h"
#include "objects/enemytarget/enemytarget.h"


GameObject::ObjectType g_objectTypeToAdd;


TwoDRenderer::TwoDRenderer():
	QGraphicsView()
{
	g_objectTypeToAdd = GameObject::ObjectNone;

	_scene = new QGraphicsScene();
	_scene->setSceneRect(-400, -300, 800, 600);
	setScene( _scene );

    setCacheMode(CacheBackground);
	
    setRenderHint(QPainter::Antialiasing);
	setRenderHint(QPainter::TextAntialiasing);
	setRenderHint(QPainter::TextAntialiasing);
	
	setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);
}

TwoDRenderer::~TwoDRenderer()
{}

void TwoDRenderer::setRuningMode( RunningMode runningMode )
{
	_runningMode = runningMode;
	
	if( Play == _runningMode )
		initPlayMode();
	else
		initEditMode();
}

void TwoDRenderer::mousePressEvent( QMouseEvent* event )
{
	// fixing the moment when the mouse button is pressed down
	_timeStartMousePress = QTime::currentTime();

	emit mousePress( event );

	QGraphicsView::mousePressEvent( event );
}

void TwoDRenderer::mouseReleaseEvent( QMouseEvent* event )
{
	// fixing the moment when the mouse button is released
	_timeEndMousePress = QTime::currentTime();

	emit mouseRelease( event );

	QGraphicsView::mouseReleaseEvent( event );
}

void TwoDRenderer::mouseMoveEvent( QMouseEvent* event )
{
	emit mouseMove( event );
	
	QGraphicsView::mouseMoveEvent( event );
}

void TwoDRenderer::wheelEvent( QWheelEvent* event )
{
	if( event->modifiers() == Qt::AltModifier )
	{
		qreal angle = event->delta()>0 ? -2.0 : 2.0;
		rotate( angle );
	}
	else
	{
		qreal scaleFactor = pow( (double)2, event->delta() / 240.0);
		qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
		if (factor < 0.07 || factor > 100)
			return;
		else
			scale( scaleFactor, scaleFactor );
	}
}

//############################################################################# edit
void TwoDRenderer::objectTypeToAddChangedToWall()
{
	g_objectTypeToAdd = GameObject::ObjectWall;
}

void TwoDRenderer::objectTypeToAddChangedToBush()
{
	g_objectTypeToAdd = GameObject::ObjectBush;
}

void TwoDRenderer::objectTypeToAddChangedToTarget()
{
	g_objectTypeToAdd = GameObject::ObjectTarget;
}

void TwoDRenderer::objectTypeToAddChangedToBall()
{
	g_objectTypeToAdd = GameObject::ObjectBall;
}

void TwoDRenderer::objectTypeToAddChangedToEnemyTarget()
{
	g_objectTypeToAdd = GameObject::ObjectEnemyTarget;
}

void TwoDRenderer::objectTypeToAddChangedToEnemyBall()
{
	g_objectTypeToAdd = GameObject::ObjectEnemyBall;
}

void TwoDRenderer::initEditMode()
{
	setDragMode( QGraphicsView::RubberBandDrag );

	connect( this, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(editMouseWasPressed(QMouseEvent*)) );
	connect( this, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(editMouseWasReleased(QMouseEvent*)) );
	connect( this, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(editMouseWasMoved(QMouseEvent*)) );
}

QDomNode getChildByName( QDomNode parentNode, QString childNodeName )
{
	if( false == parentNode.hasChildNodes() )
		return QDomNode();

	QDomNode childNode = parentNode.firstChild();
	while( false == childNode.isNull() )
	{
		if( childNodeName == childNode.toElement().nodeName() )
			break;

		childNode = childNode.nextSibling();
	}
	
	return childNode;
}

void TwoDRenderer::saveMap( QString filePath )
{
	QFile file( filePath );
	if( false == file.open(QFile::WriteOnly) )
		return;
		
	#define MAP_STRUCTURE "<?xml version=\"1.0\" ?>" \
								"<delleriumMap>" \
									"<walls></walls>" \
									"<bushes></bushes>" \
									"<me>" \
										"<balls></balls>" \
										"<targets></targets>" \
									"</me>" \
									"<enemy>" \
										"<balls></balls>" \
										"<targets></targets>" \
									"</enemy>" \
								"</delleriumMap>"
	
	// here we build the XML strucuture for the map
	QDomDocument domDocument;
	domDocument.setContent( QString(MAP_STRUCTURE) );

	QDomNode rootNode = domDocument.documentElement();

	QDomNode wallsNode = getChildByName( rootNode, "walls" );
	QDomNode bushesNode = getChildByName( rootNode, "bushes" );

	QDomNode meBallsNode = getChildByName( getChildByName(rootNode,"me"), "balls" );
	QDomNode meTargetsNode = getChildByName( getChildByName(rootNode,"me"), "targets" );
	
	QDomNode enemyBallsNode = getChildByName( getChildByName(rootNode,"enemy"), "balls" );
	QDomNode enemyTargetsNode = getChildByName( getChildByName(rootNode,"enemy"), "targets" );

	QList<QGraphicsItem*> gameObjects = _scene->items();
	foreach( QGraphicsItem* item, gameObjects )
	{
		GameObject* gameObject = static_cast<GameObject*>( item );
		if( 0 != gameObject )
		{
			switch( gameObject->getObjectType() )
			{
				case GameObject::ObjectWall:
					{
						QDomElement element = domDocument.createElement( "wall" );
						QDomAttr attributeX = domDocument.createAttribute( "x" ); attributeX.setValue( QString("%1").arg(gameObject->x()) );
						QDomAttr attributeY = domDocument.createAttribute( "y" ); attributeY.setValue( QString("%1").arg(gameObject->y()) );
						element.setAttributeNode( attributeX );
						element.setAttributeNode( attributeY );
						wallsNode.appendChild( element );
					}
					break;
					
				case GameObject::ObjectBush:
					{
						QDomElement element = domDocument.createElement( "bush" );
						QDomAttr attributeX = domDocument.createAttribute( "x" ); attributeX.setValue( QString("%1").arg(gameObject->x()) );
						QDomAttr attributeY = domDocument.createAttribute( "y" ); attributeY.setValue( QString("%1").arg(gameObject->y()) );
						element.setAttributeNode( attributeX );
						element.setAttributeNode( attributeY );
			
						bushesNode.appendChild( element );
					}
					break;
		
				case GameObject::ObjectBall:
					{
						QDomElement element = domDocument.createElement( "ball" );
						QDomAttr attributeX = domDocument.createAttribute( "x" ); attributeX.setValue( QString("%1").arg(gameObject->x()) );
						QDomAttr attributeY = domDocument.createAttribute( "y" ); attributeY.setValue( QString("%1").arg(gameObject->y()) );
						element.setAttributeNode( attributeX );
						element.setAttributeNode( attributeY );

						meBallsNode.appendChild( element );
					}
					break;
					
				case GameObject::ObjectTarget:
					{
						QDomElement element = domDocument.createElement( "target" );
						QDomAttr attributeX = domDocument.createAttribute( "x" ); attributeX.setValue( QString("%1").arg(gameObject->x()) );
						QDomAttr attributeY = domDocument.createAttribute( "y" ); attributeY.setValue( QString("%1").arg(gameObject->y()) );
						element.setAttributeNode( attributeX );
						element.setAttributeNode( attributeY );

						meTargetsNode.appendChild( element );
					}
					break;
		
				case GameObject::ObjectEnemyBall:
					{
						QDomElement element = domDocument.createElement( "ball" );
						QDomAttr attributeX = domDocument.createAttribute( "x" ); attributeX.setValue( QString("%1").arg(gameObject->x()) );
						QDomAttr attributeY = domDocument.createAttribute( "y" ); attributeY.setValue( QString("%1").arg(gameObject->y()) );
						element.setAttributeNode( attributeX );
						element.setAttributeNode( attributeY );

						enemyBallsNode.appendChild( element );
					}
					break;
					
				case GameObject::ObjectEnemyTarget:
					{
						QDomElement element = domDocument.createElement( "target" );
						QDomAttr attributeX = domDocument.createAttribute( "x" ); attributeX.setValue( QString("%1").arg(gameObject->x()) );
						QDomAttr attributeY = domDocument.createAttribute( "y" ); attributeY.setValue( QString("%1").arg(gameObject->y()) );
						element.setAttributeNode( attributeX );
						element.setAttributeNode( attributeY );

						enemyTargetsNode.appendChild( element );
					}
					break;
					
				case GameObject::ObjectNone:
				default:
					break;
			}
		}
	}
	
	// saving to storage
	QTextStream textStream( &file );
	domDocument.save( textStream, 4 );
	
	file.close();
}

void TwoDRenderer::loadMap( QString filePath )
{
	QFile file( filePath );
	if( false == file.open(QFile::ReadOnly) )
		return;

	// here we build the XML strucuture for the map
	QDomDocument domDocument;
	domDocument.setContent( &file );

	QDomNode rootNode = domDocument.documentElement();

	QDomNode wallsNode = getChildByName( rootNode, "walls" );
	QDomNode bushesNode = getChildByName( rootNode, "bushes" );

	QDomNode meBallsNode = getChildByName( getChildByName(rootNode,"me"), "balls" );
	QDomNode meTargetsNode = getChildByName( getChildByName(rootNode,"me"), "targets" );
	
	QDomNode enemyBallsNode = getChildByName( getChildByName(rootNode,"enemy"), "balls" );
	QDomNode enemyTargetsNode = getChildByName( getChildByName(rootNode,"enemy"), "targets" );

	GameObject* gameObject = 0;

	// adding walls
	QDomNode node = wallsNode.firstChild();
	while( false == node.isNull() )
	{
		float x = node.attributes().namedItem("x").nodeValue().toFloat();
		float y = node.attributes().namedItem("y").nodeValue().toFloat();
		
		gameObject = new NeutralWall();
		gameObject->setFlag( QGraphicsItem::ItemIsMovable, _runningMode == Edit );
		_scene->addItem( gameObject );
		gameObject->setPos( x,y );
		
		node = node.nextSibling();
	}
	
	// adding bushes
	node = bushesNode.firstChild();
	while( false == node.isNull() )
	{
		float x = node.attributes().namedItem("x").nodeValue().toFloat();
		float y = node.attributes().namedItem("y").nodeValue().toFloat();
		
		gameObject = new NeutralBush();
		gameObject->setFlag( QGraphicsItem::ItemIsMovable, _runningMode == Edit );
		_scene->addItem( gameObject );
		gameObject->setPos( x,y );
		
		node = node.nextSibling();
	}

	// adding balls
	node = meBallsNode.firstChild();
	while( false == node.isNull() )
	{
		float x = node.attributes().namedItem("x").nodeValue().toFloat();
		float y = node.attributes().namedItem("y").nodeValue().toFloat();
		
		gameObject = new MeBall();
		gameObject->setFlag( QGraphicsItem::ItemIsMovable, _runningMode == Edit );
		_scene->addItem( gameObject );
		gameObject->setPos( x,y );
		
		node = node.nextSibling();
	}

	// adding targets
	node = meTargetsNode.firstChild();
	while( false == node.isNull() )
	{
		float x = node.attributes().namedItem("x").nodeValue().toFloat();
		float y = node.attributes().namedItem("y").nodeValue().toFloat();
		
		gameObject = new MeTarget();
		gameObject->setFlag( QGraphicsItem::ItemIsMovable, _runningMode == Edit );
		_scene->addItem( gameObject );
		gameObject->setPos( x,y );
		
		node = node.nextSibling();
	}

	// adding enemy balls
	node = enemyBallsNode.firstChild();
	while( false == node.isNull() )
	{
		float x = node.attributes().namedItem("x").nodeValue().toFloat();
		float y = node.attributes().namedItem("y").nodeValue().toFloat();
		
		gameObject = new EnemyBall();
		gameObject->setFlag( QGraphicsItem::ItemIsMovable, _runningMode == Edit );
		_scene->addItem( gameObject );
		gameObject->setPos( x,y );
		
		node = node.nextSibling();
	}

	// adding enemy targets
	node = enemyTargetsNode.firstChild();
	while( false == node.isNull() )
	{
		float x = node.attributes().namedItem("x").nodeValue().toFloat();
		float y = node.attributes().namedItem("y").nodeValue().toFloat();
		
		gameObject = new EnemyTarget();
		gameObject->setFlag( QGraphicsItem::ItemIsMovable, _runningMode == Edit );
		_scene->addItem( gameObject );
		gameObject->setPos( x,y );
		
		node = node.nextSibling();
	}
}

void TwoDRenderer::editMouseWasPressed( QMouseEvent* event )
{
}

void TwoDRenderer::editMouseWasReleased( QMouseEvent* event )
{
	// if the time differ we don't react
	if( _timeStartMousePress.minute() != _timeEndMousePress.minute() )
		return;

	if( _timeStartMousePress.second() != _timeEndMousePress.second() )
		return;
		
	if( abs(_timeStartMousePress.msec()-_timeEndMousePress.msec()) > 100 )
		return;

	// we put objects only in case of left mouse-button press
	if( Qt::LeftButton == event->button() )
	{
		GameObject* gameObject = 0;

		switch( g_objectTypeToAdd )
		{
			case GameObject::ObjectWall: gameObject = new NeutralWall(); break;
			case GameObject::ObjectBush: gameObject = new NeutralBush(); break;
			case GameObject::ObjectBall: gameObject = new MeBall(); break;
			case GameObject::ObjectTarget: gameObject = new MeTarget(); break;
			case GameObject::ObjectEnemyBall: gameObject = new EnemyBall(); break;
			case GameObject::ObjectEnemyTarget: gameObject = new EnemyTarget(); break;
			default: break;
		}

		if( 0 != gameObject )
		{
			_scene->addItem( gameObject );
			gameObject->setPos( mapToScene(event->x(),event->y()) );
		}
	}
	
	// in case of right button we remove the objects
	else if( Qt::RightButton == event->button() )
	{
		if( 0 == _scene->selectedItems().count() )
		{
			QGraphicsItem* item = itemAt( event->x(), event->y() );
			if( 0 != item )
				item->setSelected( true );
		}

		QList<QGraphicsItem*> selectedItems = _scene->selectedItems();
		foreach( QGraphicsItem* item, selectedItems )
		{
			_scene->removeItem( item );
			delete item;
		}

		selectedItems.clear();
	}
}

void TwoDRenderer::editMouseWasMoved( QMouseEvent* event )
{
	if( (GameObject::ObjectWall != g_objectTypeToAdd) || (Qt::LeftButton != event->buttons()) )
		return;

	QGraphicsItem* item = itemAt( event->x(), event->y() );
	if( 0 != item )
		return;
		
	GameObject* gameObject = new NeutralWall();
	_scene->addItem( gameObject );
	gameObject->setPos( mapToScene(event->x(),event->y()) );
}

//############################################################################# play
void TwoDRenderer::initPlayMode()
{
	setDragMode( QGraphicsView::ScrollHandDrag );
	
	connect( this, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(playMouseWasPressed(QMouseEvent*)) );
	connect( this, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(playMouseWasReleased(QMouseEvent*)) );
	connect( this, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(playMouseWasMoved(QMouseEvent*)) );
}

void TwoDRenderer::playMouseWasPressed( QMouseEvent* event )
{
}

void TwoDRenderer::playMouseWasReleased( QMouseEvent* event )
{
	Move* move = new Move();
	
	QByteArray moveContent( (const char*)move, sizeof(Move) );

	emit userDidMoved( moveContent );
}

void TwoDRenderer::playMouseWasMoved( QMouseEvent* event )
{
}

void TwoDRenderer::timerEvent( QTimerEvent* event )
{
	QMutexLocker mutexLocker( &_mutex );
	
	foreach( Move* move, _moves )
	{
		if( true == _myBalls.contains(move->_ballID) )
		{
			MeBall* ball = _myBalls[move->_ballID];
			ball->moveBy( 1,1 );
		}
		else if( true == _enemyBalls.contains(move->_ballID) )
		{
		}
	}
}

void TwoDRenderer::addMove( QByteArray moveContent )
{
	QMutexLocker mutexLocker( &_mutex );
	
	Move* move = new Move();
	
	memcpy( (void*)move, (void*)moveContent.data(), moveContent.size() );
	
	_moves.append( move ); 
}
