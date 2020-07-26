
// ToolView.cpp : implementation of the CToolView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "MainFrm.h"
#include "ToolDoc.h"
#include "ToolView.h"
#include "MyForm.h"
#include "DynamicCamera.h"
#include "OriginGrid.h"
#include "Navigation.h"
#include "Navigation_Tri.h"
#include "AxisLines.h"
#include "MyMath.h"
#include "StaticObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_SYSCHAR()
	ON_WM_SYSCOMMAND()
	ON_WM_SYSKEYDOWN()
	ON_WM_SYSKEYUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

//	Global Variable
HWND g_hWnd;

// CToolView construction/destruction

CToolView::CToolView()
	: m_pDeviceClass(Engine::CGraphicDev::GetInstance())
	, m_pOriginGrid(nullptr)
{
	// TODO: add construction code here

}

CToolView::~CToolView()
{
	Safe_Release(m_pCollToolObject);

	for (auto& pObj : m_ObjectList)
		Safe_Release(pObj);

	Safe_Release(m_pAxisLines);
	Safe_Release(m_pSphere);
	Safe_Release(m_pNavigation);
	Engine::Safe_Release(m_pOriginGrid);
	Engine::Safe_Release(m_pCamera);
	Engine::Safe_Release(m_pGraphicDev);
	Safe_Release(m_pDeviceClass);
	Engine::Safe_Release(m_pManagement);

	//m_pDeviceClass->DestroyInstance();

	Release_Utility();
	Release_System();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CToolView drawing

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	m_pDeviceClass->Render_Begin(D3DXCOLOR(181.f / 255.f, 214.f / 255.f, 146.f / 255.f, 1.f));

	m_pCamera->Update_Camera(0.f);	
	m_pOriginGrid->Update_GameObject(0.f);
	
	Render_Scene();

	m_pDeviceClass->Render_End();
}


// CToolView printing

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CToolView diagnostics

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView message handlers

void CToolView::Render_Scene()
{

	switch (m_eToolMode)
	{
	case NAVI_TOOL:
	case OBJECT_TOOL:
		//m_pOriginGrid->Render_GameObject();
		//Get_Renderer()->Render_GameObject();
		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pOriginGrid->Render_GameObject(0.f);
		m_pNavigation->Render_Navigation_Tri();
		//Draw_AxisSphere();
		m_pAxisLines->Render_AxisLines(m_pCamera->Get_Distance(), &m_matAxisLines);
		Render_Objects();
		break;
	case COLLIDER_TOOL:
		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
		//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		m_pOriginGrid->Render_GameObject(0.f);
		Render_ColliderTool();
		

		break;
	}
}

void CToolView::Draw_PickVertex()
{
	DWORD	dwPreFill;
	m_pGraphicDev->GetRenderState(D3DRS_FILLMODE, &dwPreFill);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	for (auto& Pair : m_qPickPair)
	{
		memcpy(&matWorld.m[3][0], &Pair.second, sizeof(_vec3));
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
		m_pSphere->DrawSubset(0);
	}
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, dwPreFill);
}

void CToolView::Draw_AxisSphere()
{
	_vec3 vAxes[3], vAxisOrigin, vTemp;
	_matrix matView, matProj;

	m_pCamera->Get_View(&matView);
	m_pCamera->Get_Projection(&matProj);

	memcpy(&vAxisOrigin, &m_matAxisLines.m[3][0], sizeof(_vec3));

	for (_uint i = 0; i < 3; ++i)
	{
		memcpy(&vTemp, &m_matAxisLines.m[i][0], sizeof(_vec3));
		D3DXVec3Normalize(&vTemp, &vTemp);
		vAxes[i] = vAxisOrigin + vTemp * m_pCamera->Get_Distance()* 0.3f;

		_matrix matTrans;
		D3DXMatrixTranslation(&matTrans, vAxes[i].x, vAxes[i].y, vAxes[i].z);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matTrans);
		m_pSphere->DrawSubset(0);
	}
}

