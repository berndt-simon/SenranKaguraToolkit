//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GLError.h"

#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/component_wise.hpp>

#include <iostream>
#include <sstream>
#include <memory>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <iostream>

#include "UBO.h"
#include "Program.h"
#include "Texture2D.h"
#include "UniformUtil.h"

#include "Camera.h"

#include "FileProcessing.h"
#include "CAT.h"
#include "TMD.h"
#include "GXT.h"

#include "RAIIgl.h"
#include "Bounds.h"
#include "TriangleMesh.h"
#include "QuadVAO.h"

#include "Color.h"
#include "BasicShaders.h"
#include "DebugShaders.h"
#include "ShaderAssembler.h"

#include <stdio.h>

#define IMGUI_ENABLE

//#define INDIVIDUAL_MESHES

#ifdef IMGUI_ENABLE
#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw_gl3.h"
#endif // IMGUI_ENABLE


struct uni_lights_t {
	alignas(16) glm::vec3 light_position;
	alignas(16) glm::vec3 light_intensities;
};

struct uni_matrices_t {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 model_view_projection;
};

struct uni_material_t {
	alignas(16) glm::vec3 ambient_color;
	alignas(16) glm::vec3 diffuse_color;
	alignas(16) glm::vec3 specular_color;
};

namespace globals {
	std::unique_ptr<UBO<uni_matrices_t>> uni_matrices(nullptr);
	std::unique_ptr<UBO<uni_lights_t>> uni_light(nullptr);
	std::unique_ptr<UBO<uni_material_t>> uni_material(nullptr);

	Camera camera;
	std::unique_ptr<Projection> projection(nullptr);

	GLFWwindow* window(nullptr);

	bool draw_normals(false);
	bool draw_flat_uvs(false);
	bool draw_wireframe(false);
	bool draw_uvmap(false);

	std::map<std::string, std::unique_ptr<Texture2D>> textures;
	std::vector<std::unique_ptr<TriangleMeshVAO>> meshes;
	std::map<size_t, std::string> material_table;
	Bounds bounds{};
}
using namespace globals;

static void update_uni_matrices(float width, float height) {
	projection->width = std::max(width, 1.f);
	projection->height = std::max(height, 1.f);
	if (!uni_matrices) {
		throw std::runtime_error("nullptr");
	}
	uni_matrices->data().view = camera.make_view_mat();
	uni_matrices->data().model_view_projection = projection->make_projection_mat() * uni_matrices->data().view * uni_matrices->data().model;
	uni_matrices->buffer();
}

static bool hasEnding(const std::string& string, const std::string& ending) {
	if (string.length() >= ending.length()) {
		return (0 == string.compare(string.length() - ending.length(), ending.length(), ending));
	} else {
		return false;
	}
}

static GLFWwindow* make_window() {
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	return glfwCreateWindow(1024, 1024, "SKTK Viewer", nullptr, nullptr);
}

static void enable_debug_output() {
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param) {
		if (type == GL_DEBUG_TYPE_OTHER) {
			return;
		}
		std::cerr
			<< GLError::source_to_string(source) << " - "
			<< GLError::type_to_string(type) << " - "
			<< GLError::severity_to_string(severity) << ": "
			<< std::string(message, length) << std::endl;
	}, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_ERROR, GL_DONT_CARE, 0, nullptr, true);
}

static void init_common() {
	std::cout << "Init Graphics-Backend..." << std::endl;
	// Handle GLFW-Errors
	glfwSetErrorCallback([](int error, const char* description) {
		std::cout << description << std::endl;
	});

	if (!glfwInit()) {
		throw std::runtime_error("GLFW Init Failed");
	}

	// Init GLFW Window
	std::cout << "Init Window..." << std::endl;
	window = make_window();
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	// Init GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("GLEW-Init failed");
	}

	enable_debug_output();

	// Enable V-Sync
	glfwSwapInterval(1);

	std::cout << "Setup UI-Callbacks..." << std::endl;
	// Handle Key-Events
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (action == GLFW_PRESS) {
			if (key == GLFW_KEY_ESCAPE) {
				glfwSetWindowShouldClose(window, GL_TRUE);
			}
		}
	});
}

