#ifndef __NGX_CONF_H__
#define __NGX_CONF_H__

#include <vector>

#include "ngx_global.h"

class CConfig
{
private:
    CConfig();
public:
    ~CConfig();
private:
    static CConfig *m_instance;

public:
    static CConfig* GetInstance()
    {
        if(nullptr == m_instance)
        {
            if(nullptr == m_instance)
            {
                m_instance = new CConfig();
                static CGarhuishou cl;
            }
        }
        return m_instance;
    }
    class CGarhuishou
    {
    public:
        ~CGarhuishou()
        {
            if(CConfig::m_instance)
            {
                delete CConfig::m_instance;
                CConfig::m_instance = nullptr;
            }
        }
    };

public:
    //装载配置文件
    bool Load(const char *pconfName);
	const char *GetString(const char *p_itemname);
	int  GetIntDefault(const char *p_itemname,const int def);

public:
	std::vector<LPCConfItem> m_ConfigItemList;
};
#endif