#pragma once
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <cassert>
#include "Application.h"
#include "AnimBone.h"

#define CHARINLENGTH 1000

class Animation
{
public:
	Animation(std::string& name, bool loopable);
	~Animation();

	bool loadAnim(std::string &filePath);

	void GetBoneAnimation(int bone, float& time, std::pair<XMFLOAT3, XMFLOAT3>& retTrans, std::pair<XMFLOAT3, XMFLOAT3>& retRot, float& current, float& next, float& endKey);

	std::string& GetName() { return name; }

private:

	//Vector of Animation Bones
	//Each part contains the translation and rotation for each bone in an animation

	std::vector<AnimBone*> boneVector;
	std::string name;
	bool loopable;



};