void CToolView::Pick_Axis(const CPoint* pMousePoint)
{
	_vec2 vMousePos = { (_float)pMousePoint->x, (_float)pMousePoint->y };
	_vec3 vAxes[3], vAxisOrigin, vTemp;
	_matrix matView, matProj;
	
	m_pCamera->Get_View(&matView);
	m_pCamera->Get_Projection(&matProj);
	
	//D3DXVec3TransformCoord(&vAxisOrigin, &_vec3(0.f, 0.f, 0.f), &m_matAxisLines);
	memcpy(&vAxisOrigin, &m_matAxisLines.m[3][0], sizeof(_vec3));

	for (_uint i = 0; i < 3; ++i)
	{
		memcpy(&vTemp, &m_matAxisLines.m[i][0], sizeof(_vec3));
		D3DXVec3Normalize(&vTemp, &vTemp);
		vAxes[i] = vAxisOrigin + vTemp * m_pCamera->Get_Distance() * 0.3f;

		D3DXVec3TransformCoord(&vAxes[i], &vAxes[i], &matView);
		D3DXVec3TransformCoord(&vAxes[i], &vAxes[i], &matProj);
	}

	D3DXVec3TransformCoord(&vAxisOrigin, &vAxisOrigin, &matView);
	D3DXVec3TransformCoord(&vAxisOrigin, &vAxisOrigin, &matProj);

	_vec3 vMouseProj;
	CMyMath::ViewPortToProj(&vMouseProj, &vMousePos, WINCX, WINCY);

	_float fMinDist = D3DXVec2Length(&((_vec2)vAxes[0] - (_vec2)vMouseProj));
	_float fDist;
	_ulong dwIndex = 0;

	for (_uint i = 1; i < 3; ++i)
	{
		fDist = D3DXVec2Length(&((_vec2)vAxes[i] - (_vec2)vMouseProj));
		if (fDist < fMinDist)
		{
			fMinDist = fDist;
			dwIndex = i;
		}
	}

	m_pAxisLines->Change_ActvateAxis((CAxisLines::ACT_AXIS)dwIndex);

}

void CToolView::Move_Axis(const CPoint * pMousePoint)
{
	_vec2 vPoint = { (_float)pMousePoint->x, (_float)pMousePoint->y };

	CAxisLines::ACT_AXIS eActAxis = m_pAxisLines->Get_ActivatedAxis();

	_vec3 vMoveDir = {};
	memcpy(&vMoveDir, &m_matAxisLines.m[eActAxis][0], sizeof(_vec3));

	_vec3 vChangedPos;


	m_pNavigation->Click_Update(&vPoint, &vMoveDir, m_qPickPair.back().first, m_pCamera->Get_View(), m_pCamera->Get_Projection(), &vChangedPos, m_pCamera->Get_Distance());
	memcpy(&m_matAxisLines.m[3][0], &vChangedPos, sizeof(_vec3));
	memcpy(&m_qPickPair.back().second, &vChangedPos, sizeof(_vec3));
}

void CToolView::Move_Object(const CPoint * pMousePoint)
{
	if (!m_pPickedObject)
		return;

	_vec2 vPoint = { (_float)pMousePoint->x, (_float)pMousePoint->y };

	CAxisLines::ACT_AXIS eActAxis = m_pAxisLines->Get_ActivatedAxis();

	_vec3 vMoveDir = {};
	memcpy(&vMoveDir, &m_matAxisLines.m[eActAxis][0], sizeof(_vec3));
	_vec3 vDirProj = {};
	_vec3 vOriginProj = {};
	D3DXVec3TransformCoord(&vDirProj, &vMoveDir, m_pCamera->Get_View());
	D3DXVec3TransformCoord(&vDirProj, &vDirProj, m_pCamera->Get_Projection());

	D3DXVec3TransformCoord(&vOriginProj, &COORD_ORIGIN, m_pCamera->Get_View());
	D3DXVec3TransformCoord(&vOriginProj, &vOriginProj, m_pCamera->Get_Projection());

	_vec2 vDir2D = { vDirProj.x - vOriginProj.x, vDirProj.y - vOriginProj.y };
	D3DXVec2Normalize(&vDir2D, &vDir2D);

	_vec3 vMousePos;
	CMyMath::ViewPortToProj(&vMousePos, &vPoint, WINCX, WINCY);
	
	_vec2 vDelta = { vMousePos.x - m_vClickedPos.x, vMousePos.y - m_vClickedPos.y };

	_float	fDot = D3DXVec2Dot(&vDir2D, &vDelta);

	_vec3 vChangedPos = m_vObjOldData + vMoveDir * fDot * m_pCamera->Get_Distance();
	m_pPickedObject->Get_Transform()->Set_Pos(&vChangedPos);

	memcpy(&m_matAxisLines.m[3][0], &vChangedPos, sizeof(_vec3));
}

