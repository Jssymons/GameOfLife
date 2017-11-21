#pragma once
#include <array>
#include "Cell.h"
#include "Constants.h"

class Grid
{
public:
	Grid();
	void Draw();
	void Update();
	void ClearGrid();
	void KillCell(int idx);
	void LiveCell(int idx);
	void SetCellColour(int idx, GLfloat r, GLfloat g, GLfloat b, bool pushToCells = true);
	void ResetCellColour(int idx);
private:
	int GetCellIdx(int x, int y);
	std::vector<int> GetCellPos(int index);
	int GetMeshCellIdx(int idx);
	void AddKillCell(int idx);
	void AddLiveCell(int idx);
	void UpdateColourBuffer();
	void CreateLife();
	void SimulateLife();
	void HandleQueues();

	Cell* m_cells = new Cell[GRID_VOLUME];
	std::vector<GLfloat> m_cellMesh;
	std::vector<GLfloat> m_colourMesh;
	std::vector<int> m_lifeQueue;
	std::vector<int> m_killQueue;
	std::vector<int> m_liveCells;
	GLuint m_vertexBuffer;
	GLuint m_colourBuffer;
	GLuint m_programID;
	GLuint m_timeUniformID;
};

