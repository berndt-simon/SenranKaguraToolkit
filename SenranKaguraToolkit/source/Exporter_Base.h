#pragma once

#include <boost\filesystem.hpp>
#include <string>

#include "TMD.h"

class Exporter {
public:
	Exporter();

	void enable_uv_export(bool enable = true);
	void enable_normal_export(bool enable = true);
	void enable_material_export(bool enable = true);

	void set_rescale_factor(float rescale_factor);
	void set_flip_normals(bool flip = true);

	boost::filesystem::path& export_folder();

	std::string& material_resource_prefix();
	std::string& material_resource_suffix();
	
	virtual void save(const TMD::PP::Data_t& data) = 0;

protected:
	bool _export_normals;
	bool _export_uvs;
	bool _export_materials;

	boost::filesystem::path _export_folder;
	std::string _material_resource_prefix;
	std::string _material_resource_suffix;

	float _rescale_fator;

	bool _flip_normals;

};