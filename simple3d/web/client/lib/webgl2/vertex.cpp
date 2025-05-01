#include "vertex.h"

namespace NSimple3D::NWebGL2 {

////////////////////////////////////////////////////////////////////////////////

void Vertex::BindAttributes()
{
    glVertexAttribPointer(
        0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Pos)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Normal)));
    glEnableVertexAttribArray(1);
}

void TexturedVertex::BindAttributes()
{
    glVertexAttribPointer(
        0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TexturedVertex),
		reinterpret_cast<void*>(offsetof(TexturedVertex, Pos)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TexturedVertex),
		reinterpret_cast<void*>(offsetof(TexturedVertex, Normal)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(
        2,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TexturedVertex),
		reinterpret_cast<void*>(offsetof(TexturedVertex,
		TexCoords)));
    glEnableVertexAttribArray(2);
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NSimple3D::NWebGL2