static void run_texture_view() {
	projection = std::make_unique<Ortho>();

	camera.up = glm::vec3(0, 1, 0);
	camera.target = glm::vec3(0, 0, 1);
	camera.pos = glm::vec3(0, 0, 0);
	{
#pragma region uniform_blocks
		std::cout << "Create UnifromBlocks..." << std::endl;
		uni_matrices = std::make_unique<UBO<uni_matrices_t>>();
		uni_matrices->data().model = glm::mat4(1);
		uni_matrices->data().view = glm::mat4(1);
		uni_matrices->data().model_view_projection = glm::mat4(1);
		uni_matrices->buffer();
		uni_matrices->bind(MATRICES_UNIFORM_BLOCKBINDING);
#pragma endregion

		std::cout << "Create Shader..." << std::endl;

		auto image_program = ShaderAssembler::make_program({
			std::make_tuple(GL_VERTEX_SHADER, BASIC_SHADER::IMAGE_PLANE::VERTEX),
			std::make_tuple(GL_FRAGMENT_SHADER, BASIC_SHADER::IMAGE_PLANE::FRAGMENT),
		});

		auto quad_vao = std::make_unique<QuadVAO>();

		Color::set_clearcolor(glm::vec3(0.2));

		GLfloat last_time = static_cast<GLfloat>(glfwGetTime());
		{
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);
			update_uni_matrices(width, height);
		}

		bool wireframe(false);
		int img(0);

#ifdef IMGUI_ENABLE
		ImGui_ImplGlfwGL3_Init(window, true);
#endif // IMGUI_ENABLE

		std::cout << "Start Render-Loop..." << std::endl;
		while (!glfwWindowShouldClose(window)) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glfwPollEvents();
#ifdef IMGUI_ENABLE
			ImGui_ImplGlfwGL3_NewFrame();
#endif // IMGUI_ENABLE

			int width, height;
			glfwGetFramebufferSize(window, &width, &height);
			glViewport(0, 0, width, height);

			auto ratio(static_cast<float>(width) / static_cast<float>(height));
			auto half_width(static_cast<float>(width) / 2);
			auto half_height(static_cast<float>(height) / 2);

			// Delta-Time Management
			const GLfloat time = static_cast<GLfloat>(glfwGetTime());
			GLfloat dt(time - last_time);
			last_time = time;

			// Update Uniform Buffers
			update_uni_matrices(width, height);


			{	
				auto map_itter(textures.begin());
				std::advance(map_itter, img);
				map_itter->second->bind(TEXTURE_DIFFUSE_BINDING);
				if (wireframe) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}
				glRAII::Use_Program use_prog(image_program->id());
				glRAII::Bind_VAO bind_vao(quad_vao->id());
				QuadVAO::DRAW_CALL.execute();
			}

#ifdef IMGUI_ENABLE
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				ImGui::SetNextWindowPos(ImVec2(8, 8), ImGuiSetCond_FirstUseEver);
				ImGui::SetNextWindowSize(ImVec2(256, 96), ImGuiSetCond_FirstUseEver);
				ImGui::Begin("Settings");
				ImGui::Checkbox("Wireframe", &wireframe);
				ImGui::SliderInt("Texture", &img, 0, textures.size() - 1);
				ImGui::End();
			}

			ImGui::Render();
#endif // IMGUI_ENABLE

			glfwSwapBuffers(window);
		}

		uni_matrices.release();
	}

	std::cout << "Application Exit Requested..." << std::endl;
	glfwDestroyWindow(window);
	glfwTerminate();
#ifdef IMGUI_ENABLE
	ImGui_ImplGlfwGL3_Shutdown();
#endif // IMGUI_ENABLE
}

