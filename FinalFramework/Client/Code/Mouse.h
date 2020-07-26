#ifndef Mouse_h__
#define Mouse_h__

#include "UI.h"

class CMouse : public CUI
{
protected:
	explicit CMouse(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMouse();

public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

public:
	void	ShowOn()			{ m_bCursorRender = true; }
	void	ShowOff()			{ m_bCursorRender = false; }
	void	Toggle_Cursor()		{ m_bCursorRender = !m_bCursorRender; }

private:
	_bool						m_bCursorRender;

public:
	static CMouse* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free();
};

#endif // Mouse_h__
