#include "stdafx.h"

namespace DuiLib
{
	class CRenameEditWnd : public CWindowWnd
	{
	public:
		CRenameEditWnd();

		void Init(CRenameEditUI* pOwner);
		RECT CalPos();

		LPCTSTR GetWindowClassName() const;
		LPCTSTR GetSuperClassName() const;
		void OnFinalMessage(HWND hWnd);

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	protected:
		CRenameEditUI* m_pOwner;
		HBRUSH m_hBkBrush;
		bool m_bInit;
		bool m_bDrawCaret;
	};

	CRenameEditWnd::CRenameEditWnd() : m_pOwner(NULL), m_hBkBrush(NULL), m_bInit(false), m_bDrawCaret(false)
	{
	}

	void CRenameEditWnd::Init(CRenameEditUI* pOwner)
	{
		m_pOwner = pOwner;
		RECT rcPos = CalPos();
		UINT uStyle = 0;
		if (m_pOwner->GetManager()->IsLayered()) {
			uStyle = WS_POPUP | ES_AUTOHSCROLL | WS_VISIBLE;
			RECT rcWnd = { 0 };
			::GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWnd);
			rcPos.left += rcWnd.left;
			rcPos.right += rcWnd.left;
			rcPos.top += rcWnd.top - 1;
			rcPos.bottom += rcWnd.top - 1;
		}
		else {
			uStyle = WS_CHILD | ES_AUTOHSCROLL;
		}
		UINT uTextStyle = m_pOwner->GetTextStyle();
		if (uTextStyle & DT_LEFT) uStyle |= ES_LEFT;
		else if (uTextStyle & DT_CENTER) uStyle |= ES_CENTER;
		else if (uTextStyle & DT_RIGHT) uStyle |= ES_RIGHT;
		if (m_pOwner->IsPasswordMode()) uStyle |= ES_PASSWORD;
		Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, uStyle, 0, rcPos);
		HFONT hFont = NULL;
		int iFontIndex = m_pOwner->GetFont();
		if (iFontIndex != -1)
			hFont = m_pOwner->GetManager()->GetFont(iFontIndex);
		if (hFont == NULL)
			hFont = m_pOwner->GetManager()->GetDefaultFontInfo()->hFont;

		SetWindowFont(m_hWnd, hFont, TRUE);
		Edit_LimitText(m_hWnd, m_pOwner->GetMaxChar());
		if (m_pOwner->IsPasswordMode()) Edit_SetPasswordChar(m_hWnd, m_pOwner->GetPasswordChar());
		Edit_SetText(m_hWnd, m_pOwner->GetText());
		Edit_SetModify(m_hWnd, FALSE);
		SendMessage(EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(1, 10));
		Edit_Enable(m_hWnd, m_pOwner->IsEnabled() == true);
		Edit_SetReadOnly(m_hWnd, m_pOwner->IsReadOnly() == true);

		//Styles
		LONG styleValue = ::GetWindowLong(m_hWnd, GWL_STYLE);
		styleValue |= pOwner->GetWindowStyls();
		::SetWindowLong(GetHWND(), GWL_STYLE, styleValue);
		::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);

		int cchLen = ::GetWindowTextLength(m_hWnd);
		if (cchLen <= 0) cchLen = 1;
		::SetFocus(m_hWnd);
		::SendMessage(m_hWnd, EM_SETSEL, 0, cchLen);
		m_bInit = true;
	}

	RECT CRenameEditWnd::CalPos()
	{
		CDuiRect rcPos = m_pOwner->GetPos();
		RECT rcInset = m_pOwner->GetTextPadding();
		// calculate width
		CDuiString sText = m_pOwner->GetText();
		RECT rcText = { 0, 0, rcPos.GetWidth(), rcPos.GetHeight() };
		int nLinks = 0;
		if (m_pOwner->IsShowHtml()) CRenderEngine::DrawHtmlText(m_pOwner->m_pManager->GetPaintDC(), 
			m_pOwner->m_pManager, rcText, sText, m_pOwner->GetTextColor(), 
			NULL, NULL, nLinks, DT_CALCRECT | m_pOwner->GetTextStyle());
		else CRenderEngine::DrawText(m_pOwner->m_pManager->GetPaintDC(), 
			m_pOwner->m_pManager, rcText, sText, m_pOwner->GetTextColor(), 
			m_pOwner->GetFont(), DT_CALCRECT | m_pOwner->GetTextStyle());
		rcPos.right = rcPos.left + MulDiv(rcText.right - rcText.left + 
			m_pOwner->GetManager()->GetDPIObj()->Scale(rcInset.left) + 
			m_pOwner->GetManager()->GetDPIObj()->Scale(rcInset.right), 100, 
			m_pOwner->GetManager()->GetDPIObj()->GetScale());
		
		rcPos.left += m_pOwner->GetManager()->GetDPIObj()->Scale(rcInset.left);
		rcPos.top += rcInset.top;
		rcPos.right -= m_pOwner->GetManager()->GetDPIObj()->Scale(rcInset.right);
		rcPos.bottom -= rcInset.bottom;
		LONG lEditHeight = m_pOwner->GetManager()->GetFontInfo(m_pOwner->GetFont())->tm.tmHeight;
		if (lEditHeight < rcPos.GetHeight()) {
			rcPos.top += (rcPos.GetHeight() - lEditHeight) / 2;
			rcPos.bottom = rcPos.top + lEditHeight;
		}

		CControlUI* pParent = m_pOwner;
		RECT rcParent;
		while (pParent = pParent->GetParent()) {
			if (!pParent->IsVisible()) {
				rcPos.left = rcPos.top = rcPos.right = rcPos.bottom = 0;
				break;
			}
			rcParent = pParent->GetClientPos();
			if (!::IntersectRect(&rcPos, &rcPos, &rcParent)) {
				rcPos.left = rcPos.top = rcPos.right = rcPos.bottom = 0;
				break;
			}
		}

		return rcPos;
	}

	LPCTSTR CRenameEditWnd::GetWindowClassName() const
	{
		return _T("RenameEditWnd");
	}

	LPCTSTR CRenameEditWnd::GetSuperClassName() const
	{
		return WC_EDIT;
	}

	void CRenameEditWnd::OnFinalMessage(HWND hWnd)
	{
		m_pOwner->Invalidate();
		// Clear reference and die
		if (m_hBkBrush != NULL) ::DeleteObject(m_hBkBrush);
		if (m_pOwner->GetManager()->IsLayered()) {
			m_pOwner->GetManager()->RemovePaintChildWnd(hWnd);
		}
		m_pOwner->SetRenameState(UIRENAME_UNINITIAL);
		m_pOwner->m_pWindow = NULL;
		delete this;
	}

	LRESULT CRenameEditWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		if (uMsg == WM_CREATE) {
			if (m_pOwner->GetManager()->IsLayered()) {
				m_pOwner->GetManager()->AddPaintChildWnd(m_hWnd);
				::SetTimer(m_hWnd, CARET_TIMERID, ::GetCaretBlinkTime(), NULL);
			}
			bHandled = FALSE;
		}
		else if (uMsg == WM_KILLFOCUS) lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
		else if (uMsg == OCM_COMMAND) {
			if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE) lRes = OnEditChanged(uMsg, wParam, lParam, bHandled);
			else if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_UPDATE) {
				RECT rcClient;
				::GetClientRect(m_hWnd, &rcClient);
				::InvalidateRect(m_hWnd, &rcClient, FALSE);
			}
		}
		else if (uMsg == WM_KEYDOWN && TCHAR(wParam) == VK_RETURN) {
			m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_RETURN);
			SendMessage(WM_KILLFOCUS);
		}
		else if (uMsg == WM_KEYDOWN && TCHAR(wParam) == VK_TAB) {
			if (m_pOwner->GetManager()->IsLayered()) {
				m_pOwner->GetManager()->SetNextTabControl();
			}
		}
		else if (uMsg == OCM__BASE + WM_CTLCOLOREDIT || uMsg == OCM__BASE + WM_CTLCOLORSTATIC) {
			if (m_pOwner->GetNativeEditBkColor() == 0xFFFFFFFF) return NULL;
			::SetBkMode((HDC)wParam, TRANSPARENT);

			DWORD dwTextColor;
			if (m_pOwner->GetNativeEditTextColor() != 0x000000)
				dwTextColor = m_pOwner->GetNativeEditTextColor();
			else
				dwTextColor = m_pOwner->GetTextColor();

			::SetTextColor((HDC)wParam, RGB(GetBValue(dwTextColor), GetGValue(dwTextColor), GetRValue(dwTextColor)));
			if (m_hBkBrush == NULL) {
				DWORD clrColor = m_pOwner->GetNativeEditBkColor();
				m_hBkBrush = ::CreateSolidBrush(RGB(GetBValue(clrColor), GetGValue(clrColor), GetRValue(clrColor)));
			}
			return (LRESULT)m_hBkBrush;
		}
		else if (uMsg == WM_PRINT) {
			if (m_pOwner->GetManager()->IsLayered()) {
				lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
				if (m_pOwner->IsEnabled() && m_bDrawCaret) {
					RECT rcClient;
					::GetClientRect(m_hWnd, &rcClient);
					POINT ptCaret;
					::GetCaretPos(&ptCaret);
					RECT rcCaret = { ptCaret.x, ptCaret.y, ptCaret.x, ptCaret.y + rcClient.bottom - rcClient.top };
					CRenderEngine::DrawLine((HDC)wParam, rcCaret, 1, 0xFF000000);
				}
				return lRes;
			}
			bHandled = FALSE;
		}
		else if (uMsg == WM_TIMER) {
			if (wParam == CARET_TIMERID) {
				m_bDrawCaret = !m_bDrawCaret;
				RECT rcClient;
				::GetClientRect(m_hWnd, &rcClient);
				::InvalidateRect(m_hWnd, &rcClient, FALSE);
				return 0;
			}
			bHandled = FALSE;
		}
		else bHandled = FALSE;

		if (!bHandled) return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		return lRes;
	}

	LRESULT CRenameEditWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (m_bInit && m_pOwner != NULL) {
			// Copy text back
			int cchLen = ::GetWindowTextLength(m_hWnd) + 1;
			LPTSTR pstr = static_cast<LPTSTR>(_alloca(cchLen * sizeof(TCHAR)));
			ASSERT(pstr);
			if (pstr == NULL) return 0;
			::GetWindowText(m_hWnd, pstr, cchLen);
			m_pOwner->m_sText = pstr;
			if (m_pOwner->GetManager()->IsLayered()) m_pOwner->Invalidate();
		}

		LRESULT lRes = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
		PostMessage(WM_CLOSE);
		return lRes;
	}

	LRESULT CRenameEditWnd::OnEditChanged(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		if (!m_bInit) return 0;
		if (m_pOwner == NULL) return 0;
		//// Copy text back
		//int cchLen = ::GetWindowTextLength(m_hWnd) + 1;
		//LPTSTR pstr = static_cast<LPTSTR>(_alloca(cchLen * sizeof(TCHAR)));
		//ASSERT(pstr);
		//if (pstr == NULL) return 0;
		//::GetWindowText(m_hWnd, pstr, cchLen);
		//m_pOwner->m_sText = pstr;
		m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_TEXTCHANGED, wParam, lParam);
		if (m_pOwner->GetManager()->IsLayered()) m_pOwner->Invalidate();
		return 0;
	}

	IMPLEMENT_DUICONTROL(CRenameEditUI)

	CRenameEditUI::CRenameEditUI()
		:m_iRenameState(UIRENAME_UNINITIAL)
	{
	}

	CRenameEditUI::~CRenameEditUI()
	{
	}

	LPCTSTR CRenameEditUI::GetClass() const
	{
		return _T("RenameEditUI");
	}

	LPVOID CRenameEditUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcsicmp(pstrName, DUI_CTR_RENAMEEDIT) == 0) return static_cast<CRenameEditUI*>(this);
		return CEditUI::GetInterface(pstrName);
	}

	void CRenameEditUI::DoInit()
	{
		LPVOID pControl = NULL;
		CControlUI* pParent = GetParent();
		while (pParent = pParent->GetParent()) {
			pControl = pParent->GetInterface(DUI_CTR_LISTITEM);
			if (NULL != pControl) {
				static_cast<IListItemUI*>(pControl)->SetListItemCallback(this);
				break;
			}
		}
	}

	void CRenameEditUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		CControlUI::SetPos(rc, bNeedInvalidate);
		if (m_pWindow != NULL) {
			RECT rcPos = static_cast<CRenameEditWnd*>(m_pWindow)->CalPos();
			::SetWindowPos(m_pWindow->GetHWND(), NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left,
				rcPos.bottom - rcPos.top, SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}

	void CRenameEditUI::Move(SIZE szOffset, bool bNeedInvalidate)
	{
		CControlUI::Move(szOffset, bNeedInvalidate);
		if (m_pWindow != NULL) {
			RECT rcPos = static_cast<CRenameEditWnd*>(m_pWindow)->CalPos();
			::SetWindowPos(m_pWindow->GetHWND(), NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left,
				rcPos.bottom - rcPos.top, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}

	void CRenameEditUI::SetRenameState(RENAMESTATE_UI state)
	{
		m_iRenameState = state;
	}

	RENAMESTATE_UI CRenameEditUI::GetRenameState() const
	{
		return m_iRenameState;
	}

	bool CRenameEditUI::Select(CControlUI* pItem, bool bSelect)
	{
		if (!IsEnabled()) {
			m_iRenameState = UIRENAME_UNINITIAL;
			return bSelect;
		}
		if (NULL == pItem) {
			m_iRenameState = UIRENAME_UNINITIAL;
			return false;
		}
		if (m_iRenameState == UIRENAME_CLICK && !bSelect) {
			m_iRenameState = UIRENAME_UNINITIAL;
		}
		return bSelect;
	}

	void CRenameEditUI::DoEvent(TEventUI& event)
	{
		if (!IsEnabled()) {
			return CLabelUI::DoEvent(event);
		}
		if (event.Type == UIEVENT_SETCURSOR) {
			if (m_iRenameState != UIRENAME_ACTIVATE) {
				::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
			}
			return;
		}
		if (event.Type == UIEVENT_DBLCLICK) {
			m_iRenameState = UIRENAME_UNINITIAL;
			return CLabelUI::DoEvent(event);
		}
		if (event.Type == UIEVENT_SETFOCUS || event.Type == UIEVENT_MOUSEENTER || event.Type == UIEVENT_MOUSELEAVE) {
			return CLabelUI::DoEvent(event);
		}
		if (event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_RBUTTONDOWN) {
			if (m_iRenameState == UIRENAME_UNINITIAL) {
				m_iRenameState = UIRENAME_CLICK;
			}
			else if (m_iRenameState == UIRENAME_CLICK) {
				GetManager()->ReleaseCapture();
				if (IsFocused() && m_pWindow == NULL) {
					m_pWindow = new CRenameEditWnd();
					ASSERT(m_pWindow);
					static_cast<CRenameEditWnd*>(m_pWindow)->Init(this);

					if (PtInRect(&m_rcItem, event.ptMouse)) {
						int nSize = GetWindowTextLength(*m_pWindow);
						if (nSize == 0) nSize = 1;
						Edit_SetSel(*m_pWindow, 0, nSize);
					}
				}
				else if (m_pWindow != NULL) {
#if 1
					int nSize = GetWindowTextLength(*m_pWindow);
					if (nSize == 0) nSize = 1;
					Edit_SetSel(*m_pWindow, 0, nSize);
#else
					POINT pt = event.ptMouse;
					pt.x -= m_rcItem.left + m_rcTextPadding.left;
					pt.y -= m_rcItem.top + m_rcTextPadding.top;
					::SendMessage(*m_pWindow, WM_LBUTTONDOWN, event.wParam, MAKELPARAM(pt.x, pt.y));
#endif
				}
				m_iRenameState = UIRENAME_ACTIVATE;
				return;
			}
			return CLabelUI::DoEvent(event);
		}
		return CEditUI::DoEvent(event);
	}
}