static void insert_textures(const std::vector<GXT::Entry_t>& gxt_entries) {
	std::cout << "Try to load " << gxt_entries.size() << " Textures" << std::endl;
	
	for (const auto& gxt_entry : gxt_entries) {
		std::string tex_key(GXT::to_string(gxt_entry));
		std::cout << "Load " << tex_key << std::endl;
		const std::string binary_string(reinterpret_cast<const char*>(gxt_entry.data.data()), gxt_entry.data.size());
		std::istringstream data(binary_string);

		textures.insert_or_assign(tex_key, Texture2D::load_dds(data));
	}
}

static void run_model_view() {
	projection = std::make_unique<Perspective>();

	// Setup Camera
	projection->near = 0.5f;
	projection->far = 512.0f;
	static_cast<Perspective*>(projection.get())->fov = glm::radians(90.0f);

	camera.up = glm::vec3(0, 1, 0);
	camera.target = glm::vec3(0, 0, 0);
	camera.pos = glm::vec3(0, 0, -1);

	// New Block to make sure Heap Objects will be destroyed before GLFW Terminates
	{
#pragma region uniform_blocks
		std::cout << "Create UnifromBlocks..." << std::endl;
		uni_matrices = std::make_unique<UBO<uni_matrices_t>>();
		uni_matrices->data().model = glm::mat4(1);
		uni_matrices->data().view = glm::mat4(1);
		uni_matrices->data().model_view_projection = glm::mat4(1);
		uni_matrices->buffer();
		uni_matrices->bind(MATRICES_UNIFORM_BLOCKBINDING);

		uni_light = std::make_unique<UBO<uni_lights_t>>();
		uni_light->data().light_intensities = glm::vec3(1.0, 1.0, 1.0);
		uni_light->data().light_position = glm::vec3(0, 0.25, 0.5);
		uni_light->buffer();
		uni_light->bind(LIGHT_UNIFORM_BLOCKBINDING);

		uni_material = std::make_unique<UBO<uni_material_t>>();
		uni_material->data().ambient_color = glm::vec3(0.1);
		uni_material->data().diffuse_color = glm::vec3(0.8, 0.8, 0.8);
		uni_material->data().specular_color = glm::vec3(0.2);
		uni_material->buffer();
		uni_material->bind(MATERIAL_UNIFORM_BLOCKBINDING);
#pragma endregion

		std::cout << "Create Shader..." << std::endl;

		auto default_program = ShaderAssembler::make_program({
			std::make_tuple(GL_VERTEX_SHADER, BASIC_SHADER::UNLIT_TEXTURED::VERTEX),
			std::make_tuple(GL_FRAGMENT_SHADER, BASIC_SHADER::UNLIT_TEXTURED::FRAGMENT),
		});

		auto normals_program = ShaderAssembler::make_program({
			std::make_tuple(GL_VERTEX_SHADER, DEBUG_SHADER::DRAW_NORMALS::VERTEX),
			std::make_tuple(GL_GEOMETRY_SHADER, DEBUG_SHADER::DRAW_NORMALS::GEOMETRY),
			std::make_tuple(GL_FRAGMENT_SHADER, DEBUG_SHADER::DRAW_NORMALS::FRAGMENT),
		});

		auto flatuv_program = ShaderAssembler::make_program({
			std::make_tuple(GL_VERTEX_SHADER, DEBUG_SHADER::FLAT_UVCOORDS::VERTEX),
			std::make_tuple(GL_FRAGMENT_SHADER, DEBUG_SHADER::FLAT_UVCOORDS::FRAGMENT),
		});

		auto uvmap_program = ShaderAssembler::make_program({
			std::make_tuple(GL_VERTEX_SHADER, DEBUG_SHADER::UV_MAP::VERTEX),
			std::make_tuple(GL_FRAGMENT_SHADER, DEBUG_SHADER::UV_MAP::FRAGMENT),
		});

		// Setup Depth-Testing
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		Color::set_clearcolor(glm::vec3(0.2));


		GLfloat last_time = static_cast<GLfloat>(glfwGetTime());
		{
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);
			update_uni_matrices(width, height);
		}

#ifdef IMGUI_ENABLE
		ImGui_ImplGlfwGL3_Init(window, true);
#endif // IMGUI_ENABLE

		std::cout << "Start Render-Loop..." << std::endl;
		while (!glfwWindowShouldClose(window)) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glfwPollEvents();
#ifdef IMGUI_ENABLE
			ImGui_ImplGlfwGL3_NewFrame();
#endif // IMGUI_ENABLE

			int width, height;
			glfwGetFramebufferSize(window, &width, &height);
			glViewport(0, 0, width, height);

			auto ratio(static_cast<float>(width) / static_cast<float>(height));
			auto half_width(static_cast<float>(width) / 2);
			auto half_height(static_cast<float>(height) / 2);

			// Delta-Time Management
			const GLfloat time = static_cast<GLfloat>(glfwGetTime());
			GLfloat dt(time - last_time);
			last_time = time;

			// Update Uniform Buffers
			update_uni_matrices(width, height);
			uni_light->buffer();
			uni_material->buffer();

			const auto cam_rot_speed(5.f);

			auto& model_mat = uni_matrices->data().model;
			model_mat = glm::mat4(1);
			auto trans = bounds.min + ((bounds.max - bounds.min) / 2.0f);
			model_mat = glm::scale(model_mat, glm::vec3(1.0f / glm::compMax(bounds.max - bounds.min)));
			model_mat = glm::translate(model_mat, -trans);
			model_mat = glm::rotate(model_mat, time / cam_rot_speed, glm::vec3(0, 1, 0));

			if (draw_uvmap) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glRAII::Use_Program use_prog(uvmap_program->id());
				for (const auto& mesh : meshes) {
					if (mesh) {
						glRAII::Bind_VAO bind_vao(mesh->id());
						mesh->make_drawcall().execute();
					}
				}
			} else {
				if (draw_wireframe) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}
				GLuint program_id = draw_flat_uvs ? flatuv_program->id() : default_program->id();
				glRAII::Use_Program use_prog(program_id);
				for (auto mesh_idx(0); mesh_idx < meshes.size(); ++mesh_idx) {
					const auto& mesh(meshes[mesh_idx]);
					if (mesh) {
						const auto material_name(material_table[mesh_idx]);
						const auto material(textures.find(material_name));
						if (material != textures.end()) {
							material->second->bind(TEXTURE_DIFFUSE_BINDING);
						} else {
							std::cerr << "Can't find Material '" << material_name << "'!" << std::endl;
						}
						glRAII::Bind_VAO bind_vao(mesh->id());
						mesh->make_drawcall().execute();
					}
				}
			}

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			if (draw_normals) {
				glRAII::Use_Program use_prog(normals_program->id());
				for (const auto& mesh : meshes) {
					if (mesh) {
						glRAII::Bind_VAO bind_vao(mesh->id());
						mesh->make_drawcall().execute();
					}
				}
			}

