#pragma once
class Effect3D
{
private:
	bool m_isExpired;

public:
	bool IsExpired() { return m_isExpired; }
	void Expire() { m_isExpired = true; }

public:
	virtual void Update() PURE;
	virtual void Render() PURE;

public:
	Effect3D();
	~Effect3D();
};

