#pragma once
enum class MoveDirection
{
	Left,
	Top,
	Right,
	Bottom
};

struct _2048Block;
class GameWorld
{
private:
	//WorldMap
	POINT_ex m_worldGridSize;
	std::pair<__int64, POINT_ex> m_gameWorld[128][128];

	//NextMap
	std::pair<__int64, bool> m_nextWorld[128][128];
	double m_timeAfterDeparture;
	bool m_worldUpdated;

	//Render
	LPD3DXFONT m_font;

	//Score
	__int64 m_score;
	__int64 m_scoreTempStack;

//Wolrd Control
public:
	void MoveBox(const MoveDirection& moveDirection);
	bool CreateBlock(const __int64& point = 2);
	bool CreateBlock(const POINT_ex& position, const __int64& point = 2);

//Data
public:
	__int64 GetScore() { return m_score; }

public:
	void Update();
	void Render(const D3DXVECTOR2& drawCenter, const D3DXVECTOR2& worldSize);

public:
	GameWorld(const POINT_ex& worldGridSize);
	~GameWorld();
};
