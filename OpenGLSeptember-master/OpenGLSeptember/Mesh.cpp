#include "Mesh.h"
#include <vector>


Mesh::Mesh(std::vector <Vertex> &vertices, std::vector<unsigned int> &indices)
{
	IndexedModel model;

	for (size_t i = 0; i < vertices.size(); i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
		model.normals.push_back(*vertices[i].GetNormal());
	}

	for (size_t i = 0; i < indices.size(); i++)
	{
		model.indices.push_back(indices[i]);
	}

	this->InitMesh(model);
}

Mesh::Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
	IndexedModel model;

	for (size_t i = 0; i < numVertices; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
		model.normals.push_back(*vertices[i].GetNormal());
	}

	for (size_t i = 0; i < numIndices; i++)
	{
		model.indices.push_back(indices[i]);
	}

	this->InitMesh(model);
}

Mesh::Mesh(const std::string& fileName)
{
	IndexedModel model = OBJModel(fileName).ToIndexedModel();
	this->InitMesh(model);
}

void Mesh::InitMesh(const IndexedModel& model)
{
	this->m_drawCount = model.indices.size();
	glGenBuffers(NUM_BUFFERS, this->m_vertexArrayBuffers);

	glBindBuffer(GL_ARRAY_BUFFER, this->m_vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, this->m_vertexArrayBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, model.texCoords.size() * sizeof(model.texCoords[0]), &model.texCoords[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, this->m_vertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, model.normals.size() * sizeof(model.normals[0]), &model.normals[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_vertexArrayBuffers[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}
void Mesh::Draw()
{
	glBindVertexArray(this->m_vertexArrayObject);

	glDrawElements(GL_TRIANGLES, this->m_drawCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}
Mesh::~Mesh()
{

}
