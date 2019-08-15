#include "CModel.h"
#include "CDevice.h"
#include "CDeviaceContext.h"
#include "CCamera.h"
#include "CSwapChian.h"
#include "DirectXHeader.h"
#include "Structs.h" // is used when directX is active
#include "Custom_Structs.h"// is used when opengl is active 
#include <array>

CModel::CModel()
{}

CModel::~CModel()
{
	for (CMesh * mesh : m_Meshs)
	{
		delete mesh;
		mesh = nullptr;
	}

}

bool CModel::LoadModelFromFile(const char * FilePath, CDevice &Device)
{
	Assimp::Importer importer;

	const aiScene* TheScene = importer.ReadFile(FilePath, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);

	if (TheScene == nullptr)
	{
		return false;
	}
	

	this->GetTreeInfo(TheScene, TheScene->mRootNode, Device);

	return true;
}

void CModel::DrawAllMeshes(CDeviceContext &DeviceContext, std::vector<CBuffer *> &BufferArray, CCamera &camera)
{
#ifdef USING_DIRECTX
	CBNeverChanges_View cbNeverChanges;
	CBChangesEveryFrame_World cb;
	CBChangeOnResize_Projection cbChangesOnResize;
	Light light;

	for (CMesh* mesh : m_Meshs)
	{
		DeviceContext.IASetIndexBuffer(mesh->GetIndexBuffer(), 57, 0);
		DeviceContext.IASetVertexBuffers(0, 1, mesh->GetVertexBuffer(), mesh->GetVertexBuffer().GetStride(), mesh->GetVertexBuffer().GetOffset());
		DeviceContext.DrawIndexed(mesh->GetIndexBuffer().GetElementCount(), 0, 0);
	}

	cbNeverChanges.mView = XMMatrixTranspose(camera.GetViewMatrice());
	cbNeverChanges.AtVector = camera.GetFront();
	DeviceContext.UpdateSubresource(*BufferArray[0], static_cast<void*> (&cbNeverChanges), 0);

	float MyCubeColor[4] = { 0.7f,0.7f,0.05f,1.0f };

	cb.mWorld = XMMatrixTranspose(XMMatrixIdentity() * camera.GetTrasformMatrice());
	cb.vMeshColor = MyCubeColor;
	DeviceContext.UpdateSubresource(*BufferArray[1], static_cast<void*>(&cb), 0);

	cbChangesOnResize.mProjection = XMMatrixTranspose(camera.GetProyectionMatrice());
	DeviceContext.UpdateSubresource(*BufferArray[2], static_cast<void*>(&cbChangesOnResize), 0);

	light.Pos = camera.GetEye() + glm::vec4(100.0f, 0.0f, 10.0f, 1.0f);
	light.Color;// = glm::vec4{ 1.0f,.5f,1.0f,1.0f };
	light.Direction = camera.GetFront();// glm::vec3(-10.0f, 0.0f, 0.0f);
	light.Intensity = 1.0f;


	DeviceContext.UpdateSubresource(*BufferArray[3], static_cast<void*>(&light), 0);

#else

	for (CMesh* mesh : m_Meshs)
	{
		mesh->PrepareForDarwCall(DeviceContext);
		DeviceContext.DrawIndexed(mesh->GetIndexBuffer());
	}

#endif
}

uint64_t CModel::GetTotalVertices() const
{
	m_CountVertices = 0;
	for (CMesh * mesh : m_Meshs)
	{
		m_CountVertices += mesh->GetVertexBuffer().GetElementCount();
	}

	return m_CountVertices;
}

void CModel::GetTreeInfo(const aiScene * Scene, aiNode * node, CDevice & Device)
{
	// get the meshes
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *meshes = Scene->mMeshes[node->mMeshes[i]];
		ExtracMesh(meshes, Device);
	}
	// get the children
	for (int j = 0; j < node->mNumChildren; j++)
	{
		GetTreeInfo(Scene, node->mChildren[j], Device);
	}
}

void CModel::ExtracMesh(aiMesh * meshInfo, CDevice & Device)
{
	// indeces are constant 
	std::vector<WORD> Indeces;
	std::vector<VertexPosNormTex> Vertexes;

	// getting the vertexes  
	for (int i = 0; i < meshInfo->mNumVertices; ++i)
	{
		VertexPosNormTex ResultVertex;

		ResultVertex.Pos.x = meshInfo->mVertices[i].x;
		ResultVertex.Pos.y = meshInfo->mVertices[i].y;
		ResultVertex.Pos.z = meshInfo->mVertices[i].z;
		ResultVertex.Pos.w = 1.0f;

	
		//	 check for texture 
		if (meshInfo->HasTextureCoords(0))
		{
			ResultVertex.TexCoord.y = (float) meshInfo->mTextureCoords[0][i].y;
			ResultVertex.TexCoord.x = (float) meshInfo->mTextureCoords[0][i].x;
		}
		else// for when there are no TexCoord's 
		{
			ResultVertex.TexCoord.x = 0.0f;
			ResultVertex.TexCoord.y = 0.0f;
		}

		if (meshInfo->HasNormals())
		{
			ResultVertex.Norm.x = meshInfo->mNormals[i].x;
			ResultVertex.Norm.y = meshInfo->mNormals[i].y;
			ResultVertex.Norm.z = meshInfo->mNormals[i].z;
		}

		Vertexes.emplace_back(ResultVertex);
	}

	// getting the indeces 
	for (int i = 0; i < meshInfo->mNumFaces; ++i)
	{
		WORD indecesArray[3] = { 0,0,0 };

		for (unsigned int j = 0; j < meshInfo->mFaces[i].mNumIndices; ++j)
		{
			indecesArray[j] = meshInfo->mFaces[i].mIndices[j];
		}
		Indeces.emplace_back(indecesArray[0]);
		Indeces.emplace_back(indecesArray[1]);
		Indeces.emplace_back(indecesArray[2]);
	}
	// add mesh 
	m_Meshs.emplace_back(new CMesh(Indeces, Vertexes, Device));

	return;
}