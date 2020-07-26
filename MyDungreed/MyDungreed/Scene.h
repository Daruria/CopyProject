#pragma once
class CScene
{
public:
	CScene();
	virtual ~CScene();

public:
	virtual void Initialize() = 0;
	virtual int Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void Render(HDC hDC) = 0;
	virtual void Release() = 0;

public:
	void ObjectListClear();

public:
	void SetName(const string& strName) { m_strName = strName; }
	const string& GetName() const { return m_strName; }

protected:
	string m_strName;
};

