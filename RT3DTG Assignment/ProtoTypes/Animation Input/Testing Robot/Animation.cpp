#include "Animation.h"
#include <cassert>


Animation::Animation()
{
}


Animation::~Animation()
{
	std::for_each(begin(boneVector), end(boneVector), [](AnimBone* b) {delete b; });
}

bool Animation::loadAnim(std::string &filePath)
{
	//Open file

	std::ifstream fileIN;
	fileIN.open(filePath.c_str());
	if (!fileIN.is_open())
		return false;

	char charIN[CHARINLENGTH];
	std::string strIN;
	std::string subStr;

	std::string inputArray = "input-array";
	std::string outputArray = "output-array";
	std::string floatArray = "float_array";
	std::string countStr = "count=";
	char endCount = '"';
	std::stringstream converter;
	std::string subString;
	std::string temp;
	int count(0); //Size of the data array we are retrieving

	std::vector<float> keyFrameStorage;
	std::vector<XMFLOAT3> TranslationStorage;
	std::vector<XMFLOAT3> RotationStorage;
	XMFLOAT3 f3_Temp;

	//Setup a pointer for use later
	AnimBone* animBone = NULL;

	//This will be used to tell the code whether its should be reading in a trans, rot(x / y / z)
	enum typeOfInput { TRANS = 0, ROTX = 1, ROTY = 2, ROTZ = 3 };

	//File is now open can start reading from it
	bool isRunning = true;

	int loopCounter = 0; //To be used whenever I need to keep track of how many times I have been through a loop, should have a comment next to each implementation explaining what its used for

	//Our first load will be a translation
	typeOfInput typeIn = TRANS;

	do {
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Translations
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//read in a line
		fileIN.getline(charIN, CHARINLENGTH);
		isRunning = !fileIN.eof();
		if (isRunning)
		{
			//Move into a string
			strIN = charIN;
			assert(!fileIN.fail());

			//Check to see if it has an input array
			if (strIN.find(inputArray) != std::string::npos)
			{
				//Check if its a line we want
				if (strIN.find(floatArray) != std::string::npos)
				{
					//If on the Translation or the first Rotation then read in the keyframes, otherwise don't care
					switch (typeIn)
					{
					case TRANS:
					case ROTX:
					{
						//Retrieve keyframes

						//Find out how many values we're retriving
						//Find where the count is situation
						auto size = strIN.find(countStr);
						auto breakPoint = strIN.begin() + size + countStr.length() + 1;
						auto endPoint = std::find(strIN.rbegin(), strIN.rend(), '"'); //This is a reverse iterator so we need to account for that

		//Retrieve the count value---------------------------------------------------------------------------------------------------------------------------------------------
						std::copy(breakPoint, endPoint.base() - 1, std::back_inserter(subString));

						//So we now have the value in speech marks after count= (for the root trans this is count="1")
						converter << subString;
						converter >> count;

						//reset the converter
						converter.clear();
						converter.str(std::string());
						subString = "";

						//Retrieve the Keyframes---------------------------------------------------------------------------------------------------------------------------------------------
						std::copy(endPoint.base() + 1, strIN.end(), std::back_inserter(subString));
						for (int i = 0; i < count; ++i)
						{
							//First search for a space/divider
							breakPoint = std::find(subString.begin(), subString.end(), ' ');
							if (breakPoint != subString.end())
							{
								//Read in that value
								std::copy(subString.begin(), breakPoint, std::back_inserter(temp));
								converter << temp;
								float tempKey;
								converter >> tempKey;

								//reset
								converter.clear();
								converter.str(std::string());
								keyFrameStorage.push_back(tempKey);

								//Remove the now discovered value from the subString
								subString.erase(subString.begin(), breakPoint + 1);
								//Store the set of key frames
							}
							else
							{
								//look for a "<"
								breakPoint = std::find(subString.begin(), subString.end(), '<');

								//We should be on the last value of the 'array' here
								std::copy(subString.begin(), breakPoint, std::back_inserter(temp));

								//Convert the value to a float
								converter << temp;
								float tempKey;
								converter >> tempKey;

								//Reset converter and store the key value
								converter.clear();
								converter.str(std::string());
								keyFrameStorage.push_back(tempKey);
							}

							temp = "";
						}
						subString = "";
					}
					break;
					case ROTY:
					case ROTZ:
					{
						//Don't read the keyframes
					}
					}
				}
			}
			else
			{
				//Retrieve the transformation data
				if (strIN.find(outputArray) != std::string::npos)
				{
					if (strIN.find(floatArray) != std::string::npos)
					{

						auto size = strIN.find(countStr);
						auto breakPoint = strIN.begin() + size + countStr.length() + 1;
						auto endPoint = std::find(strIN.rbegin(), strIN.rend(), '"'); //This is a reverse iterator so we need to account for that

	//Retrieve the count value---------------------------------------------------------------------------------------------------------------------------------------------
						std::copy(breakPoint, endPoint.base() - 1, std::back_inserter(subString));

						//So we now have the value in speech marks after count= (for the root trans out this is count="3")
						converter << subString;
						converter >> count;

						//reset the converter
						converter.clear();
						converter.str(std::string());
						subString = "";

						//Retrieve the Translations--------------------------------------------------------------------------------------------------------------------------------------------
						std::copy(endPoint.base() + 1, strIN.end(), std::back_inserter(subString));
						switch (typeIn)
						{
						case TRANS:
						{
							//We are loading in XMFLOAT3's so we want to loop everytime we have loaded one of them
							for (int i = 0; i < (count / 3); ++i)
							{
								//Do this 3 times (Since data is a float3)
								for (int i = 0; i < 3; ++i)
								{
									//Find the first break point
									breakPoint = std::find(subString.begin(), subString.end(), ' ');
									if (breakPoint != subString.end())
									{
										//Read in that value
										std::copy(subString.begin(), breakPoint, std::back_inserter(temp));
										converter << temp;
										//Depending on what loop we're on store in a different part of the float3
										if (i == 0)
											converter >> f3_Temp.x;
										else
											if (i == 1)
												converter >> f3_Temp.y;
											else
												converter >> f3_Temp.z;
										//reset
										converter.clear();
										converter.str(std::string());
										//Remove the now discovered value from the subString
										subString.erase(subString.begin(), breakPoint + 1);
									}
									else
									{
										//We are at the last value of the array here
										breakPoint = std::find(subString.begin(), subString.end(), '<');

										std::copy(subString.begin(), breakPoint, std::back_inserter(temp));
										//Convert the value to a float
										converter << temp;
										converter >> f3_Temp.z;
										//Reset and store the key value
										converter.clear();
										converter.str(std::string());
									}
									temp = "";
								}
								TranslationStorage.push_back(f3_Temp);
								f3_Temp = { 0,0,0 };
								subString = "";
							}
						}
						break;
						case ROTX:
						case ROTY:
						case ROTZ:
						{
							//We are loading in a float so we want to load differrently
							for (int i = 0; i < count; ++i)
							{
								//Find the first break point
								breakPoint = std::find(subString.begin(), subString.end(), ' ');
								if (breakPoint != subString.end())
								{
									//Read in that value
									std::copy(subString.begin(), breakPoint, std::back_inserter(temp));
									converter << temp;
									//Depending on what loop we're on store in a different part of the float3
									switch (typeIn)
									{
									case ROTX:
									{
										converter >> f3_Temp.x;
									}
									break;
									case ROTY:
									{
										converter >> f3_Temp.y;
									}
									break;
									case ROTZ:
									{
										converter >> f3_Temp.z;
									}
									}
									//reset
									converter.clear();
									converter.str(std::string());
									//Remove the now discovered value from the subString
									subString.erase(subString.begin(), breakPoint + 1);
								}
								else
								{
									//We are at the last value of the array here
									breakPoint = std::find(subString.begin(), subString.end(), '<');

									std::copy(subString.begin(), breakPoint, std::back_inserter(temp));
									//Convert the value to a float
									converter << temp;
									switch (typeIn)
									{
									case ROTX:
									{
										converter >> f3_Temp.x;
									}
									break;
									case ROTY:
									{
										converter >> f3_Temp.y;
									}
									break;
									case ROTZ:
									{
										converter >> f3_Temp.z;
									}
									}
									//Reset and store the key value
									converter.clear();
									converter.str(std::string());
								}
								temp = "";
								switch (typeIn)
								{
								case ROTX:
								{
									RotationStorage.push_back(f3_Temp);
								}
								break;
								case ROTY:
								{
									RotationStorage.at(i).y = f3_Temp.y;
								}
								break;
								case ROTZ:
								{
									RotationStorage.at(i).z = f3_Temp.z;
								}
								}
							}

							f3_Temp = { 0,0,0 };
							subString = "";
						}
						}

						//Now that the switch has loaded in the data wanted
						switch (typeIn)
						{
						case TRANS:
						{
							animBone = new AnimBone();
							for (int i = 0; i < keyFrameStorage.size(); ++i)
							{
								animBone->addTransPair(keyFrameStorage.at(i), TranslationStorage.at(i));
							}
							//Erase the storage it needs to be reused for the next loop for rotations
							keyFrameStorage.erase(keyFrameStorage.begin(), keyFrameStorage.end());
							TranslationStorage.erase(TranslationStorage.begin(), TranslationStorage.end());
							typeIn = ROTX;
						}
						break;
						case ROTZ:
						{
							for (int i = 0; i < keyFrameStorage.size(); ++i)
							{
								animBone->addRotPair(keyFrameStorage.at(i), RotationStorage.at(i));
							}
							//We have now loaded in a full bones animation so we can reset the keyframes and rotations
							keyFrameStorage.erase(keyFrameStorage.begin(), keyFrameStorage.end());
							RotationStorage.erase(RotationStorage.begin(), RotationStorage.end());
							//The animBone is now completed so we can push it onto the boneVector
							boneVector.push_back(animBone);
							typeIn = TRANS;
						}
						break;
						case ROTX:
						{
							typeIn = ROTY;
						}
						break;
						case ROTY:
						{
							typeIn = ROTZ;
						}
						}

					}
				}
			}


			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		}
	} while (isRunning);

	int x = 0;

}