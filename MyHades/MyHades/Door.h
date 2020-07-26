#pragma once
#include "StaticObject.h"
class CDoor :
	public CStaticObject
{
private:
	enum STATE { CLOSE, OPENING, OPENED, END};

private:
	CDoor();
public:
	virtual ~CDoor();

public:// Inherited via CStaticObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	virtual float GetLayerStd() override { return 0.f; }

public:
	static CDoor* Create(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vScale);

public:
	const bool GetCanOpen() const { return m_bCanOpen; }
	void DoorOpen() { m_eCurState = CDoor::OPENING; }

private:
	void StateUpdate();
	void CloseUpdate();
	void OpeningUpdate();
	void OpenUpdate();

private:
	CDoor::STATE m_eCurState;

	FRAME	m_tFrame;
	bool	m_bCanOpen;
	int		m_iCX;
	int		m_iCY;
};

