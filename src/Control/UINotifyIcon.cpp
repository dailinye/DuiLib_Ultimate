#include "StdAfx.h"

namespace DuiLib
{
	static void copy_notifyicon_str(wchar_t* dest, size_t cnt1, const wchar_t* source, size_t cnt2)
	{
		if (cnt1 <= cnt2) 
		{
			if (cnt1 > 3)
			{
				dest[cnt1-1] = 0;
				dest[cnt1-2] = L'.';
				dest[cnt1-3] = L'.';
				dest[cnt1-4] = L'.';
				memcpy_s(dest, cnt1*sizeof(wchar_t), source, (cnt1-4)*sizeof(wchar_t));
			} 
		}
		else 
		{
			wcsncpy_s(dest, cnt1, source, cnt2);
		}
	}

	IMPLEMENT_DUICONTROL_UINIT_DATATEMPLATE(CNotifyIconUI)

	CNotifyIconUI::CNotifyIconUI(void)
		:m_hIcon(NULL)
		,m_id(0)
	{
		
	}

	CNotifyIconUI::~CNotifyIconUI(void)
	{
		if (NULL != m_hIcon) {
			NOTIFYICONDATA nID = {0};
			nID.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
			nID.hWnd = GetManager()->GetPaintWindow();
			nID.uID = m_id;
			(void)::Shell_NotifyIcon(NIM_DELETE, &nID);
			::DestroyIcon(m_hIcon);
		}
	}

	CNotifyIconUI* CNotifyIconUI::DoInitDataTemplate(CNotifyIconUI* pInstance)
	{
		pInstance->m_sNotifyIcon = this->m_sNotifyIcon;
		pInstance->m_id = this->m_id;
		pInstance->m_hIcon = NULL;
		return pInstance;
	}

	LPCTSTR CNotifyIconUI::GetClass() const
	{
		return _T("NotifyIconUI");
	}

	LPVOID CNotifyIconUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_NOTIFYICON) == 0 ) return static_cast<CNotifyIconUI*>(this);
		return CControlUI::GetInterface(pstrName);
	}

	int CNotifyIconUI::GetId() const
	{
		return m_id;
	}

	void CNotifyIconUI::SetNotifyIcon(LPCTSTR pstrIcon)
	{
#ifdef _UNICODE
		m_sNotifyIcon = pstrIcon;
		if (NULL != m_hIcon) ::DestroyIcon(m_hIcon);
		m_hIcon = (HICON)::LoadImage(NULL, m_sNotifyIcon, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
		if (NULL == m_hIcon) return;
		NOTIFYICONDATA nID = { 0 };
		nID.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
		nID.hIcon = m_hIcon;
		copy_notifyicon_str(nID.szTip, sizeof(nID.szTip) / sizeof(WCHAR), m_sToolTip, m_sToolTip.GetLength());
		nID.hWnd = GetManager()->GetPaintWindow();
		nID.uID = m_id;
		nID.uFlags = NIF_GUID | NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
		nID.uCallbackMessage = WM_NOTIFYICON;
		(void)::Shell_NotifyIcon(NIM_MODIFY, &nID);
#endif
	}

	LPCTSTR CNotifyIconUI::GetNotifyIcon() const
	{
		return m_sNotifyIcon;
	}

	void CNotifyIconUI::Init()
	{
#ifdef _UNICODE
		if (NULL != m_hIcon) ::DestroyIcon(m_hIcon);
		m_hIcon = (HICON)::LoadImage(NULL, m_sNotifyIcon, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
		if (NULL == m_hIcon) return;
		NOTIFYICONDATA nID = {0};
		nID.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
		nID.hIcon = m_hIcon;
		copy_notifyicon_str(nID.szTip, sizeof(nID.szTip)/sizeof(WCHAR), m_sToolTip, m_sToolTip.GetLength());
		nID.hWnd = GetManager()->GetPaintWindow();
		nID.uID = m_id;
		nID.uFlags = NIF_GUID|NIF_ICON|NIF_MESSAGE|NIF_TIP;
		nID.uCallbackMessage = WM_NOTIFYICON;
		(void)::Shell_NotifyIcon(NIM_ADD, &nID);
#endif
	}

	void CNotifyIconUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		LPTSTR pstr = NULL;
		if( _tcscmp(pstrName, _T("notifyicon")) == 0 ) 
		{
			m_sNotifyIcon = pstrValue;
			return;
		}
		if( _tcscmp(pstrName, _T("id")) == 0 ) {
			m_id = _tcstol(pstrValue, &pstr, 10);
			return;
		}
		return CControlUI::SetAttribute(pstrName, pstrValue);
	}

	void CNotifyIconUI::DoEvent(TEventUI& event)
	{
		if ( UIEVENT_NOTIFYICON != event.Type ) return;
		if ( event.lParam == WM_RBUTTONUP && IsContextMenuUsed() ) event.Type = UIEVENT_CONTEXTMENU;
		switch (event.lParam)
		{
		case WM_RBUTTONUP:
			GetManager()->SendNotify(this, DUI_MSGTYPE_NOTIFYICON_EVENT, event.wParam, NOTIFYICON_RCLICK);
			break;
		case WM_LBUTTONUP:
			GetManager()->SendNotify(this, DUI_MSGTYPE_NOTIFYICON_EVENT, event.wParam, NOTIFYICON_CLICK);
			break;
		case WM_LBUTTONDBLCLK:
			GetManager()->SendNotify(this, DUI_MSGTYPE_NOTIFYICON_EVENT, event.wParam, NOTIFYICON_DBCLICK);
			break;
		case WM_RBUTTONDBLCLK:
			GetManager()->SendNotify(this, DUI_MSGTYPE_NOTIFYICON_EVENT, event.wParam, NOTIFYICON_RDBCLICK);
			break;
		default:
			break;
		}
		return CControlUI::DoEvent(event);
	}

	void CNotifyIconUI::ShowBalloon(LPCTSTR pstrContent)
	{
#ifdef _UNICODE
#endif
	}

	bool CNotifyIconUI::ReInit()
	{
#ifdef _UNICODE
		if (NULL != m_hIcon) ::DestroyIcon(m_hIcon);
		m_hIcon = (HICON)::LoadImage(NULL, m_sNotifyIcon, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
		if (NULL == m_hIcon) return false;
		NOTIFYICONDATA nID = {0};
		nID.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
		nID.hIcon = m_hIcon;
		copy_notifyicon_str(nID.szTip, sizeof(nID.szTip)/sizeof(WCHAR), m_sToolTip, m_sToolTip.GetLength());
		nID.hWnd = GetManager()->GetPaintWindow();
		nID.uID = m_id;
		nID.uFlags = NIF_GUID|NIF_ICON|NIF_MESSAGE|NIF_TIP;
		nID.uCallbackMessage = WM_NOTIFYICON;
		(void)::Shell_NotifyIcon(NIM_ADD, &nID);
		return true;
#endif
	}
}