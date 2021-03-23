#include "Bone.h"



Bone::Bone(std::string &name, XMFLOAT3 &Offset, const Bone* parent)
{
	this->name = name;
	AnimOffset = OriginOffset = Offset;
	p_BoneMesh = NULL;
	this->parent = parent;
	AnimRot = OriginRot = XMFLOAT4{0,0,0,0};
	//Check to see if mesh successfully loaded, if not report error
	//assert(LoadMesh());
}


Bone::~Bone()
{
}

const std::string& Bone::GetParentName() const
{
	if (parent != NULL)
		return parent->GetName();
	else
		return std::string();
}

const XMFLOAT3& Bone::GetOffset() const
{
	return OriginOffset;
}


const XMMATRIX* Bone::GetWorldMatrix() const
{
	return &m_WorldMatrix;
}

//void Bone::UpdateWorldMatrix()
//{
//	//If this bone has a parent (every bone but the root) then use the parent to get its own world matrix
//	if (parent != NULL)
//		m_WorldMatrix = m_RotZ * m_RotY * m_RotX * m_Offset * *(parent->GetWorldMatrix());
//	//For the root (which doesn't have a parent) don't use a parent to get its own world matrix
//	else
//		m_WorldMatrix = m_RotZ * m_RotY * m_RotX * m_Offset;
//}

//bool Bone::LoadMesh()
//{
//	//Construct the file name
//	std::string fileName = "Resources/robot/"+ name + ".x";
//
//	//Load the Mesh
//	p_BoneMesh = CommonMesh::LoadFromXFile(Application::s_pApp, fileName.c_str());
//	if (!p_BoneMesh)
//		return false;
//	return true;
//}

const std::string& Bone::GetName() const
{
	return name;
}