#ifdef IMGUI_ENABLE
			{
				ImGui::SetNextWindowPos(ImVec2(8, 8), ImGuiSetCond_FirstUseEver);
				ImGui::SetNextWindowSize(ImVec2(256, 96), ImGuiSetCond_FirstUseEver);
				ImGui::Begin("Settings");
				ImGui::InputFloat3("Intensity", glm::value_ptr(uni_light->data().light_intensities), 5);
				ImGui::InputFloat3("Position", glm::value_ptr(uni_light->data().light_position), 5);
				ImGui::Checkbox("Show Normals", &draw_normals);
				ImGui::Checkbox("Show UVs", &draw_flat_uvs);
				ImGui::Checkbox("Wireframe", &draw_wireframe);

				ImGui::Checkbox("UVMap", &draw_uvmap);
				ImGui::End();
			}

			ImGui::Render();
#endif // IMGUI_ENABLE

			glfwSwapBuffers(window);
		}

		uni_matrices.release();
		uni_light.release();
		uni_material.release();
	}

	std::cout << "Application Exit Requested..." << std::endl;
	glfwDestroyWindow(window);
#ifdef IMGUI_ENABLE
	ImGui_ImplGlfwGL3_Shutdown();
#endif // IMGUI_ENABLE
	glfwTerminate();
}

