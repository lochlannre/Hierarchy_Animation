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

//I feel like I should redo this so that the previous pair is passed in in an attempt to get the animation blending to work
std::pair<XMFLOAT3, XMFLOAT3>& AnimBone::GetTrans(float time, bool loopable)
{

	float endKey = (*(end(transVector) - 1)).first;
	time = fmod(time, endKey);
	PairFinder finder(time);
	//find the first keyframe pair that is larger than the current time, aka the next keyframe, -1 to get the current keyframe
	auto next = std::find_if(begin(transVector), end(transVector), finder);
	if (next != begin(transVector))
	{
		return std::make_pair((*next).second, (*(next - 1)).second);
	}
	else
	{
		if (loopable)
		{
			return std::make_pair((*next).second, (*(end(transVector) - 1)).second);
		}
	}
	return std::make_pair((*next).second, ((*next)).second);

}

std::pair<XMFLOAT3, XMFLOAT3>& AnimBone::GetRot(float time, bool loopable)
{

	float endKey = (*(end(rotVector) - 1)).first;
	time = fmod(time, endKey);
	PairFinder finder(time);
	//find the first keyframe pair that is larger than the current time, aka the next keyframe, -1 to get the current keyframe
	auto next = std::find_if(begin(rotVector), end(rotVector), finder);
	if (next != begin(rotVector))
	{
		return std::make_pair((*next).second, (*(next - 1)).second);
	}
	else
	{
		if (loopable)
		{
			return std::make_pair((*next).second, (*(end(rotVector) - 1)).second);
		}
	}
	return std::make_pair((*next).second, ((*next)).second);

}

//std::pair<XMFLOAT3, XMFLOAT3>& AnimBone::GetTrans(float time, bool loopable, XMFLOAT3& previousTrans)
//{
//	float endKey = (*(end(transVector) - 1)).first;
//	time = fmod(time, endKey);
//	PairFinder finder(time);
//	auto next = std::find_if(begin(transVector), end(transVector), finder);
//	if (next != begin(transVector))
//	{
//		//Is the previous keyframe in this animation the same as the previous trans passed in
//		if ((previousTrans.x == (*(next - 1)).second.x) && (previousTrans.y == (*(next - 1)).second.y) && (previousTrans.z == (*(next - 1)).second.z))
//		{
//			//If not then use it as the previous
//			return std::make_pair((*next).second, previousTrans);
//		}
//		else
//		{
//			return std::make_pair((*next).second, (*(next-1)).second);
//		}
//	}
//	else
//	{
//		return std::make_pair((*next).second, (*next).second);
//	}
//}
//
//std::pair<XMFLOAT3, XMFLOAT3>& AnimBone::GetRot(float time, bool loopable, XMFLOAT3& previousRot)
//{
//	float endKey = (*(end(rotVector) - 1)).first;
//	time = fmod(time, endKey);
//	PairFinder finder(time);
//	auto next = std::find_if(begin(rotVector), end(rotVector), finder);
//	if (next != begin(rotVector))
//	{
//		//Is the previous keyframe in this animation the same as the previous trans passed in
//		if ((previousRot.x != (*(next - 1)).second.x) && (previousRot.y != (*(next - 1)).second.y) && (previousRot.z != (*(next - 1)).second.z))
//		{
//			//If not then use it as the previous
//			return std::make_pair((*next).second, previousRot);
//		}
//		else
//		{
//			//If it is then just use the previous one?
//			return std::make_pair((*next).second, (*(next - 1)).second);
//		}
//	}
//	return std::make_pair((*next).second, (*next).second);
//}


void AnimBone::GetKeys(float time, float& current, float& next, float& endKey)
{
	endKey = (*(end(rotVector) - 1)).first;
	time = fmod(time, endKey);
	PairFinder finder(time);

	auto nextIt = std::find_if(begin(rotVector), end(rotVector), finder);

	next = (*nextIt).first;
	if (nextIt == begin(rotVector))
	{
		current = 0;
	}
	else
		current = (*(nextIt - 1)).first;
}