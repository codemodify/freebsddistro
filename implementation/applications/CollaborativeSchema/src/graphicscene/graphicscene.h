#ifndef graphicscene_h
#define graphicscene_h

#include <QtGui/QGraphicsScene>

#include <QtCore/QList>
#include <QtGui/QGraphicsItem>

class GraphicsScene : public QGraphicsScene
{
	Q_OBJECT
	
public:
	GraphicsScene();
	~GraphicsScene();
	
	//void clear();

public slots:	
	void addObject( QByteArray objectToSend );
	void slot_Clear();
	void slot_SendSnapshot();	
	
signals:
	void sendObject( QByteArray objectToAdd );
	
protected:
	void mouseMoveEvent ( QGraphicsSceneMouseEvent* mouseEvent );
	void mousePressEvent ( QGraphicsSceneMouseEvent* mouseEvent );
	void mouseReleaseEvent ( QGraphicsSceneMouseEvent* mouseEvent );
	
private:
	bool _haveToDraw;
			
};

#endif
