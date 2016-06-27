#include <fstream>
#include <iostream>

#include "TMD.h"
#include "FileProcessing.h"
#include <vector>

void processTMD(std::ifstream& file) {
	const std::streampos tmdStart = file.tellg();

	std::vector<int32_t> BoneParent_array;
	std::vector<BonePos> BonePos_array;
	// 	BoneArray = #()
	// 	PolyGroupAmount_array = #()
	// 	PolyGroupAdd_array = #()
	std::vector<uint16_t> PolyGroupAmount_array;
	std::vector<uint32_t> PolyGroupAdd_array;
	std::vector<int32_t> RigBone_array;
	std::vector<Face> 	Face_array;
	// 	Face_array2 = #()
	std::vector<Vert> 	Vert_array;
	// 	Normal_array = #()
	std::vector<UV> 	UV_array;
	// 	Color_array = #()
	// 	Alpha_array = #()
	std::vector<Weight_Info_Struct<>> W1_array;
	std::vector<Bone_Info_Struct<>> B1_array;
	// 	Weight_array = #()*/


	uint32_t magic_number = read<uint32_t>(file);
	if (magic_number != MAGIC_NUMBER) {
		reportAndThrow("Magic-Number missmatch!");
	}

	uint16_t Unknown1 = read<uint16_t>(file);
	uint8_t VertFormat = read<uint8_t>(file);
	uint8_t VertFormat2 = read<uint8_t>(file);
	/*-- 00000001 - Vertex ?
	-- 00000010 - Normals
	-- 00000100 - UV
	-- 00001000 - UV 2
	-- 00010000 - UV 3 ?
	-- 00100000 - Colors
	-- 01000000 - Always on ?
	-- 10000000 - Rigging
	-- 0x61 01100001 | 0x00 / 0000 (NepU BootstrapAct) - Vertex, colors
	-- 0x65 01100101 | 0x00 / 0000 (NepU BGs) - Vertex, colors, UV
	-- 0x65 01100101 | 0x0D / 1101 (NepU Effects) - Vertex, colors, UV
	-- 0xC7 11000111 | 0x0D / 1101 (SK Chars) - Vertex, rigging, normals, UV
	-- 0xCF 11001111 | 0x0D / 1101 (SK Chars) - Vertex, rigging, normals, UV, UV2
	-- 0xE7 11100111 | 0x08 / 1000 (NepU Chars) - Vertex, rigging, normals, colors, UV*/
	uint16_t Unknown3 = read<uint16_t>(file);
	uint16_t Unknown4 = read<uint16_t>(file);
	uint16_t Unknown5 = read<uint16_t>(file);
	uint16_t Unknown6 = read<uint16_t>(file);

	Bounds bounds;
	bounds.min.x = read<float>(file);
	bounds.min.y = read<float>(file);
	bounds.min.z = read<float>(file);

	bounds.max.x = read<float>(file);
	bounds.max.y = read<float>(file);
	bounds.max.z = read<float>(file);

	uint32_t UnknownStart1 = read<uint32_t>(file);
	uint32_t UnknownCount1 = read<uint32_t>(file);

	uint32_t CommandsStart = read<uint32_t>(file);
	uint32_t CommandsCount = read<uint32_t>(file);
	uint32_t UnknownStart2 = read<uint32_t>(file);
	uint32_t UnknownCount2 = read<uint32_t>(file);

	uint32_t RigStart = read<uint32_t>(file);
	uint32_t RigCount = read<uint32_t>(file);
	uint32_t ShaderStart = read<uint32_t>(file);
	uint32_t ShaderCount = read<uint32_t>(file);

	uint32_t PolyGroupStart = read<uint32_t>(file);
	uint32_t PolyGroupCount = read<uint32_t>(file);
	uint32_t FaceStart = read<uint32_t>(file);
	uint32_t FaceCount = read<uint32_t>(file);

	uint32_t VertexCount = read<uint32_t>(file);
	uint32_t VertexStart = read<uint32_t>(file);
	uint32_t UnknownOff1 = read<uint32_t>(file);
	uint32_t UnknownOff2 = read<uint32_t>(file);

	uint32_t UnknownOff3 = read<uint32_t>(file);
	uint32_t UnknownOff4 = read<uint32_t>(file);
	uint32_t UnknownOff5 = read<uint32_t>(file);
	uint32_t UnknownOff6 = read<uint32_t>(file);

	uint32_t UnknownOff7 = read<uint32_t>(file);
	uint32_t BoneStart = read<uint32_t>(file);
	uint32_t BoneCount = read<uint32_t>(file);
	uint32_t BoneHierarchyStart = read<uint32_t>(file);

	uint32_t BoneHierarchyCount = read<uint32_t>(file);
	uint32_t TexInfoStart = read<uint32_t>(file);
	uint32_t TexInfoCount = read<uint32_t>(file);
	uint32_t Blank = read<uint32_t>(file);

	file.seekg(UnknownStart1, std::ios::beg);
	std::cout << "Info1 Start: 0x" << std::hex << file.tellg() << ", Count = " << UnknownCount1 << std::endl;
	for (decltype(UnknownCount1) g = 1; g <= UnknownCount1; g++) {
		bounds.min.x = read<float>(file);
		bounds.min.y = read<float>(file);
		bounds.min.z = read<float>(file);
		bounds.max.x = read<float>(file);
		bounds.max.y = read<float>(file);
		bounds.max.z = read<float>(file);
		uint32_t UnknownA1 = read<uint32_t>(file);
		uint32_t UnknownA2 = read<uint32_t>(file);
	}
	std::cout << "Info1 End: 0x" << std::hex << file.tellg() << std::endl;

	file.seekg(RigStart, std::ios::beg);
	std::cout << "Rig Start: 0x" << std::hex << file.tellg() << ", Count = " << RigCount << std::endl;
	for (decltype(RigCount) g = 1; g <= RigCount; g++) {
		int32_t RigIDCount = read<int32_t>(file) + 1;
		for (int h = 1; h <= 32; h++) {
			int32_t RigBone = read<int32_t>(file) + 1;
			if (h < RigIDCount) {
				RigBone_array.push_back(RigBone);
			} else {
				// TODO
				// append RigBone_array undefined
			}
		}
	}
	std::cout << "Rig End: 0x" << std::hex << file.tellg() << std::endl;

	file.seekg(ShaderStart, std::ios::beg);
	std::cout << "Shader Start: 0x" << std::hex << file.tellg() << ", Count = " << ShaderCount << std::endl;
	for (decltype(ShaderCount) g = 1; g <= ShaderCount; g++) {
		file.seekg(0xC8, std::ios::cur);
	}
	std::cout << "Shader End: 0x" << std::hex << file.tellg() << std::endl;

	file.seekg(PolyGroupStart, std::ios::beg);
	std::cout << "PolyGroup Start: 0x" << std::hex << file.tellg() << ", Count = " << PolyGroupCount << std::endl;
	for (decltype(PolyGroupCount) g = 1; g <= PolyGroupCount; g++) {
		uint16_t PolyGroupAmount = read<uint16_t>(file);
		uint16_t PolyGroupUnknown = read<uint16_t>(file);
		uint32_t PolyGroupAdd = read<uint32_t>(file);
		PolyGroupAmount_array.push_back(PolyGroupAmount);
		PolyGroupAdd_array.push_back(PolyGroupAdd);
	}
	std::cout << "PolyGroup End: 0x" << std::hex << file.tellg() << std::endl;

	file.seekg(FaceStart, std::ios::beg);
	std::cout << "Face Start: 0x" << std::hex << file.tellg() << ", Count = " + FaceCount << std::endl;
	for (decltype(FaceCount) g = 1; g <= FaceCount; g++) {
		uint16_t f1 = read<uint16_t>(file) + 1;
		uint16_t f2 = read<uint16_t>(file) + 1;
		uint16_t f3 = read<uint16_t>(file) + 1;
		Face_array.push_back(Face{ f1, f2, f3 });
	}
	std::cout << "Face End: 0x" << std::hex << file.tellg() << std::endl;

	file.seekg(TexInfoStart, std::ios::beg);
	std::cout << "Texture Info Start: 0x" << std::hex << file.tellg() << ", Count = " + TexInfoCount << std::endl;
	for (decltype(TexInfoCount) g = 1; g <= TexInfoCount; g++) {
		uint32_t TexNum = read<uint32_t>(file);
		uint32_t Blank = read<uint32_t>(file);
		uint32_t UnknownTex = read<uint32_t>(file);
		uint16_t TexU = read<uint16_t>(file);
		uint16_t TexV = read<uint16_t>(file);
	}
	std::cout << "Texture Info End: 0x" << std::hex << file.tellg() << std::endl;

	file.seekg(VertexStart, std::ios::beg);
	std::cout << "Vertex Start: 0x" << std::hex << file.tellg() << ", Count = " << VertexCount << std::endl;
	for (decltype(VertexCount) g = 1; g <= VertexCount; g++) {
		float tu = 0;
		float tv = 0;
		float vx = read<float>(file);
		float vy = read<float>(file);
		float vz = read<float>(file);
		if (VertFormat > 0x6F) {
			float weight1 = static_cast<float>(read<uint8_t>(file)) / 255.0f;
			float weight2 = static_cast<float>(read<uint8_t>(file)) / 255.0f;
			float weight3 = static_cast<float>(read<uint8_t>(file)) / 255.0f;
			float weight4 = static_cast<float>(read<uint8_t>(file)) / 255.0f;
			uint8_t	bone1 = read<uint8_t>(file) + 1;
			uint8_t	bone2 = read<uint8_t>(file) + 1;
			uint8_t	bone3 = read<uint8_t>(file) + 1;
			uint8_t	bone4 = read<uint8_t>(file) + 1;
		}
		if (VertFormat == 0xC7 || VertFormat == 0xCF || VertFormat == 0xE7) {
			int8_t nx = read<int8_t>(file);
			int8_t ny = read<int8_t>(file);
			int8_t nz = read<int8_t>(file);
			int8_t nq = read<int8_t>(file);
		}
		if (VertFormat == 0x61 || VertFormat == 0x65 || VertFormat == 0xE7) {
			uint8_t colorr = read<uint8_t>(file);
			uint8_t colorg = read<uint8_t>(file);
			uint8_t colorb = read<uint8_t>(file);
			float colora = static_cast<float>(read<uint8_t>(file)) / 255.0f;
		}
		if (VertFormat > 0x61) {
			tu = static_cast<float>(read<int16_t>(file)) / 1024.0f;
			tv = static_cast<float>(read<int16_t>(file)) / -1024.0f;
		}
		if (VertFormat == 0xCF) {
			float tu2 = static_cast<float>(read<int16_t>(file)) / 1024.0f;
			float tv2 = static_cast<float>(read<int16_t>(file)) / -1024.0f;
		}

		Vert_array.push_back(Vert{ vx, vy, vz });
		UV_array.push_back(UV{ tu, tv, 0 });
		// TODO
		// append B1_array(Bone_Info_Struct Bone1 : Bone1 Bone2 : Bone2 Bone3 : Bone3 Bone4 : Bone4)
		// append W1_array(Weight_Info_Struct Weight1 : Weight1 Weight2 : Weight2 Weight3 : Weight3 Weight4 : Weight4)
	}
	std::cout << "Vertex End: 0x" << std::hex << file.tellg() << std::endl;

	file.seekg(BoneHierarchyStart, std::ios::beg);
	std::cout << "Bone Hierarchy Start: 0x" << std::hex << file.tellg() << ", Count = " << BoneHierarchyCount << std::endl;
	for (decltype(BoneCount) b = 1; b <= BoneHierarchyCount; b++) {
		uint32_t BoneHash = read<uint32_t>(file);
		float BoneHX = read<float>(file);
		float BoneHY = read<float>(file);
		float BoneHZ = read<float>(file);
		int32_t BoneParent = read<int32_t>(file);
		uint32_t	BoneH0 = read<uint32_t>(file);
		BoneParent_array.push_back(BoneParent);
		BonePos_array.push_back(BonePos{ BoneHX, BoneHY, BoneHZ });
	}
	std::cout << "Bone Hierarchy End: 0x" << std::hex << file.tellg() << std::endl;

	file.seekg(BoneStart, std::ios::beg);
	std::cout << "Bone Start: 0x" << std::hex << file.tellg() << ", Count = " << BoneCount << std::endl;
	for (decltype(BoneCount) b = 1; b <= BoneCount; b++) {
		std::string BoneName = "Bone" + b;
		auto BoneParent = BoneParent_array[b];
		float m11 = read<float>(file); float m12 = read<float>(file); float m13 = read<float>(file); float m14 = read<float>(file);
		float m21 = read<float>(file); float m22 = read<float>(file); float m23 = read<float>(file); float m24 = read<float>(file);
		float m31 = read<float>(file); float m32 = read<float>(file); float m33 = read<float>(file); float m34 = read<float>(file);
		float m41 = read<float>(file); float m42 = read<float>(file); float m43 = read<float>(file); float m44 = read<float>(file);
		// TODO
		// tfm = matrix3[m11, m12, m13][m21, m22, m23][m31, m32, m33][m41, m42, m43]
		// tfm.row4 = BonePos_array[b]

		// TODO
		// if (getNodeByName BoneName) != undefined do (
		// append BoneArray(getNodeByName BoneName)
		// )

		// TODO
		// if (getNodeByName BoneName) == undefined do (
		//		if (BoneParent != -1) do (
		//			tfm = tfm * BoneArray[(BoneParent + 1)].objecttransform
		//		)
		// )

		// TODO
		/*
		newBone = bonesys.createbone	\	tfm.row4	\	(tfm.row4 + 0.01 * (normalize tfm.row1)) \	(normalize tfm.row3)
			newBone.name = BoneName
			newBone.width = 0.03
			newBone.height = 0.03
			newBone.transform = tfm
			newBone.setBoneEnable false 0
			newBone.wirecolor = yellow
			newbone.showlinks = true
			newBone.pos.controller = TCB_position()
			newBone.rotation.controller = TCB_rotation()
		if (BoneParent != -1) {
			newBone.parent = BoneArray[BoneParent + 1]
		}
		append BoneArray newBone
		*/
	}
	std::cout << "Bone End: 0x" << std::hex << file.tellg() << std::endl;

	file.seekg(CommandsStart, std::ios::beg);
	std::cout << "Commands Start: 0x" << std::hex << file.tellg() << ", Count = " << CommandsCount << std::endl;
	int MatID = 1;
	int RigID = 1;
	for (decltype(CommandsCount) g = 1; g <= CommandsCount; g++) {
		// TODO
		// Face_array2 = #()
		// ActualB1_array = #()
		// Weight_array = #()

		uint8_t CommandID = read<uint8_t>(file) + 1;
		uint8_t Command = read<uint8_t>(file);

		if (Command == 0x20) {
			MatID = CommandID;
		}

		if (Command == 0x40) {
			RigID = CommandID;
		}

		if (Command == 0x30) {
			// TODO
			// for (int y = 1; y <= PolyGroupAmount_array[CommandID]; y++) {
				// append Face_array2 Face_array[y + PolyGroupAdd_array[CommandID]]
			//}

			int RigShift = (32 * RigID) - 32;

			if (VertFormat > 0x6F) {
				for (int x = 1; x <= B1_array.size(); x++) {
					// TODO
					auto ImproperBone1 = B1_array[x].bone[0];
					auto ImproperBone2 = B1_array[x].bone[1];
					auto ImproperBone3 = B1_array[x].bone[2];
					auto ImproperBone4 = B1_array[x].bone[3];
					auto ProperBone1 = RigBone_array[ImproperBone1 + RigShift];
					auto ProperBone2 = RigBone_array[ImproperBone2 + RigShift];
					auto ProperBone3 = RigBone_array[ImproperBone3 + RigShift];
					auto ProperBone4 = RigBone_array[ImproperBone4 + RigShift];
					// if ProperBone1 == undefined do(ProperBone1 = 1)
					// if ProperBone2 == undefined do(ProperBone2 = 1)
					// if ProperBone3 == undefined do(ProperBone3 = 1)
					// if ProperBone4 == undefined do(ProperBone4 = 1)
					// append ActualB1_array(ActualBoneInfo_Struct Bone1 : ProperBone1 Bone2 : ProperBone2 Bone3 : ProperBone3 Bone4 : ProperBone4)
				}

				for (int b = 1; b <= W1_array.size(); b++) {
					// TODO
					// w = (weight_data boneids : #() weights : #())
					auto maxweight = 0;

					if (W1_array[b].weight[0] != 0) {
						maxweight = maxweight + W1_array[b].weight[0];
					}
					if (W1_array[b].weight[1] != 0) {
						maxweight = maxweight + W1_array[b].weight[1];
					}
					if (W1_array[b].weight[2] != 0) {
						maxweight = maxweight + W1_array[b].weight[2];
					}
					if (W1_array[b].weight[3] != 0) {
						maxweight = maxweight + W1_array[b].weight[3];
					}

					if (maxweight != 0) {
						if (W1_array[b].weight[0] != 0) {
							float w1 = static_cast<float>(W1_array[b].weight[0]);
							// append w.boneids(ActualB1_array[b].Bone1)
							// append w.weights(w1)
						}
						if (W1_array[b].weight[1] != 0) {
							float w2 = static_cast<float>(W1_array[b].weight[1]);
							// append w.boneids(ActualB1_array[b].Bone2)
							// append w.weights(w2)
						}
						if (W1_array[b].weight[2] != 0) {
							float w3 = static_cast<float>(W1_array[b].weight[2]);
							// append w.boneids(ActualB1_array[b].Bone3)
							// append w.weights(w3)
						}
						if (W1_array[b].weight[3] != 0) {
							float w4 = static_cast<float>(W1_array[b].weight[3]);
							// append w.boneids(ActualB1_array[b].Bone4)
							// append w.weights(w4)
						}
					}
					//append Weight_array w
				}
			}

			// TODO
			/*
			msh = mesh vertices : Vert_array faces : Face_array2
			msh.numTVerts = Vert_array.count
			setNumCPVVerts msh msh.numTVerts
			-- setCVertMode msh true
			--setShadeCVerts msh true
			defaultVCFaces msh
			buildTVFaces msh
			-- for j = 1 to Color_array.count do setvertcolor msh j Color_array[j]
			-- for j = 1 to Alpha_array.count do(meshop.setVertAlpha msh - 2 j Alpha_array[j])
			*/
			for (int j = 1; j <= Vert_array.size(); j++) {
				//setTVert msh j UV_array[j];
			}
			// for (int j = 1; j <= Face_array2.size(); j++) {
			//	setTVFace msh j Face_array2[j];
			//}
			// for (int j = 1; j <= msh.numfaces; j++) {
			// 	setFaceSmoothGroup msh j 1;
			// }

			// msh.name = (MatID as string);

			if (VertFormat > 0x6F) {
				//	max modify mode
				//	select msh

				//	skinMod = skin()
				//	addModifier msh skinMod

				// for (int i = 1; i <= BoneArray.size(); i++) {
				// 	maxbone = getnodebyname BoneArray[i].name;
				// 	if (i != BoneCount) {
						// skinOps.addBone skinMod maxbone 0
						// 	} else {
						// skinOps.addBone skinMod maxbone 1
						// 	}

						// }

				//modPanel.setCurrentObject skinMod

				/*
				for (int i = 1; i <= Weight_array.count; i++) {
					w = Weight_array[i]
						bi = #()
						wv = #()

						for (int j = 1; j <= w.boneids.count; j++) {
							boneid = w.boneids[j];
							weight = w.weights[j];
							bi.push_back(boneid);
							wv.push_back(weight);
						}

					skinOps.ReplaceVertexWeights skinMod i bi wv
				}
				*/
			}

		}

		if (Command == 0x10) {
			g = CommandsCount;
		}

	}
	std::cout << "Commands End: 0x" << std::hex << file.tellg() << std::endl;
	std::cout << "Ended: 0x" << std::hex << file.tellg() << std::endl;
}
