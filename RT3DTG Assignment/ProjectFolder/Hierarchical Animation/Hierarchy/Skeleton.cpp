#include "Skeleton.h"

Skeleton::Skeleton(XMFLOAT3 WorldOffset) :bones(ActiveAnimation, 0.f)
{
	this->worldOffset = WorldOffset;
	ActiveAnimation = NULL;
	frameCount = 0.f;
}

Skeleton::~Skeleton()
{
	//Make sure to release the memory for each bone
	std::for_each(v_Bones.begin(), v_Bones.end(), [](Bone* bone) {delete bone; });
}

//Functor for finding a string within the list of bones
struct str_finder
{
	//Set up the functor with the string value to find
	str_finder(std::string &t) : to_Find(t) {};

	std::string to_Find;

	//Test against the given bone
	bool operator()(const Bone* t)
	{
		return t->GetName() == to_Find;
	}
};

//Filename = hierarchy
bool Skeleton::setUpBones(std::string &filePath)
{
	std::ifstream fileIN;
	fileIN.open(filePath + ".txt");
	//Check if file is open
	if (!fileIN.is_open())
	{
		std::cerr << "Heirarchy file unable to be accessed.";
		fileIN.close();
		return false;
	}
	char char_input[100];
	std::string stl_input;

	//Variables to hold the data during processing
	std::string str_name, str_parentName;

	XMFLOAT3 Offset;

	//Set up a tempory variable for use in converting from floats
	std::string temp;

	//Setup a string stream for converting from text to float values
	std::stringstream str_stream;

	bool isRunning = true;

	//while not at the end of the file
	while (isRunning)
	{
		//Get Bone name
		fileIN.getline(char_input, 100);
		isRunning = !fileIN.eof();



		// Have we found the end of the file (this should only ever happen here if the heirarchy is in the correct format)
		if (isRunning)
		{
			//Move into an std::string so I can use the stl on it
			stl_input = char_input;

			//Remove the " *text* " quotation marks from the data
			stl_input.erase(std::remove(stl_input.begin(), stl_input.end(), '"'), stl_input.end());

			//input should now only contain the name of the bone (having removed the speech marks) so store it
			str_name = stl_input;

			//Read in the parent name
			fileIN.getline(char_input, 100);

			//Again move into an std::string so I can use the stl on it
			stl_input = char_input;

			//Again remove the speech marks
			stl_input.erase(std::remove(stl_input.begin(), stl_input.end(), '"'), stl_input.end());;

			//Input should now just be the name of the parent
			str_parentName = stl_input;

			//Read in the offset
			fileIN.getline(char_input, 100);

			//Again move into an std::string so I can use the stl on it
			stl_input = char_input;



			//Find the end of the first component
			auto breakPoint = std::find(stl_input.begin(), stl_input.end(), ',');
			//Copy it into a temp variable
			std::copy(stl_input.begin(), breakPoint, std::back_inserter(temp));

			//Put this value into the stream
			str_stream << temp;
			//Output to offset x component
			str_stream >> Offset.x;

			//Empty the string stream
			str_stream.clear();
			str_stream.str(std::string());
			temp.erase(temp.begin(), temp.end());

			//Remove the now used section of the input (first component and the comma and space following it)
			stl_input.erase(stl_input.begin(), breakPoint + 2);

			//Rinse and repeat for Y
			breakPoint = std::find(stl_input.begin(), stl_input.end(), ',');
			std::copy(stl_input.begin(), breakPoint, std::back_inserter(temp));

			str_stream << temp;
			str_stream >> Offset.y;

			str_stream.clear();
			str_stream.str(std::string());
			temp.erase(temp.begin(), temp.end());

			stl_input.erase(stl_input.begin(), breakPoint + 2);

			//At this point we only have 1 component left so just covert it immediately
			str_stream << stl_input;
			str_stream >> Offset.z;

			str_stream.clear();
			str_stream.str(std::string());
			temp.erase(temp.begin(), temp.end());

			//Fix the scale difference between DirectX and 3D Studio (Maya)
			Offset.x /= 10;
			Offset.y /= 10;
			Offset.z /= 10;

			//Does the object have a parent
			if (str_parentName != "")
			{
				//Yes then find the parent from the vector of existing bones
				str_finder finder(str_parentName);
				auto p_Parent = std::find_if(v_Bones.begin(), v_Bones.end(), finder);

				//Check to see if it found the parent
				assert(p_Parent != v_Bones.end());

				//Create an 
				v_Bones.push_back(new Bone(str_name, Offset, *p_Parent));
			}
			else //No then just create a default Bone
				v_Bones.push_back(new Bone(str_name, Offset));
		}

	}
	fileIN.close();
	return true;

}


void Skeleton::UpdateBones()
{
	//If animation = NULL then just t-pos
	if (ActiveAnimation == NULL)
	{
	}
	else
	{//Else run the animation
		bones.setParams(ActiveAnimation, frameCount);
		std::for_each(begin(v_Bones), end(v_Bones), bones);
	}
	std::for_each(begin(v_Bones), end(v_Bones), [this](Bone* b) {b->UpdateMatrices(this->worldOffset); });
	//std::for_each(begin(v_Bones), end(v_Bones), [](Bone* b) {b->UpdateWorldMatrix(); });
	frameCount += DELTATIME;
}

void Skeleton::Draw()
{
	std::for_each(begin(v_Bones), end(v_Bones), [](Bone* b) {b->Draw(); });
}

void Skeleton::setAnimation(Animation* anim)
{
	frameCount = 0;
	ActiveAnimation = anim;
}