#pragma once
#include "MoveObj.h"
class CItem;
class CFieldItem :
	public CMoveObj
{
public:
	CFieldItem();
	virtual ~CFieldItem();

public:
	virtual void Initialize()override;
	virtual int Update()override;
	virtual void LateUpdate()override;
	virtual void Render(HDC hDC)override;
	virtual void Release() override;

public:
	virtual void Stop();

private:
	bool		m_bIsStop;
	CItem*		m_pItem;
	POSITION	m_tDir;

public:
	void SetItem(CItem* pItem) { m_pItem = pItem; }
	void SetDir(const POSITION& tDir) { m_tDir = tDir; }

public:
	CItem* GetItem() { return m_pItem; }
	const bool GetIsStop() const { return m_bIsStop; }
};

