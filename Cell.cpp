#include "Cell.h"
#include "Shader.h"
#include "Constants.h"
#include <vector>
#include <GL\glew.h>
Cell::Cell()
{ }

void Cell::Init(int x, int y)
{
	float xMin = 2.0f * ((float)x / (float)GRID_WIDTH) - 1.0f;
	float yMin = 2.0f * ((float)y / (float)GRID_HEIGHT) - 1.0f;
	float xMax = (2*(1.0f / (float)GRID_WIDTH) + xMin);
	float yMax = (2*(1.0f / (float)GRID_HEIGHT) + yMin);

	m_vertexPositions = {
		xMin, yMin, 1.0f,
		xMin, yMax, 1.0f,
		xMax, yMin, 1.0f,
		xMax, yMin, 1.0f,
		xMin, yMax, 1.0f,
		xMax, yMax, 1.0f
	};
	
	SetColour(0.0f, 0.0f, 0.0f);
	m_state = CELL_DEAD;

	m_meshIdx = CalcCellIdx(x, y);
	
	if (x > 0 && y > 0)
		m_neighbours.push_back(CalcCellIdx(x - 1, y - 1));
	if (x > 0)
		m_neighbours.push_back(CalcCellIdx(x - 1, y));
	if (x > 0 && y < GRID_HEIGHT - 1)
		m_neighbours.push_back(CalcCellIdx(x - 1, y + 1));
	if (y < GRID_HEIGHT - 1)
		m_neighbours.push_back(CalcCellIdx(x, y + 1));
	if (x < GRID_WIDTH - 1 && y < GRID_HEIGHT - 1)
		m_neighbours.push_back(CalcCellIdx(x + 1, y + 1));
	if (x < GRID_WIDTH - 1)
		m_neighbours.push_back(CalcCellIdx(x + 1, y ));
	if (x < GRID_WIDTH - 1 && y > 0)
		m_neighbours.push_back(CalcCellIdx(x + 1, y - 1));
	if (y > 0)
		m_neighbours.push_back(CalcCellIdx(x, y - 1));
}

void Cell::SetColour(float r, float g, float b)
{
	m_colour = {
		r, g, b,
		r, g, b,
		r, g, b,
		r, g, b,
		r, g, b,
		r, g, b
	};
}

std::vector<GLfloat> Cell::GetColour()
{
	return m_colour;
}

std::vector<GLfloat> Cell::GetVertices()
{
	return m_vertexPositions;
}

int Cell::GetIdx()
{
	return m_meshIdx;
}

std::vector<int> Cell::GetNeighbours()
{
	return m_neighbours;
}

int Cell::CalcCellIdx(int x, int y)
{
	return x + y * GRID_WIDTH;
}

void Cell::IncrementPingCounter()
{
	m_pingCounter += 1;
}

void Cell::ResetPingCounter()
{
	m_pingCounter = 0;
}

int Cell::GetPingCounter()
{
	return m_pingCounter;
}