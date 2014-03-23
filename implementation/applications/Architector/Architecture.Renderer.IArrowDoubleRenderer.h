#ifndef __Architecture_Renderer_IArrowDoubleRenderer_H__
#define __Architecture_Renderer_IArrowDoubleRenderer_H__

// local headers
#include "Architecture.Shapes.ArrowDouble.h"


class IArrowDoubleRenderer
{
    public:
        virtual ~IArrowDoubleRenderer(){}

    protected:
        Shape::ArrowDouble _arrowDouble;
};


#endif
