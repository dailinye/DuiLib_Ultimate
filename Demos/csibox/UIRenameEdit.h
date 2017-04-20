#pragma once

namespace DuiLib
{
	class CRenameEditWnd;

	typedef enum RENAMESTATE_UI
	{
		UIRENAME_UNINITIAL,
		UIRENAME_CLICK,
		UIRENAME_ACTIVATE
	};

	class CRenameEditUI : public CEditUI, public IListItemCallbackUI
	{
		DECLARE_DUICONTROL(CRenameEditUI)
		friend class CRenameEditWnd;
	public:
		CRenameEditUI();
		virtual ~CRenameEditUI();

		void Init();

		void SetPos(RECT rc, bool bNeedInvalidate = true);
		void Move(SIZE szOffset, bool bNeedInvalidate = true);

		bool Select(CControlUI* pItem, bool bSelect = true);

		void DoEvent(TEventUI& event);

	protected:
		void SetRenameState(RENAMESTATE_UI state);
		RENAMESTATE_UI GetRenameState() const;

	private:
		RENAMESTATE_UI m_iRenameState;
	};
}
