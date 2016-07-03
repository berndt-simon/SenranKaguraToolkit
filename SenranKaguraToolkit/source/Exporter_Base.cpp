#include "Exporter_Base.h"

Exporter::Exporter()
	: _export_folder()
	, _export_normals(true)
	, _export_uvs(true)
	, _export_materials(true)
	, _rescale_fator(1.0f)
	, _flip_normals(false) {
};

void Exporter::enable_uv_export(bool enable) {
	_export_uvs = enable;
}

void Exporter::enable_normal_export(bool enable) {
	_export_normals = enable;
}

void Exporter::enable_material_export(bool enable) {
	_export_materials = enable;
}

void Exporter::set_rescale_factor(float rescale_factor) {
	_rescale_fator = rescale_factor;
}

void Exporter::set_flip_normals(bool flip) {
	_flip_normals = flip;
}

boost::filesystem::path& Exporter::export_folder() {
	return _export_folder;
}
