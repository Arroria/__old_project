#include "stdafx.h"
#include "ShellAdmin.h"

#include "PlayerAdmin.h"
#include "EnemyTankAdmin.h"
#include "Map.h"

#include "BaseShell.h"
#include "BaseTank.h"
#include "BaseBlock.h"

ShellAdmin::ShellAdmin(PlayerAdmin** playerAdmin, EnemyTankAdmin** enemyAdmin, Map** map)
	: m_playerAdmin(playerAdmin)
	, m_enemyAdmin(enemyAdmin)
	, m_map(map)
{
}
ShellAdmin::~ShellAdmin()
{
	for (auto& iter : m_playerShell)
		SAFE_DELETE(iter);
	for (auto& iter : m_enemyShell)
		SAFE_DELETE(iter);
	m_playerShell.clear();
	m_enemyShell.clear();
}


void ShellAdmin::RegisterPlayerShell(BaseShell * shell)
{
	if (shell)
		m_playerShell.push_back(shell);
}

void ShellAdmin::RegisterEnemyShell(BaseShell * shell)
{
	if (shell)
		m_enemyShell.push_back(shell);
}

bool ShellAdmin::ShellUpdate()
{
	auto ShellContainerUpdate = [this](std::list<BaseShell*>& shellContainer)
	{
		///auto iter = shellContainer.begin();
		///while (iter != shellContainer.end())
		///{
		///	(*iter)->Update();
		///	if ((*iter)->CheckDestroyThis())
		///	{
		///		SAFE_DELETE(*iter);
		///		iter = shellContainer.erase(iter);
		///	}
		///	else
		///		iter++;
		///}
		for (auto& iter : shellContainer)
			iter->Update();
	};

	ShellContainerUpdate(m_playerShell);
	ShellContainerUpdate(m_enemyShell);
	this->ShellDestroy();
	return false;
}

bool ShellAdmin::ShellInterAction()
{
	auto CreateCollisionBox = [](RECT& out, const RECT& collisionSize, const D3DXVECTOR3& pos)
	{
		out = collisionSize;
		out.left += pos.x;
		out.right += pos.x;
		out.top += pos.y;
		out.bottom += pos.y;
	};

	//Crash check player's shell to enemy;
	if (*m_enemyAdmin && (*m_enemyAdmin)->GetEnemyContainer() && (*m_playerAdmin)->GetPlayer())
	{
		auto CheckShellCollisionToTank = [&](std::list<BaseShell*>* shellContainer, BaseTank* tank)
		{
			for (auto& shellIter : *shellContainer)
			{
				D3DXVECTOR2 shellPosition(shellIter->GetPosition());
				D3DXVECTOR2 shellCollisionSize(shellIter->GetCollisionSize());

				D3DXVECTOR2 tankPosition(tank->GetPosition());
				D3DXVECTOR2 tankCollisionSize(tank->GetCollisionSize());

				if (shellPosition.x - shellCollisionSize.x < tankPosition.x + tankCollisionSize.x&&
					shellPosition.x + shellCollisionSize.x > tankPosition.x - tankCollisionSize.x&&
					shellPosition.y - shellCollisionSize.y < tankPosition.y + tankCollisionSize.y&&
					shellPosition.y + shellCollisionSize.y > tankPosition.y - tankCollisionSize.y)
				{
					tank->Hit(shellIter->GetFirePower(), shellIter->GetPenetration());
					shellIter->DestroyThis();
				}
			}
		};
		
		for (auto& iter : *(*m_enemyAdmin)->GetEnemyContainer())
			CheckShellCollisionToTank(&m_playerShell, iter);
		CheckShellCollisionToTank(&m_enemyShell, (*m_playerAdmin)->GetPlayer());
	}

	//Crash check player's shell to enemy
	if (*m_map && (*m_map)->GetMapBlockContainer())
	{
		auto CheckShellCollisionToBlock = [&](std::list<BaseShell*>* shellContainer, BaseBlock* block)
		{
			for (auto& shellIter : *shellContainer)
			{
				if (block->IsCanPenetrateForMove())
					continue;

				D3DXVECTOR2 shellPosition(shellIter->GetPosition());
				D3DXVECTOR2 shellCollisionSize(shellIter->GetCollisionSize());

				D3DXVECTOR3 blockPosition = block->GetPosition();
				D3DXVECTOR2 blockCollisionSize = block->GetCollisionSize();

				if (shellPosition.x - shellCollisionSize.x < blockPosition.x + blockCollisionSize.x &&
					shellPosition.x + shellCollisionSize.x > blockPosition.x - blockCollisionSize.x &&
					shellPosition.y - shellCollisionSize.y < blockPosition.y + blockCollisionSize.y &&
					shellPosition.y + shellCollisionSize.y > blockPosition.y - blockCollisionSize.y)
				{
					block->Hit(shellIter->GetFirePower(), shellIter->GetPenetration());
					shellIter->DestroyThis();
				}
			}
		};

		for (auto& mapBlockIter : *(*m_map)->GetMapBlockContainer())
		{
			CheckShellCollisionToBlock(&m_playerShell, mapBlockIter);
			CheckShellCollisionToBlock(&m_enemyShell, mapBlockIter);
		}
	}

	this->ShellDestroy();
	return false;
}

bool ShellAdmin::ShellRender()
{
	for (auto& iter : m_playerShell)	iter->Render();
	for (auto& iter : m_enemyShell)		iter->Render();
	return false;
}

void ShellAdmin::ShellDestroy()
{
	auto _ShellDestroy = [](std::list<BaseShell*>& shellContainer)
	{
		auto iter = shellContainer.begin();
		while (iter != shellContainer.end())
		{
			if ((*iter)->CheckDestroyThis())
			{
				SAFE_DELETE(*iter);
				iter = shellContainer.erase(iter);
			}
			else
				iter++;
		}
	};

	_ShellDestroy(m_playerShell);
	_ShellDestroy(m_enemyShell);
}
