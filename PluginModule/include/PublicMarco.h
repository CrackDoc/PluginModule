#ifndef PublicMarco_h__
#define PublicMarco_h__


#define DECLARE_SINGLETON_CLASS(baseClass, className, EXPORT_MARK) \
class EXPORT_MARK className:public baseClass \
	{\
	private:\
	className();\
	~className();\
	public:\
	static className* GetInstance();\
	static void ExitInstance();\
	private:\
	static className* m_pInstance;\
};

#define IMPLEMENT_SINGLETON_CLASS(className) \
	className* className::m_pInstance = NULL; \
	className::className(){}\
	className::~className(){}\
	className* className::GetInstance()\
	{\
		if(NULL == m_pInstance)\
		{\
			m_pInstance = new className();\
		}\
		return m_pInstance;\
	}\
	void className::ExitInstance(){\
		if(NULL != m_pInstance) \
		{\
			delete m_pInstance;\
			m_pInstance = NULL;\
		}\
	}

//另外一种单件[使用这种单件的原因是显示的调用单件创建,防止静态库中多份单件的情况,也可以指控释放顺序]
#define DECLARE_SINGLETON_EX(className) \
	public:\
		static className* GetInstance(); \
	protected: \
		static className* m_pInstance;

#define IMPLEMENT_SINGLETON_EX(className) \
	className* className::m_pInstance = NULL; \
	className* className::GetInstance() \
	{ \
		return m_pInstance; \
	}

#define DECLARE_SINGLETON_CLASS_EX(baseClass, className, EXPORT_MARK) \
	class EXPORT_MARK className : public baseClass \
	{ \
	public: \
		className(){} \
		~className(){} \
		\
		static baseClass* CreateInstance() \
		{ \
			if (NULL == m_pInstance) \
			{ \
				m_pInstance = new className(); \
			} \
			return m_pInstance; \
		}\
		static void ReleaseInstance() \
		{ \
			if (NULL != m_pInstance) \
			{ \
				delete m_pInstance; \
				m_pInstance = NULL; \
			} \
		}\
	};

#define DECLARE_ACMODULE(ModuleName, EXPORT_TAG) \
	EXPORT_TAG I##ModuleName##Module* GetOrCreate##ModuleName##Module(); \
	EXPORT_TAG void Destroy##ModuleName##Module();


#define IMPLEMENT_ACMODULE(ModuleName) \
	static C##ModuleName##Module* g_p##ModuleName##Module = NULL; \
	I##ModuleName##Module* GetOrCreate##ModuleName##Module() \
	{ \
	if (NULL != g_p##ModuleName##Module) \
		{ \
			return g_p##ModuleName##Module; \
		} \
		g_p##ModuleName##Module = new C##ModuleName##Module(); \
		bool bInitOk = g_p##ModuleName##Module->Initialise(); \
		if (!bInitOk) \
		{ \
			delete g_p##ModuleName##Module; \
			g_p##ModuleName##Module = NULL; \
			return NULL; \
		} \
		return g_p##ModuleName##Module; \
	}\
	\
	void Destroy##ModuleName##Module()\
	{ \
		if(NULL == g_p##ModuleName##Module) \
		{ \
			return; \
		} \
		\
		g_p##ModuleName##Module->Uninitialise(); \
		delete g_p##ModuleName##Module; \
		g_p##ModuleName##Module = NULL; \
	}

#define SEGMENT_CREATE_FUNCTION(typeName, container, str0) \
		typeName* pObject = new typeName(); \
		bool bInitOK = pObject->Initialise(str0); \
		\
		if (!bInitOK) \
		{ \
			delete pObject; \
			pObject = NULL; \
			return NULL; \
		} \
		\
		container.insert(std::make_pair(pObject->GetID(), pObject)); \
		\
		return pObject; 

#define SEGMENT_OBJECT_CREATE_FUNCTION(strClassName, typeName, creator, container, str0) \
	IObject* pObject = creator->CreateObject(strClassName); \
	if (NULL == pObject) \
	{ \
		return NULL; \
	} \
	\
	typeName* pTypeObject = dynamic_cast<typeName*>(pObject); \
	if (NULL == pTypeObject) \
	{ \
		delete pObject; \
		return NULL; \
	} \
	bool bInitOK = pTypeObject->Initialise(str0); \
	\
	if (!bInitOK) \
	{ \
		delete pTypeObject; \
		pTypeObject = NULL; \
		return NULL; \
	} \
	\
	container.insert(std::make_pair(pTypeObject->GetID(), pTypeObject)); \
	\
	return pTypeObject; 

#define SEGMENT_DESTROY_FUNCTION(typeName, container) \
	if(NULL != pObject) \
	{ \
		typeName* p##typeName = dynamic_cast<typeName*>(pObject); \
		\
		if (NULL != p##typeName) \
		{ \
			p##typeName->Uninitialise(); \
		} \
			\
		delete p##typeName; \
		p##typeName = NULL; \
		\
		auto it = container.begin(); \
		for (; it != container.end(); ++it) \
		{ \
			if (it->second == pObject) \
			{ \
				container.erase(it); \
				return; \
			} \
		} \
		 \
		pObject = NULL; \
	}

// Brief:新增代码片段,查找(LOOKUP)和获取(GET) [2020/5/26 kyske] 
#define SEGMENT_GET_FUNCTION(ID, container) \
	auto it = container.find(ID); \
	if(it == container.end()) \
	{ \
		return NULL; \
	} \
	else \
	{ \
		return it->second; \
	}

#define SEGMENT_LOOKUP_VIA_NAME_FUNCTION(name, container) \
	auto it = container.begin(); \
	for(; it != container.end(); ++it) \
	{ \
		if(it->second == NULL) \
		{ \
			continue; \
		} \
		if(it->second->GetName() == name) \
		{ \
			return it->second; \
		} \
	} \
	return NULL;

#define DECLARE_PROPERTY() \
	public: \
		virtual void SetProperty(const std::string& strProperty, const QVariant& rData); \
		virtual QVariant GetProperty(const std::string& strProperty) const; \
		virtual CPropertySetObject& ReadPropertySet(); \
	protected: \
		CPropertySetObject m_PropertySetObject;

#define IMPLEMENT_PROPERTY(className) \
	void className::SetProperty(const std::string& strProperty, const QVariant& rData){\
		m_PropertySetObject.SetProperty(strProperty, rData); \
	}\
	QVariant className::GetProperty(const std::string& strProperty) const{\
		return m_PropertySetObject.GetProperty(strProperty); \
	}\
	CPropertySetObject& className::ReadPropertySet() {\
		return m_PropertySetObject;\
	}
//浮点数对比。
#define COMPARE_FLOAT(F1, F2) (fabs((F1) - (F2)) < 0.0000001)

//////////////////////////////////////////////////////////////////////////

#define _IMAX(VAR1,VAR2)(VAR1>VAR2?VAR1:VAR2)

#define _IMIN(VAR1,VAR2)(VAR1<VAR2?VAR1:VAR2)


#define BYTESWAP16(n) (((n&0xFF00)>>8)|((n&0x00FF)<<8))

#define BYTESWAP32(n) ((BYTESWAP16((n&0xFFFF0000)>>16))|((BYTESWAP16(n&0x0000FFFF))<<16))

#define BYTESWAP64(n) ((BYTESWAP32((n&0xFFFFFFFF00000000)>>32))|((BYTESWAP32(n&0x00000000FFFFFFFF))<<32))
#endif // PublicMarco_h__