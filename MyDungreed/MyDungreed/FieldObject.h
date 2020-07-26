#pragma once
#include "MoveObj.h"

enum FIELD_OBJECT_TYPE { FOT_STELE, FOT_THORN, FOT_END };
class CFieldObject :
	public CMoveObj
{
public:
	CFieldObject();
	virtual ~CFieldObject();

public:
	virtual void Initialize();
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();

private:
	FIELD_OBJECT_TYPE m_eFieldObjectType;

public:
	void SetFieldObjectType(FIELD_OBJECT_TYPE eFieldObjectType) { m_eFieldObjectType = eFieldObjectType; }

public:
	const FIELD_OBJECT_TYPE GetFieldObjectType() const { return m_eFieldObjectType; }
};

