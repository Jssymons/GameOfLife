#pragma once

#include <glm\glm.hpp>
#include <GL\glew.h>
#include <vector>


enum CellState {
	CELL_DEAD = 0,
	CELL_ACTIVE = 1,
	CELL_ALIVE = 2
};

class Cell
{
public:
	Cell();
	void Init(int x, int y);
	void SetColour(float r, float g, float b);
	void IncrementPingCounter();
	void ResetPingCounter();
	int GetPingCounter();
	CellState m_state;
	std::vector<GLfloat> GetColour();
	std::vector<GLfloat> GetVertices();
	int GetIdx();
	std::vector<int> GetNeighbours();
private:
	int CalcCellIdx(int x, int y);
	int m_meshIdx;
	int m_pingCounter = 0;
	std::vector<int> m_neighbours;
	std::vector<GLfloat> m_colour;
	std::vector<GLfloat> m_vertexPositions;
};

