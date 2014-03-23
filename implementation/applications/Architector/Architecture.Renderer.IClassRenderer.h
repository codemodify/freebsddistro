#ifndef __Architecture_Renderer_IClassRenderer_H__
#define __Architecture_Renderer_IClassRenderer_H__

// local headers
#include "Architecture.Shapes.Class.h"


class IClassRenderer
{
    public:
        virtual ~IClassRenderer(){}

    protected:
        Shape::Class _class;
};


#endif
