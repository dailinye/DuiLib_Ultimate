#include "StdAfx.h"

namespace DuiLib
{
	class CContextMenuWnd : public CWindowWnd, public INotifyUI
	{
	public:
		CContextMenuWnd(CControlUI* pOwner, LPCTSTR pstrXmlFile, IDialogBuilderCallback* pCallback = NULL)
			:m_pOwner(pOwner)
			, m_pRealOwner(NULL)
			, m_sXmlFile(pstrXmlFile)
			, m_pCallback(pCallback)
			, m_iSelectMenuId(-1)
		{
			Init();
		}

		virtual ~CContextMenuWnd()
		{

		}

		virtual UINT GetClassStyle() const
		{
			LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
			//styleValue |= CS_DROPSHADOW;
			return styleValue;
		}

		virtual LPCTSTR GetWindowClassName(void) const
		{
			return _T("MenuWnd");
		}

		virtual void OnFinalMessage(HWND hWnd)
		{
			if (m_iSelectMenuId >= 0)
				m_pRealOwner->GetManager()->SendNotify(m_pRealOwner, DUI_MSGTYPE_MENUITEM_CLICK, NULL, m_iSelectMenuId, true);
			CWindowWnd::OnFinalMessage(hWnd);
			delete m_pOwner;
			delete this;
		}

		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			if (uMsg == WM_CREATE) {
				LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
				styleValue &= ~WS_CAPTION;
				::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

				if (!m_pRealOwner->GetManager()) return 0;

				m_paintManager.Init(m_hWnd);

				CDialogBuilder builder;
				CControlUI* pControl = builder.Create(m_sXmlFile.GetData(), NULL, m_pCallback, &m_paintManager);
				if (NULL == pControl) return 0;
				if (NULL == pControl->GetInterface(DUI_CTR_MENU)) return 0;
				CMenuUI* pMenu = static_cast<CMenuUI*>(pControl);
				if (NULL == pMenu) return 0;
				// set the owner of the menu
				pMenu->SetOwner(m_pOwner);
				m_paintManager.AttachDialog(pMenu);

				m_paintManager.SetDPI(m_pRealOwner->GetManager()->GetDPIObj()->GetDPI());

				// send the notify for user to change the menu item
				m_pRealOwner->GetManager()->SendNotify(m_pRealOwner, DUI_MSGTYPE_MENUADDED, (WPARAM)pMenu);
				pMenu->GetHeader()->SetVisible(false);

				// initial the window height
				SIZE cxyBorderRound = pMenu->GetBorderRound();
				m_paintManager.SetRoundCorner(cxyBorderRound.cx, cxyBorderRound.cy);
				int cyHeight = 0, cxWidth = pMenu->GetFixedWidth();
				CMenuItemUI* pMenuItem = NULL;
				for (int i = 0; i < pMenu->GetCount(); ++i)
				{
					pMenuItem = static_cast<CMenuItemUI*>(pMenu->GetItemAt(i));
					if (pMenuItem && pMenuItem->IsVisible()) {
						cyHeight += pMenuItem->GetFixedHeight();
					}
				}
				m_paintManager.SetInitSize(cxWidth, cyHeight);				

				POINT mousePos = m_pRealOwner->GetManager()->GetMousePos();
				if (pMenu->GetMenuPaddingEnabled())
				{
					const RECT realOwnerPos = m_pRealOwner->GetPos();
					if (pMenu->GetMenuPadding().right != 0)
					{
						mousePos.x = realOwnerPos.right + pMenu->GetMenuPadding().right;
					}
					else
					{
						mousePos.x = realOwnerPos.left + pMenu->GetMenuPadding().left;
					}
					mousePos.y = realOwnerPos.top + pMenu->GetMenuPadding().top;
				}

				// calculate the position of the context menu
				SIZE size = m_paintManager.GetInitSize();
				RECT rc = { 0 };
				rc.left = mousePos.x;
				rc.top = mousePos.y;
				if (pMenu->GetTriangleEnabled())
				{
					rc.top = m_pRealOwner->GetPos().top + pMenu->GetTrianglePadding().top;
				}
				rc.right = rc.left + size.cx;
				rc.bottom = rc.top + size.cy;

				::MapWindowRect(m_pRealOwner->GetManager()->GetPaintWindow(), HWND_DESKTOP, &rc);

				if (!pMenu->GetMenuPaddingEnabled())
				{
					MONITORINFO oMonitor = { 0 };
					oMonitor.cbSize = sizeof(oMonitor);
					::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
					CDuiRect rcWork = oMonitor.rcWork;
					if (rc.right > rcWork.right)
					{
						rc.right = rc.left;
						rc.left = rc.right - size.cx;
					}
					if (rc.bottom > rcWork.bottom)
					{
						rc.bottom = rc.top;
						rc.top = rc.bottom - size.cy;
					}
				}
				::MoveWindow(*this, rc.left, rc.top, size.cx, size.cy, TRUE);

				// paint the corner and the triangle
				HRGN hRoudRgn = NULL, hTriangleRgn = NULL;
				SIZE szRoundCorner = m_paintManager.GetRoundCorner();
				LONG triangleHeight = pMenu->GetTriangleEnabled() ? (pMenu->GetMenuPadding().top - pMenu->GetTrianglePadding().top) : 0;

				CDuiRect rcWnd(rc);
				rcWnd.Offset(-rcWnd.left, -rcWnd.top);

				if (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) {
					hRoudRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top + triangleHeight, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
				}
				else {
					hRoudRgn = ::CreateRectRgn(rcWnd.left, rcWnd.top + triangleHeight, rcWnd.right, rcWnd.bottom);
				}
				if (pMenu->GetTriangleEnabled()) {
					LONG triangleWidth = triangleHeight * 1000 / 1732;
					POINT trianglePos;
					trianglePos.x = pMenu->GetTrianglePadding().right;
					trianglePos.y = pMenu->GetTrianglePadding().top;
					trianglePos.x += m_pRealOwner->GetPos().right;
					trianglePos.y += m_pRealOwner->GetPos().top;
					::ClientToScreen(m_pRealOwner->GetManager()->GetPaintWindow(), &trianglePos);
					::ScreenToClient(*this, &trianglePos);

					POINT points[3] = { 0 };

					points[1].x = trianglePos.x;
					points[1].y = trianglePos.y;

					points[0].x = points[1].x - triangleWidth - 3;
					points[0].y = rcWnd.top + triangleHeight;

					points[2].x = points[1].x + triangleWidth + 3;
					points[2].y = rcWnd.top + triangleHeight;

					hTriangleRgn = ::CreatePolygonRgn(points, 3, ALTERNATE);
				}

				if (NULL != hRoudRgn && NULL != hTriangleRgn) {
					::CombineRgn(hRoudRgn, hRoudRgn, hTriangleRgn, RGN_OR);
					::SetWindowRgn(*this, hRoudRgn, TRUE);
					::DeleteObject(hTriangleRgn);
					::DeleteObject(hRoudRgn);
				}
				else if (NULL != hRoudRgn) {
					::SetWindowRgn(*this, hRoudRgn, TRUE);
					::DeleteObject(hRoudRgn);
				}
				else if (NULL != hTriangleRgn) {
					::SetWindowRgn(*this, hTriangleRgn, TRUE);
					::DeleteObject(hTriangleRgn);
				}

				m_paintManager.AddNotifier(this);

				return 0;
			}
			else if (uMsg == WM_KILLFOCUS) {
				if (m_hWnd != (HWND)wParam) PostMessage(WM_CLOSE, NULL, -1);
			}
			else if (uMsg == WM_CLOSE) {
				m_iSelectMenuId = (int)lParam;
			}
			LRESULT lRes = 0;
			if (m_paintManager.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
			return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		}

