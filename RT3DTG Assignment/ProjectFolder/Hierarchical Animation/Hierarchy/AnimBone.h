#pragma once
#include <cmath>
#include <vector>
#include "Application.h"
#include <algorithm>



struct PairFinder
{
	float to_find;

	PairFinder(float key) :to_find(key) {};
	void setToFind(float key) { to_find = key; };
	bool operator()(std::pair<float, XMFLOAT3> pair)
	{
		//Is the current pair thats been passed in the next pair
		return pair.first >= to_find;
	}
};

class AnimBone
{
public:
	AnimBone();
	~AnimBone();

	void addTransPair(float key, XMFLOAT3& Trans);
	void addRotPair(float key, XMFLOAT3& Rot);

	std::pair<XMFLOAT3, XMFLOAT3>& GetTrans(float time, bool loopable);
	std::pair<XMFLOAT3, XMFLOAT3>& GetRot(float time, bool loopable);
	void GetKeys(float time, float& current, float& next, float& endKey);

private:
	//Stores the key frame translation pairs for each Bone
	std::vector<std::pair<float, XMFLOAT3>> transVector;
	//Stores the key frame rotation pairs for each Bone
	std::vector<std::pair<float, XMFLOAT3>> rotVector;
	
};

