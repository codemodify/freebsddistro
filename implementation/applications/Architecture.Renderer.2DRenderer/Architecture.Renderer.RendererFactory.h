#ifndef __Architecture_Renderer_RendererFactory_H__
#define __Architecture_Renderer_RendererFactory_H__

// Qt Headers
#include <QtGui/QWidget>

// blocks
#include <Pluginer/PluginImplementation.h>

// Plugin to implement
#include <Architecture.Renderer.IRendererFactory.h>

// locals
#include "Architecture.Renderer.RendererGraphicsScene.h"
#include "Architecture.Renderer.RendererGraphicsView.h"


class RendererFactory : public QObject, public AbstractPlugin, public IRendererFactory
{
    Q_OBJECT
    Q_INTERFACES( IPlugin )
    Q_PLUGINABLE( RendererFactory )

    public:
        bool doInit();
        void unInit();

        QWidget*                getWidget                   ();
        IPropertiesRenderer*    getPropertiesRenderer       ();
        ITextRenderer*          getTextRenderer             ();
        IInterfaceRenderer*     getInterfaceRenderer        ();
        IAbstractClassRenderer* getAbstractClassRenderer    ();
        IClassRenderer*         getClassRenderer            ();
        IArrowRenderer*         getArrowRenderer            ();
        IArrowDoubleRenderer*   getArrowDoubleRenderer      ();

    private:
        RendererGraphicsScene* _graphicsScene;
        RendererGraphicsView*  _graphicsView;
};


#endif
