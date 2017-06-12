#ifndef __UIHORIZONTALLAYOUT_H__
#define __UIHORIZONTALLAYOUT_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CHorizontalLayoutUI : public CContainerUI
	{
		DECLARE_DUICONTROL(CHorizontalLayoutUI)
		typedef CContainerUI parent_type;

	public:
		CHorizontalLayoutUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		void SetSepWidth(int iWidth);
		int GetSepWidth() const;
		void SetSepImmMode(bool bImmediately);
		bool IsSepImmMode() const;
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void DoEvent(TEventUI& event);

		void SetPos(RECT rc, bool bNeedInvalidate = true);
		void DoPostPaint(HDC hDC, const RECT& rcPaint);
		SIZE EstimateSize(SIZE szAvailable);

		RECT GetThumbRect(bool bUseNew = false) const;
		virtual bool GetAutoCalcWidth() const;
		virtual void SetAutoCalcWidth(bool bAutoCalcWidth);

	protected:
		int m_iSepWidth;
		UINT m_uButtonState;
		POINT ptLastMouse;
		RECT m_rcNewPos;
		bool m_bImmMode;
		bool m_bAutoCalcWidth;
	};
}
#endif // __UIHORIZONTALLAYOUT_H__
