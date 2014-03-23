
// local headers
#include "Architecture.Renderer.ThreeDRenderer.h"


Architecture::Renderer::ThreeDRenderer::ThreeDRenderer() :
    AbstractRenderer()
{}

Architecture::Renderer::ThreeDRenderer::~ThreeDRenderer()
{}

QWidget* Architecture::Renderer::ThreeDRenderer::getWidget()
{
    return new QWidget();
}
 
