#include "stdafx.h"

//int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;
	HRESULT hRes = ::OleInitialize(NULL);
	if (FAILED(Hr))
	{
		::CoUninitialize();
		return 0;
	}
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetCurrentPath() + _T("\\skin\\cisbox"));
	
	// init custom controls
	REGIST_DUICONTROL(CRenameEditUI);

	MainFrame* pMainFrame = new MainFrame();
	if (pMainFrame == NULL) return 0;
	//pMainFrame->getWindow().Create(NULL, _T("csibox"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 600);
	pMainFrame->getWindow().Create(NULL, _T("csibox"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pMainFrame->getWindow().ShowWindow();
	//CPaintManagerUI::SetAllDPI(CDPI::GetMainMonitorDPI());
	pMainFrame->getWindow().CenterWindow();

	CPaintManagerUI::MessageLoop();

	delete pMainFrame;
	pMainFrame = NULL;

	::OleUninitialize();
	::CoUninitialize();

	return 0;
}