		virtual void Notify(TNotifyUI& msg)
		{
			if (!m_pRealOwner->GetManager()) return;
			m_pRealOwner->GetManager()->SendNotify(msg);
		}

		void Init()
		{
			if (NULL == m_pOwner) return;
			m_pRealOwner = m_pOwner->GetParent();
			// the real owner is null, should release the memory of the owner
			if (NULL == m_pRealOwner) return OnFinalMessage(NULL);

			Create(NULL, NULL, WS_VISIBLE | WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_TOPMOST);

			if (!::IsWindow(m_hWnd)) return;

			ShowWindow();

			::SetForegroundWindow(m_hWnd);
		}

	private:
		CControlUI* m_pOwner;
		CControlUI* m_pRealOwner;
		CDuiString m_sXmlFile;
		IDialogBuilderCallback* m_pCallback;
		CPaintManagerUI m_paintManager;
		int m_iSelectMenuId;
	};

	IMPLEMENT_DUICONTROL_UINIT_DATATEMPLATE(CMenuItemUI)

	CMenuItemUI::CMenuItemUI()
		:m_id(-1)
	{
	}

	CMenuItemUI::~CMenuItemUI(void)
	{
	}

	CMenuItemUI* CMenuItemUI::DoInitDataTemplate(CMenuItemUI* pInstance)
	{
		pInstance->m_id = this->m_id;
		return pInstance;
	}

