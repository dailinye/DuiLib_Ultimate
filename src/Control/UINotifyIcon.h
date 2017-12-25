#pragma once

namespace DuiLib
{
#ifndef WM_NOTIFYICON
#define WM_NOTIFYICON (WM_APP+2)
#endif

	enum NotifyIconEvent
	{
		NOTIFYICON_CLICK,
		NOTIFYICON_RCLICK,
		NOTIFYICON_DBCLICK,
		NOTIFYICON_RDBCLICK
	};

	class UILIB_API CNotifyIconUI : public CControlUI
	{
		DECLARE_DUICONTROL(CNotifyIconUI)
		typedef CControlUI parent_type;

	public:
		CNotifyIconUI(void);
		virtual ~CNotifyIconUI(void);

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		int GetId() const;
		void SetNotifyIcon(LPCTSTR pstrIcon);
		LPCTSTR GetNotifyIcon() const;
		//void SetBalloonEnabled(bool bEnabled);
		//bool GetBalloonEnabled() const;
		//void SetBalloonTimeout(const UINT uiTimeout);
		//UINT GetBalloonTimeout() const;

		void Init();
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void DoEvent(TEventUI& event);

		void ShowBalloon(LPCTSTR pstrContent);
		bool ReInit();

	private:
		CDuiString m_sNotifyIcon;
		int m_id;
		HICON m_hIcon;
	};
}
