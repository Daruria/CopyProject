#pragma once

#include "Obj.h"

class CTile : public CObj
{
public:
	CTile();
	~CTile();

private:
	int	m_iDrawID;
	int	m_iOptionID;

public:
	void SetOptionID(const TILE_OPTION_ID eOptionID) { m_iOptionID = static_cast<int>(eOptionID); }
	void SetDrawID(const int iDrawID) { m_iDrawID = iDrawID; }

public:
	const int GetDrawID() const { return m_iDrawID; }
	TILE_OPTION_ID GetOptionID() const { return static_cast<TILE_OPTION_ID>(m_iOptionID); }
	
public:
	void RenderForEdit(HDC hDC);

public:
	virtual void Initialize();
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();
};

