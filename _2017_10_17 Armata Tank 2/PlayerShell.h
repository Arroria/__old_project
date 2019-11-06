#pragma once
#include "BaseShell.h"

enum class PlayerShellType
{
	Cannon,
	AutoCannon,
	Rocket
};

class PlayerShell :
	public BaseShell
{
private:
	Texture* m_txShell;

public:
	void Move() override;

	bool Render() override;

public:
	PlayerShell(const D3DXVECTOR3& position, const D3DXVECTOR3& direction, const PlayerShellType& shellType);
	virtual ~PlayerShell();
};

