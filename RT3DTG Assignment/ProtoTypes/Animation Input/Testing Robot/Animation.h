#pragma once
#include "Support.h"
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "AnimBone.h"

#define CHARINLENGTH 500

class Animation
{
public:
	Animation();
	~Animation();

	bool loadAnim(std::string &filePath);

	//int GetPartMatrix(int part, float time);

private:

	//Vector of Animation Bones
	//Each part contains the translation and rotation for each bone in an animation

	std::vector<AnimBone*> boneVector;



};

