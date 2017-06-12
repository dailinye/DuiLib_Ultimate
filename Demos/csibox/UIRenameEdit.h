#pragma once

namespace DuiLib
{
#define DUI_CTR_RENAMEEDIT (_T("RenameEdit"))

	class CRenameEditWnd;

	enum RENAMESTATE_UI
	{
		UIRENAME_UNINITIAL,
		UIRENAME_CLICK,
		UIRENAME_ACTIVATE
	};

	class CRenameEditUI : public CEditUI, public IListItemCallbackUI
	{
		DECLARE_DUICONTROL(CRenameEditUI)
		typedef CEditUI parent_type;
		friend class CRenameEditWnd;

	public:
		CRenameEditUI();
		virtual ~CRenameEditUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		void DoInit();

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
