#pragma once

class TestScene
{
private:
	ASE* m_model1;
	ASE* m_model2;

	D3DXVECTOR3 m_cameraPos;
	float		m_cameraRotationForX;
	float		m_cameraRotationForY;

	D3DXVECTOR3 m_pos;
	D3DXVECTOR2 m_rot;

	LPD3DXFONT m_font;

public:
	void Initialize();
	void Update();
	void Render();
	void Release();

public:
	TestScene();
	~TestScene();
};

