#include "DXUT.h"
#include "Fader.h"

Fader::Fader()
	: m_frame(0)
	, m_stay(0)
	, m_isIn(false)
{
	TextureLoad(L"./Resource/FadeL.png", L"FadeL");
	TextureLoad(L"./Resource/FadeR.png", L"FadeR");
}
Fader::~Fader()
{
}


constexpr UINT MAX_FRAME = 60;
void Fader::Create(const bool & isIn, const UINT & stay)
{
	m_isIn = isIn;
	m_frame = MAX_FRAME;
	m_stay = stay;
}

bool Fader::Update()
{
	if (m_stay)
		m_stay--;
	else if (m_frame)
	{
		m_frame--;
		return !m_frame;
	}
	return false;
}

void Fader::Render()
{
	if (!m_frame)
		return;

	SPRITE->Begin(D3DXSPRITE_ALPHABLEND);

	D3DXMATRIX m;
	D3DXMatrixIdentity(&m);
	SPRITE->SetTransform(&m);


	Texture* texL = TextureGet(L"FadeL");
	Texture* texR = TextureGet(L"FadeR");

	float _1to0 = ((float)m_frame - 1) / (MAX_FRAME - 1);

	float posL = m_isIn ? -(texL->info.Width * _1to0) : -(texL->info.Width * (1 - _1to0));
	float posR = (m_isIn ? texR->info.Width * _1to0 : texR->info.Width * (1 - _1to0)) + 1600;

	SPRITE->Draw(texL->tex, nullptr, nullptr								, &D3DXVECTOR3(posL, 0, 0), D3DXCOLOR(1, 1, 1, 1));
	SPRITE->Draw(texR->tex, nullptr, &D3DXVECTOR3(texR->info.Width, 0, 0)	, &D3DXVECTOR3(posR, 0, 0), D3DXCOLOR(1, 1, 1, 1));
	cout << _1to0 << " "
		<< posL << " "
		<< posR << endl;

	SPRITE->End();
}

