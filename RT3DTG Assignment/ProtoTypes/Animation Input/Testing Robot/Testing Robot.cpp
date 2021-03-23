// Testing Robot.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include "pch.h"
#include <iostream>
#include "Skeleton.h"
#include "Animation.h"



int main()
{
	Skeleton skelly;
	std::string filename = "hierarchy";
	skelly.setUpBones(filename);

	std::string filePath("RobotIdleAnim.dae.txt");

	std::vector<Animation> Animations;
	Animation Idle, Attack, Die;
	Idle.loadAnim();
	Attack.loadAnim();
	Die.loadAnim();

}
