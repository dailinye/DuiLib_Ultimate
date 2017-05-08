#pragma once
#include <map>
namespace DuiLib 
{
	typedef CControlUI* (*CreateClass)();
	typedef std::map<CDuiString, CreateClass> MAP_DUI_CTRATECLASS;

	class UILIB_API CControlFactory
	{
	public:
		CControlUI* CreateControl(CDuiString strClassName);
		void RegistControl(CDuiString strClassName, CreateClass pFunc);

		static CControlFactory* GetInstance();
		void Release();

	private:	
		CControlFactory();
		virtual ~CControlFactory();

	private:
		MAP_DUI_CTRATECLASS m_mapControl;
	};

#define DECLARE_DUICONTROL(class_name)\
public:\
	static CControlUI* CreateControl();\
	virtual CControlUI* CreateDataTemplateControl(CControlUI* pInstance = NULL);

#define IMPLEMENT_DUICONTROL(class_name)                                              \
	CControlUI* class_name::CreateControl()                                           \
	{															                      \
		return new class_name;                                                        \
	}                                                                                 \
	                                                                                  \
	CControlUI* class_name::CreateDataTemplateControl(CControlUI* pInstance)          \
	{                                                                                 \
		class_name* pControl = static_cast<class_name*>(pInstance);                   \
		if( NULL == pControl ) {                                                      \
			pControl = static_cast<class_name*>(class_name::CreateControl());         \
			if (NULL == pControl) return NULL;                                        \
			*pControl = *this;                                                        \
		}                                                                             \
		return parent_type::CreateDataTemplateControl(pControl);                      \
	}

#define IMPLEMENT_DUICONTROL_DATATEMPLATE_NONE(class_name)\
	CControlUI* class_name::CreateControl()\
	{ return new class_name; }

#define REGIST_DUICONTROL(class_name)\
	CControlFactory::GetInstance()->RegistControl(_T(#class_name), (CreateClass)class_name::CreateControl);

#define INNER_REGISTER_DUICONTROL(class_name)\
	RegistControl(_T(#class_name), (CreateClass)class_name::CreateControl);
}