#pragma once
namespace AF
{
	class CameraManager
	{
	private:
		D3DXVECTOR3	m_cameraEye;
		D3DXVECTOR3	m_cameraAt;
		D3DXVECTOR3	m_cameraUp;

		float		m_viewAngle;
		float		m_sightAngle;

	public:
		void	SetViewTransform		(const D3DXVECTOR3& eye, const D3DXVECTOR3& at, const D3DXVECTOR3& up = D3DXVECTOR3(0, 1, 0));
		void	SetProjectionTransform	(const float& viewingAngle, const float& sightRange);

	public:
		CameraManager();
		~CameraManager();
	};
}

