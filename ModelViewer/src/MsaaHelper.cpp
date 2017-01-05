#include "MsaaHelper.h"

MsaaHelper::MsaaHelper(GLsizei samples, const std::vector<GLenum>& attachments, const std::vector<BufferUsage>& buffer_descriptors)
	: _samples(samples)
	, _fbo(nullptr)
	, _renderbuffer_desc(buffer_descriptors) {
	_fbo = std::make_unique<FBO>();
	_renderbuffer.resize(_renderbuffer_desc.size());
	_fbo->draw_buffers(attachments);
}

void MsaaHelper::update(GLsizei width, GLsizei height) {
	for (auto i(0); i < _renderbuffer.size(); ++i) {
		if (!_renderbuffer[i] || !_renderbuffer[i]->matches(width, height)) {
			_renderbuffer[i] = std::make_unique<Renderbuffer>(_renderbuffer_desc[i].internalformat, width, height, _samples);
			_fbo->attach_renderbuffer(_renderbuffer_desc[i].attachment, _renderbuffer[i]->id());
		}
	}
	if (_fbo->check() != GL_FRAMEBUFFER_COMPLETE) {
		throw std::runtime_error("Incomplete Framebuffer");
	}
}

void MsaaHelper::bind_fbo() const {
	_fbo->bind();
}

void MsaaHelper::unbind_fbo() const {
	FBO::unbind();
}

void MsaaHelper::blit_to_screen(GLsizei width, GLsizei height) const {
	_fbo->blit_to(0, width, height, GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

GLsizei MsaaHelper::samples() const {
	return _samples;
}

GLsizei& MsaaHelper::samples() {
	return _samples;
}
