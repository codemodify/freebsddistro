#ifndef __Architecture_Renderer_Factory_H__
#define __Architecture_Renderer_Factory_H__

// blocks
#include <Pluginer/PluginDefinition.h>

// locals
#include "Architecture.Renderer.IAbstractClassRenderer.h"
#include "Architecture.Renderer.IArrowDoubleRenderer.h"
#include "Architecture.Renderer.IArrowRenderer.h"
#include "Architecture.Renderer.IClassRenderer.h"
#include "Architecture.Renderer.IInterfaceRenderer.h"
#include "Architecture.Renderer.IPropertiesRenderer.h"
#include "Architecture.Renderer.ITextRenderer.h"


DeclarePluginInterface8
(
    IRendererFactory,

    QWidget*                getWidget                   (),
    IPropertiesRenderer*    getPropertiesRenderer       (),
    ITextRenderer*          getTextRenderer             (),
    IInterfaceRenderer*     getInterfaceRenderer        (),
    IAbstractClassRenderer* getAbstractClassRenderer    (),
    IClassRenderer*         getClassRenderer            (),
    IArrowRenderer*         getArrowRenderer            (),
    IArrowDoubleRenderer*   getArrowDoubleRenderer      ()
)


#endif
