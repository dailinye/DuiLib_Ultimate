#include "StdAfx.h"
#include "UIButton.h"

namespace DuiLib
{
	IMPLEMENT_DUICONTROL_UINIT_DATATEMPLATE(CButtonUI)

	CButtonUI::CButtonUI()
		: m_uButtonState(0)
		, m_dwHotTextColor(0)
		, m_dwPushedTextColor(0)
		, m_dwFocusedTextColor(0)
		, m_dwHotBkColor(0)
		, m_dwPushedBkColor(0)
		, m_dwHotBorderColor(0)
		, m_iBindTabIndex(-1)
		, m_nStateCount(0)
	{
		m_uTextStyle = DT_SINGLELINE | DT_VCENTER | DT_CENTER;
	}

	CButtonUI* CButtonUI::DoInitDataTemplate(CButtonUI* pInstance)
	{
		pInstance->m_uButtonState = this->m_uButtonState;
		pInstance->m_dwHotBkColor = this->m_dwHotBkColor;
		pInstance->m_dwPushedBkColor = this->m_dwPushedBkColor;
		pInstance->m_dwHotBorderColor = this->m_dwHotBorderColor;
		pInstance->m_dwHotTextColor = this->m_dwHotTextColor;
		pInstance->m_dwPushedTextColor = this->m_dwPushedTextColor;
		pInstance->m_dwFocusedTextColor = this->m_dwFocusedTextColor;
		pInstance->m_sNormalImage = this->m_sNormalImage;
		pInstance->m_sHotImage = this->m_sHotImage;
		pInstance->m_sHotForeImage = this->m_sHotForeImage;
		pInstance->m_sPushedImage = this->m_sPushedImage;
		pInstance->m_sPushedForeImage = this->m_sPushedForeImage;
		pInstance->m_sFocusedImage = this->m_sFocusedImage;
		pInstance->m_sDisabledImage = this->m_sDisabledImage;
		pInstance->m_nStateCount = this->m_nStateCount;
		pInstance->m_sStateImage = this->m_sStateImage;
		pInstance->m_iBindTabIndex = this->m_iBindTabIndex;
		pInstance->m_sBindTabLayoutName = this->m_sBindTabLayoutName;
		return pInstance;
	}

	LPCTSTR CButtonUI::GetClass() const
	{
		return _T("ButtonUI");
	}

