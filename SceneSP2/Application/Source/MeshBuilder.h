#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "Vertex.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateQuad(const std::string& meshName, Color color, float length = 1.f);
	static Mesh* GenerateCube(const std::string &meshName, Color color, float length = 1.f);
	
	static Mesh* GenerateSphere(const std::string& meshName, Color color, unsigned numStacks, unsigned numSlices = 10, float radius = 1.f);

	static Mesh* GenerateCylinder(const std::string& meshName, Color color, unsigned numSlices = 10, float height = 2.f, float radius = 1.f);

	
	static Mesh* GenerateTorus(const std::string& meshName, Color color, unsigned numStacks, unsigned numSlices = 10, 
		float outerradius = 1.f, float innerradius = 1.f);

	static Mesh* GenerateHalfTorus(const std::string& meshName, Color color, unsigned numStacks, unsigned numSlices = 10,
		float outerradius = 1.f, float innerradius = 1.f);

	static Mesh* GenerateOBJ(const std::string& meshName, const std::string& file_path);



	static Mesh* GenerateOBJMTL(const std::string& meshName,
		const std::string& file_path, const std::string& mtl_path);
	
	


	static Mesh* GenerateText(const std::string& meshName, unsigned numRow, unsigned numCol);
	//Read vertices, texcoords & normals from OBJ


	int getX();
	
	int getY();
};

#endif