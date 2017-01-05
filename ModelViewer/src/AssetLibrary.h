#pragma once
 
#include <string>
#include <tuple>
#include <map>
#include <vector>
#include <memory>

#include <Texture.h>
#include <TriangleMesh.h>

using texture_key_t = std::tuple<std::string, std::string>;
using texture_ptr_t = std::unique_ptr<Texture2D>;
using mesh_ptr_t = std::unique_ptr<TriangleMeshVAO>;

using texture_lib_t = std::map<texture_key_t, texture_ptr_t>;

using mesh_lib_idx_t = size_t;
using mesh_mat_idx_t = uint8_t;
using shaded_mesh_t = std::tuple<mesh_lib_idx_t, mesh_mat_idx_t, bool>;

class AssetLibrary;

class ModelAsset {
public:
	ModelAsset() = default;
	~ModelAsset() = default;

	const texture_ptr_t& get_texture(const AssetLibrary& asset_lib, mesh_mat_idx_t material_id) const;

	void update_local_texture_table(const texture_lib_t& texture_lib, const std::vector<texture_key_t>& references);

	std::vector<shaded_mesh_t>& shaded_meshes();
	const std::vector<shaded_mesh_t>& shaded_meshes() const;

	bool visible = true;

	const std::vector<ptrdiff_t>& local_texture_table() const;

private:
	std::vector<shaded_mesh_t> _meshes;
	std::vector<ptrdiff_t> _local_texture_table;
};

class AssetLibrary {
public:
	AssetLibrary() = default;
	~AssetLibrary() = default;

	texture_lib_t& textures();
	const texture_lib_t& textures() const;

	std::vector<mesh_ptr_t>& meshes();
	const std::vector<mesh_ptr_t>& meshes() const;

	std::vector<ModelAsset>& models();
	const std::vector<ModelAsset>& models() const;

	std::vector<int> nested_lookup;
	mesh_mat_idx_t get_nested_or_default(mesh_mat_idx_t mat_idx) const;

	static texture_key_t alt_key(const texture_key_t& texture_key);

private:
	texture_lib_t _textures;
	std::vector<mesh_ptr_t> _meshes;

	std::vector<ModelAsset> _models;
};

std::string readable_texture_key(const texture_key_t& texture_key);

std::ostream& operator<<(std::ostream& out, const texture_lib_t& texture_lib);