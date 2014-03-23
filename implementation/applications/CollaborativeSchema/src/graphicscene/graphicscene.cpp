
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QGraphicsItem>

#include "graphicscene.h"

GraphicsScene::GraphicsScene() :
	QGraphicsScene(),
	_haveToDraw( false )
{
	setSceneRect( -600, -600, 600, 600 );
}

GraphicsScene::~GraphicsScene()
{
}

QList<QLineF> g_currentObjects;
QList<QLineF> g_allTheObjects;
void GraphicsScene::mouseMoveEvent ( QGraphicsSceneMouseEvent* mouseEvent )
{
	if( _haveToDraw )
	{
		if( mouseEvent->lastScenePos() != mouseEvent->scenePos() )
		{
			QLineF lineF( mouseEvent->lastScenePos(), mouseEvent->scenePos() );
			g_currentObjects.append( lineF );
			g_allTheObjects.append( lineF );
			addLine( lineF );
		}
	}
}

void GraphicsScene::mousePressEvent ( QGraphicsSceneMouseEvent* mouseEvent )
{
	_haveToDraw = true;
}

void GraphicsScene::mouseReleaseEvent ( QGraphicsSceneMouseEvent* mouseEvent )
{
	_haveToDraw = false;
	
	QString linesAsSerialized = "";
	foreach( QLineF line, g_currentObjects )
	{
		linesAsSerialized += QString("%1,").arg(line.x1());
		linesAsSerialized += QString("%1,").arg(line.y1());
		linesAsSerialized += QString("%1,").arg(line.x2());
		linesAsSerialized += QString("%1,").arg(line.y2());
	}
	
	g_currentObjects.clear();
		
	emit sendObject( linesAsSerialized.toAscii() );
}

void GraphicsScene::addObject( QByteArray objectToAdd )
{
	QString lines = objectToAdd;
	QStringList values = lines.split(",");
	
	int size = values.size();
	if( 0 == size )
		return;

	int beginIndex = 0;		
	if( "clear" == values.at(0) )
	{
		beginIndex++;
		clear();
	}
	
	for( int index=beginIndex; index+4<size; )
	{
		qreal x1 = values.at(index++).toDouble(); 	
		qreal y1 = values.at(index++).toDouble();
		qreal x2 = values.at(index++).toDouble();
		qreal y2 = values.at(index++).toDouble();
		
		QLineF lineF( x1, y1, x2, y2 );
		addLine( lineF );
	}
	
	values.empty();
}

void GraphicsScene::slot_Clear()
{
	clear();
	
	emit sendObject( "clear," );
}

void GraphicsScene::slot_SendSnapshot()
{
	emit sendObject( "clear," );

	g_currentObjects = g_allTheObjects;
	
	mouseReleaseEvent( 0 );
}

//void GraphicsScene::clear()
//{
//	QList<QGraphicsItem*> objects = items();
//	foreach( QGraphicsItem* item, objects )
//	{	
//		removeItem( item );
//		delete item;
//	}
//}
