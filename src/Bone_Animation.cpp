#include "Bone_Animation.h"
//Fernando de Salvidea
//FXD210003
//Assignment3 Animation


Bone_Animation::Bone_Animation()
{
}


Bone_Animation::~Bone_Animation()
{
}

void Bone_Animation::init()
{
	root_position = { 2.0f,1.0f,2.0f };

	scale_vector =
	{
		{1.0f,1.0f,1.0f},
		{0.5f,4.0f,0.5f},
		{0.5f,3.0f,0.5f},
		{0.5f,2.0f,0.5f}
	};

	rotation_degree_vector = 
	{
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};

	colors = 
	{
		{0.7f,0.0f,0.0f,1.0f}, //red
		{0.7f,0.7f,0.0f,1.0f}, //yellow
		{0.7f,0.0f,0.7f,1.0f}, //magenta
		{0.0f,0.7f,0.7f,1.0f} //cyan
	};

}

void Bone_Animation::update(float delta_time)
{

}

void Bone_Animation::reset()
{
	for (int i = 0; i < rotation_degree_vector.size(); i++)
    {
        rotation_degree_vector[i] = { 0.0f, 0.0f, 0.0f };
    }

    std::cout << "Bone rotations have been reset." << std::endl;
}



