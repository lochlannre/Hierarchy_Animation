#include "AnimBone.h"



AnimBone::AnimBone()
{
}


AnimBone::~AnimBone()
{
}

//Create a pair and add it to the vector
void AnimBone::addTransPair(float keyFrame, XMFLOAT3& Translation)
{
	transVector.push_back(std::make_pair(keyFrame, Translation));
}

//Create a pair and add it to the vector
void AnimBone::addRotPair(float keyFrame, XMFLOAT3& Rotation)
{
	rotVector.push_back(std::make_pair(keyFrame, Rotation));
}