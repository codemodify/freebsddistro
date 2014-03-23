#ifndef __Architecture_Renderer_IInterfaceRenderer_H__
#define __Architecture_Renderer_IInterfaceRenderer_H__

// local headers
#include "Architecture.Shapes.Interface.h"


class IInterfaceRenderer
{
    public:
        virtual ~IInterfaceRenderer(){}

    protected:
        Shape::Interface _interface;
};


#endif
