#ifndef __Architecture_Renderer_IPropertiesRenderer_H__
#define __Architecture_Renderer_IPropertiesRenderer_H__

// local headers
#include "Architecture.Shapes.Properties.h"


class IPropertiesRenderer
{
    public:
        virtual ~IPropertiesRenderer(){}

    protected:
        Shape::Properties _properties;
};


#endif
