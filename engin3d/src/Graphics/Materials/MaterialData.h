#ifndef MAT_DATA_H
#define MAT_DATA_H

#include <string>
#include <aiMaterial.h>


struct cMaterialData
{
	std::string macPath;		// Textures folder path 
	aiMaterial* mpMaterial;		// Assimp´s Material Node 
};

#endif
