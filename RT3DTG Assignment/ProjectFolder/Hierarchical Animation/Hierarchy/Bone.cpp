#include "Bone.h"

Bone::Bone(std::string &name, XMFLOAT3 &Offset, const Bone* parent)
{
	this->name = name;
	Offset = Offset;
	Rot = { 0,0,0 };
	this->parent = parent;
	p_BoneMesh = NULL;
	//Check to see if mesh successfully loaded, if not report error
	assert(LoadMesh());
}


Bone::~Bone()
{
	delete p_BoneMesh;
}


const XMMATRIX* Bone::GetWorldMatrix() const
{
	return &m_WorldMatrix;
}

void Bone::UpdateWorldMatrix()
{

	//If this bone has a parent (every bone but the root) then use the parent to get its own world matrix
	if (parent != NULL)
		m_WorldMatrix = m_RotX * m_RotY * m_RotZ * m_Offset * (*(parent->GetWorldMatrix()));
	//For the root (which doesn't have a parent) don't use a parent to get its own world matrix
	else
		m_WorldMatrix = m_RotX * m_RotY * m_RotZ * m_Offset;
}


void Bone::UpdateMatrices(XMFLOAT3& worldOffset)
{
	//Testing thing
	//static bool fudge = true;
	//if (name == "root" && fudge)
	//{
	//	OriginOffset.y += 2;
	//	fudge = false;
	//}
	m_RotX = XMMatrixRotationX(XMConvertToRadians(Rot.x));
	m_RotY = XMMatrixRotationY(XMConvertToRadians(Rot.y));
	m_RotZ = XMMatrixRotationZ(XMConvertToRadians(Rot.z));
	if (name == "root")
	{
		m_Offset = XMMatrixTranslation(Offset.x + worldOffset.x, Offset.y + worldOffset.y, Offset.z + worldOffset.z);
	}
	else
		m_Offset = XMMatrixTranslation(Offset.x, Offset.y, Offset.z);
	UpdateWorldMatrix();
}

bool Bone::LoadMesh()
{
	if (name != "root")
	{
		//Construct the file name
		std::string fileName = "Resources/Robot/" + name + ".x";

		//Load the Mesh
		p_BoneMesh = CommonMesh::LoadFromXFile(Application::s_pApp, fileName.c_str());
		if (!p_BoneMesh)
			return false;
		return true;
	}
	return true;
}

const std::string& Bone::GetName() const
{
	return name;
}

void Bone::Draw()
{
	if (p_BoneMesh != NULL)
	{
		Application::s_pApp->SetWorldMatrix(m_WorldMatrix);
		p_BoneMesh->Draw();
	}
}

//Comment out this bit for testing lerp.h
////////////////////////////////////////////////////////////////////////////////////////
XMFLOAT3 sub(XMFLOAT3& a, XMFLOAT3& b)
{
	return { a.x - b.x, a.y - b.y, a.z - b.z };
}

XMFLOAT3 mul(XMFLOAT3& a, float b)
{
	return { a.x * b, a.y * b, a.z * b };
}

XMFLOAT3 add(XMFLOAT3& a, XMFLOAT3& b)
{
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}

// a + (t * ( b-a ) );
XMFLOAT3& lerp3f(XMFLOAT3& a, XMFLOAT3& b, float t)
{
	assert(t <= 1 && t >= 0);
	return add(mul(sub(b, a), t), a);
	XMFLOAT3 subf3 = sub(b, a); //(b-a)
	XMFLOAT3 mulf3 = mul(subf3, t); //(t * sub)
	XMFLOAT3 addf3 = add(mulf3, a); // (a + mul)
	return addf3;
}
////////////////////////////////////////////////////////////////////////////////////////

//This lerps between the previous position and the next position
void Bone::UpdateAnimation(std::pair<XMFLOAT3, XMFLOAT3>& trans, std::pair<XMFLOAT3, XMFLOAT3>& rot, float time, float current, float next, float endKey)
{
	time = fmod(time, endKey);

	//Lerp value should be between 0 and 1
	float lerpValue = (time - current) / (next - current);


	Offset = lerp3f(trans.second, trans.first, lerpValue);
	Rot = lerp3f(rot.second, rot.first, lerpValue);
	//std:pair<Previous transformation, next Transformation>
	//Offset = lerpf3(trans.second, trans.first, lerpValue);
	//Rot = lerpf3(rot.second, rot.first, lerpValue);
}