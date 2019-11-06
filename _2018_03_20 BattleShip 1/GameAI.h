#pragma once
class GameAI
{
private:
	std::array <bool, 100 > m_myData;
	
	POINT m_lastAttackPoint;

	/* Hit */
	std::list<POINT> m_hitPos;
	std::array<bool, 4> m_dir;

	/* Diagonal */
	bool m_isEven;
	bool m_diagonalErrorPoint;

private:
	bool& MyData(const UINT&x, const UINT& y) { return m_myData[x + y * 10]; }

private:
	bool FindHit(POINT &output);
	bool FindDiagonal(POINT &output, const bool& even);

public:
	bool Output(POINT &output);
	void Input(const bool& attackSucceeded, const bool& hit);

public:
	GameAI();
	~GameAI();
};





//this AI do not activate in game used x1Ship
//	class GameAI2
//	{
//		struct TileData
//		{
//			bool m_attacked;
//			bool m_isEnemy;
//			bool m_isExpired;
//			TileData()
//				: m_attacked(false)
//				, m_isEnemy(false)
//				, m_isExpired(false)
//			{}
//		};
//	private:
//		std::vector<std::pair<UINT, bool>> m_enemyShip;	// < size, live >
//		std::array<TileData, 100> m_map;
//	
//		POINT m_lastAttackPos;
//	
//	private:
//		/* Simpler */
//		bool OutOfMap(const int& pos) { return pos < 0 || pos >= 100; }
//		bool OutOfMap(const POINT& pos) { return OutOfMap(PointToLine(pos)); }
//		POINT LineToPoint(const int& pos) { POINT point; point.x = pos % 10; point.y = pos / 10; return point; }
//		int PointToLine(const POINT& pos) { return pos.x + pos.y * 10; }
//	
//		/* Left Ship */
//		UINT ShipCount(const UINT& size); // 0 will return all ship count
//	
//	
//		/* MapReferencer */
//		
//	
//	
//		/* Calculate Target Tile */
//		//bool DiagonalTile
//	
//	
//	public:
//		void Calculate();
//		void ResultUpdate(const bool& attackHit, const UINT& attackKill = 0);
//	
//	public:
//		GameAI2();
//		~GameAI2();
//	};
