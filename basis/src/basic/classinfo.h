#pragma once

namespace mania
{
typedef void* ( *CONSTRUCTOR_FUNC )();
struct MANIA_BASIS_API classinfo
{
	bool is( const classinfo* another )
	{
		if ( this == another )
			return true;
		else
			for ( unsigned int i = 0; i < parent_class.size(); i++ )
				if ( parent_class[i]->is( another ) )
					return true;
		return false;
	}

	mania::string			name;
	std::vector<classinfo*>	parent_class;
	CONSTRUCTOR_FUNC		construct;
};


#define MANIA_DEFINE_PARENT_CLASS( childclass, parentclass ) childclass::get_classinfo()->parent_class.push_back(parentclass::get_classinfo());
#define MANIA_DECLAR_CLASS( classname )\
	public:\
	static classinfo*	get_classinfo(){ return &classname::static_classinfo; }\
	static void*		constructor(){ return new classname(); }\
	void bind_classinfo() { m_classinfo = &classname::static_classinfo; }\
	private:\
	static classinfo	static_classinfo;

#define MANIA_DECLAR_ABSTRACTL_CLASS( classname )\
	public:\
	static classinfo*	get_classinfo(){ return &classname::static_classinfo; }\
	void bind_classinfo() { m_classinfo = &classname::static_classinfo; }\
	private:\
	static classinfo	static_classinfo;

#define MANIA_CLASS_CONSTRUCTOR_INCLUDE( classname )\
	bind_classinfo();\
	m_id = classname::static_classinfo.name;

#define	MANIA_REGISTER_CLASS( classname )\
	classname::get_classinfo()->name = _T(#classname);\
	classname::get_classinfo()->construct = ((mania::CONSTRUCTOR_FUNC)(classname::constructor));

#define MANIA_DEFINE_PTR(classname)	typedef sptr<classname> classname##_ptr;

#define MANIA_IMPLEMENT_CLASS( classname ) mania::classinfo classname::static_classinfo;
}