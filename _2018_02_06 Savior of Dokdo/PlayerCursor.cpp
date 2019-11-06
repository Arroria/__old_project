#include "stdafx.h"
#include "PlayerCursor.h"


constexpr UINT KILLSTREAK_EFFECT_DURATION = 60;

PlayerCursor::PlayerCursor()
	: m_texDefault(nullptr)
	, m_texTargetDetected(nullptr)
	, m_texPlayerAttackEffect(nullptr)

	, m_texKill(nullptr)

	, m_killStreak(0)
	, m_killView(0)
{
	m_texKillNum.fill(0);


	m_texDefault			= GetSingleton(TextureLoader)->LoadTexture(L"./Resource/ui/Scope_1.png",		L"Aim");
	m_texTargetDetected		= GetSingleton(TextureLoader)->LoadTexture(L"./Resource/ui/Scope_2.png",		L"Aim_On");
	m_texPlayerAttackEffect	= GetSingleton(TextureLoader)->LoadTexture(L"./Resource/ui/Attack_Effect.PNG",	L"AttackEffect");

	m_texKill = GetSingleton(TextureLoader)->LoadTexture(L"./Resource/ui/kill/Kill_Score_kill.png", L"Kill_Kill");
	for (UINT i = 0; i < 10; i++)
		m_texKillNum[i] = GetSingleton(TextureLoader)->LoadTexture(std::wstring(L"./Resource/ui/kill/Kill_Score_") + std::to_wstring(i) + L".png", L"Kill_" + std::to_wstring(i));
}
PlayerCursor::~PlayerCursor()
{
}



void PlayerCursor::Render(LPD3DXSPRITE sprite, const bool & targetDetected)
{
	if (GetSingleton(InputManager)->IsKeyDown('Z'))
		PlayerAttackKillToEnemy();
	if (GetSingleton(InputManager)->IsKeyDown('X'))
		m_killStreak *= 10;

	POINT mousePos = GetSingleton(InputManager)->GetMousePos();

	//Cursor
	{
		Texture* texture = targetDetected ? m_texTargetDetected : m_texDefault;
		D3DXMATRIX tm;
		D3DXMatrixTranslation(&tm, mousePos.x, mousePos.y, 0);
		sprite->SetTransform(&tm);
		sprite->Draw(texture->texture,
			nullptr,
			&D3DXVECTOR3(texture->imageInfo.Width * 0.5f, texture->imageInfo.Height * 0.5f, 0),
			nullptr,
			D3DXCOLOR(1, 1, 1, 1));
	}

	//Attack effect
	{
		auto AttackEffectRender = [&sprite, &mousePos](std::vector<UINT>& effectState, Texture* texture, const D3DXCOLOR& color)
		{
			constexpr UINT DURATION = 10;
			constexpr FLOAT MAXIMUM_UPSCALE_SIZE = 0.2f;

			for (auto& iter = effectState.begin(); iter != effectState.end();)
			{
				if (*iter >= DURATION)
				{
					iter = effectState.erase(iter);
					continue;
				}

				float progress = (float)*iter / DURATION;
				float result_scale = 1 + progress * MAXIMUM_UPSCALE_SIZE;

				D3DXMATRIX sm, tm;
				D3DXMatrixScaling(&sm, result_scale, result_scale, 1);
				D3DXMatrixTranslation(&tm, mousePos.x, mousePos.y, 0);
				sprite->SetTransform(&(sm * tm));
				sprite->Draw(texture->texture,
					nullptr,
					&D3DXVECTOR3(texture->imageInfo.Width * 0.5f, texture->imageInfo.Height * 0.5f, 0),
					nullptr,
					D3DXCOLOR(color.r, color.g, color.b, color.a * (1 - progress)) );

				(*iter)++;
				iter++;
			}
		};

		AttackEffectRender(m_playerAttackEffect, m_texPlayerAttackEffect, D3DXCOLOR(1, 1, 1, 1));
		AttackEffectRender(m_playerKillEffect, m_texPlayerAttackEffect, D3DXCOLOR(1, 0, 0, 1));
	}

	//Kill Streak
	if (m_killView)
	{
		constexpr FLOAT SCALE = 1.5f;
		constexpr FLOAT BOUNCE = -20;
		constexpr UINT BOUNCEDURATION = 6;
		constexpr UINT ALPHADOWNTIMING = 15;

		float bounce = 1;
		if (m_killView > KILLSTREAK_EFFECT_DURATION - BOUNCEDURATION)
		{
			bounce = (float)(KILLSTREAK_EFFECT_DURATION - m_killView) / (BOUNCEDURATION / 2);
			if (bounce > 1)
				bounce = 2 - bounce;

			bounce *= BOUNCE;
		}

		float alpha = 1;
		if (m_killView < ALPHADOWNTIMING)
			alpha = (float)m_killView / ALPHADOWNTIMING;


		D3DXMATRIX sm, localTM, worldTM;
		D3DXMatrixScaling(&sm, SCALE, SCALE, SCALE);

		
		std::wstring num(std::to_wstring(m_killStreak));
		float offsetForNumLength = -(int)num.size() * 9;
		UINT renderCount = 0;
		
		//Kill Count
		for (auto& number : num)
		{
			Texture* texture = m_texKillNum[number - 48];
			D3DXMatrixTranslation(&localTM, renderCount * 18 + 35 + offsetForNumLength, bounce, 0);
			D3DXMatrixTranslation(&worldTM, mousePos.x + 160, mousePos.y + 160, 0);
			sprite->SetTransform(&(localTM * sm * worldTM));
			sprite->Draw(texture->texture, nullptr, &D3DXVECTOR3(texture->imageInfo.Width * 0.5f, texture->imageInfo.Height * 0.5f, 0), nullptr, D3DXCOLOR(1, 1, 1, alpha));

			renderCount++;
		}

		//Kill Text
		{
			D3DXMatrixTranslation(&localTM, offsetForNumLength, bounce, 0);
			D3DXMatrixTranslation(&worldTM, mousePos.x + 160, mousePos.y + 160, 0);
			sprite->SetTransform(&(localTM * sm * worldTM));
			sprite->Draw(m_texKill->texture, nullptr, &D3DXVECTOR3(m_texKill->imageInfo.Width * 0.5f, m_texKill->imageInfo.Height * 0.5f, 0), nullptr, D3DXCOLOR(1, 1, 1, alpha));
		}


		m_killView--;
	}
}



void PlayerCursor::PlayerAttackKillToEnemy()
{
	m_killStreak++;
	m_killView = KILLSTREAK_EFFECT_DURATION;
	m_playerKillEffect.push_back(0);
}
