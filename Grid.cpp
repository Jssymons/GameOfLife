#include "Grid.h"
#include "Cell.h"
#include "Shader.h"
#include <GL\glfw3.h>
#include <random>

Grid::Grid()
{
	for (int i = 0; i < GRID_VOLUME; i++)
	{
		int x = i % GRID_WIDTH;
		int y = (i - x) / GRID_WIDTH;
		m_cells[i].Init(x, y);

		std::vector<GLfloat> cellVertices = m_cells[i].GetVertices();
		m_cellMesh.insert(m_cellMesh.end(), cellVertices.begin(), cellVertices.end());
		std::vector<GLfloat> cellColour = m_cells[i].GetColour();
		m_colourMesh.insert(m_colourMesh.end(), cellColour.begin(), cellColour.end());
	}

	CreateLife();

	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_cellMesh.size() * sizeof(m_cellMesh[0]), m_cellMesh.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_colourMesh.size() * sizeof(m_colourMesh[0]), m_colourMesh.data(), GL_DYNAMIC_READ);
	
	m_programID = LoadShaders("triangles.vert", "triangles.frag");
	m_timeUniformID = glGetUniformLocation(m_programID, "time");
}

void Grid::Draw()
{
	glUseProgram(m_programID);
	// 1st attribute buffer : vertices
	UpdateColourBuffer();
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// 2nd attribute buffer : vertices
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
	glVertexAttribPointer(
		1,                  // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	float time = (float)glfwGetTime();
	glUniform1f(m_timeUniformID, sin(time*2));
	glDrawArrays(GL_TRIANGLES, 0, m_cellMesh.size());
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(0);
	
}

void Grid::SetCellColour(int idx, GLfloat r, GLfloat g, GLfloat b, bool pushToCells)
{
	if (pushToCells)
		m_cells[idx].SetColour(r, g, b);

	int meshIdx = GetMeshCellIdx(idx);

	for (int i = 0; i < 6*3; i = i + 3)
	{
		m_colourMesh[meshIdx + i + 0] = r;
		m_colourMesh[meshIdx + i + 1] = g;
		m_colourMesh[meshIdx + i + 2] = b;
	}
}

void Grid::Update()
{
	SimulateLife();
}

int Grid::GetCellIdx(int x, int y)
{
	return x + y * GRID_WIDTH;
}

int Grid::GetMeshCellIdx(int idx)
{
	return idx * 6 * 3;
}

std::vector<int> Grid::GetCellPos(int index)
{
	std::vector<int> retVal;
	retVal.push_back(index % GRID_WIDTH);
	retVal.push_back((index - (index % GRID_WIDTH)) / GRID_WIDTH);
	return retVal;
}

void Grid::ResetCellColour(int idx)
{
	std::vector<GLfloat> colours = m_cells[idx].GetColour();
	SetCellColour(idx, colours[0], colours[1], colours[2], false);
}

void Grid::UpdateColourBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_colourMesh.size() * sizeof(m_colourMesh[0]), m_colourMesh.data(), GL_STATIC_DRAW);
}

void Grid::CreateLife()
{
	LiveCell(0);
	for (int i = GRID_WIDTH; i < GRID_VOLUME; i = i + GRID_WIDTH) {
		LiveCell(i);
		LiveCell(i - 1);
	}
	LiveCell(GRID_VOLUME - 1);
}

void Grid::SimulateLife()
{
	std::vector<int> activeCells;
 	for (int cellIdx : m_liveCells) {
		Cell cell = m_cells[cellIdx];
		if (cell.m_state == CELL_ALIVE)
		{
			for (int idx : cell.GetNeighbours())
			{
				if (m_cells[idx].m_state == CELL_ALIVE)
					cell.IncrementPingCounter();

				if (m_cells[idx].m_state == CELL_DEAD)
					m_cells[idx].m_state = CELL_ACTIVE;

				if (m_cells[idx].m_state == CELL_ACTIVE)
				{
					m_cells[idx].IncrementPingCounter();
					activeCells.push_back(idx);
				}
			}

			int numNeighbours = cell.GetPingCounter();
			if (numNeighbours < 2) {
				AddKillCell(cell.GetIdx());
			}
			else if (numNeighbours > 3) {
				AddKillCell(cell.GetIdx());
			}
			cell.ResetPingCounter();
		}
	}
	for (int cellIdx : activeCells) {
		Cell cell = m_cells[cellIdx];
		if (cell.m_state == CELL_ACTIVE)
		{
			int numNeighbours = cell.GetPingCounter();
			if (numNeighbours == 3) {
				AddLiveCell(cell.GetIdx());
			} 
			else
			{
				AddKillCell(cell.GetIdx());
			}
			m_cells[cell.GetIdx()].ResetPingCounter();
		}
	}

	HandleQueues();
}

void Grid::AddKillCell(int idx)
{
	m_killQueue.push_back(idx);
}

void Grid::AddLiveCell(int idx)
{
	m_lifeQueue.push_back(idx);
}

void Grid::KillCell(int idx)
{
	if (m_cells[idx].m_state == CELL_ALIVE)
	{
		m_cells[idx].m_state = CELL_DEAD;
		m_liveCells.erase(std::remove(m_liveCells.begin(), m_liveCells.end(), idx), m_liveCells.end());
		SetCellColour(idx, 0.0f, 0.0f, 0.0f);
	}
}

void Grid::LiveCell(int idx)
{
	if (m_cells[idx].m_state != CELL_ALIVE)
	{
		m_cells[idx].m_state = CELL_ALIVE;
		m_liveCells.push_back(idx);
		SetCellColour(idx, 1.0f, 1.0f, 0.0f);
	}
}

void Grid::HandleQueues()
{
	for (int j = 0; j < m_killQueue.size(); j++)
	{
		KillCell(m_killQueue[j]);
	}

	for (int k = 0; k < m_lifeQueue.size(); k++)
	{
		LiveCell(m_lifeQueue[k]);
	}
	m_killQueue.clear();
	m_lifeQueue.clear();
}

void Grid::ClearGrid()
{
	for (int x = 0; x < GRID_VOLUME; x++)
	{
		m_killQueue.push_back(x);
	}
	HandleQueues();
	UpdateColourBuffer();
}