	LPVOID CButtonUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_BUTTON) == 0 ) return static_cast<CButtonUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}

	UINT CButtonUI::GetControlFlags() const
	{
		return (IsKeyboardEnabled() ? UIFLAG_TABSTOP : 0) | (IsEnabled() ? UIFLAG_SETCURSOR : 0);
	}

	void CButtonUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else CLabelUI::DoEvent(event);
			return;
		}

		if( event.Type == UIEVENT_SETFOCUS ) 
		{
			Invalidate();
		}
		if( event.Type == UIEVENT_KILLFOCUS ) 
		{
			Invalidate();
		}
		if( event.Type == UIEVENT_KEYDOWN )
		{
			if (IsKeyboardEnabled()) {
				if( event.chKey == VK_SPACE || event.chKey == VK_RETURN ) {
					Activate();
					return;
				}
			}
		}		
		if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK)
		{
			if( ::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled() ) {
				m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
				Invalidate();
			}
			return;
		}	
		if( event.Type == UIEVENT_MOUSEMOVE )
		{
			if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
				if( ::PtInRect(&m_rcItem, event.ptMouse) ) m_uButtonState |= UISTATE_PUSHED;
				else m_uButtonState &= ~UISTATE_PUSHED;
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_BUTTONUP )
		{
			if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
				m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
				Invalidate();
				if( ::PtInRect(&m_rcItem, event.ptMouse) ) Activate();				
			}
			return;
		}
		//if( event.Type == UIEVENT_CONTEXTMENU )
		//{
		//	if( IsContextMenuUsed() ) {
		//		m_pManager->SendNotify(this, DUI_MSGTYPE_MENU, event.wParam, event.lParam);
		//	}
		//	return;
		//}
		if( event.Type == UIEVENT_MOUSEENTER )
		{
			if( IsEnabled() ) {
				m_uButtonState |= UISTATE_HOT;
				Invalidate();
			}
		}
		if( event.Type == UIEVENT_MOUSELEAVE )
		{
			if( IsEnabled() ) {
				m_uButtonState &= ~UISTATE_HOT;
				Invalidate();
			}
		}
		CLabelUI::DoEvent(event);
	}

	bool CButtonUI::Activate()
	{
		if( !CControlUI::Activate() ) return false;
		if( m_pManager != NULL )
		{
			m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
			BindTriggerTabSel();
		}
		return true;
	}

	void CButtonUI::SetEnabled(bool bEnable)
	{
		CControlUI::SetEnabled(bEnable);
		if( !IsEnabled() ) {
			m_uButtonState = 0;
		}
	}

	void CButtonUI::SetHotBkColor( DWORD dwColor )
	{
		m_dwHotBkColor = dwColor;
	}

	DWORD CButtonUI::GetHotBkColor() const
	{
		return m_dwHotBkColor;
	}
	
	void CButtonUI::SetPushedBkColor( DWORD dwColor )
	{
		m_dwPushedBkColor = dwColor;
	}

	DWORD CButtonUI::GetPushedBkColor() const
	{
		return m_dwPushedBkColor;
	}

	void CButtonUI::SetHotBorderColor(DWORD dwColor)
	{
		m_dwHotBorderColor = dwColor;
	}

	DWORD CButtonUI::GetHotBorderColor() const
	{
		return m_dwHotBorderColor;
	}

	void CButtonUI::SetHotTextColor(DWORD dwColor)
	{
		m_dwHotTextColor = dwColor;
	}

	DWORD CButtonUI::GetHotTextColor() const
	{
		return m_dwHotTextColor;
	}

	void CButtonUI::SetPushedTextColor(DWORD dwColor)
	{
		m_dwPushedTextColor = dwColor;
	}

	DWORD CButtonUI::GetPushedTextColor() const
	{
		return m_dwPushedTextColor;
	}

	void CButtonUI::SetFocusedTextColor(DWORD dwColor)
	{
		m_dwFocusedTextColor = dwColor;
	}

	DWORD CButtonUI::GetFocusedTextColor() const
	{
		return m_dwFocusedTextColor;
	}

	LPCTSTR CButtonUI::GetNormalImage()
	{
		return m_sNormalImage;
	}

	void CButtonUI::SetNormalImage(LPCTSTR pStrImage)
	{
		m_sNormalImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CButtonUI::GetHotImage()
	{
		return m_sHotImage;
	}

	void CButtonUI::SetHotImage(LPCTSTR pStrImage)
	{
		m_sHotImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CButtonUI::GetPushedImage()
	{
		return m_sPushedImage;
	}

	void CButtonUI::SetPushedImage(LPCTSTR pStrImage)
	{
		m_sPushedImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CButtonUI::GetFocusedImage()
	{
		return m_sFocusedImage;
	}

	void CButtonUI::SetFocusedImage(LPCTSTR pStrImage)
	{
		m_sFocusedImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CButtonUI::GetDisabledImage()
	{
		return m_sDisabledImage;
	}

	void CButtonUI::SetDisabledImage(LPCTSTR pStrImage)
	{
		m_sDisabledImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CButtonUI::GetHotForeImage()
	{
		return m_sHotForeImage;
	}

	void CButtonUI::SetHotForeImage( LPCTSTR pStrImage )
	{
		m_sHotForeImage = pStrImage;
		Invalidate();
	}

	void CButtonUI::SetStateCount(int nCount)
	{
		m_nStateCount = nCount;
		Invalidate();
	}

	int CButtonUI::GetStateCount() const
	{
		return m_nStateCount;
	}

	LPCTSTR CButtonUI::GetStateImage()
	{
		return m_sStateImage;
	}

	void CButtonUI::SetStateImage( LPCTSTR pStrImage )
	{
		m_sNormalImage.Empty();
		m_sStateImage = pStrImage;
		Invalidate();
	}

	void CButtonUI::BindTabIndex(int _BindTabIndex )
	{
		if( _BindTabIndex >= 0)
			m_iBindTabIndex	= _BindTabIndex;
	}

	void CButtonUI::BindTabLayoutName( LPCTSTR _TabLayoutName )
	{
		if(_TabLayoutName)
			m_sBindTabLayoutName = _TabLayoutName;
	}

	void CButtonUI::BindTriggerTabSel( int _SetSelectIndex /*= -1*/ )
	{
		LPCTSTR pstrName = GetBindTabLayoutName();
		if(pstrName == NULL || (GetBindTabLayoutIndex() < 0 && _SetSelectIndex < 0))
			return;

		CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(GetManager()->FindControl(pstrName));
		if(!pTabLayout) return;
		pTabLayout->SelectItem(_SetSelectIndex >=0?_SetSelectIndex:GetBindTabLayoutIndex());
	}

	void CButtonUI::RemoveBindTabIndex()
	{
		m_iBindTabIndex	= -1;
		m_sBindTabLayoutName.Empty();
	}

	int CButtonUI::GetBindTabLayoutIndex()
	{
		return m_iBindTabIndex;
	}

	LPCTSTR CButtonUI::GetBindTabLayoutName()
	{
		return m_sBindTabLayoutName;
	}

	void CButtonUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("normalimage")) == 0 ) SetNormalImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("hotimage")) == 0 ) SetHotImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("pushedimage")) == 0 ) SetPushedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("focusedimage")) == 0 ) SetFocusedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("disabledimage")) == 0 ) SetDisabledImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("hotforeimage")) == 0 ) SetHotForeImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("stateimage")) == 0 ) SetStateImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("statecount")) == 0 ) SetStateCount(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("bindtabindex")) == 0 ) BindTabIndex(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("bindtablayoutname")) == 0 ) BindTabLayoutName(pstrValue);
		else if( _tcsicmp(pstrName, _T("hotbkcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetHotBkColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("pushedbkcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetPushedBkColor(clrColor);
		}
		else if (_tcsicmp(pstrName, _T("hotbordercolor")) == 0)
		{
			if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetHotBorderColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("hottextcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetHotTextColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("pushedtextcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetPushedTextColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("focusedtextcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetFocusedTextColor(clrColor);
		}
		else CLabelUI::SetAttribute(pstrName, pstrValue);
	}

	void CButtonUI::PaintText(HDC hDC)
	{
		if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
		else m_uButtonState &= ~ UISTATE_FOCUSED;
		if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
		else m_uButtonState &= ~ UISTATE_DISABLED;

		if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
		if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();
		
		CDuiString sText = GetText();
		if( sText.IsEmpty() ) return;

		RECT m_rcTextPadding = CButtonUI::m_rcTextPadding;
		GetManager()->GetDPIObj()->Scale(&m_rcTextPadding);
		int nLinks = 0;
		RECT rc = m_rcItem;
		rc.left += m_rcTextPadding.left;
		rc.right -= m_rcTextPadding.right;
		rc.top += m_rcTextPadding.top;
		rc.bottom -= m_rcTextPadding.bottom;

		DWORD clrColor = IsEnabled()?m_dwTextColor:m_dwDisabledTextColor;

		if( ((m_uButtonState & UISTATE_PUSHED) != 0) && (GetPushedTextColor() != 0) )
			clrColor = GetPushedTextColor();
		else if( ((m_uButtonState & UISTATE_HOT) != 0) && (GetHotTextColor() != 0) )
			clrColor = GetHotTextColor();
		else if( ((m_uButtonState & UISTATE_FOCUSED) != 0) && (GetFocusedTextColor() != 0) )
			clrColor = GetFocusedTextColor();

		if( m_bShowHtml )
			CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, sText, clrColor, \
			NULL, NULL, nLinks, m_uTextStyle);
		else
			CRenderEngine::DrawText(hDC, m_pManager, rc, sText, clrColor, \
			m_iFont, m_uTextStyle);
	}

	void CButtonUI::PaintBkColor(HDC hDC)
	{
		if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
			if(m_dwPushedBkColor != 0) {
				CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwPushedBkColor));
				return;
			}
		}
		else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
			if(m_dwHotBkColor != 0) {
				CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwHotBkColor));
				return;
			}
		}

		return CControlUI::PaintBkColor(hDC);
	}

	void CButtonUI::PaintStatusImage(HDC hDC)
	{
		if(!m_sStateImage.IsEmpty() && m_nStateCount > 0)
		{
			TDrawInfo info;
			info.Parse(m_sStateImage, _T(""), m_pManager);
			const TImageInfo* pImage = m_pManager->GetImageEx(info.sImageName, info.sResType, info.dwMask, info.bHSL);
			if(m_sNormalImage.IsEmpty() && pImage != NULL)
			{
				SIZE szImage = {pImage->nX, pImage->nY};
				SIZE szStatus = {pImage->nX / m_nStateCount, pImage->nY};
				if( szImage.cx > 0 && szImage.cy > 0 )
				{
					RECT rcSrc = {0, 0, szImage.cx, szImage.cy};
					if(m_nStateCount > 0) {
						int iLeft = rcSrc.left + 0 * szStatus.cx;
						int iRight = iLeft + szStatus.cx;
						int iTop = rcSrc.top;
						int iBottom = iTop + szStatus.cy;
						m_sNormalImage.Format(_T("res='%s' restype='%s' dest='%d,%d,%d,%d' source='%d,%d,%d,%d'"), info.sImageName.GetData(), info.sResType.GetData(), info.rcDest.left, info.rcDest.top, info.rcDest.right, info.rcDest.bottom, iLeft, iTop, iRight, iBottom);
					}
					if(m_nStateCount > 1) {
						int iLeft = rcSrc.left + 1 * szStatus.cx;
						int iRight = iLeft + szStatus.cx;
						int iTop = rcSrc.top;
						int iBottom = iTop + szStatus.cy;
						m_sHotImage.Format(_T("res='%s' restype='%s' dest='%d,%d,%d,%d' source='%d,%d,%d,%d'"), info.sImageName.GetData(), info.sResType.GetData(), info.rcDest.left, info.rcDest.top, info.rcDest.right, info.rcDest.bottom, iLeft, iTop, iRight, iBottom);
					}
					if(m_nStateCount > 2) {
						int iLeft = rcSrc.left + 2 * szStatus.cx;
						int iRight = iLeft + szStatus.cx;
						int iTop = rcSrc.top;
						int iBottom = iTop + szStatus.cy;
						m_sPushedImage.Format(_T("res='%s' restype='%s' dest='%d,%d,%d,%d' source='%d,%d,%d,%d'"), info.sImageName.GetData(), info.sResType.GetData(), info.rcDest.left, info.rcDest.top, info.rcDest.right, info.rcDest.bottom, iLeft, iTop, iRight, iBottom);
					}
					if(m_nStateCount > 3) {
						int iLeft = rcSrc.left + 3 * szStatus.cx;
						int iRight = iLeft + szStatus.cx;
						int iTop = rcSrc.top;
						int iBottom = iTop + szStatus.cy;
						m_sDisabledImage.Format(_T("res='%s' restype='%s' dest='%d,%d,%d,%d' source='%d,%d,%d,%d'"), info.sImageName.GetData(), info.sResType.GetData(), info.rcDest.left, info.rcDest.top, info.rcDest.right, info.rcDest.bottom, iLeft, iTop, iRight, iBottom);
					}
				}
			}
		}

		if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
		else m_uButtonState &= ~ UISTATE_FOCUSED;
		if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
		else m_uButtonState &= ~ UISTATE_DISABLED;
		if(!::IsWindowEnabled(m_pManager->GetPaintWindow())) {
			m_uButtonState &= UISTATE_DISABLED;
		}
		if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
			if( !m_sDisabledImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sDisabledImage) ) {}
				else return;
			}
		}
		else if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
			if( !m_sPushedImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sPushedImage) ) {}
				else return;
			}
		}
		else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
			if( !m_sHotImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sHotImage) ) {}
				else return;
			}
		}
		else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
			if( !m_sFocusedImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sFocusedImage) ) {}
				else return;
			}
		}

		if( !m_sNormalImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sNormalImage) ) {}
		}
	}

	void CButtonUI::PaintForeImage(HDC hDC)
	{
		if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
			if( !m_sPushedForeImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sPushedForeImage) ) {}
				else return;
			}
		}
		else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
			if( !m_sHotForeImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sHotForeImage) ) {}
				else return;
			}
		}
		if(!m_sForeImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sForeImage) ) {}
		}
	}

	void CButtonUI::PaintBorder(HDC hDC)
	{
		if ((m_uButtonState & UISTATE_HOT) == 0 || m_dwHotBorderColor == 0) {
			return CLabelUI::PaintBorder(hDC);
		}
		int nBorderSize;
		SIZE cxyBorderRound;
		RECT rcBorderSize;
		if (m_pManager) {
			nBorderSize = GetManager()->GetDPIObj()->Scale(m_nBorderSize);
			cxyBorderRound = GetManager()->GetDPIObj()->Scale(m_cxyBorderRound);
			rcBorderSize = GetManager()->GetDPIObj()->Scale(m_rcBorderSize);
		}
		else {
			nBorderSize = m_nBorderSize;
			cxyBorderRound = m_cxyBorderRound;
			rcBorderSize = m_rcBorderSize;
		}
		if (cxyBorderRound.cx > 0 || cxyBorderRound.cy > 0) {
			CRenderEngine::DrawRoundRect(hDC, m_rcItem, nBorderSize, cxyBorderRound.cx, cxyBorderRound.cy, GetAdjustColor(m_dwHotBorderColor), m_nBorderStyle);
		}
		else if (nBorderSize > 0) {
			CRenderEngine::DrawRect(hDC, m_rcItem, nBorderSize, GetAdjustColor(m_dwHotBorderColor), m_nBorderStyle);
		}
		else {
			RECT rcBorder;
			if (rcBorderSize.left > 0) {
				rcBorder = m_rcItem;
				rcBorder.right = rcBorder.left;
				CRenderEngine::DrawLine(hDC, rcBorder, rcBorderSize.left, GetAdjustColor(m_dwHotBorderColor), m_nBorderStyle);
			}
			if (rcBorderSize.top > 0) {
				rcBorder = m_rcItem;
				rcBorder.bottom = rcBorder.top;
				CRenderEngine::DrawLine(hDC, rcBorder, rcBorderSize.top, GetAdjustColor(m_dwHotBorderColor), m_nBorderStyle);
			}
			if (rcBorderSize.right > 0) {
				rcBorder = m_rcItem;
				rcBorder.right -= 1;
				rcBorder.left = rcBorder.right;
				CRenderEngine::DrawLine(hDC, rcBorder, rcBorderSize.right, GetAdjustColor(m_dwHotBorderColor), m_nBorderStyle);
			}
			if (rcBorderSize.bottom > 0) {
				rcBorder = m_rcItem;
				rcBorder.bottom -= 1;
				rcBorder.top = rcBorder.bottom;
				CRenderEngine::DrawLine(hDC, rcBorder, rcBorderSize.bottom, GetAdjustColor(m_dwHotBorderColor), m_nBorderStyle);
			}
		}
	}
}