static void insert_models(const TMD::RAW::Data_t& tmd_data_raw, const CAT::ResourceEntry_t& entry) {
	std::cout << "Load Model..." << std::endl;

	for (const auto& vert : tmd_data_raw.vertices) {
		bounds.include(glm::vec3(vert.pos[0], vert.pos[1], vert.pos[2]));
	}
	
	TMD::PostProcessed::Data_t tmd_data_processed;
	TMD::post_process(tmd_data_raw, entry.sub_entries, tmd_data_processed);
	for (const auto& in_mesh : tmd_data_processed.meshes) {
		TriangleMesh mesh_out;
		for (const auto& vert : tmd_data_processed.vertices) {
			mesh_out.vertices().emplace_back(vert[0], vert[1], vert[2]);
		}
		for (const auto& face : in_mesh.faces) {
			mesh_out.faces().emplace_back(face[0], face[1], face[2]);
		}
		const auto normal_flip_fac(-1.f);
		for (const auto& normal : tmd_data_processed.normals) {
			auto normalized_normal = TMD::PostProcessed::Data_t::normalize_normals(normal);
			glm::vec3 n(normalized_normal[0], normalized_normal[1], normalized_normal[2]);
			n *= normal_flip_fac;
			n = glm::normalize(n);
			mesh_out.normals().push_back(n);
		}
		for (const auto& uv : tmd_data_processed.uvs) {
			auto normalized_uv = TMD::PostProcessed::Data_t::normalize_uvs(uv);
			mesh_out.texture_coordinates().emplace_back(normalized_uv[0], normalized_uv[1]);
		}
		const auto material(tmd_data_processed.materials[in_mesh.material_id]);
		std::string mat_name(material.package + '/' + material.material_name);
		std::cout << "Create Mesh for " << mat_name << std::endl;
		meshes.push_back(std::make_unique<TriangleMeshVAO>(mesh_out));
		material_table.insert_or_assign(meshes.size() - 1, mat_name);
	}
}

int main(int argc, char** argv) {

	init_common();

	for (auto arg_idx(1); arg_idx < argc; ++arg_idx) {
		std::string filename(argv[arg_idx]);
		try {
			boost::filesystem::path input_file(filename);
			if (input_file.extension().string() == ".cat") {
				std::vector<CAT::ResourceEntry_t> cat_entries;
				std::ifstream file;
				open_to_read(file, input_file);
				CAT::load(input_file, file, cat_entries);
				for (const auto& entry : cat_entries) {
					switch (entry.type) {
						case ElementType_e::TMD:
						case ElementType_e::TMD_TOON:
							{
								std::cout << "Load TMD Sub-File" << std::endl;
								TMD::RAW::Data_t tmd_data_raw;
								TMD::load_raw(file, entry.offset, tmd_data_raw);
								insert_models(tmd_data_raw, entry);
								break;
							}
						case ElementType_e::GXT:
							{
								std::cout << "Load GXT Sub-File" << std::endl;
								std::vector<GXT::Entry_t> gxt_entries;
								GXT::load(file, entry.offset, entry.sub_entries, gxt_entries);
								insert_textures(gxt_entries);
								break;
							}
					}
				}
			} else {
				throw std::runtime_error("Not a known File");
			}
		} catch (const std::exception&) {
			throw std::runtime_error("Can't find given File");
		}
	}
	if (meshes.empty()) {
		if (!textures.empty()) {
			run_texture_view();
		}		
	} else {
		run_model_view();
	}

	std::cout << "Have a nice Day!" << std::endl;
	return EXIT_SUCCESS;
}


