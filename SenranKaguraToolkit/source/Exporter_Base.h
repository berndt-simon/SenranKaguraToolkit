#pragma once

#include <boost\filesystem.hpp>

#include "Filetype_TMD.h"

class Exporter {
public:
	Exporter();

	void enable_uv_export(bool enable = true);
	void enable_normal_export(bool enable = true);
	void enable_material_export(bool enable = true);

	void set_rescale_factor(float rescale_factor);
	void set_flip_normals(bool flip = true);

	boost::filesystem::path& export_folder();
	
	virtual void save(const TMD::PP::Data_t& data) = 0;

protected:
	bool _export_normals;
	bool _export_uvs;
	bool _export_materials;

	boost::filesystem::path _export_folder;

	float _rescale_fator;

	bool _flip_normals;

};