#include "Exporter_Base.h"

Exporter::Exporter()
	: _export_folder()
	, _export_normals(true)
	, _export_uvs(true)
	, _export_materials(true)
	, _rescale_fator(1.0f)
	, _flip_normals(false)
	, _material_resource_suffix(".dds")
	, _material_resource_prefix("..\\") {
}

bool& Exporter::uv_export() {
	return _export_uvs;
}
bool Exporter::uv_export() const {
	return _export_uvs;
}

bool& Exporter::normal_export() {
	return _export_normals;
}
bool Exporter::normal_export() const {
	return _export_normals;
}

bool& Exporter::material_export() {
	return _export_materials;
}
bool Exporter::material_export() const {
	return _export_materials;
}

float& Exporter::rescale_factor() {
	return _rescale_fator;
}
float Exporter::rescale_factor() const {
	return _rescale_fator;
}

bool& Exporter::flip_normals() {
	return _flip_normals;
}
bool Exporter::flip_normals() const {
	return _flip_normals;
}

boost::filesystem::path& Exporter::export_folder() {
	return _export_folder;
}
const boost::filesystem::path& Exporter::export_folder() const {
	return _export_folder;
}

std::string& Exporter::material_resource_prefix() {
	return _material_resource_prefix;
}
const std::string& Exporter::material_resource_prefix() const {
	return _material_resource_prefix;
}

std::string& Exporter::material_resource_suffix() {
	return _material_resource_suffix;
}
const std::string& Exporter::material_resource_suffix() const {
	return _material_resource_suffix;
}