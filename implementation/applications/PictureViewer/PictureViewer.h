#ifndef __PictureViewer_H__
#define __PictureViewer_H__

#include <QtGui/QWidget>


class PictureViewer : public QWidget
{
    public:
        PictureViewer( QString imageFilePath );
        ~PictureViewer();

};

#endif