void CToolView::Rotation_Object(const CPoint * pMousePoint)
{
	if (!m_pPickedObject)
		return;

	_vec2 vPoint = { (_float)pMousePoint->x, (_float)pMousePoint->y };

	CAxisLines::ACT_AXIS eActAxis = m_pAxisLines->Get_ActivatedAxis();

	_vec3 vMoveDir = {};
	memcpy(&vMoveDir, &m_matAxisLines.m[eActAxis][0], sizeof(_vec3));
	
	_vec3 vCamLook, vDotDir;
	m_pCamera->Get_Look(&vCamLook);
	D3DXVec3Cross(&vDotDir, &vMoveDir, &vCamLook);

	_vec3 vDirProj = {};
	_vec3 vOriginProj = {};
	D3DXVec3TransformCoord(&vDirProj, &vDotDir, m_pCamera->Get_View());
	D3DXVec3TransformCoord(&vDirProj, &vDirProj, m_pCamera->Get_Projection());

	D3DXVec3TransformCoord(&vOriginProj, &COORD_ORIGIN, m_pCamera->Get_View());
	D3DXVec3TransformCoord(&vOriginProj, &vOriginProj, m_pCamera->Get_Projection());

	_vec2 vDir2D = { vDirProj.x - vOriginProj.x, vDirProj.y - vOriginProj.y };
	D3DXVec2Normalize(&vDir2D, &vDir2D);

	_vec3 vMousePos;
	CMyMath::ViewPortToProj(&vMousePos, &vPoint, WINCX, WINCY);

	_vec2 vDelta = { vMousePos.x - m_vClickedPos.x, vMousePos.y - m_vClickedPos.y };

	_float	fDot = D3DXVec2Dot(&vDir2D, &vDelta);

	_matrix matRotAxis;
	D3DXMatrixRotationAxis(&matRotAxis, &vMoveDir, fDot * m_pCamera->Get_Distance() / D3DX_PI);
	_float	fRotX = atan2f(matRotAxis._32, matRotAxis._33);
	_float	fRotY = -asinf(matRotAxis._31);
	_float	fRotZ = atan2f(matRotAxis._21, matRotAxis._11);

	_vec3 vChangedAngle = m_vObjOldData + _vec3(fRotX, fRotY, fRotZ);
	m_pPickedObject->Get_Transform()->Set_Angle(&vChangedAngle);

}

void CToolView::Scale_Object(const CPoint * pMousePoint)
{
	if (!m_pPickedObject)
		return;

	_vec2 vPoint = { (_float)pMousePoint->x, (_float)pMousePoint->y };

	CAxisLines::ACT_AXIS eActAxis = m_pAxisLines->Get_ActivatedAxis();

	_vec3 vMoveDir = {};
	memcpy(&vMoveDir, &m_matAxisLines.m[eActAxis][0], sizeof(_vec3));
	_vec3 vDirProj = {};
	_vec3 vOriginProj = {};

	D3DXVec3TransformCoord(&vDirProj, &vMoveDir, m_pCamera->Get_View());
	D3DXVec3TransformCoord(&vDirProj, &vDirProj, m_pCamera->Get_Projection());

	D3DXVec3TransformCoord(&vOriginProj, &COORD_ORIGIN, m_pCamera->Get_View());
	D3DXVec3TransformCoord(&vOriginProj, &vOriginProj, m_pCamera->Get_Projection());

	_vec2 vDir2D = { vDirProj.x - vOriginProj.x, vDirProj.y - vOriginProj.y };
	D3DXVec2Normalize(&vDir2D, &vDir2D);

	_vec3 vMousePos;
	CMyMath::ViewPortToProj(&vMousePos, &vPoint, WINCX, WINCY);

	_vec2 vDelta = { vMousePos.x - m_vClickedPos.x, vMousePos.y - m_vClickedPos.y };

	_float	fDot = D3DXVec2Dot(&vDir2D, &vDelta);

	_vec3 vScaleDir = { 1.f, 1.f, 1.f };
	//switch (eActAxis)
	//{
	//case CAxisLines::ACT_AXIS_X:	vScaleDir = AXIS_X;	break;
	//case CAxisLines::ACT_AXIS_Y:	vScaleDir = AXIS_Y;	break;
	//case CAxisLines::ACT_AXIS_Z:	vScaleDir = AXIS_Z;	break;
	//}

	_vec3 vChangedScale = m_vObjOldData + vScaleDir * fDot * m_pCamera->Get_Distance() * 0.00001f;

	m_pPickedObject->Get_Transform()->Set_Scale(&vChangedScale);


}

