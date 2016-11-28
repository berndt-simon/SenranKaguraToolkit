#include "ShaderAssembler.h"

#include <vector>
#include "Shader.h"

std::unique_ptr<Program> ShaderAssembler::make_program(std::initializer_list<shader_def_t> shader_defs) {
	std::vector<std::unique_ptr<Shader>> shaders;
	for (const auto& shader_def : shader_defs) {
		shaders.push_back(Shader::from_source(std::get<0>(shader_def), std::get<1>(shader_def)));
	}
	return std::make_unique<Program>(shaders);
}
