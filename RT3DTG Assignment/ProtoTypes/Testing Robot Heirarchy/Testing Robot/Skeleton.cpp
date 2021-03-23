#include "Skeleton.h"



Skeleton::Skeleton()
{
}

void PrintBones(Bone* bone)
{
	std::cout << "Name: " << bone->GetName();
	std::cout << "\n";
	std::cout << "Parent: " << bone->GetParentName();
	std::cout << "\n";
	std::cout << "Offset: " << bone->GetOffset().x << ", " << bone->GetOffset().y << ", " << bone->GetOffset().z;
}

Skeleton::~Skeleton()
{
	std::for_each(v_Bones.begin(), v_Bones.end(), [](Bone* bone) {delete bone; });
}

struct str_finder
{
	str_finder(std::string &t) : to_Find(t) {};
	std::string to_Find;
	bool operator()(const Bone* t)
	{
		return t->GetName() == to_Find;
	}
};


//Filename = hierarchy
bool Skeleton::setUpBones(std::string &filePath)
{
	std::ifstream fileIN;
	fileIN.open(filePath+".txt");
	//Check if file is open
	if (!fileIN.is_open())
	{
		std::cerr << "Heirarchy file unable to be accessed.";
		fileIN.close();
		return false;
	}
	char char_input[100];
	std::string input;

	std::string str_name, str_parentName;


	XMFLOAT3 Offset;


	bool isRunning = true;

	//while not at the end of the file
	while (isRunning)
	{
		//Get Bone name
		fileIN.getline(char_input, 100);
		isRunning = !fileIN.eof();

		if (isRunning)
		{
			input = char_input;
			input.erase(std::remove(input.begin(), input.end(), '"'));
			input.erase(std::remove(input.begin(), input.end(), '"'));
			//input should now only contain the name of the bone (having removed the speech marks)
			str_name = input;

			//Get Parent name
			fileIN.getline(char_input, 100);

			isRunning = !fileIN.eof();

			input = char_input;
			auto newEnd = std::remove(input.begin(), input.end(), '"');
			input.erase(newEnd, input.end()); //[TODO] this isn't compiling, probably due to the value used to check

			//Input should now just be the name of the parent
			str_parentName = input;


			//Get the offset
			//read in line
			std::stringstream str_stream;
			fileIN.getline(char_input, 100);

			input = char_input;

			std::string temp;
			auto breakPoint = std::find(input.begin(), input.end(), ',');
			std::copy(input.begin(), breakPoint, std::back_inserter(temp));
			//str_stream.str("");
			str_stream << temp;

			str_stream >> Offset.x;
			//Empty the string stream
			str_stream.clear();
			str_stream.str(std::string());

			input.erase(input.begin(), breakPoint + 2);//Remove comma and space

			breakPoint = std::find(input.begin(), input.end(), ',');
			temp.erase(temp.begin(), temp.end());
			std::copy(input.begin(), breakPoint, std::back_inserter(temp));

			str_stream << temp;

			str_stream >> Offset.y;

			str_stream.clear();
			str_stream.str(std::string());

			input.erase(input.begin(), breakPoint + 2);


			str_stream << input;
			str_stream >> Offset.z;



			//We now can start to setup the Bone

			if (str_parentName != "")
			{
				str_finder finder(str_parentName);
				//Make this find the bone from the parent name, and then pass a pointer to that bone to the new bone being created.[TODO]
				auto p_Parent = std::find_if(v_Bones.begin(), v_Bones.end(), finder);
				if (p_Parent != v_Bones.end())
				{
					v_Bones.push_back( new Bone(str_name, Offset, *p_Parent));
				}
			}
			else
				v_Bones.push_back(new Bone(str_name, Offset));
		}

	}
	fileIN.close();
	std::for_each(v_Bones.begin(), v_Bones.end(), PrintBones);
	return true;

}