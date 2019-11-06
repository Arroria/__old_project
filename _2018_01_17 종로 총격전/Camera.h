#pragma once
class Camera
{
private:
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_size;

public:
	D3DXVECTOR3 GetPosition()	const { return m_position; }
	D3DXVECTOR3 GetSize()		const { return m_size; }

	void SetCameraTranslation(const D3DXVECTOR3* position, const D3DXVECTOR3* size);
	void InputCameraDevice(LPDIRECT3DDEVICE9 device);

public:
	Camera();
	~Camera();
};

