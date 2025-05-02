#pragma once

#include <glad/glad.h>
#include <vector>

class Framebuffer
{
public:
	Framebuffer();

	void Bind();
	unsigned int getTargetTexture();
	void attachColorBufferHDR(unsigned int pWidth, unsigned int pHeight);
	void attachRenderbuffer(unsigned int pWidth, unsigned int pHeight);
	unsigned int getRenderbuffer();

	static void UnbindAll();

private:
	unsigned int m_FramebufferObject;
	unsigned int m_TargetTexture = 0;
	unsigned int m_RenderbufferObject = 0;
};
