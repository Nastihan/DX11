#pragma once
#include "Vertex.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags



static void test()
{
	Assimp::Importer imp;
	const auto& scene = imp.ReadFile("Models\\brick_wall\\brickwall.jpg",
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);

	
}
