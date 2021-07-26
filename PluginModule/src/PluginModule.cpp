
#include "PluginModule.h"
#include "Library.h"
#include "StlUtil.h"
#include "XDir.h"
#include "XFile.h"
#include "IPlugin.h"
#include <map>
#include <list>
#include <vector>

CPluginModule::CPluginModule(void)
	:m_mapPluginInfos(new std::map<std::string, CPluginInfo*>())
	, m_lstObserver(new std::list<CPluginModuleObserver*>())
{

}

CPluginModule::~CPluginModule(void)
{
	// 析构函数再释放资源
}
std::string CPluginModule::GetName() const
{
	return "";
}

std::string CPluginModule::GetCompany() const
{
	return "";
}

std::string CPluginModule::GetVersion() const
{
	return "";
}

std::string CPluginModule::GetDescription() const
{
	return "";
}

bool CPluginModule::Initialise()
{
	return true;
}

bool CPluginModule::Uninitialise()
{
	return true;
}
bool CPluginModule::LoadPlugin(const char* strName, const char* strDllPath, const char* strIconPath /*= ""*/, const char* strDuty /*= "system"*/)
{
	// 然后根据strDllPath的路径获取到相应的dll
	CLibrary* pCLibrary = new CLibrary(strDllPath);
	if (pCLibrary->Load())
	{
		CPluginInfo* PluginInfo = new CPluginInfo;
		strcat_s(PluginInfo->m_strDuty, strDuty);
		strcat_s(PluginInfo->m_strBelong, strName);
		PluginInfo->m_pLibrary = pCLibrary;

		//加载插件
		FuncCreatePlugin CreateFunc = (FuncCreatePlugin)pCLibrary->Resolve("CreatePlugin");
		IPlugin* pIPlugin = CreateFunc();
		PluginInfo->m_pPlugin = pIPlugin;

		//m_PluginsInfoLock.Acquire();
		std::map<std::string, CPluginInfo*>& mapPluginInfos = *static_cast<std::map<std::string, CPluginInfo*>*>(m_mapPluginInfos);

		std::map<std::string, CPluginInfo*>::iterator it = mapPluginInfos.find(strName);
		if (it != mapPluginInfos.end())
		{
			return false;
		}
		mapPluginInfos.insert(std::map<const std::string, CPluginInfo*>::value_type(strName, PluginInfo));
		//m_PluginsInfoLock.Release();
		return true;
	}
	else
	{
		delete pCLibrary;
		pCLibrary = NULL;
	}
	return false;
}

bool CPluginModule::LoadPluginsFromFile(const char* strPath, const char* strDuty)
{
	IOx::XDir xDir(strPath);

	std::vector<IOx::XFile> rLstFile;
	std::string strSuffix = "*.dll";

#ifdef _WIN32
#elif defined __linux__
	strSuffix = "*.so";
#elif defined VXWORKS
#endif // _WIN32

	xDir.getAllFiles(&rLstFile, strSuffix.c_str());

	if (rLstFile.empty())
	{
		return false;
	}
	std::vector<IOx::XFile>::iterator it0 = rLstFile.begin();
	for (; it0 != rLstFile.end(); ++it0)
	{
		IOx::XFile& file = *it0;

		std::string strPluginName = file.baseName();

		LoadPlugin(strPluginName.c_str(), file.absolutePath(), "", strDuty);

	}
	return true;
}

bool CPluginModule::UnloadPlugin(const char* strName, bool bForceUnload /*= false*/)
{
	//m_PluginsInfoLock.Acquire();

	std::map<std::string, CPluginInfo*>& mapPluginInfos = *static_cast<std::map<std::string, CPluginInfo*>*>(m_mapPluginInfos);

	std::map<const std::string, CPluginInfo*>::iterator it = mapPluginInfos.find(strName);

	if (it != mapPluginInfos.end())
	{
		CPluginInfo* cPluginInfo = it->second;

		IPlugin* pIPlugin = cPluginInfo->m_pPlugin;

		if (pIPlugin == NULL)
		{
			//m_PluginsInfoLock.Release();
			return false;
		}
		CLibrary* pCLibrary = cPluginInfo->m_pLibrary;

		if (pCLibrary == NULL)
		{
			//m_PluginsInfoLock.Release();
			return false;
		}


		if (bForceUnload || (!bForceUnload && pIPlugin->CanUnintall()))
		{
			// 卸载插件
			FuncDestroyPlugin DestroyPluginFunc = (FuncDestroyPlugin)pCLibrary->Resolve("DestroyPlugin");
			DestroyPluginFunc();

			delete pCLibrary;
			pCLibrary = nullptr;
			delete cPluginInfo;
			cPluginInfo = nullptr;
			mapPluginInfos.erase(it);
			//m_PluginsInfoLock.Release();
			return true;
		}
	}
	//m_PluginsInfoLock.Release();
	return false;
}

bool CPluginModule::UnloadPlugin(IPlugin* pPlugin, bool bForceUnload /*= false*/)
{
	return UnloadPlugin(pPlugin->GetName(), bForceUnload);
}

bool CPluginModule::UnloadPluginByDllPath(const char* strDllPath, bool bForceUnload /*= false*/)
{
	IOx::XFile xFile = strDllPath;
	return UnloadPlugin(xFile.baseName(), bForceUnload);
}
//linux 动态库 .so  静态库.a

