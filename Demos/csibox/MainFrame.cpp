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
