#pragma once
#include <string>
#include <vector>
#include "Mesh.h"

struct BMPData
{
	unsigned char header[52];
	unsigned int dataPos;		// Position in file where data starts
	unsigned int width, height;
	unsigned int imageSize;		// bmpWidth * bmpHeight * 3				
	unsigned char* data;		// rgb data
	bool good;
};


class Terrain
{
public:
	Terrain(std::string fileName, unsigned int maxHeight);
	void Draw();
	Mesh* GetMesh();
	glm::vec3 GetCenter();
	~Terrain();
private:
	BMPData m_BMPData;
	Mesh* m_terrainMeshPtr;

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;

	int m_width, m_height;
	unsigned int MAX_HEIGHT;

	bool m_computedNormals;
	bool m_computedIndices;

	void LoadBMPData(std::string fileName);
	void SetMeshData(BMPData BMPData);
	void SetHeight(unsigned int zPos, unsigned int xPos, float height);
	void ComputePos();
	void ComputeNormals();
	void ComputeTexCoords();
	void ComputeIndices();

	void PrintAllPositions();


};

