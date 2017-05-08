#include "stdafx.h"

class MainFrame::Impl : public WindowImplBase
{
public:
	virtual CDuiString GetSkinFile()
	{
		return _T("MainFrame.xml");
	}

	virtual LPCTSTR GetWindowClassName(void) const
	{
		return _T("MainFrame");
	}

	CWindowWnd& getWindow()
	{
		return *this;
	}

	void InitWindow()
	{
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("adressbar")));
		if (NULL == pControl)
		{
			return;
		}
		CContainerUI *pAddressBar = static_cast<CContainerUI*>(pControl->GetInterface(DUI_CTR_CONTAINER));
		if (NULL == pAddressBar)
		{
			return;
		}
		CDataTemplateUI *pDataTemplate = pAddressBar->GetDataTemplate();
		if (NULL == pDataTemplate)
		{
			return;
		}
		for (int i = 0; i < 10; ++i)
		{
			CControlUI *pItem = pDataTemplate->CreateTemplateItem();
			if (NULL != pItem)
			{
				if (pItem->GetInterface(DUI_CTR_CONTAINER))
				{
					CControlUI *pContent = m_pm.FindSubControlByClass(pItem, _T("ButtonUI"));
					if (pContent)
					{
						CDuiString text;
						text.Format(_T("Hello%d"), i);
						pContent->SetText(text);
					}
				}
				pAddressBar->Add(pItem);
			}			
		}
	}

	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = TRUE;
		::PostQuitMessage(0);
		return 0;
	}
};

MainFrame::MainFrame()
	:impl_(std::make_shared<Impl>())
{
}

MainFrame::~MainFrame()
{
}

CWindowWnd& MainFrame::getWindow()
{
	assert(impl_);
	return impl_->getWindow();
}
