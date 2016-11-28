#pragma once

#include <tuple>
#include <initializer_list>
#include <memory>
#include "Program.h"


namespace ShaderAssembler {
	
	using shader_def_t = std::tuple<GLenum, const char*>;

	std::unique_ptr<Program> make_program(std::initializer_list<shader_def_t> shader_defs);

}