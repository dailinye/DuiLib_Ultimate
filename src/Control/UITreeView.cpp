#include "StdAfx.h"

namespace DuiLib {
	IMPLEMENT_DUICONTROL_INIT_DATATEMPLATE(CTreeNodeElementUI)
	IMPLEMENT_DUICONTROL_INIT_DATATEMPLATE(CTreeNodeBodyUI)
	IMPLEMENT_DUICONTROL_INIT_DATATEMPLATE(CTreeNodeUI)
	IMPLEMENT_DUICONTROL_INIT_DATATEMPLATE(CTreeViewUI)		

	//////////////////////////////////////////////////////////////////////////
	// TreeNodeElement
	CTreeNodeElementUI::CTreeNodeElementUI()
		:m_pOwner(NULL)
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

	CTreeViewUI *CTreeNodeElementUI::GetOwner()
	{
		return m_pOwner;
	}

	void CTreeNodeElementUI::SetOwner(CTreeViewUI* pOwner)
	{
		m_pOwner = pOwner;
	}

	void CTreeNodeElementUI::DoEvent(TEventUI& event)
	{
		CTreeNodeUI *pParent = (CTreeNodeUI*)GetParent();
		if (pParent) pParent = (CTreeNodeUI*)pParent->GetInterface(DUI_CTR_TREENODE);

		if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND) {			
			if (pParent) return pParent->DoEvent(event);
			else CHorizontalLayoutUI::DoEvent(event);
		}

