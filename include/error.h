#pragma once

void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum serverity, GLsizei length, const GLchar* message, const void* userParam)
{
	if (type == GL_DEBUG_TYPE_ERROR)
	{
		fprintf(stderr, "DebugCallback: ERROR - %s\n", message);
	}
}