void CPluginModule::UnloadPluginsFromFile(const char* strPath, bool bForceUnload /*= false*/)
{
	IOx::XDir xDir(strPath);
	std::vector<IOx::XFile> rLstFile;
	std::string strSuffix = "*.dll";

#ifdef _WIN32
#elif defined __linux__
	strSuffix = "*.so";
#elif defined VXWORKS
#endif // _WIN32

	xDir.getAllFiles(&rLstFile, strSuffix.c_str());

	if (rLstFile.empty())
	{
		return;
	}
	std::vector<IOx::XFile>::iterator it0 = rLstFile.begin();
	for (; it0 != rLstFile.end(); ++it0)
	{
		IOx::XFile& file = *it0;
		UnloadPlugin(file.baseName(), bForceUnload);
	}
}

bool CPluginModule::UnloadPluginsByDuty(const char* strDuty, bool bForceUnload /*= false*/)
{
	std::map<std::string, CPluginInfo*>& mapPluginInfos = *static_cast<std::map<std::string, CPluginInfo*>*>(m_mapPluginInfos);

	std::map<const std::string, CPluginInfo*>::iterator it = mapPluginInfos.begin();

	bool IsExsit = false;
	for (; it != mapPluginInfos.end(); ++it)
	{
		CPluginInfo* cPluginInfo = it->second;

		std::string strPluginDuty = cPluginInfo->m_strDuty;
		if (strcmp(strPluginDuty.c_str(), strDuty) == 0)
		{
			IPlugin* pIPlugin = cPluginInfo->m_pPlugin;
			if (pIPlugin == NULL)
			{
				continue;
			}
			std::string strName = pIPlugin->GetName();
			IsExsit = UnloadPlugin(strName.c_str(), bForceUnload);
		}
	}
	return IsExsit;
}

void CPluginModule::UnloadAllPlugins()
{
	std::map<std::string, CPluginInfo*>& mapPluginInfos = *static_cast<std::map<std::string, CPluginInfo*>*>(m_mapPluginInfos);

	std::map<const std::string, CPluginInfo*>::iterator it = mapPluginInfos.begin();

	for (; it != mapPluginInfos.end(); ++it)
	{
		CPluginInfo* cPluginInfo = it->second;

		IPlugin* pIPlugin = cPluginInfo->m_pPlugin;
		if (pIPlugin == NULL)
		{
			continue;
		}
		std::string strName = pIPlugin->GetName();

		UnloadPlugin(strName.c_str(), true);
	}
}

bool CPluginModule::RegisterObserver(CPluginModuleObserver* pObserver)
{
	//m_ObserverLock.Acquire();
	if (pObserver != NULL)
	{
		std::list<CPluginModuleObserver*>& lstObservers = *static_cast<std::list<CPluginModuleObserver*>*>(m_lstObserver);
		lstObservers.push_back(pObserver);

		//m_ObserverLock.Release();
		return true;
	}
	//m_ObserverLock.Release();
	return false;
}

void CPluginModule::UnregisterObserver(CPluginModuleObserver* pObserver)
{
	//m_ObserverLock.Acquire();
	std::list<CPluginModuleObserver*>& lstObservers = *static_cast<std::list<CPluginModuleObserver*>*>(m_lstObserver);

	std::list<CPluginModuleObserver*>::iterator it0 = lstObservers.begin();

	for (; it0 != lstObservers.end(); ++it0)
	{
		CPluginModuleObserver* observer = *it0;
		if (observer == pObserver)
		{
			lstObservers.erase(it0);
			if (observer != NULL)
			{
				delete observer;
				observer = NULL;
			}
			break;
		}
	}
	//m_ObserverLock.Release();
}

void CPluginModule::Travel(PluginInfoVisitor& rVisitor)
{
	std::map<std::string, CPluginInfo*>& mapPluginInfos = *static_cast<std::map<std::string, CPluginInfo*>*>(m_mapPluginInfos);

	std::map<const std::string, CPluginInfo*>::iterator it = mapPluginInfos.begin();

	for (; it != mapPluginInfos.end(); ++it)
	{
		CPluginInfo* cPluginInfo = it->second;

		std::string strName = it->first;

		rVisitor.Visit(strName.c_str(), cPluginInfo);
	}
}

bool CPluginModule::IsPluginLoaded(const char* strPluginName)
{
	std::map<std::string, CPluginInfo*>& mapPluginInfos = *static_cast<std::map<std::string, CPluginInfo*>*>(m_mapPluginInfos);

	std::map<std::string, CPluginInfo*>::iterator it = mapPluginInfos.find(strPluginName);

	if (it != mapPluginInfos.end())
	{
		return false;
	}
	return true;
}

void CPluginModule::NotifyPluginLoad(const char* strModulePath, const char* strPluginPath, IPlugin* pPlugin, const char* strDuty)
{
	//m_ObserverLock.Acquire();
	std::list<CPluginModuleObserver*>& lstObservers = *static_cast<std::list<CPluginModuleObserver*>*>(m_lstObserver);

	std::list<CPluginModuleObserver*>::iterator it0 = lstObservers.begin();

	for (; it0 != lstObservers.end(); ++it0)
	{
		CPluginModuleObserver* observer = *it0;
		if (observer != NULL)
		{
			observer->OnPluginLoad(strModulePath, strPluginPath, pPlugin, strDuty);
		}
	}
	//m_ObserverLock.Release();
}

void CPluginModule::NotifyPluginUnload(IPlugin* pPlugin, const char* strDuty)
{
	//m_ObserverLock.Acquire();
	std::list<CPluginModuleObserver*>& lstObservers = *static_cast<std::list<CPluginModuleObserver*>*>(m_lstObserver);

	std::list<CPluginModuleObserver*>::iterator it0 = lstObservers.begin();

	for (; it0 != lstObservers.end(); ++it0)
	{
		CPluginModuleObserver* observer = *it0;
		if (observer != NULL)
		{
			observer->OnPluginUnload(pPlugin, strDuty);
		}
	}
	//m_ObserverLock.Release();
}
IMPLEMENT_ACMODULE(Plugin)