#include "AssetLibrary.h"

#include <array>

std::map<texture_key_t, texture_ptr_t>& AssetLibrary::textures() {
	return _textures;
}

const std::map<texture_key_t, texture_ptr_t>& AssetLibrary::textures() const {
	return _textures;
}

std::vector<mesh_ptr_t>& AssetLibrary::meshes() {
	return _meshes;
}

const std::vector<mesh_ptr_t>& AssetLibrary::meshes() const {
	return _meshes;
}

std::vector<ModelAsset>& AssetLibrary::models() {
	return _models;
}

const std::vector<ModelAsset>& AssetLibrary::models() const {
	return _models;
}

std::string readable_texture_key(const texture_key_t& texture_key) {
	return std::get<0>(texture_key) + '/' + std::get<1>(texture_key);
}

std::ostream& operator<<(std::ostream& out, const texture_lib_t& texture_lib) {
	out << "Texture Library:";
	const auto tex_cnt(texture_lib.size());
	for (auto i(0); i < tex_cnt; ++i) {
		auto lib_itter(texture_lib.begin());
		std::advance(lib_itter, i);
		out << std::endl;
		out << "\tTexture #" << i << ": \"" << readable_texture_key(lib_itter->first) << '"';
	}
	return out;
}

const texture_ptr_t& ModelAsset::get_texture(const AssetLibrary& asset_lib, mesh_mat_idx_t material_id) const {
	auto lib_itter(asset_lib.textures().begin());
	const auto mat_id(_local_texture_table[asset_lib.get_nested_or_default(material_id)]);
	if (mat_id < asset_lib.textures().size()) {
		std::advance(lib_itter, mat_id);
		return lib_itter->second;
	}	
	static const texture_ptr_t none(nullptr);
	return none;
}

void ModelAsset::update_local_texture_table(const texture_lib_t& texture_lib, const std::vector<texture_key_t>& references) {
	const auto ref_cnt(references.size());
	_local_texture_table.resize(ref_cnt);
	for (auto i(0); i < ref_cnt; ++i) {
		auto ref_tex(texture_lib.find(references[i]));
		if (ref_tex == texture_lib.end()) {
			ref_tex = texture_lib.find(AssetLibrary::alt_key(references[i]));
		}
		if (ref_tex != texture_lib.end()) {
			_local_texture_table[i] = std::distance(texture_lib.begin(), ref_tex);
		} else {
			std::runtime_error("Can't find " + readable_texture_key(references[i]));
		}
	}
}

std::vector<shaded_mesh_t>& ModelAsset::shaded_meshes() {
	return _meshes;
}

const std::vector<shaded_mesh_t>& ModelAsset::shaded_meshes() const {
	return _meshes;
}

const std::vector<ptrdiff_t>& ModelAsset::local_texture_table() const {
	return _local_texture_table;
}

mesh_mat_idx_t AssetLibrary::get_nested_or_default(mesh_mat_idx_t mat_idx) const {
	try {
		return nested_lookup.at(mat_idx);
	} catch (const std::out_of_range&) {
		return mat_idx;
	}
}

texture_key_t AssetLibrary::alt_key(const texture_key_t& texture_key) {
	static const std::string alt_package("conv0");
	return std::make_tuple(alt_package, std::get<1>(texture_key));
}
