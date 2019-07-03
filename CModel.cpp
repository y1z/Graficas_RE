#include "CModel.h"
#include "DirectXHeader.h"
#include "Structs.h"

CModel::CModel()
{}


CModel::~CModel()
{}

bool CModel::LoadModelFromFile(const char * FilePath)
{
	Assimp::Importer importer;

	const aiScene* TheScene = importer.ReadFile(FilePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);


	if (TheScene == nullptr)
	{
		return false;
	}

	this->GetTreeInfo(TheScene, TheScene->mRootNode);


	return true;
}


void CModel::GetTreeInfo(const aiScene * Scene, aiNode * node)
{
	// get the meshes

	for (int i = 0; i < Scene->mNumMeshes; i++)
	{
		aiMesh *meshes = Scene->mMeshes[i];
		ExtracMesh(meshes);
	}
	
	// get the children
	for (int i = 0; i < node->mNumChildren; i++) {
		GetTreeInfo(Scene, node->mChildren[i]);
	}


}

void CModel::ExtracMesh(aiMesh * meshInfo)
{
	std::vector<unsigned short> Indeces;
#ifdef USING_DIRECTX

	std::vector<SimpleVertex> Vertexes;
	// geting the vertexes  
	for (int i = 0; i < meshInfo->mNumVertices; ++i)
	{
		SimpleVertex ResultVertex;

		ResultVertex.Pos.x = meshInfo->mVertices[i].x;
		ResultVertex.Pos.y = meshInfo->mVertices[i].y;
		ResultVertex.Pos.z = meshInfo->mVertices[i].z;

		ResultVertex.Tex.y = 0.1f;
		ResultVertex.Tex.x = 0.1f;
		// check for texture 
		//if (meshInfo->mTextureCoords[0])
		//{
		//	ResultVertex.Tex.y = (float) meshInfo->mTextureCoords[0][i].y;
		//	ResultVertex.Tex.x = (float) meshInfo->mTextureCoords[0][i].x;
		//}

		Vertexes.emplace_back(ResultVertex);
	}

	// getting the indeces 
	for (int i = 0; i < meshInfo->mNumFaces; ++i)
	{

		unsigned short indecesArray[3];
		indecesArray[0] = meshInfo->mFaces[i].mIndices[0];
		indecesArray[1] = meshInfo->mFaces[i].mIndices[1];
		indecesArray[2] = meshInfo->mFaces[i].mIndices[2];

		Indeces.emplace_back(indecesArray[0]);
		Indeces.emplace_back(indecesArray[1]);
		Indeces.emplace_back(indecesArray[2]);
	}
	// add mesh 
	m_Meshs.emplace_back(Indeces, Vertexes);

#endif // USING_DIRECTX


}



