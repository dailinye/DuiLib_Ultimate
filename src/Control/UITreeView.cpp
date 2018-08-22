#include "StdAfx.h"

namespace DuiLib {
	IMPLEMENT_DUICONTROL_INIT_DATATEMPLATE(CTreeNodeElementUI)
	IMPLEMENT_DUICONTROL_INIT_DATATEMPLATE(CTreeNodeBodyUI)
	IMPLEMENT_DUICONTROL_INIT_DATATEMPLATE(CTreeNodeUI)
	IMPLEMENT_DUICONTROL_INIT_DATATEMPLATE(CTreeViewUI)		

	//////////////////////////////////////////////////////////////////////////
	// TreeNodeElement
	CTreeNodeElementUI::CTreeNodeElementUI()
	{
	}

	CTreeNodeElementUI::~CTreeNodeElementUI()
	{
	}

	LPCTSTR CTreeNodeElementUI::GetClass() const
	{
		return _T("TreeNodeElementUI");
	}

	LPVOID CTreeNodeElementUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcsicmp(pstrName, DUI_CTR_TREENODEELEMENT) == 0) return static_cast<CTreeNodeElementUI*>(this);
		return CHorizontalLayoutUI::GetInterface(pstrName);
	}

	//////////////////////////////////////////////////////////////////////////
	// TreeNodeBody
	CTreeNodeBodyUI::CTreeNodeBodyUI()
		:m_pOwner(NULL)
	{
	}

	CTreeNodeBodyUI::~CTreeNodeBodyUI()
	{
	}

	LPCTSTR CTreeNodeBodyUI::GetClass() const
	{
		return _T("TreeNodeBodyUI");
	}

	LPVOID CTreeNodeBodyUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcsicmp(pstrName, DUI_CTR_TREENODEBODY) == 0) return static_cast<CTreeNodeBodyUI*>(this);
		return CVerticalLayoutUI::GetInterface(pstrName);
	}

	SIZE CTreeNodeBodyUI::EstimateSize(SIZE szAvailable)
	{
		SIZE cXY = { GetFixedWidth(), 0 };
		for (int it = 0; it < m_items.GetSize(); it++) {
			cXY.cy += static_cast<CControlUI*>(m_items[it])->EstimateSize(szAvailable).cy;
		}
		return cXY;
	}

	//////////////////////////////////////////////////////////////////////////
	// TreeNode
	CTreeNodeUI::CTreeNodeUI()
		:m_pOwner(NULL)
		,m_pParentNode(NULL)
		,m_pElement(NULL)
		,m_pChildren(NULL)
	{
		m_pChildren = new CTreeNodeBodyUI;
		CVerticalLayoutUI::Add(m_pChildren);
		SetVisible(false);
	}

	CTreeNodeUI::~CTreeNodeUI(void)
	{
		m_pOwner = NULL;
		m_pParentNode = NULL;
	}

	LPCTSTR CTreeNodeUI::GetClass() const
	{
		return _T("TreeNodeUI");
	}

	LPVOID CTreeNodeUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcsicmp(pstrName, DUI_CTR_TREENODE) == 0) return static_cast<CTreeNodeUI*>(this);
		return CVerticalLayoutUI::GetInterface(pstrName);
	}

	void CTreeNodeUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		return CVerticalLayoutUI::SetAttribute(pstrName, pstrValue);
	}

	CControlUI * CTreeNodeUI::GetItemAt(int iIndex) const
	{
		return m_pChildren->GetItemAt(iIndex);
	}

	int CTreeNodeUI::GetItemIndex(CControlUI * pControl) const
	{
		if (pControl->GetInterface(DUI_CTR_TREENODEELEMENT) != NULL) return CVerticalLayoutUI::GetItemIndex(pControl);
		return m_pChildren->GetItemIndex(pControl);
	}

	bool CTreeNodeUI::SetItemIndex(CControlUI * pControl, int iIndex)
	{
		if (pControl->GetInterface(DUI_CTR_TREENODEELEMENT) != NULL) return CVerticalLayoutUI::SetItemIndex(pControl, iIndex);
		return m_pChildren->SetItemIndex(pControl, iIndex);
	}

	int CTreeNodeUI::GetCount() const
	{
		return m_pChildren->GetCount();
	}

	bool CTreeNodeUI::Add(CControlUI * pControl)
	{
		if (pControl->GetInterface(DUI_CTR_TREENODEELEMENT) != NULL) {
			if (m_pElement != NULL) CVerticalLayoutUI::Remove(m_pElement);
			m_pElement = static_cast<CTreeNodeElementUI*>(pControl);
			return CVerticalLayoutUI::AddAt(pControl, 0);
		}
		return m_pChildren->Add(pControl);
	}

	bool CTreeNodeUI::AddAt(CControlUI * pControl, int iIndex)
	{
		if (pControl->GetInterface(DUI_CTR_TREENODEELEMENT) != NULL) {
			if (m_pElement != NULL) CVerticalLayoutUI::Remove(m_pElement);
			m_pElement = static_cast<CTreeNodeElementUI*>(pControl);
			return CVerticalLayoutUI::AddAt(pControl, 0);
		}
		return m_pChildren->AddAt(pControl, iIndex);
	}

	bool CTreeNodeUI::Remove(CControlUI * pControl)
	{
		if (pControl->GetInterface(DUI_CTR_TREENODEELEMENT) != NULL) {
			return CVerticalLayoutUI::Remove(pControl);
		}
		if (pControl->GetInterface(DUI_CTR_TREENODE)) return m_pChildren->Remove(pControl);
		return true;
	}

	bool CTreeNodeUI::RemoveAt(int iIndex)
	{
		return m_pChildren->RemoveAt(iIndex);
	}

	void CTreeNodeUI::RemoveAll()
	{
		return m_pChildren->RemoveAll();
	}

	void CTreeNodeUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		CVerticalLayoutUI::SetPos(rc, bNeedInvalidate);

		//if (m_pElement != NULL) {
		//	if (!m_pElement->IsVisible()) {
		//		m_pElement->SetPos(CDuiRect(rc.left, 0, rc.right, 0), bNeedInvalidate);
		//	} else {
		//		m_pElement->SetPos(m_pElement->GetPos(), bNeedInvalidate);
		//	}
		//}

		//for (int i = 0; i < m_ListInfo.nColumns; i++) {
		//	CControlUI* pControl = static_cast<CControlUI*>(m_pHeader->GetItemAt(i));
		//	if (!pControl->IsVisible()) continue;
		//	if (pControl->IsFloat()) continue;
		//	RECT rcPos = pControl->GetPos();
		//	m_ListInfo.rcColumn[i] = pControl->GetPos();
		//}
		//if (!m_pHeader->IsVisible()) {
		//	for (int it = 0; it < m_pHeader->GetCount(); it++) {
		//		static_cast<CControlUI*>(m_pHeader->GetItemAt(it))->SetInternVisible(false);
		//	}
		//}
		//m_pList->SetPos(m_pList->GetPos(), bNeedInvalidate);
	}

	SIZE CTreeNodeUI::EstimateSize(SIZE szAvailable)
	{
		SIZE cXY = { 0, 0 };
		if (!IsVisible()) return cXY;
		if (m_pElement == NULL) return cXY;

		cXY.cx = GetFixedWidth();
		cXY.cy += m_pElement->EstimateSize(szAvailable).cy;
		cXY.cy += m_pChildren->EstimateSize(szAvailable).cy;
		
		return cXY;
	}

	CTreeViewUI *CTreeNodeUI::GetOwner()
	{
		return m_pOwner;
	}

	void CTreeNodeUI::SetOwner(CTreeViewUI* pOwner)
	{
		m_pOwner = pOwner;
		CTreeNodeUI *pTreeNode = NULL;
		for (int it = 0; it < m_pChildren->GetCount(); it++) {
			pTreeNode = static_cast<CTreeNodeUI*>(m_pChildren->GetItemAt(it)->GetInterface(DUI_CTR_TREENODE));
			if (pTreeNode == NULL) continue;
			pTreeNode->SetOwner(m_pOwner);
			pTreeNode->SetParentNode(this);
		}
		RECT rcInset = m_pChildren->GetInset();
		rcInset.left += m_pOwner->GetLeafSep();
		m_pChildren->SetInset(rcInset);
	}

	CTreeNodeUI *CTreeNodeUI::GetParentNode()
	{
		return m_pParentNode;
	}

	void CTreeNodeUI::SetParentNode(CTreeNodeUI *pParentNode)
	{
		m_pParentNode = pParentNode;
	}

	//////////////////////////////////////////////////////////////////////////
	// TreeView
	CTreeViewUI::CTreeViewUI(void)
		:m_iLeafSep(10)
		,m_bEnableMultiSelect(false)
	{
	}

	CTreeViewUI::~CTreeViewUI(void)
	{
	}

	LPCTSTR CTreeViewUI::GetClass() const
	{
		return _T("TreeViewUI");
	}

	UINT CTreeViewUI::GetControlFlags() const
	{
		return UIFLAG_TABSTOP;
	}

	LPVOID CTreeViewUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcsicmp(pstrName, DUI_CTR_TREEVIEW) == 0) return static_cast<CTreeViewUI*>(this);
		return CVerticalLayoutUI::GetInterface(pstrName);
	}

	void CTreeViewUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcsicmp(pstrName, _T("leafsep")) == 0) SetLeafSep(_ttoi(pstrValue));
		else return CVerticalLayoutUI::SetAttribute(pstrName, pstrValue);
	}

	void CTreeViewUI::Init()
	{
		CTreeNodeUI *pTreeNode = NULL;
		for (int it = 0; it < m_items.GetSize(); it++) {
			pTreeNode = static_cast<CTreeNodeUI*>(static_cast<CControlUI*>(m_items[it])->GetInterface(DUI_CTR_TREENODE));
			if (pTreeNode == NULL) continue;
			pTreeNode->SetOwner(this);
			pTreeNode->SetVisible(true);
		}
	}

	void CTreeViewUI::SetLeafSep(const int iLeafSep)
	{
		m_iLeafSep = iLeafSep;
	}

	int CTreeViewUI::GetLeafSep(void) const
	{
		return m_iLeafSep;
	}

	void CTreeViewUI::SetEnableMultiSelect(const bool bMultiSel)
	{
		m_bEnableMultiSelect = bMultiSel;
	}

	bool CTreeViewUI::GetEnableMultiSelect(void) const
	{
		return m_bEnableMultiSelect;
	}
}