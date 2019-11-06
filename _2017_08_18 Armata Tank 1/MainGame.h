#pragma once

namespace AF
{
	class InputManager;
	class TextureManager;

	class PlayerManager;
	class PlayerAttackManager;
	class EnemyManager;
	class EnemyBulletManager;
	class MapManager;
	class ItemManager;
}


class MainGame
{
private:
	AF::InputManager*			m_inputManager;
	AF::TextureManager*			m_textureManager;
	
	AF::PlayerManager*			m_playerManager;
	AF::PlayerAttackManager*	m_playerAttackManager;
	AF::EnemyManager*			m_enemyManager;
	AF::EnemyBulletManager*		m_enemyBulletManager;
	AF::MapManager*				m_mapManager;
	AF::ItemManager*			m_itemManager;

public:
	HRESULT	Initialize	();
	HRESULT	Update		();
	HRESULT	Render		();
	HRESULT	Release		();
	LRESULT	WndProc		(const HWND& hWnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);

public:
	MainGame();
	~MainGame();
};

