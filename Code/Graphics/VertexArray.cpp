#include "VertexArray.h"
#include "Platforms/OpenGL/OpenGLVertexArray.h"

std::shared_ptr<VertexArray> VertexArray::Create()
{
	return std::make_shared<OpenGLVertexArray>();
}