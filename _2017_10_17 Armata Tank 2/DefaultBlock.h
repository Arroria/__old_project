#pragma once
#include "BaseBlock.h"
class DefaultBlock :
	public BaseBlock
{
private:
	Texture* m_txBlock;
	ASE* m_aseBlock;

public:
	virtual bool Update() override;
	virtual bool Render() override;

public:
	DefaultBlock(const int& blockId, const D3DXVECTOR3& position, const D3DXVECTOR3& scale, const D3DXVECTOR2& collisionSize, const float& durability, Texture* texture, const bool& breakable, const bool& ObjectMovable);
	virtual ~DefaultBlock();
};

