#ifndef IPlugin_h__
#define IPlugin_h__


class IPlugin
{
public:
	virtual ~IPlugin(){}
	/**
	 * @fn       GetName
	 * @author   Crack
	 * @brief    获取插件名称
	 * @date     2021/4/11 22:10
	 * @param    
	 * @return   
	*/
	virtual const char* GetName() const  = 0;

	virtual const char* GetCompany() const  = 0;

	virtual const char* GetVersion() const  = 0;

	virtual const char* GetDescription() const = 0;

	virtual bool CanUnintall() = 0;

	virtual bool Initialize(const char*strModulePath, const char* strDuty) = 0;

	virtual bool Destroy() = 0;
};

#endif // IPlugin_h__
