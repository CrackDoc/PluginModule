#ifndef IPluginModule_h__
#define IPluginModule_h__
#include "PluginModuleExport.h"

class IPlugin;
class CLibrary;

// 定义一个结构体 PluginInfo 
class PluginModule_EXPORT CPluginInfo
{
public:
	CPluginInfo():
		  m_pPlugin(nullptr)
		, m_pLibrary(nullptr)
	{
		for (int i = 0; i < 128; ++i)
		{
			m_strBelong[i] = '\0';
			m_strDuty[i] = '\0';
		}
	}

	~CPluginInfo()
	{

	}

public:
	IPlugin* m_pPlugin;			//插件实体
	CLibrary* m_pLibrary;		//插件DLL信息
	char m_strBelong[128];	    //插件归属
	char m_strDuty[128];		//插件职责
};
/**
* @class CPluginModuleObserver
* @brief 插件模块观察者
* @note  
*/
class PluginModule_EXPORT CPluginModuleObserver
{
public:
	virtual ~CPluginModuleObserver(){}

	virtual bool OnPluginLoad(const char* strModulePath, const char* strConfigPluginPath, IPlugin* pPlugin, const char* strDuty) = 0;

	virtual bool OnPluginUnload(IPlugin* pPlugin, const char* strDuty) = 0;
};
class PluginModule_EXPORT PluginInfoVisitor
{
public:
	virtual ~PluginInfoVisitor(){}

	/**
		* @fn  Visit			
		* @brief 访问器接口  
		* @param const std::string & strName: 被访问插件名
		* @param const CPluginInfo & rPluginInfo: 被访问插件信息
		* @return void
		*/
	virtual void Visit(const char* strName,CPluginInfo *pluginInfo)  = 0;
};

class  PluginModule_EXPORT IPluginModule
{
public:
	virtual ~IPluginModule(){}
	/**
	 * @fn       GetName
	 * @author   Crack
	 * @brief    获取模块名称
	 * @date     2021/4/11 22:10
	 * @param    
	 * @return   
	*/
	virtual const char* GetName() const  = 0;

	virtual const char* GetCompany() const  = 0;

	virtual const char* GetVersion() const  = 0;

	virtual const char* GetDescription() const = 0;
	
	virtual bool Initialise() = 0;
	
	virtual bool Uninitialise() = 0;
	
public:
/**
	 * @fn  LoadPlugin        
	 * @brief 通过插件名,插件路径,插件图标等信息,加载插件  
	 * @param const std::string & strName:		插件名
	 * @param const std::string & strDllPath:	插件路径
	 * @param const std::string & strIconPath:	图标路径
	 * @param const std::string & strDuty:		职责信息,默认为system,[空或者'system'均认为是隐含职责system]
	 * @return bool 返回是否成功
	 */
	virtual bool LoadPlugin(const char* strName, 
			const char *strDllPath, 
			const char* strIconPath = "", 
			const char* strDuty = "system") = 0;

	/**
	 * @fn  LoadPluginsFromFile        
	 * @brief 从配置文件加载插件  
	 * @param const std::string & strPath: 配置文件路径
	 * @return bool 返回是否成功
	 */
	virtual bool LoadPluginsFromFile(const char* strPath, const char* strDuty) = 0;

	/**
	 * @fn  UnloadPlugin  [废弃]      
	 * @brief   通过插件名(该名字来自IPlugin::GetName())卸载插件
	 * @param const std::string & strName: 插件名
	 * @param bool bForceUnload: 强制卸载
	 * @return bool 
	 */
	virtual bool UnloadPlugin(const char* strName, bool bForceUnload = false) = 0;

	/**
	 * @fn  UnloadPlugin        
	 * @brief   通过插件,卸载插件
	 * @param IPlugin * pPlugin: 插件指针
	 * @param bool bForceUnload: 强制卸载
	 * @return bool
	 */
	virtual bool UnloadPlugin(IPlugin* pPlugin, bool bForceUnload = false) = 0;

	/**
	 * @fn  UnloadPluginByDllPath        
	 * @brief   通过插件路劲卸载插件(完整匹配路径)
	 * @param const std::string & strDllPath: 插件完整路径,必须和插件配置文件中一致
	 * @param bool bForceUnload: 强制卸载
	 * @return bool 返回是否成功
	 */
	virtual bool UnloadPluginByDllPath(const char* strDllPath, bool bForceUnload = false) = 0;

	/**
	 * @fn  UnloadPluginsFromFile        
	 * @brief 通过配置文件卸载插件  
	 * @param const std::string & strPath: 配置文件路径
	 * @param bool bForceUnload: 强制卸载
	 * @return void
	 */
	virtual void UnloadPluginsFromFile(const char* strPath, bool bForceUnload = false) = 0;

	/**
	 * @fn  UnloadPluginsByDuty        
	 * @brief 按照插件职责卸载插件  
	 * @param const std::string & strDuty: 职责
	 * @param bool bForceUnload: 强制卸载
	 * @return void
	 */
	virtual bool UnloadPluginsByDuty(const char* strDuty, bool bForceUnload = false) = 0;

	/**
	 * @fn  UnloadAllPlugins        
	 * @brief 卸载所有插件  
	 * @return void
	 */
	virtual void UnloadAllPlugins() = 0;

	/**
	 * @fn  RegisterObserver        
	 * @brief 注册插件模块的观察者,可以处理插件加载和卸载的事件通知  
	 * @param CPluginModuleObserver * pObserver: 插件模块观察者
	 * @return bool 返回是否注册成功
	 */
	virtual bool RegisterObserver(CPluginModuleObserver* pObserver) = 0;

	/**
	 * @fn  UnregisterObserver        
	 * @brief 取消插件模块的观察者注册,  
	 * @param CPluginModuleObserver * pObserver: 需要取消注册的观察者
	 * @return void
	 */
	virtual void UnregisterObserver(CPluginModuleObserver* pObserver) = 0;

	/**
	 * @fn  Travel        
	 * @brief 遍历所有插件  
	 * @param PluginInfoVisitor & rVisitor: 
	 * @return void
	 */
	virtual void Travel(PluginInfoVisitor& rVisitor) = 0;

	/**
	 * @fn  IsPluginLoaded        
	 * @brief 返回某个插件是否加载
	 * @param const std::string& strPluginName: 
	 * @return bool
	 */
	virtual bool IsPluginLoaded(const char* strPluginName) = 0;

public:
	/**
	 * @fn  NotifyPluginLoad        
	 * @brief 通知插件注册  
	 * @param const std::string & strPluginPath: 
	 * @param IPlugin * pPlugin: 
	 * @return void
	 */
	virtual void NotifyPluginLoad(const char*strModulePath, const char* strPluginPath, IPlugin* pPlugin, const char* strDuty) = 0;

	/**
	 * @fn  NotifyPluginUnload        
	 * @brief 通知插件卸载  
	 * @param IPlugin * pPlugin: 
	 * @return void
	 */
	virtual void NotifyPluginUnload(IPlugin* pPlugin, const char* strDuty) = 0;
	
};
#endif // IPluginModule_h__