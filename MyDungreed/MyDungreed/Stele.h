#pragma once
#include "FieldObject.h"

enum STELE_TYPE	 { STELE_H, STELE_VL, STELE_VR, STELE_END };
enum STELE_STATE { STELE_CLOSE, STELE_IDLE, STELE_OPEN, STELE_STATE_END };
class CStele :
	public CFieldObject
{
public:
	CStele();
	virtual ~CStele();

public:
	// Inherited via CObj
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	STELE_TYPE	m_eSteleType;
	STELE_STATE m_ePrevState;
	STELE_STATE m_eState;
	int			m_iStartFrame;

private:
	void Idle();
	void Close();
	void Open();

public:
	void SetRatio(float fRatio) { m_fRatio = fRatio; }
	void SetSteleType(STELE_TYPE eSteleType);
	void StateChange(STELE_STATE eCurType);
};

