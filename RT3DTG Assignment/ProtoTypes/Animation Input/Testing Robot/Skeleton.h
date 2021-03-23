#pragma once
//#include "Application.h"
#include "Bone.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include "Support.h"

//Container class for the bones
//Will be used to run the update function on the bones and create the links between the bones
class Skeleton
{
public:
	Skeleton();
	~Skeleton();

	//Takes the path to the heirarchy file will setup the v_Bones with their names, parents, offsets
	//This will inturn setup the meshes within each bone
	bool setUpBones(std::string &filePath);
private:
	std::vector<Bone*> v_Bones;
};

