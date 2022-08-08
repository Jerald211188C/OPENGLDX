#include "MeshBuilder.h"
#include <GL\glew.h>
#include <vector>

#include "MyMath.h"

/******************************************************************************/
/*!
\brief
Generate the vertices of a reference Axes; Use red for x-axis, green for y-axis, blue for z-axis
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	//x-axis
	v.pos.Set(-lengthX, 0, 0);	v.color.Set(1, 0, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX, 0, 0);	v.color.Set(1, 0, 0);	vertex_buffer_data.push_back(v);
	//y-axis
	v.pos.Set(0, -lengthY, 0);	v.color.Set(0, 1, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(0, lengthY, 0);	v.color.Set(0, 1, 0);	vertex_buffer_data.push_back(v);
	//z-axis
	v.pos.Set(0, 0, -lengthZ);	v.color.Set(0, 0, 1);	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, lengthZ);	v.color.Set(0, 0, 1);	vertex_buffer_data.push_back(v);

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(5);

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of quad
\param lengthY - height of quad

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateQuad(const std::string &meshName, Color color, float length)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	////Add the vertices here
	//v.pos.Set(0.5f, 0.f, 0.5f); vertex_buffer_data.push_back(v);// top right
	//v.pos.Set(-0.5f, 0.f, 0.5f); vertex_buffer_data.push_back(v);// top left
	//v.pos.Set(-0.5f, 0.f, -0.5f); vertex_buffer_data.push_back(v);// bottom left
	//v.pos.Set(0.5f, 0.f, -0.5f); vertex_buffer_data.push_back(v);// bottom right

	// Add the vertices here
	v.pos.Set(0.5f, 0.f, 0.5f); v.color.Set(color.r, color.g, color.b);
	vertex_buffer_data.push_back(v); // top-right
	v.pos.Set(-0.5f, 0.f, 0.5f); v.color.Set(color.r, color.g, color.b);
	vertex_buffer_data.push_back(v); // top-left
	v.pos.Set(-0.5f, 0.f, -0.5f); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // bottom-left
	v.pos.Set(0.5f, 0.f, -0.5f); v.color.Set(color.r, color.g, color.b);
	vertex_buffer_data.push_back(v); // bottom-right
	
	// Default winding order is Counter-clockwise (CCW)
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(2);

	// Create the new mesh
	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;


}

Mesh* MeshBuilder::GenerateCube(const std::string& meshName, Color
	color, float size)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	// Add the vertices here
	

	

	// Default winding order is Counter-clockwise (CCW)
	//define vertices
	v.pos.Set(0.5f, 0.5f, 0.5f); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // top-right
	v.pos.Set(-0.5f, 0.5f, 0.5f); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // top-left
	v.pos.Set(-0.5f, 0.5f, -0.5f); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // bottom-left
	v.pos.Set(0.5f, 0.5f, -0.5f); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // bottom-right

	v.pos.Set(0.5f, -0.5f, 0.5f); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // top-right
	v.pos.Set(-0.5f, -0.5f, 0.5f); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // top-left
	v.pos.Set(-0.5f, -0.5f, -0.5f); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // bottom-left
	v.pos.Set(0.5f, -0.5f, -0.5f); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // bottom-right

	//top
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(0);

	index_buffer_data.push_back(4);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(4);

	index_buffer_data.push_back(5);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(5);

	index_buffer_data.push_back(7);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(7);

	index_buffer_data.push_back(5);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(5);

	index_buffer_data.push_back(7);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(7);
	// Create the new mesh
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() *
		sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()
		* sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;
	return mesh;
}



Mesh* MeshBuilder::GenerateSphere(const std::string& meshName, Color
	color, unsigned stackCount, unsigned sliceCount, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	// Add the vertices for the spheres
	float anglePerSlice = Math::TWO_PI / sliceCount * 2 ;
	float anglePerStack = Math::PI/ stackCount * 2;

	for (unsigned stack = 0; stack <= stackCount; stack++) {
		for (unsigned slice = 0; slice <= sliceCount; slice++) {
			// Find theta (Fill the underlined part with necessary code)
			float theta = slice*anglePerSlice;
			// Find phi (Fill the underlined part with necessary code)
			float phi =  stack*anglePerStack - Math::HALF_PI;
			// Compute the vertices using the formulae given to find x, y and z(Fill the underlined area)
			float firstPoint = cos(phi) * cos(theta);
			float secondPoint = cos(phi) * sin(theta);
			float thirdPoint = sin(phi);


				v.pos.Set(firstPoint,secondPoint,thirdPoint);
			v.color.Set(color.r, color.g, color.b);
			vertex_buffer_data.push_back(v);
		}
	}

	// Define the triangles that make up the surface of the spheres
	// //-----
	for (unsigned stack = 0; stack < stackCount; stack++) {
		for (unsigned slice = 0; slice < sliceCount; slice++) {
			// Find phi (Fill the underlined part with necessary code)
			int k1 = slice + stack * stackCount;
			// Find phi (Fill the underlined part with necessary code)
			int k2 = k1 + sliceCount;
			// Form the faces required as seen in the diagram on step 7
				// Fill the underlined part with necessary code
				index_buffer_data.push_back(k2);
			index_buffer_data.push_back(k1+1);
			index_buffer_data.push_back(k1);
			index_buffer_data.push_back(k2);
			index_buffer_data.push_back(k2+1);
			index_buffer_data.push_back(k1+1);
		}
	}
		// Create the new mesh
		Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() *
		sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()
		* sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;
	return mesh;

}

Mesh* MeshBuilder::GenerateCylinder(const std::string& meshName, Color color, unsigned res, float depth, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float angle = Math::TWO_PI / res;

	v.pos.Set(0, depth, 0); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v);
	for (int i = 1; i <= res; i++)
	{
		v.pos.Set(sin(angle * i) * radius, depth, cos(angle * i) * radius); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v);
	}
	v.pos.Set(0, -depth, 0); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v);
	for (int i = 1; i <= res; i++)
	{
		v.pos.Set(sin(angle * i) * radius, -depth, cos(angle * i) * radius); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v);
	}

	for (int i = 0; i < res; i++)
	{
		if (i < (res - 1))
		{
			index_buffer_data.push_back(0);
			index_buffer_data.push_back(i + 1);
			index_buffer_data.push_back(i + 2);
			index_buffer_data.push_back(res + 1);
			index_buffer_data.push_back(res + 3 + i);
			index_buffer_data.push_back(res + 2 + i);
		}
		else
		{
			index_buffer_data.push_back(0);
			index_buffer_data.push_back(i + 1);
			index_buffer_data.push_back(1);
			index_buffer_data.push_back(res + 1);
			index_buffer_data.push_back(res + 2);
			index_buffer_data.push_back(res + 2 + i);
		}
	}
	for (int i = 1; i <= res; i++)
	{
		if (i < res)
		{
			index_buffer_data.push_back(i);
			index_buffer_data.push_back(i + res + 1);
			index_buffer_data.push_back(i + 1);
			index_buffer_data.push_back(i + 1);
			index_buffer_data.push_back(i + res + 1);
			index_buffer_data.push_back((i + res + 1) + 1);
		}
		else
		{
			index_buffer_data.push_back(res);
			index_buffer_data.push_back(res + res + 1);
			index_buffer_data.push_back(1);
			index_buffer_data.push_back(1);
			index_buffer_data.push_back(res + res + 1);
			index_buffer_data.push_back(res + 2);
		}
	}

	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;

}

Mesh* MeshBuilder::GenerateHemisphere(const std::string& meshName, Color
	color, unsigned stackCount, unsigned sliceCount, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	// Add the vertices for the spheres
	float anglePerSlice = Math::TWO_PI / sliceCount / 2 ;
	float anglePerStack = Math::PI / stackCount  / 2;

	for (unsigned stack = 0; stack <= stackCount; stack++) {
		for (unsigned slice = 0; slice <= sliceCount; slice++) {
			// Find theta (Fill the underlined part with necessary code)
			float theta = slice * anglePerSlice;
			// Find phi (Fill the underlined part with necessary code)
			float phi = stack * anglePerStack - Math::HALF_PI;
			// Compute the vertices using the formulae given to find x, y and z(Fill the underlined area)
			float firstPoint = cos(phi) * cos(theta);
			float secondPoint = cos(phi) * sin(theta);
			float thirdPoint = sin(phi);


			v.pos.Set(firstPoint, secondPoint, thirdPoint);
			v.color.Set(color.r, color.g, color.b);
			vertex_buffer_data.push_back(v);
		}
	}

	// Define the triangles that make up the surface of the spheres
	// //-----
	for (unsigned stack = 0; stack < stackCount; stack++) {
		for (unsigned slice = 0; slice < sliceCount; slice++) {
			// Find phi (Fill the underlined part with necessary code)
			int k1 = slice + stack * stackCount;
			// Find phi (Fill the underlined part with necessary code)
			int k2 = k1 + sliceCount;
			// Form the faces required as seen in the diagram on step 7
				// Fill the underlined part with necessary code
			index_buffer_data.push_back(k2);
			index_buffer_data.push_back(k1 + 1);
			index_buffer_data.push_back(k1);
			index_buffer_data.push_back(k2);
			index_buffer_data.push_back(k2 + 1);
			index_buffer_data.push_back(k1 + 1);
		}
	}
	// Create the new mesh
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() *
		sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()
		* sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;
	return mesh;

}