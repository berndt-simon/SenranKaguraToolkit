#pragma once

#include <inttypes.h>

namespace tmd {
	 
	const uint32_t MAGIC_NUMBER = 0x30646D74;
	
	template<typename T = int32_t>
	struct Bone_Info_Struct {
		T bone[4];
	};

	template<typename T = int32_t>
	struct ActualBoneInfo_Struct {
		T bone[4];
	};

	template<typename T = int32_t>
	struct Weight_Info_Struct {
		T weight[4];
	};

	template<typename T = int32_t>
	struct weight_data {
		T boneids;
		T weights;
	};
			
	struct Bounds{
		struct {
			float x, y, z;
		} min, max;	
	};

	struct Face {
		uint16_t vert[3];
	};

	struct Vert {
		float pos[3];
	};

	struct BonePos {
		float pos[3];
	};

	struct UV {
		float coord[3];
	};

}