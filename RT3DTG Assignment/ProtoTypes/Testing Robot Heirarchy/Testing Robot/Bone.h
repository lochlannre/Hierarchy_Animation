#pragma once
#include <string>
//#include "Application.h"
#include <cassert>
#include "Support.h"

class Bone
{
public:
	//Note name should be the same as the filename minus the extensions
	Bone(std::string &name, XMFLOAT3 &Offset, const Bone* parent = NULL);
	~Bone();

	//Returns the world matrix of the bone, used by the child of this bone to set its own world matrix
	const XMMATRIX* GetWorldMatrix() const;

	//Loads the mesh for the bone
	bool LoadMesh();

	const std::string& GetName() const;
	void UpdateWorldMatrix();
	const std::string& GetParentName() const;
	const XMFLOAT3& GetOffset() const;

	//Some function to update the rot/pos/offset of the bone (aka run the animation)
	

private:
	std::string name;
	const Bone* parent;

	//Offset of the bone from the parent - relative position
	XMFLOAT3 f3_Offset;

	//Rotation of the bone
	XMFLOAT4 f4_Rot;

	//Matrices storing the rotations of the bone
	XMMATRIX m_RotX;
	XMMATRIX m_RotY;
	XMMATRIX m_RotZ;
	XMMATRIX m_Offset;
	XMMATRIX m_WorldMatrix;


	//Mesh
	CommonMesh* p_BoneMesh = NULL;
};