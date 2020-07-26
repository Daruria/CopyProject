#pragma once

#define VK_MAX 0xff
class CKeyManager
{
public:
	CKeyManager();
	~CKeyManager();

	DECLARE_SINGLE(CKeyManager)

private:
	bool m_bKeyState[VK_MAX];

public:
	void Initialize();
	void Release();

public:
	bool KeyPressing(int iKey);
	bool KeyDown(int iKey);
	bool KeyUp(int iKey);
	void KeyUpdate();
};

