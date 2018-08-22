#ifndef __UITREEVIEW_H__
#define __UITREEVIEW_H__

#pragma once

namespace DuiLib {

	class CTreeViewUI;

	class UILIB_API CTreeNodeElementUI : public CHorizontalLayoutUI
	{
		DECLARE_DUICONTROL(CTreeNodeElementUI)
		typedef CHorizontalLayoutUI parent_type;

	public:
		CTreeNodeElementUI();
		~CTreeNodeElementUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
	};

	class UILIB_API CTreeNodeBodyUI : public CVerticalLayoutUI
	{
		DECLARE_DUICONTROL(CTreeNodeBodyUI)
		typedef CVerticalLayoutUI parent_type;

	public:
		CTreeNodeBodyUI();
		~CTreeNodeBodyUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		SIZE EstimateSize(SIZE szAvailable);

	private:
		CTreeViewUI *m_pOwner;
	};

	class UILIB_API CTreeNodeUI : public CVerticalLayoutUI
	{
		DECLARE_DUICONTROL(CTreeNodeUI)
		typedef CVerticalLayoutUI parent_type;

	public:
		CTreeNodeUI();
		~CTreeNodeUI(void);

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		CControlUI* GetItemAt(int iIndex) const;
		int GetItemIndex(CControlUI* pControl) const;
		bool SetItemIndex(CControlUI* pControl, int iIndex);
		int GetCount() const;
		bool Add(CControlUI* pControl);
		bool AddAt(CControlUI* pControl, int iIndex);
		bool Remove(CControlUI* pControl);
		bool RemoveAt(int iIndex);
		void RemoveAll();

		void SetPos(RECT rc, bool bNeedInvalidate = true);
		SIZE EstimateSize(SIZE szAvailable);

		CTreeViewUI *GetOwner();
		void SetOwner(CTreeViewUI* pOwner);
		CTreeNodeUI *GetParentNode();
		void SetParentNode(CTreeNodeUI *pParentNode);
		//bool IsSelected() const;
		//bool Select(bool bSelect = true);
		//bool SelectMulti(bool bSelect = true);
		//bool IsExpanded() const;
		//bool Expand(bool bExpand = true);
		//
		//SIZE EstimateSize(SIZE szAvailable);
		//void SetPos(RECT rc, bool bNeedInvalidate = true);

	private:
		CTreeViewUI *m_pOwner;
		CTreeNodeElementUI *m_pElement;
		CTreeNodeBodyUI *m_pChildren;
		CTreeNodeUI *m_pParentNode;
	};

	class UILIB_API CTreeViewUI : public CVerticalLayoutUI
	{
		DECLARE_DUICONTROL(CTreeViewUI)
		typedef CVerticalLayoutUI parent_type;

	public:
		CTreeViewUI(void);
		~CTreeViewUI(void);

		LPCTSTR GetClass() const;
		UINT GetControlFlags() const;
		LPVOID	GetInterface(LPCTSTR pstrName);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void Init();

		void SetLeafSep(const int iLeafSep);
		int GetLeafSep(void) const;
		void SetEnableMultiSelect(const bool bMultiSel);
		bool GetEnableMultiSelect(void) const;

	private:
		int m_iLeafSep;
		bool m_bEnableMultiSelect;
	};
}	// namespace DuiLib
#endif // __UITREEVIEW_H__