		if (event.Type == UIEVENT_DBLCLICK)
		{
			if (IsEnabled()) {
				if (m_pOwner && m_pOwner->GetCanExpandOnDbClick()) {
					if (pParent) pParent->Expand(!pParent->IsExpanded());
				} else {
					Activate();
					Invalidate();
				}
				m_pManager->SendNotify(this, DUI_MSGTYPE_ITEMDBCLICK);
			}
			return;
		}
		if (event.Type == UIEVENT_BUTTONUP)
		{
			if (IsEnabled()) {
				if (m_pOwner && m_pOwner->GetCanExpandOnClick()) {
					if (pParent) pParent->Expand(!pParent->IsExpanded());
				}
				m_pManager->SendNotify(this, DUI_MSGTYPE_ITEMCLICK);
			}
			return;
		}
		if (event.Type == UIEVENT_MOUSEMOVE)
		{
			return;
		}
		if (event.Type == UIEVENT_MOUSEENTER)
		{
			if (IsEnabled()) {
				m_uButtonState |= UISTATE_HOT;
				Invalidate();
			}
			return;
		}
		if (event.Type == UIEVENT_MOUSELEAVE)
		{
			if ((m_uButtonState & UISTATE_HOT) != 0) {
				m_uButtonState &= ~UISTATE_HOT;
				Invalidate();
			}
			return;
		}
		if (pParent) pParent->DoEvent(event);
	}

	void CTreeNodeElementUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		return CHorizontalLayoutUI::SetAttribute(pstrName, pstrValue);
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
		if (_tcsicmp(pstrName, _T("TreeNodeBody")) == 0) return static_cast<CTreeNodeBodyUI*>(this);
		return CVerticalLayoutUI::GetInterface(pstrName);
	}

	SIZE CTreeNodeBodyUI::EstimateSize(SIZE szAvailable)
	{
		SIZE cXY = { GetFixedWidth(), 0 };
		for (int it = 0; it < m_items.GetSize(); it++) {
			if (IsVisible()) cXY.cy += static_cast<CControlUI*>(m_items[it])->EstimateSize(szAvailable).cy;
		}
		return cXY;
	}

	void CTreeNodeBodyUI::SetVisible(bool bVisible)
	{
		CVerticalLayoutUI::SetVisible(bVisible);
		for (int it = 0; it < m_items.GetSize(); it++) {
			static_cast<CControlUI*>(m_items[it])->SetVisible(bVisible);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// TreeNode
	CTreeNodeUI::CTreeNodeUI()
		:m_pOwner(NULL)
		,m_pParentNode(NULL)
		,m_pElement(NULL)
		,m_pChildren(NULL)
		,m_bExpanded(false)
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

		if (m_pParentNode == NULL) SetVisible(true);
		if (m_pOwner && pOwner->GetAutoExpand()) m_bExpanded = true;

		CTreeNodeUI *pTreeNode = NULL;
		for (int it = 0; it < m_pChildren->GetCount(); it++) {
			pTreeNode = static_cast<CTreeNodeUI*>(m_pChildren->GetItemAt(it)->GetInterface(DUI_CTR_TREENODE));
			if (pTreeNode == NULL) continue;
			pTreeNode->SetParentNode(this);
			pTreeNode->SetOwner(m_pOwner);
			if (pOwner && pOwner->GetAutoExpand()) pTreeNode->SetVisible(true);
		}
		m_pElement->SetOwner(pOwner);
		RECT rcInset = m_pChildren->GetInset();
		rcInset.left += m_pOwner->GetNodeSepWidth();
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

	bool CTreeNodeUI::IsExpanded() const
	{
		return m_bExpanded;
	}

	bool CTreeNodeUI::Expand(bool bExpand)
	{
		if (m_bExpanded == bExpand) return true;
		m_pChildren->SetVisible(bExpand);
		m_bExpanded = bExpand;

		if (m_pOwner) m_pOwner->SetPos(m_pOwner->GetPos(), true);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// TreeView
	CTreeViewUI::CTreeViewUI(void)
		:m_iNodeSepWidth(10)
		,m_bEnableMultiSelect(false)
		,m_bAutoExpand(false)
		,m_bCanExpandOnClick(false)
		,m_bCanExpandOnDbClick(false)
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
		if (_tcsicmp(pstrName, _T("nodesepwidth")) == 0) SetNodeSepWidth(_ttoi(pstrValue));
		if (_tcsicmp(pstrName, _T("autoexpand")) == 0) SetAutoExpand(_tcsicmp(pstrValue, _T("true")) == 0);
		if (_tcsicmp(pstrName, _T("expandaction")) == 0) SetExpandAction(pstrValue);
		else return CVerticalLayoutUI::SetAttribute(pstrName, pstrValue);
	}

	void CTreeViewUI::Init()
	{
		CTreeNodeUI *pTreeNode = NULL;
		for (int it = 0; it < m_items.GetSize(); it++) {
			pTreeNode = static_cast<CTreeNodeUI*>(static_cast<CControlUI*>(m_items[it])->GetInterface(DUI_CTR_TREENODE));
			if (pTreeNode == NULL) continue;
			pTreeNode->SetOwner(this);
		}
	}

	void CTreeViewUI::SetNodeSepWidth(const int iNodeSepWidth)
	{
		m_iNodeSepWidth = iNodeSepWidth;
	}

	int CTreeViewUI::GetNodeSepWidth(void) const
	{
		return m_iNodeSepWidth;
	}

	void CTreeViewUI::SetEnableMultiSelect(const bool bMultiSel)
	{
		m_bEnableMultiSelect = bMultiSel;
	}

	bool CTreeViewUI::GetEnableMultiSelect(void) const
	{
		return m_bEnableMultiSelect;
	}

	void CTreeViewUI::SetAutoExpand(const bool bAutoExpand)
	{
		m_bAutoExpand = bAutoExpand;
	}

	bool CTreeViewUI::GetAutoExpand() const
	{
		return m_bAutoExpand;
	}

	void CTreeViewUI::SetExpandAction(LPCTSTR ptrAction)
	{
		m_bCanExpandOnClick = (_tcsicmp(ptrAction, _T("click")) == 0);
		m_bCanExpandOnDbClick = (_tcsicmp(ptrAction, _T("dbclick")) == 0);
	}

	bool CTreeViewUI::GetCanExpandOnClick() const
	{
		return m_bCanExpandOnClick;
	}

	bool CTreeViewUI::GetCanExpandOnDbClick() const
	{
		return m_bCanExpandOnDbClick;
	}
}