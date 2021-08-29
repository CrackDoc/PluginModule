#ifndef PluginModule_h__
#define PluginModule_h__


#include "IPluginModule.h"
#include "PublicMarco.h"

typedef IPlugin* (*FuncCreatePlugin)();
typedef void (*FuncDestroyPlugin)();

/**
* @class CPluginInfo
* @brief �����Ϣ[�ṹ��]
* @note  ����������Ϣ
*/
class  PluginModule_EXPORT CPluginModule : public IPluginModule
{
public:
	CPluginModule();
	~CPluginModule();
	/**
 * @fn       GetName
 * @author   Crack
 * @brief    ��ȡģ������
 * @date     2021/4/11 22:10
 * @param
 * @return
*/
	virtual const char* GetName() const;

	virtual const char* GetCompany() const;

	virtual const char* GetVersion() const;

	virtual const char* GetDescription() const;

	virtual bool Initialise();

	virtual bool Uninitialise();
public:
	/**
	 * @fn  LoadPlugin
	 * @brief ͨ�������,���·��,���ͼ�����Ϣ,���ز��
	 * @param const std::string & strName:		�����
	 * @param const std::string & strDllPath:	���·��
	 * @param const std::string & strIconPath:	ͼ��·��
	 * @param const std::string & strDuty:		ְ����Ϣ,Ĭ��Ϊsystem,[�ջ���'system'����Ϊ������ְ��system]
	 * @return bool �����Ƿ�ɹ�
	 */
	virtual bool LoadPlugin(const char* strName,
		const char* strDllPath,
		const char* strIconPath = "",
		const char* strDuty = "system");

	/**
	 * @fn  LoadPluginsFromFile
	 * @brief �������ļ����ز��
	 * @param const std::string & strPath: �����ļ�·��
	 * @return bool �����Ƿ�ɹ�
	 */
	bool LoadPluginsFromFile(const char* strPath, const char* strDuty);

	/**
	 * @fn  UnloadPlugin  [����]
	 * @brief   ͨ�������(����������IPlugin::GetName())ж�ز��
	 * @param const std::string & strName: �����
	 * @param bool bForceUnload: ǿ��ж��
	 * @return bool
	 */
	bool UnloadPlugin(const char* strName, bool bForceUnload = false);

	/**
	 * @fn  UnloadPlugin
	 * @brief   ͨ�����,ж�ز��
	 * @param IPlugin * pPlugin: ���ָ��
	 * @param bool bForceUnload: ǿ��ж��
	 * @return bool
	 */
	bool UnloadPlugin(IPlugin* pPlugin, bool bForceUnload = false);

	/**
	 * @fn  UnloadPluginByDllPath
	 * @brief   ͨ�����·��ж�ز��(����ƥ��·��)
	 * @param const std::string & strDllPath: �������·��,����Ͳ�������ļ���һ��
	 * @param bool bForceUnload: ǿ��ж��
	 * @return bool �����Ƿ�ɹ�
	 */
	bool UnloadPluginByDllPath(const char* strDllPath, bool bForceUnload = false);

	/**
	 * @fn  UnloadPluginsFromFile
	 * @brief ͨ�������ļ�ж�ز��
	 * @param const std::string & strPath: �����ļ�·��
	 * @param bool bForceUnload: ǿ��ж��
	 * @return void
	 */
	void UnloadPluginsFromFile(const char* strPath, bool bForceUnload = false);

	/**
	 * @fn  UnloadPluginsByDuty
	 * @brief ���ղ��ְ��ж�ز��
	 * @param const std::string & strDuty: ְ��
	 * @param bool bForceUnload: ǿ��ж��
	 * @return void
	 */
	bool UnloadPluginsByDuty(const char* strDuty, bool bForceUnload = false);

	/**
	 * @fn  UnloadAllPlugins
	 * @brief ж�����в��
	 * @return void
	 */
	void UnloadAllPlugins();

	/**
	 * @fn  RegisterObserver
	 * @brief ע����ģ��Ĺ۲���,���Դ��������غ�ж�ص��¼�֪ͨ
	 * @param CPluginModuleObserver * pObserver: ���ģ��۲���
	 * @return bool �����Ƿ�ע��ɹ�
	 */
	bool RegisterObserver(CPluginModuleObserver* pObserver);

	/**
	 * @fn  UnregisterObserver
	 * @brief ȡ�����ģ��Ĺ۲���ע��,
	 * @param CPluginModuleObserver * pObserver: ��Ҫȡ��ע��Ĺ۲���
	 * @return void
	 */
	void UnregisterObserver(CPluginModuleObserver* pObserver);

	/**
	 * @fn  Travel
	 * @brief �������в��
	 * @param PluginInfoVisitor & rVisitor:
	 * @return void
	 */
	void Travel(PluginInfoVisitor& rVisitor);

	/**
	 * @fn  IsPluginLoaded
	 * @brief ����ĳ������Ƿ����
	 * @param const std::string& strPluginName:
	 * @return bool
	 */
	bool IsPluginLoaded(const char* strPluginName);

protected:
	/**
	 * @fn  NotifyPluginLoad
	 * @brief ֪ͨ���ע��
	 * @param const std::string & strPluginPath:
	 * @param IPlugin * pPlugin:
	 * @return void
	 */
	void NotifyPluginLoad(const char* strModulePath, const char* strPluginPath, IPlugin* pPlugin, const char* strDuty);

	/**
	 * @fn  NotifyPluginUnload
	 * @brief ֪ͨ���ж��
	 * @param IPlugin * pPlugin:
	 * @return void
	 */
	void NotifyPluginUnload(IPlugin* pPlugin, const char* strDuty);

protected:
	//CThreadMutex m_PluginsInfoLock;//�����Ϣ��
	void* m_mapPluginInfos;
	//std::map<const std::string, CPluginInfo> m_mapPluginInfos;	//�����Ϣ����

	//CThreadMutex m_ObserverLock;//���ģ��۲�����
	void* m_lstObserver;
	//std::list<CPluginModuleObserver*> m_lstObserver;			//���ģ��۲�������
};
DECLARE_ACMODULE(Plugin, PluginModule_EXPORT)
#endif // PluginModule_h__