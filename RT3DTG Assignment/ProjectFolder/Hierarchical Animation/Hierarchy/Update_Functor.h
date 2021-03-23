#pragma once

#include "Application.h"
#include "Animation.h"
// This causes linker errors everywhere and I have no clue why, if you want to test it go to bone.cpp and comment out the indicated section, then include Lerp.h in Bone
//#include "Lerp.h"
#include "Bone.h"

//For the skeleton to update the bones
struct Update
{
	Animation* active;
	Animation* prev;
	std::pair<XMFLOAT3, XMFLOAT3> trans, rot; //Stores both current and previous trans and rot in that order
	std::pair<XMFLOAT3, XMFLOAT3> prevTrans, prevRot; //Stores both current and previous trans and rot in that order
	int index;
	float time;
	float current;
	float next;
	float endKey;
	float blend;

	Update(Animation* anim, float t)
	{
		prev = NULL;
		blend = 0;
		index = 0;
		time = t;
		active = anim;
		current = next = endKey = 0;

	}
	void operator()(Bone* b)
	{
		active->GetBoneAnimation(index, time, trans, rot, current, next, endKey);

		b->UpdateAnimation(trans, rot, time, current, next, endKey);
		++index;
	}
	void setParams(Animation* anim, float t)
	{

		active = anim;
		time = t;
	}
};