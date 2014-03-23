
// Qt's headers
#include <QtGui/QWidget>
#include <QtGui/QDirModel>
#include <QtCore/QProcess>
#include <QtGui/QMenu>

// local headers
#include "ui_creator.h"
#include "common/commoncode.h"

class Creator : public CommonCode, public Ui::Creator 
{

	Q_OBJECT

public:
	Creator( QWidget* parent = 0 );
	~Creator();

	void setImageType( QString imageType );



private slots:
	void start();
	void stop();

	void choosePath();

	void filesContextMenu( const QPoint& point );
	void addFilesToImage();
	void refreshFiles();

	void resizeColumnToContents();

	void imageContextMenu( const QPoint& point );
	void removeFromImage();

	QByteArray readOutput();
	void processStarted();
	void processFinished();




private:
	void addLogMessage( QString message, QString colorName="black" );
	void calculateImageSize();


private:
	QString _imageType;
	QDirModel _dirModelFiles;
	QDirModel _dirModelImage;
	QMenu _filesMenu;
	QMenu _imageMenu;
	QString _folderPath;

};

