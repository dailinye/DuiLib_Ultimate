#pragma once

class MainFrame
{
public:
	MainFrame();
	~MainFrame();

	CWindowWnd& getWindow();
private:
	class Impl;
	std::shared_ptr<Impl> impl_;
};

