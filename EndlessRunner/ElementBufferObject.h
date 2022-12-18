#ifndef ELEMENT_BUFFER_OBJECT
#define ELEMENT_BUFFER_OBJECT
#include <glad/glad.h>
class ElementBufferObject
{
public:
	GLuint ID;
	ElementBufferObject(GLuint* indices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};

#endif // !ELEMENT_BUFFER_OBJECT


