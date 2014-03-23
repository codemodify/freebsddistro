#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

// qt headers
#include <QtGui/QMainWindow>
#include <QtCore/QPointer>

class QButtonGroup;

class TwoDRenderer;

class MapEditor : public QMainWindow
{
	Q_OBJECT
	
public:
	MapEditor();
	~MapEditor();


signals:
	void deleteSelectedItems();
	void resizeItem();


private slots:
	void slot_SaveMap();
	void slot_LoadMap();


private:
	void initToolBar();


private:
	QButtonGroup* _buttonGroup;


private:
	QPointer<TwoDRenderer> _renderer;

};

#endif