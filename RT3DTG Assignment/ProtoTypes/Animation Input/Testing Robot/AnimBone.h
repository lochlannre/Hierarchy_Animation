#pragma once
#include <vector>
#include "Support.h"


class AnimBone
{
public:
	AnimBone();
	~AnimBone();

	void addTransPair(float key, XMFLOAT3& Trans);
	void addRotPair(float key, XMFLOAT3& Rot);

	//XMMatrix GetMatrix(float time);

private:
	//Stores the key frame translation pairs for each Bone
	std::vector<std::pair<float, XMFLOAT3>> transVector;
	//Stores the key frame rotation pairs for each Bone
	std::vector<std::pair<float, XMFLOAT3>> rotVector;
};

