
// local headers
#include "Architecture.Renderer.ClassicUmlRenderer.h"


Architecture::Renderer::ClassicUmlRenderer::ClassicUmlRenderer() :
    AbstractRenderer()
{}

QWidget* Architecture::Renderer::ClassicUmlRenderer::getWidget()
{
    return new QWidget();
}
 
