#pragma once

#include <boost\filesystem.hpp>
#include <string>

#include "TMD.h"

class Exporter {
public:
	Exporter();
	virtual ~Exporter() = default;

	bool& uv_export();
	bool uv_export() const;

	bool& normal_export();
	bool normal_export() const;

	bool& material_export();
	bool material_export() const;
	
	float& rescale_factor();
	float rescale_factor() const;

	bool& flip_normals();
	bool flip_normals() const;

	boost::filesystem::path& export_folder();
	const boost::filesystem::path& export_folder() const;

	std::string& material_resource_prefix();
	const std::string& material_resource_prefix() const;

	std::string& material_resource_suffix();
	const std::string& material_resource_suffix() const;
	
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