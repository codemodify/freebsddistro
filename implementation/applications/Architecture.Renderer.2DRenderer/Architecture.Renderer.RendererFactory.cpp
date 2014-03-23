
// locals
#include "Architecture.Renderer.RendererFactory.h"

bool RendererFactory::doInit()
{
    // populate infos about the plugin
    _pluginInfo.name            = "2DRenderer";
    _pluginInfo.description     = "2D Renderer Architector Plugin";
    _pluginInfo.version         = "digger";
    _pluginInfo.compatVersion   = "digger";
    _pluginInfo.vendor          = "abcdefghi";
    _pluginInfo.copyright       = "abcdefghi (C) (TM) (R) and all the rest smart letters";
    _pluginInfo.license         = "absolutely free";
    _pluginInfo.url             = "www.nikushor.eu";

    // setup the UI elements    
    _graphicsScene = new RendererGraphicsScene();
    
    _graphicsView = new RendererGraphicsView();
    _graphicsView->setScene( _graphicsScene );
    
    return true;
}

void RendererFactory::unInit()
{
    delete _graphicsView;
    delete _graphicsScene;
}

QWidget* RendererFactory::getWidget()
{
    return _graphicsView;
}

IPropertiesRenderer* RendererFactory::getPropertiesRenderer()
{
    return 0;
}

ITextRenderer* RendererFactory::getTextRenderer()
{
    return 0;
}

IInterfaceRenderer* RendererFactory::getInterfaceRenderer()
{
    return 0;
}

IAbstractClassRenderer* RendererFactory::getAbstractClassRenderer()
{
    return 0;
}

IClassRenderer* RendererFactory::getClassRenderer()
{
    return 0;
}

IArrowRenderer* RendererFactory::getArrowRenderer()
{
    return 0;
}

IArrowDoubleRenderer* RendererFactory::getArrowDoubleRenderer()
{
    return 0;
}

SetPluginImplementation( RendererFactory )

