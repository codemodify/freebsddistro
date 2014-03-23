#ifndef __Architecture_Renderer_ITextRenderer_H__
#define __Architecture_Renderer_ITextRenderer_H__

// local headers
#include "Architecture.Shapes.Text.h"


class ITextRenderer
{
    public:
        virtual ~ITextRenderer(){}

    protected:
        Shape::Text _text;
};


#endif
