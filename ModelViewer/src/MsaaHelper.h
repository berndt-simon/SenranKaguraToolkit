#pragma once

#include "FBO.h"
#include "Renderbuffer.h"

#include <memory>
#include <vector>

class MsaaHelper {
public:
	struct BufferUsage {
		GLenum internalformat;
		GLenum attachment;
	};

public:
	MsaaHelper(GLsizei samples, const std::vector<GLenum>& attachments, const std::vector<BufferUsage>& buffer_descriptors);
	~MsaaHelper() = default;

	void update(GLsizei width, GLsizei height);

	void bind_fbo() const;
	void unbind_fbo() const;

	void blit_to_screen(GLsizei width, GLsizei height) const;

	GLsizei samples() const;
	GLsizei& samples();

private:
	std::unique_ptr<FBO> _fbo;

	std::vector<std::unique_ptr<Renderbuffer>> _renderbuffer;
	std::vector<BufferUsage> _renderbuffer_desc;

	GLsizei _samples;


};