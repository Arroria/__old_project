#include "DXUT.h"
#include "Fader.h"


Fader::Fader()
	: m_in(false)
	, m_frame(0)
	, m_stay(0)
{
}
Fader::~Fader()
{
}



constexpr UINT SPEED = 30 + 1;

void Fader::Initialize()
{
	LoadTex(L"./Resource/noIngame/Loding_0.PNG", L"Fade0");
	LoadTex(L"./Resource/noIngame/Loding_1.PNG", L"Fade1");
}

void Fader::Create(const bool & in)
{
	m_in = in;
	m_frame = SPEED;
	m_stay = 0;
}

bool Fader::Update()
{
	if (m_stay)
		m_stay--;
	else if (m_frame)
	{
		m_frame--;
		if (!m_frame)
			return true;
	}
	return false;
}

void Fader::Render()
{
	if (m_stay || m_frame)
	{
		float time = ((float)(m_frame - 1) / (SPEED - 1));
		float f, s; {
			if (m_stay)
			{
				f = 800;
				s = 800;
			}
			else
			{
				f = 800 * (m_in ? 1 - time : time);
				s = 800 * (m_in ? time : 1 - time) + 800;
			}
		}


		SPRITE->Begin(D3DXSPRITE_ALPHABLEND);
		D3DXMATRIX iden;
		D3DXMatrixIdentity(&iden);
		SPRITE->SetTransform(&iden);

		auto Draw = [](Texture* tex, const float& pivot, const float& pos)
		{
			SPRITE->Draw(tex->tex, nullptr, &D3DXVECTOR3(pivot * tex->info.Width, 0, 0), &D3DXVECTOR3(pos, 0, 0), D3DXCOLOR(1, 1, 1, 1));
		};
		Texture* tex = GetTex(L"Fade0");
		Draw(GetTex(L"Fade0"), 1, f);
		Draw(GetTex(L"Fade1"), 0, s);
		
		SPRITE->End();
	}
}