	LPCTSTR CMenuItemUI::GetClass() const
	{
		return _T("MenuItemUI");
	}

	LPVOID CMenuItemUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcscmp(pstrName, DUI_CTR_MENUITEM) == 0) return static_cast<CMenuItemUI*>(this);
		return CListContainerElementUI::GetInterface(pstrName);
	}

	void CMenuItemUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcscmp(pstrName, _T("id")) == 0) {
			LPTSTR pstr = NULL;
			m_id = _tcstol(pstrValue, &pstr, 10);
		}
		return CListContainerElementUI::SetAttribute(pstrName, pstrValue);
	}

	void CMenuItemUI::SetId(LPCTSTR pstrId)
	{
		LPTSTR pstr = NULL;
		m_id = _tcstol(pstrId, &pstr, 10);
	}

	int CMenuItemUI::GetId() const
	{
		return m_id;
	}

	void CMenuItemUI::DoEvent(TEventUI& event)
	{
		if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND) {
			if (m_pOwner != NULL) m_pOwner->DoEvent(event);
			else CContainerUI::DoEvent(event);
			return;
		}
		if (event.Type == UIEVENT_BUTTONUP)
		{
			if (IsEnabled()) {
				if (m_pOwner != NULL) m_pOwner->DoEvent(event);
				CMenuUI* pOwner = static_cast<CMenuUI*>(m_pOwner);
				if (NULL == pOwner) return;
				// the notify is on the control which has menu
				// and the param is the id of the selected menu
				::PostMessage(GetManager()->GetPaintWindow(), WM_CLOSE, NULL, GetId());
			}
			return;
		}
		if (event.Type == UIEVENT_RBUTTONUP)
		{
			if (IsEnabled()) {
				if (m_pOwner != NULL) m_pOwner->DoEvent(event);
				CMenuUI* pOwner = static_cast<CMenuUI*>(m_pOwner);
				if (NULL == pOwner) return;
				// the notify is on the control which has menu
				// and the param is the id of the selected menu
				::PostMessage(GetManager()->GetPaintWindow(), WM_CLOSE, NULL, GetId());
			}
			return;
		}
		return CListContainerElementUI::DoEvent(event);
	}

	IMPLEMENT_DUICONTROL_UINIT_DATATEMPLATE(CMenuUI)

	CMenuUI::CMenuUI()
		:m_pOwner(NULL)
		, m_bMenuPaddingEnabled(false)
		, m_bTriangleEnabled(false)
	{

	}

	CMenuUI::~CMenuUI(void)
	{
	}

	CMenuUI* CMenuUI::DoInitDataTemplate(CMenuUI* pInstance)
	{
		pInstance->m_pOwner = NULL;
		pInstance->m_bMenuPaddingEnabled = this->m_bMenuPaddingEnabled;
		pInstance->m_rcMenuPadding = this->m_rcMenuPadding;
		pInstance->m_trianglePadding = this->m_trianglePadding;
		pInstance->m_bTriangleEnabled = this->m_bTriangleEnabled;
		return pInstance;
	}

	LPCTSTR CMenuUI::GetClass() const
	{
		return _T("MenuUI");
	}

	LPVOID CMenuUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcscmp(pstrName, DUI_CTR_MENU) == 0) return static_cast<CMenuUI*>(this);
		return CListUI::GetInterface(pstrName);
	}

	void CMenuUI::SetOwner(CControlUI* pOwner)
	{
		m_pOwner = pOwner;
	}

	CControlUI* CMenuUI::GetOwner() const
	{
		return m_pOwner;
	}

	void CMenuUI::SetMenuPaddingEnabled(bool bMenuPaddingEnabled)
	{
		m_bMenuPaddingEnabled = bMenuPaddingEnabled;
	}

	bool CMenuUI::GetMenuPaddingEnabled() const
	{
		return m_bMenuPaddingEnabled;
	}

	void CMenuUI::SetMenuPadding(RECT rcPadding)
	{
		m_rcMenuPadding = rcPadding;
	}

	RECT CMenuUI::GetMenuPadding() const
	{
		return m_rcMenuPadding;
	}

	void CMenuUI::SetTrianglePadding(const RECT rcPadding)
	{
		m_trianglePadding = rcPadding;
	}

	RECT CMenuUI::GetTrianglePadding() const
	{
		return m_trianglePadding;
	}

	void CMenuUI::SetTriangleEnabled(bool bTriangleUsed)
	{
		m_bTriangleEnabled = bTriangleUsed;
	}

	bool CMenuUI::GetTriangleEnabled() const
	{
		return (m_bTriangleEnabled&&m_bMenuPaddingEnabled);
	}

	void CMenuUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcscmp(pstrName, _T("menupadding")) == 0) {
			RECT rcPadding = { 0 };
			LPTSTR pstr = NULL;
			rcPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
			rcPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
			rcPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
			rcPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
			SetMenuPadding(rcPadding);
			SetMenuPaddingEnabled(true);
		}
		else if (_tcscmp(pstrName, _T("trianglepadding")) == 0) {
			RECT rcPadding = { 0 };
			LPTSTR pstr = NULL;
			rcPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
			rcPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
			rcPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
			rcPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
			SetTrianglePadding(rcPadding);
			SetTriangleEnabled(true);
		}
		return CListUI::SetAttribute(pstrName, pstrValue);
	}

	CMenuItemUI* CMenuUI::GetMenuItemById(int iMenuId)
	{
		CMenuItemUI* pMenuItem = NULL;
		CControlUI* pControl = NULL;
		for (int i = 0; i < GetCount(); ++i)
		{
			pControl = GetItemAt(i);
			if (NULL == pControl) return NULL;
			pMenuItem = static_cast<CMenuItemUI*>(pControl->GetInterface(DUI_CTR_MENUITEM));
			if (NULL == pMenuItem) continue;
			else {
				if (pMenuItem->GetId() != iMenuId) continue;
				return pMenuItem;
			}
		}
		return NULL;
	}

	IMPLEMENT_DUICONTROL_UINIT_DATATEMPLATE(CContextMenuUI)

	CContextMenuUI::CContextMenuUI()
		:m_pWindow(NULL)
		, m_pCallback(NULL)
	{
	}

	CContextMenuUI::~CContextMenuUI(void)
	{
	}

	CContextMenuUI* CContextMenuUI::DoInitDataTemplate(CContextMenuUI* pInstance)
	{
		pInstance->m_sXmlFile = this->m_sXmlFile;
		pInstance->m_pWindow = NULL;
		pInstance->m_pCallback = this->m_pCallback;
		return pInstance;
	}

	void CContextMenuUI::Init()
	{
		if (m_sXmlFile.IsEmpty()) return;
		if (NULL != m_pWindow) delete m_pWindow;
		m_pWindow = new CContextMenuWnd(this, m_sXmlFile, m_pCallback);
	}

	LPCTSTR CContextMenuUI::GetClass() const
	{
		return _T("ContextMenuUI");
	}

	LPVOID CContextMenuUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcscmp(pstrName, DUI_CTR_CONTEXTMENU) == 0) return static_cast<CContextMenuUI*>(this);
		return CControlUI::GetInterface(pstrName);
	}

	void CContextMenuUI::SetXmlFile(LPCTSTR pStrXmlFile)
	{
		m_sXmlFile = pStrXmlFile;
	}

	LPCTSTR CContextMenuUI::GetXmlFile()
	{
		return m_sXmlFile;
	}

	void CContextMenuUI::SetDialogBuilder(IDialogBuilderCallback* pCallback)
	{
		m_pCallback = pCallback;
	}

	IDialogBuilderCallback* CContextMenuUI::GetDialogBuilder() const
	{
		return m_pCallback;
	}
} // namespace DuiLib
