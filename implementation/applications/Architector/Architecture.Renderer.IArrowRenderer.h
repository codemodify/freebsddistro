#ifndef __Architecture_Renderer_IArrowRenderer_H__
#define __Architecture_Renderer_IArrowRenderer_H__

// local headers
#include "Architecture.Shapes.Arrow.h"


class IArrowRenderer
{
    public:
        virtual ~IArrowRenderer(){}

    protected:
        Shape::Arrow _arrow;
};


#endif
