#ifndef __UIMENU_H__
#define __UIMENU_H__

namespace DuiLib
{
	class UILIB_API CMenuItemUI : public CListContainerElementUI
	{
		DECLARE_DUICONTROL(CMenuItemUI)
		typedef CListContainerElementUI parent_type;

	public:
		CMenuItemUI();
		virtual ~CMenuItemUI(void);

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void SetId(LPCTSTR pstrId);
		int GetId() const;

		void DoEvent(TEventUI& event);

	private:
		int m_id;
	};

	class UILIB_API CMenuUI : public CListUI
	{
		DECLARE_DUICONTROL(CMenuUI)
		typedef CListUI parent_type;

	public:
		CMenuUI();
		virtual ~CMenuUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void SetOwner(CControlUI* pOwner);
		CControlUI* GetOwner() const;
		void SetMenuPaddingEnabled(bool bMenuPaddingEnabled);
		bool GetMenuPaddingEnabled() const;
		void SetMenuPadding(RECT rcPadding);
		RECT GetMenuPadding() const;
		void SetTrianglePadding(const RECT rcPadding);
		RECT GetTrianglePadding() const;
		void SetTriangleEnabled(bool bTriangleUsed);
		bool GetTriangleEnabled() const;

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		CMenuItemUI* GetMenuItemById(int iMenuId);

	private:
		CControlUI* m_pOwner;
		bool m_bMenuPaddingEnabled;
		RECT m_rcMenuPadding;
		RECT m_trianglePadding;
		bool m_bTriangleEnabled;
	};

	class CContextMenuWnd;
	class UILIB_API CContextMenuUI : public CControlUI
	{
		DECLARE_DUICONTROL(CContextMenuUI)
		typedef CControlUI parent_type;

	public:
		CContextMenuUI();
		virtual ~CContextMenuUI(void);

		void Init();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void SetXmlFile(LPCTSTR pStrXmlFile);
		LPCTSTR GetXmlFile();
		void SetDialogBuilder(IDialogBuilderCallback* pCallback);
		IDialogBuilderCallback* GetDialogBuilder() const;

	private:
		CDuiString m_sXmlFile;
		CContextMenuWnd* m_pWindow;
		IDialogBuilderCallback* m_pCallback;
	};
} // namespace DuiLib

#endif // __UIMENU_H__
