#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <sstream>

#include "Bone.h"
#include "Animation.h"
#include "Application.h"
#include "Update_Functor.h"



#define DELTATIME 0.016f


//Container class for the bones
//Will be used to run the update function on the bones and create the links between the bones
__declspec(align(16)) class Skeleton
{
public:
	Skeleton(XMFLOAT3 WorldOffset = { 0,0,0 });
	~Skeleton();

	//Takes the path to the heirarchy file will setup the v_Bones with their names, parents, offsets
	//This will inturn setup the meshes within each bone
	bool setUpBones(std::string &filePath);

	void UpdateBones();
	void Draw();

	void setAnimation(Animation* anim);

private:
	std::vector<Bone*> v_Bones;
	Animation* ActiveAnimation;
	Update bones;
	XMFLOAT3 worldOffset;
	float frameCount;
};