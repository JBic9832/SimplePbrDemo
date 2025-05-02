#include "Framebuffer.h"

Framebuffer::Framebuffer()
{
	glGenFramebuffers(1, &m_FramebufferObject);
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferObject);
}

void Framebuffer::attachColorBufferHDR(unsigned int pWidth, unsigned int pHeight)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferObject);
	glGenTextures(1, &m_TargetTexture);
	glBindTexture(GL_TEXTURE_2D, m_TargetTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, pWidth, pHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TargetTexture, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int Framebuffer::getTargetTexture()
{
	return m_TargetTexture;
}

void Framebuffer::attachRenderbuffer(unsigned int pWidth, unsigned int pHeight)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferObject);
	glGenRenderbuffers(1, &m_RenderbufferObject);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderbufferObject);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, pWidth, pHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderbufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int Framebuffer::getRenderbuffer()
{
	return m_RenderbufferObject;
}

void Framebuffer::UnbindAll()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
