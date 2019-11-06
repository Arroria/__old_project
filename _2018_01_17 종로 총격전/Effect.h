#pragma once
class Effect
{
private:
	bool m_isExpired;

public:
	void Expire() { m_isExpired = true; }
	bool IsExpired() { return m_isExpired; }

public:
	virtual void Initialize() PURE;
	virtual void Update() PURE;
	virtual void Render() PURE;
	virtual void Release() PURE;

public:
	Effect();
	virtual ~Effect();
};
