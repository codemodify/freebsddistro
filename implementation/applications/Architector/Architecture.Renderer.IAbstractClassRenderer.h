#ifndef __Architecture_Renderer_IAbstractClassRenderer_H__
#define __Architecture_Renderer_IAbstractClassRenderer_H__

// local headers
#include "Architecture.Shapes.AbstractClass.h"


class IAbstractClassRenderer
{
    public:
        virtual ~IAbstractClassRenderer(){}

    protected:
        Shape::AbstractClass _abstractClass;
};


#endif
