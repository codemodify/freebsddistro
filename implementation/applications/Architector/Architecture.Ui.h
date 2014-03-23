#ifndef __Architecture_Ui_H__
#define __Architecture_Ui_H__

// Qt headers
#include <QtGui/QMdiSubWindow>

// local headers
#include "Architecture.Shapes.h"
#include "Architecture.Renderer.IRendererFactory.h"


class Ui : public QMdiSubWindow
{
    Q_OBJECT

    public:
        Ui( Shapes& model, IRendererFactory& view );

    private:
        Shapes&             _model;
        IRendererFactory&   _view;
};

#endif
