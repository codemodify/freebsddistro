#ifndef __Architecture_Renderer_ClassicUmlRenderer_H__
#define __Architecture_Renderer_ClassicUmlRenderer_H__

// local headers
#include "Architecture.Renderer.AbstractRenderer.h"

namespace Architecture
{
    namespace Renderer
    {
        class ClassicUmlRenderer : public AbstractRenderer
        {
            public:
                ClassicUmlRenderer();
                
            public:
                QWidget* getWidget();
        };
    }
}

#endif

