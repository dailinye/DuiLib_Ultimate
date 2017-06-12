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
	typedef class_name this_type;\
	static CControlUI* CreateControl();\
	virtual CControlUI* CreateDataTemplateControl(CControlUI* pInstance); \
private:\
	class_name* DoInitDataTemplate(class_name* pInstance);

#define IMPLEMENT_DUICONTROL_INIT_DATATEMPLATE(class_name)                                   \
	CControlUI* class_name::CreateControl()                                                  \
	{															                             \
		return new class_name;                                                               \
	}                                                                                        \
	                                                                                         \
	CControlUI* class_name::CreateDataTemplateControl(CControlUI* pInstance)                 \
	{                                                                                        \
        /*init parent*/                                                                      \
        if( NULL == parent_type::CreateDataTemplateControl(pInstance) ) {                    \
			return NULL;                                                                     \
		}                                                                                    \
        /*init*/                                                                             \
        if( NULL == DoInitDataTemplate(static_cast<this_type*>(pInstance)) ) {               \
			return NULL;                                                                     \
		}                                                                                    \
        return pInstance;                                                                    \
	}                                                                                        \
                                                                                             \
    class_name* class_name::DoInitDataTemplate(class_name* pInstance)                        \
    {                                                                                        \
        return pInstance;                                                                    \
	}

#define IMPLEMENT_DUICONTROL_UINIT_DATATEMPLATE(class_name)                                  \
	CControlUI* class_name::CreateControl()                                                  \
	{															                             \
		return new class_name;                                                               \
	}                                                                                        \
	                                                                                         \
	CControlUI* class_name::CreateDataTemplateControl(CControlUI* pInstance)                 \
	{                                                                                        \
        /*init parent*/                                                                      \
        if( NULL == parent_type::CreateDataTemplateControl(pInstance) ) {                    \
			return NULL;                                                                     \
		}                                                                                    \
        /*init*/                                                                             \
        if( NULL == DoInitDataTemplate(static_cast<this_type*>(pInstance)) ) {               \
			return NULL;                                                                     \
		}                                                                                    \
        return pInstance;                                                                    \
	}

#define REGIST_DUICONTROL(class_name)\
	CControlFactory::GetInstance()->RegistControl(_T(#class_name), (CreateClass)class_name::CreateControl);

#define INNER_REGISTER_DUICONTROL(class_name)\
	RegistControl(_T(#class_name), (CreateClass)class_name::CreateControl);
}