void CToolView::Add_Vertex(const CPoint* pMousePoint)
{
	_vec2 vPoint = { (_float)pMousePoint->x, (_float)pMousePoint->y };

	if (m_pMyForm->m_NaviTool.m_Auto.GetCheck())
	{
		_vec3 vPickingPos;
		D3DXPLANE tPlane = { 0.f, 1.f, 0.f, -m_pNaviTool->m_fVertexY };
		CMyMath::PickingOnPlane(&vPickingPos, &vPoint, WINCX, WINCY, m_pCamera->Get_Projection(), m_pCamera->Get_View(), &tPlane);

		m_pNavigation->Add_Vertex(&vPickingPos);
	}
	else if (m_pMyForm->m_NaviTool.m_AddFromPicked.GetCheck())
	{
		if (m_qPickPair.size() >= 2)
		{
			_vec3 vPickingPos;
			D3DXPLANE tPlane = { 0.f, 1.f, 0.f, -m_pNaviTool->m_fVertexY };
			CMyMath::PickingOnPlane(&vPickingPos, &vPoint, WINCX, WINCY, m_pCamera->Get_Projection(), m_pCamera->Get_View(), &tPlane);

			_ulong dwIndex = m_pNavigation->Add_Vertex(&vPickingPos, (m_qPickPair[m_qPickPair.size()-2]).first, m_qPickPair.back().first);
			m_pNavigation->Change_VertexColor(dwIndex, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
			m_qPickPair.push_back(make_pair(dwIndex, vPickingPos));
			m_pNavigation->Change_VertexColor(m_qPickPair.front().first, D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
			m_qPickPair.pop_front();
		}
	}
	else if (m_pMyForm->m_NaviTool.m_ConnectVertex.GetCheck())
	{
		if (m_qPickPair.size() >= 2)
		{
			Connect_Vertex(pMousePoint);
		}
	}
	else if (m_pMyForm->m_NaviTool.m_AllNew.GetCheck())
	{
		_vec3 vPickingPos;
		D3DXPLANE tPlane = { 0.f, 1.f, 0.f, -m_pNaviTool->m_fVertexY };
		CMyMath::PickingOnPlane(&vPickingPos, &vPoint, WINCX, WINCY, m_pCamera->Get_Projection(), m_pCamera->Get_View(), &tPlane);

		if (m_pMyForm->m_NaviTool.m_vecVertexPos.size() < 2)
		{
			m_pMyForm->m_NaviTool.m_vecVertexPos.push_back(vPickingPos);
		}
		else
		{
			m_pMyForm->m_NaviTool.m_vecVertexPos.push_back(vPickingPos);

			m_pNavigation->Add_Vertex(&m_pMyForm->m_NaviTool.m_vecVertexPos);

			m_pMyForm->m_NaviTool.m_vecVertexPos.clear();
		}
	}
}

void CToolView::Connect_Vertex(const CPoint * pMousePoint)
{
	_vec2 vPoint = { (_float)pMousePoint->x, (_float)pMousePoint->y };

	_ulong	dwPickIdx;
	_vec3	vIdxPos;

	if (m_pNavigation->Picking_Vertex(&vPoint, m_pCamera->Get_View(), m_pCamera->Get_Projection(), &dwPickIdx, &vIdxPos))
	{
		m_pNavigation->Connect_Vertex(dwPickIdx, (m_qPickPair[m_qPickPair.size() - 2]).first, m_qPickPair.back().first);
		m_pNavigation->Change_VertexColor(dwPickIdx, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
		m_qPickPair.push_back(make_pair(dwPickIdx, vIdxPos));
		m_pNavigation->Change_VertexColor(m_qPickPair.front().first, D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
		m_qPickPair.pop_front();
	}
}

void CToolView::Disunite_Vertex()
{
	if (m_qPickPair.size() < 3)
		return;

	_ulong dwPicked[3] = {};
	
	_ulong dwIdx = 0;
	for (auto& Pair : m_qPickPair)
	{
		dwPicked[dwIdx] = Pair.first;
		++dwIdx;
	}
		
	m_pNavigation->Disunite_Vertex(dwPicked[2], dwPicked[1], dwPicked[0]);
	Invalidate(FALSE);
}

void CToolView::Remove_Vertex()
{
	if (m_qPickPair.empty())
		return;

	m_pNavigation->Remove_Vertex(m_qPickPair.back().first);
	m_qPickPair.pop_back();
	
}

void CToolView::Remove_Object()
{
	auto iter = m_ObjectList.begin();
	auto iterEnd = m_ObjectList.end();

	_bool	bFind = false;
	for (; iter != iterEnd; ++iter)
	{
		if (m_pPickedObject == *iter)
		{
			bFind = true;
			break;
		}
	}

	if (bFind)
	{
		Safe_Release(m_pPickedObject);
		m_ObjectList.erase(iter);
	}
}

void CToolView::Pick_VertexOnPlane(const CPoint * pMousePoint, D3DXPLANE* pPlane)
{
	_vec2 vPoint = { (_float)pMousePoint->x, (_float)pMousePoint->y };
	_vec3 vPickingPos;
	CMyMath::PickingOnPlane(&vPickingPos, &vPoint, WINCX, WINCY, m_pCamera->Get_Projection(), m_pCamera->Get_View(), pPlane);

	_ulong dwPickIdx;
	_vec3 vIdxPos;

	if (m_pNavigation->Picking_Vertex(&vPickingPos, &dwPickIdx, &vIdxPos))
		Add_PickPair((WORD)dwPickIdx, vIdxPos);
	
}

void CToolView::Pick_VertexOnProj(const CPoint * pMousePoint)
{
	_vec2 vPoint = { (_float)pMousePoint->x, (_float)pMousePoint->y };

	_ulong dwPickIdx;
	_vec3 vIdxPos;

	if (m_pNavigation->Picking_Vertex(&vPoint, m_pCamera->Get_View(), m_pCamera->Get_Projection(), &dwPickIdx, &vIdxPos))
		Add_PickPair((WORD)dwPickIdx, vIdxPos);
	
}

void CToolView::Render_Objects()
{
	if (m_bObjWireFrame)
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	for (auto& pObj : m_ObjectList)
	{
		pObj->Update_GameObject(0.f);
	}
	Get_Renderer()->Render_Tool(0.f);
}

void CToolView::Pick_Object(const CPoint * pMousePoint)
{
	_vec2 vPoint = { (_float)pMousePoint->x, (_float)pMousePoint->y };
	_vec3 vMousePos;
	CMyMath::ViewPortToProj(&vMousePos, &vPoint, WINCX, WINCY);

	_vec3 vPos = {};
	_matrix matView, matProj;

	m_pCamera->Get_View(&matView);;
	m_pCamera->Get_Projection(&matProj);

	_float fMinDist = FLT_MAX;
	_float fDist = 0.f;
	m_pPickedObject = nullptr;

	for (auto& pObj : m_ObjectList)
	{
		pObj->Get_Transform()->Get_Info(INFO_POS, &vPos);

		D3DXVec3TransformCoord(&vPos, &vPos, &matView);
		D3DXVec3TransformCoord(&vPos, &vPos, &matProj);

		fDist = D3DXVec2Length(&((_vec2)vPos - (_vec2)vMousePos));
		if (fDist < fMinDist)
		{
			fMinDist = fDist;
			m_pPickedObject = pObj;
		}
	}

	Update_ObjectToolVariable();

	Refresh_MatAxisFromPickedObject();
}

void CToolView::Update_ObjectToolVariable()
{
	CTransform* pTransform = m_pPickedObject->Get_Transform();

	_vec3 vScale, vPos, vAngle;
	pTransform->Get_Info(INFO_POS, &vPos);
	pTransform->Get_Angle(&vAngle);
	pTransform->Get_Scale(&vScale);

	m_pMyForm->m_ObjectTool.m_RotX = vAngle.x;
	m_pMyForm->m_ObjectTool.m_RotY = vAngle.y;
	m_pMyForm->m_ObjectTool.m_RotZ = vAngle.z;

	m_pMyForm->m_ObjectTool.m_PosX = vPos.x;
	m_pMyForm->m_ObjectTool.m_PosY = vPos.y;
	m_pMyForm->m_ObjectTool.m_PosZ = vPos.z;

	m_pMyForm->m_ObjectTool.m_ScaleX = vScale.x;
	m_pMyForm->m_ObjectTool.m_ScaleY = vScale.y;
	m_pMyForm->m_ObjectTool.m_ScaleZ = vScale.z;

	m_pMyForm->m_ObjectTool.UpdateData(FALSE);
}

void CToolView::Refresh_MatAxisFromPickedObject()
{
	if (!m_pPickedObject)
		return;
	
	_float fCamDist = m_pCamera->Get_Distance();
	_matrix matScale, matRotX, matRotY, matRotZ, matTrans;
	_vec3 vAngle, vPos;
	m_pPickedObject->Get_Transform()->Get_Angle(&vAngle);
	m_pPickedObject->Get_Transform()->Get_Info(INFO_POS, &vPos);

	//D3DXMatrixScaling(&matScale, fCamDist, fCamDist, fCamDist);
	D3DXMatrixRotationX(&matRotX, vAngle.x);
	D3DXMatrixRotationY(&matRotY, vAngle.y);
	D3DXMatrixRotationZ(&matRotZ, vAngle.z);
	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);

	m_matAxisLines = matRotX * matRotY * matRotZ * matTrans;
}

void CToolView::Change_CellOption(const _ulong & dwCellIdx, const _ulong & dwCellOption)
{

}

void CToolView::Change_CellOption(const CPoint * pMousePoint, const _ulong & dwCellOption)
{
	_vec2 vMousePos = { (_float)pMousePoint->x, (_float)pMousePoint->y };

	_vec3 vMouseProj = { 2.f * vMousePos.x / WINCX - 1.f, -2.f * vMousePos.y / WINCY + 1.f, 0.f };

	_matrix matView, matProj;

	m_pCamera->Get_View(&matView);
	m_pCamera->Get_Projection(&matProj);

	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXMatrixInverse(&matProj, NULL, &matProj);

	_vec3 vMouseWorld;
	D3DXVec3TransformCoord(&vMouseWorld, &vMouseProj, &matProj);
	D3DXVec3TransformCoord(&vMouseWorld, &vMouseWorld, &matView);

	_vec3 vMouseOrigin = { 0.f, 0.f, 0.f }; // Origin in View State
	D3DXVec3TransformCoord(&vMouseOrigin, &vMouseOrigin, &matView);

	m_pNavigation->Change_CellOption(&(vMouseOrigin), &(vMouseWorld - vMouseOrigin), dwCellOption);
}

void CToolView::Render_ColliderTool()
{
	Render_Bones();
	
	if (m_pCollToolObject)
	{
		if (m_bWireFrame)
		{
			m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		}
		m_pCollToolObject->Update_GameObject(0.f);
		m_pCollToolObject->Render_GameObject(0.f);
	}
	Get_Renderer()->Render_GameObject(0.f);

}

void CToolView::Render_Bones()
{
	if (!m_pCollToolObject)
		return;

	D3DXFRAME* pRootFrame = dynamic_cast<CDynamicMesh*>(
		m_pCollToolObject->Get_Component(ID_STATIC, L"Com_Mesh"))->Get_RootFrame();

	const _matrix* pParentObjMatrix = dynamic_cast<CTransform*>(m_pCollToolObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_WorldMatrix();

	queue<D3DXFRAME_DERIVED*> qFrames;

	if (pRootFrame->pFrameSibling)
		qFrames.push((D3DXFRAME_DERIVED*)pRootFrame->pFrameSibling);
	if (pRootFrame->pFrameFirstChild)
		qFrames.push((D3DXFRAME_DERIVED*)pRootFrame->pFrameFirstChild);

	while (!qFrames.empty())
	{
		D3DXFRAME_DERIVED* pFrame = qFrames.front();
		
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &(pFrame->CombinedTransformationMatrix * *pParentObjMatrix));
		m_pSphere->DrawSubset(0);


		if (pFrame->pFrameSibling)
			qFrames.push((D3DXFRAME_DERIVED*)pFrame->pFrameSibling);
		if (pFrame->pFrameFirstChild)
			qFrames.push((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);

		qFrames.pop();
	}
}

void CToolView::Add_PickPair(WORD wPickIdx, const D3DXVECTOR3 & vIdxPos)
{
	m_qPickPair.push_back(make_pair(wPickIdx, vIdxPos));
	m_pNaviTool->m_fVertexY = vIdxPos.y;
	m_pNaviTool->UpdateData(FALSE);

	memcpy(&m_matAxisLines.m[3][0], &vIdxPos, sizeof(_vec3));

	m_pNavigation->Change_VertexColor(wPickIdx, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	if (m_qPickPair.size() > 3)
	{
		if (m_qPickPair.front().first != wPickIdx)
			m_pNavigation->Change_VertexColor(m_qPickPair.front().first, D3DXCOLOR(0.f, 1.f, 0.f, 1.f));

		m_qPickPair.pop_front();
	}
}

void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class

	g_hWnd = m_hWnd;

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK_MSG(pFrameWnd, L"pFrameWnd is null");

	m_pMyForm = dynamic_cast<CMyForm*>(pFrameWnd->m_MainSplitter.GetPane(0, 0));
	NULL_CHECK_MSG(m_pMyForm, L"m_pMyForm is null");

	m_pNaviTool = &m_pMyForm->m_NaviTool;
	m_pObjectTool = &m_pMyForm->m_ObjectTool;
	m_pColliderTool = &m_pMyForm->m_ColliderTool;

	RECT rcFrame = {};
	pFrameWnd->GetWindowRect(&rcFrame);
	::SetRect(&rcFrame, 0, 0, rcFrame.right - rcFrame.left, rcFrame.bottom - rcFrame.top);

	RECT rcView = {};

	GetClientRect(&rcView);

	int iGapX = rcFrame.right - rcView.right;
	int iGapY = rcFrame.bottom - rcView.bottom;

	// 프레임윈도우의 크기를 새로 설정.
	pFrameWnd->SetWindowPos(nullptr, 0, 0, WINCX + iGapX, WINCY + iGapY, SWP_NOZORDER | SWP_NOMOVE);

	Engine::CGraphicDev::GetInstance()->Ready_GraphicDev(g_hWnd, Engine::MODE_WIN, WINCX, WINCY, &m_pDeviceClass);
	NULL_CHECK_MSG(m_pDeviceClass, L"m_pDeviceClass is null");
	m_pDeviceClass->AddRef();

	m_pGraphicDev = m_pDeviceClass->GetDevice();
	NULL_CHECK_MSG(m_pGraphicDev, L"m_pGraphicDev is null");
	m_pGraphicDev->AddRef();
	

	m_pCamera = CDynamicCamera::Create(m_pGraphicDev);
	NULL_CHECK_MSG(m_pCamera, L"Camera is null");
	Engine::Add_Camera(Engine::CAM_DYNAMIC, L"Cam_Dynamic", m_pCamera);
	Engine::SetUp_MainCamera(Engine::CAM_DYNAMIC, L"Cam_Dynamic");


	Reserve_ContainerSize(RESOURCE_END);

	if (FAILED(Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_RcCol", BUFFER_RCCOL)))
		MessageBox(L"Failed", L"Failed to Ready Buffer_RcCol.", MB_OK);

	if (FAILED(Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_RcColTex", BUFFER_RCCOLTEX)))
		MessageBox(L"Failed", L"Failed to Ready Buffer_RcColTex.", MB_OK);

	if (FAILED(Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_TriCol", BUFFER_TRICOL)))
		MessageBox(L"Failed", L"Failed to Ready Buffer_TriCol.", MB_OK);

	if (FAILED(Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_CubeTex", BUFFER_CUBETEX)))
		MessageBox(L"Failed", L"Failed to Ready Buffer_CubeTex.", MB_OK);

	if (FAILED(Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_LineCol", BUFFER_LINECOL)))
		MessageBox(L"Failed", L"Failed to Ready Buffer_LineCol.", MB_OK);


	m_pOriginGrid = COriginGrid::Create(m_pGraphicDev);
	NULL_CHECK_MSG(m_pOriginGrid, L"OriginGrid is null");

	m_pNavigation = CNavigation_Tri::Create(m_pGraphicDev);
	NULL_CHECK_MSG(m_pNavigation, L"Navigation is null");

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


	D3DXCreateSphere(m_pGraphicDev, 0.2f, 10, 10, &m_pSphere, NULL);

	m_pAxisLines = CAxisLines::Create(m_pGraphicDev);
	NULL_CHECK_MSG(m_pAxisLines, L"AxisLines Create Failed.");

	D3DXMatrixIdentity(&m_matAxisLines);	

	Ready_CameraMgr(m_pGraphicDev, WINCX, WINCY);

	FAILED_CHECK_MSG(Engine::Create_Management(m_pGraphicDev, &m_pManagement), L"Management Init Failed");
	m_pManagement->AddRef();

	Ready_Renderer(m_pGraphicDev);

	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f);
	tLightInfo.Direction = _vec3(0.f, -1.f, 1.f);

	FAILED_CHECK_MSG(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), L"Light_Failed");

}


void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default


	
	if (m_bMButton)
	{
		_vec2 vPoint = { (_float)point.x, (_float)point.y };
		m_pCamera->MiddleClick_Update(&vPoint);
		Invalidate(FALSE);
	}
	
	if (m_bLButton)
	{
		switch (m_eLeftFunc)
		{
		case ADD_VERTEX:
			break;
		case MODIFY_VERTEX:
			Move_Axis(&point);
			Invalidate(FALSE);
			break;
		case MOVE_OBJECT:
			if (m_bShift && m_bObjectPicked)
			{
				if (m_pMyForm->m_ObjectTool.m_PositionRadio.GetCheck())
					Move_Object(&point);
				else if (m_pMyForm->m_ObjectTool.m_RotationRadio.GetCheck())
					Rotation_Object(&point);
				else if (m_pMyForm->m_ObjectTool.m_ScaleRadio.GetCheck())
					Scale_Object(&point);

				Update_ObjectToolVariable();
			}
			Invalidate(FALSE);
			break;
		}
	}

	
	CView::OnMouseMove(nFlags, point);
}


void CToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	switch (nChar)
	{
	case VK_CONTROL:
		m_bCtrl = true;
		break;
	case VK_SHIFT:
		m_bShift = true;
		break;
	case VK_DELETE:
		switch (m_eToolMode)
		{
		case NAVI_TOOL:
			Remove_Vertex();
			break;
		case OBJECT_TOOL:
			Remove_Object();
			break;
		case COLLIDER_TOOL:
			break;
		}
		Invalidate(FALSE);
		break;
	case 'W':
		m_pCamera->Move_LookForward();
		Invalidate(FALSE);
		break;
	case 'S':
		m_pCamera->Move_LookBackward();
		Invalidate(FALSE);
		break;
	case 'A':
		m_pCamera->Move_RightBackward();
		Invalidate(FALSE);
		break;
	case 'D':
		m_pCamera->Move_RightForward();
		Invalidate(FALSE);
		break;
	case 'Z':
		if (m_pMyForm->m_NaviRadio.GetCheck())
		{
			m_pNavigation->Undo_Vertex();
			m_qPickPair.clear();
		}
		Invalidate(FALSE);
		break;
	case 'X':
		if (m_pMyForm->m_NaviRadio.GetCheck())
		{
			m_pNavigation->Redo_Vertex();
		}
		Invalidate(FALSE);
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CToolView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	switch (nChar)
	{
	case VK_CONTROL:
		m_bCtrl = false;
		break;
	case VK_SHIFT:
		m_bShift = false;
		break;
	}

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_bLButton = true;

	switch (m_eLeftFunc)
	{
	case ADD_VERTEX:
		Add_Vertex(&point);
		break;
	case MODIFY_VERTEX:
		if (m_bShift)
			m_pNavigation->Click(&_vec2((_float)point.x, (_float)point.y), &m_qPickPair.back().second);		
		else
			Pick_Axis(&point);
		break;
	case MOVE_OBJECT:
		if (m_bShift)
		{
			if (!m_bObjectPicked)
			{
				m_bObjectPicked = true;
				m_vClickedPos = { (_float)point.x, (_float)point.y };
				_vec3 vTemp;
				CMyMath::ViewPortToProj(&vTemp, &m_vClickedPos, WINCX, WINCY);
				m_vClickedPos = { vTemp.x, vTemp.y };
				if (m_pPickedObject)
				{
					if (m_pMyForm->m_ObjectTool.m_PositionRadio.GetCheck())
						m_pPickedObject->Get_Transform()->Get_Info(INFO_POS, &m_vObjOldData);
					else if(m_pMyForm->m_ObjectTool.m_RotationRadio.GetCheck())
						m_pPickedObject->Get_Transform()->Get_Angle(&m_vObjOldData);
					else if(m_pMyForm->m_ObjectTool.m_ScaleRadio.GetCheck())
						m_pPickedObject->Get_Transform()->Get_Scale(&m_vObjOldData);
				}
			}
		}
		else
			Pick_Axis(&point);
		break;
	case CHANGE_CELL_OPTION:
		Change_CellOption(&point, m_pMyForm->m_NaviTool.Get_ChangeOption());

		break;
	case LEFT_CLICK_FUNC_END:
		
		break;
	default:
		break;
	}


	Invalidate(FALSE);

	CView::OnLButtonDown(nFlags, point);
}


void CToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_bLButton = false;

	if (m_bShift)
		m_pNavigation->ClickReset();

	m_bObjectPicked = false;
	
	if (OBJECT_TOOL == m_eToolMode)
		Refresh_MatAxisFromPickedObject();

	CView::OnLButtonUp(nFlags, point);
}


BOOL CToolView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	m_pCamera->ZoomInOut(zDelta * 0.005f);
	Invalidate(FALSE);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CToolView::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if (nChar == VK_MENU)
	{
		m_bAlt = true;
		return;
	}

	CView::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

void CToolView::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_MENU)
	{
		m_bAlt = false;
		return;
	}

	CView::OnSysKeyUp(nChar, nRepCnt, nFlags);
}


void CToolView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_bMButton = true;
	_vec2 vPoint = { (_float)point.x, (_float)point.y };

	if (m_bAlt)
		m_pCamera->MiddleClick(CDynamicCamera::CAM_INPUT_ROT, &vPoint);
	else
		m_pCamera->MiddleClick(CDynamicCamera::CAM_INPUT_MOVE, &vPoint);
	

	CView::OnMButtonDown(nFlags, point);
}


void CToolView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_bMButton = false;

	m_pCamera->MiddleClickReset();

	CView::OnMButtonUp(nFlags, point);
}


void CToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	switch (m_eRightFunc)
	{
	case PICK_VERTEX:
		Pick_VertexOnProj(&point);
		break;
	case PICK_OBJECT:
		Pick_Object(&point);

		break;
	}
	Invalidate(FALSE);
	CView::OnRButtonDown(nFlags, point);
}

