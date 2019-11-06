#include "stdafx.h"
#include "Lockoner.h"


#include "Enemy.h"
#include "Frame.h"

Lockoner::Lockoner()
	: m_lockonedEnemy(nullptr)

	, m_texLockoned(nullptr)
	, m_texLockonRange(nullptr)
{
	m_texLockoned =		GetSingleton(TextureLoader)->LoadTexture(L"./Resource/ui/lockon_cursor.png", L"Lockon_Cursor");
	m_texLockonRange =	GetSingleton(TextureLoader)->LoadTexture(L"./Resource/ui/lockon_range.png", L"Lockon_Range");

	m_lockonRangeEnemy.reserve(100);
}
Lockoner::~Lockoner()
{
}



void Lockoner::Lockon(std::vector<Enemy*>* targetContainer)
{
	Enemy* prevLockonedTarget = m_lockonedEnemy ? m_lockonedEnemy->enemy : nullptr;
	std::vector<LockonedTarget> prevLockonRangeTarget = m_lockonRangeEnemy;

	m_lockonRangeEnemy.clear();
	m_lockonedEnemy = nullptr;

	for (auto& iter : *targetContainer)
	{
		D3DXVECTOR3 enemyPosition(iter->GetFrame()->GetPosition());

		//OtherSide Checking
		{
			D3DXVECTOR3 enemyDir;
			D3DXVec3Normalize(&enemyDir, &(enemyPosition - GetSingleton(CameraManager)->GetDataRef().position));
			if (D3DXVec3Dot(&enemyDir, &GetSingleton(CameraManager)->GetDataRef().direction) < 0)
				continue;
		}

		D3DXMATRIX vM, pM;
		DEVICE->GetTransform(D3DTS_VIEW, &vM);
		DEVICE->GetTransform(D3DTS_PROJECTION, &pM);
		D3DXVec3TransformCoord(&enemyPosition, &enemyPosition, &(vM * pM));

		//Is in screen
		if (fabs(enemyPosition.x) < 1 && fabs(enemyPosition.y) < 1)
		{
			D3DXVECTOR2 mousePosition; {
			POINT mouse = GetSingleton(InputManager)->GetMousePos();
			mousePosition.x = mouse.x;
			mousePosition.y = mouse.y;
		}
			
			LockonedTarget lockonedTarget;
			lockonedTarget.enemy	= iter;
			lockonedTarget.time		= 0;
			lockonedTarget.wndPos	= D3DXVECTOR2((enemyPosition.x + 1) / 2 * 1600, (-enemyPosition.y + 1) / 2 * 900);
			lockonedTarget.distance = D3DXVec2Length(&(mousePosition - lockonedTarget.wndPos));
			
			//Is in lockon range
			if (lockonedTarget.distance < 120)
			{
				//Transmission time
				for (auto& prevData : prevLockonRangeTarget)
				{
					if (prevData.enemy == iter)
					{
						lockonedTarget.time = prevData.time + 1;
						break;
					}
				}

				m_lockonRangeEnemy.push_back(lockonedTarget);
			}
		}
	}

	if (!m_lockonRangeEnemy.empty())
	{
		//Calculate find lockonTarget
		LockonedTarget* nextTarget = nullptr;
		for (auto& iter : m_lockonRangeEnemy)
		{
			if (iter.distance < 120)
			{
				if (prevLockonedTarget == iter.enemy)
				{
					m_lockonedEnemy = &iter;
					break;
				}
				
				if (!nextTarget || nextTarget->distance > iter.distance)
					nextTarget = &iter;
			}
		}

		if (!m_lockonedEnemy && nextTarget)
		{
			m_lockonedEnemy = nextTarget;
			m_lockonedEnemy->time = 0;
		}
	}
}

void Lockoner::RenderLockonEffect(LPD3DXSPRITE sprite)
{
	for (auto& iter : m_lockonRangeEnemy)
	{
		if (m_lockonedEnemy && &iter == m_lockonedEnemy)
			continue;
		
		constexpr FLOAT DEFAULT_SCALE = 1.5f;
		constexpr UINT DURATION = 6;
		constexpr FLOAT MAXIMUM_UPSCALE_SIZE = 0.5f;

		const float& targetTime = iter.time;

		float n = 1;
		if (targetTime < DURATION)
			n += (targetTime < DURATION / 2 ? targetTime : DURATION - targetTime) / ((float)DURATION / 2) * MAXIMUM_UPSCALE_SIZE;


		const FLOAT RESULT_SCALE = DEFAULT_SCALE * n;

		D3DXMATRIX sm, tm;
		D3DXMatrixScaling(&sm, RESULT_SCALE, RESULT_SCALE, RESULT_SCALE);
		D3DXMatrixTranslation(&tm, iter.wndPos.x, iter.wndPos.y, 0);
		
		sprite->SetTransform(&(sm * tm));
		sprite->Draw(m_texLockonRange->texture, nullptr, &D3DXVECTOR3(m_texLockonRange->imageInfo.Width * 0.5f, m_texLockonRange->imageInfo.Height * 0.5f, 0), nullptr, D3DXCOLOR(1, 1, 1, 1));
	}

	if (m_lockonedEnemy)
	{
		constexpr UINT DURATION = 6;
		constexpr FLOAT MAXIMUM_UPSCALE_SIZE = 0.5f;

		float n = 1;
		if (m_lockonedEnemy->time < DURATION)
			n += (m_lockonedEnemy->time < DURATION / 2 ? m_lockonedEnemy->time : DURATION - m_lockonedEnemy->time) / ((float)DURATION / 2) * MAXIMUM_UPSCALE_SIZE;

		D3DXMATRIX sm, tm;
		D3DXMatrixScaling(&sm, n, n, n);
		D3DXMatrixTranslation(&tm, m_lockonedEnemy->wndPos.x, m_lockonedEnemy->wndPos.y, 0);
		
		sprite->SetTransform(&(sm * tm));
		sprite->Draw(m_texLockoned->texture, nullptr, &D3DXVECTOR3(m_texLockoned->imageInfo.Width * 0.5f, m_texLockoned->imageInfo.Height * 0.5f, 0), nullptr, D3DXCOLOR(1, 1, 1, 1));
	}
}
