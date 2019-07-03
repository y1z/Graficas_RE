#pragma once
#include "Usable_Windows.h"
#include "DirectXHeader.h"
#include <vector>
#include "CMesh.h"
#include "CBuffer.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class CModel
{
public:
	CModel();
	~CModel();
	//! check if we can load a model from a file 
	bool LoadModelFromFile(const char * FilePath);
	
//	D3D11_BUFFER_DESC GetDescVertex(int Index);
//	D3D11_BUFFER_DESC GetDescIndex(int Index);

	std::vector<CMesh> m_Meshs;
private:
	// go recursively through the tree 
	void GetTreeInfo(const aiScene *Scene, aiNode * node);
	void ExtracMesh(aiMesh * meshInfo);


};

