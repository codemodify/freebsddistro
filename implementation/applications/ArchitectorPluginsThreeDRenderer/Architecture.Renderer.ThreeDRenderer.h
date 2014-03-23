#ifndef __Architecture_Renderer_ThreeDRenderer_H__
#define __Architecture_Renderer_ThreeDRenderer_H__

// local headers
#include "Architecture.Renderer.AbstractRenderer.h"

namespace Architecture
{
    namespace Renderer
    {
        class ThreeDRenderer : public AbstractRenderer
        {
            public:
                ThreeDRenderer();
                ~ThreeDRenderer();
                
            public:
                QWidget* getWidget();        
        };
    }
}

#endif

