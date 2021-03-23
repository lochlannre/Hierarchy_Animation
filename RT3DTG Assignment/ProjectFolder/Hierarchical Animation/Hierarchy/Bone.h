#pragma once
#include <string>
#include <cassert>
#include <cmath>
//This causes linker errors everywhere and I have no clue why, if you want to test it go to bone.cpp and comment out the indicated section
//#include "Lerp.h"
#include "Application.h"



__declspec(align(32)) class Bone
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

	void Draw();

	void UpdateAnimation(std::pair<XMFLOAT3, XMFLOAT3>& Trans, std::pair<XMFLOAT3, XMFLOAT3>& Rot, float time, float current, float next, float endKey);

	void UpdateMatrices(XMFLOAT3& worldOffset);
	//Some function to update the rot/pos/offset of the bone (aka run the animation)

private:
	std::string name;
	const Bone* parent;

	//Offset of the bone from the parent - relative position
	XMFLOAT3 Offset;
	//Rotation of the bone
	XMFLOAT3 Rot;

	//Matrices storing the rotations of the bone
	XMMATRIX m_RotX;
	XMMATRIX m_RotY;
	XMMATRIX m_RotZ;
	XMMATRIX m_Offset;
	XMMATRIX m_WorldMatrix;

	//Mesh
	CommonMesh* p_BoneMesh = NULL;
};