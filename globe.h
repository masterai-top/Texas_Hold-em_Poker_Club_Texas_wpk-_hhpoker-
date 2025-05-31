#ifndef _GLOBE_H_
#define _GLOBE_H_

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>   
#include <sys/socket.h>  
#include "tup/Tars.h"
#include "servant/Application.h"
#include "util/tc_common.h"
#include "util/tc_logger.h"
#include "util/tc_encoder.h"
#include "wbl_smap.h"
#include "wbl/pthread_util.h"
#include <google/protobuf/text_format.h>

using namespace std;
using namespace tars;

namespace JFGame
{
    struct JFGameException : public std::runtime_error
    {
        JFGameException(const std::string &s): std::runtime_error(s)
        {

        }
    };


    //日志功能
#define LOG_DEBUG  LOG->debug() << "[" << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "]|"
#define LOG_WARN   LOG->warn()  << "[" << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "]|"
#define LOG_ERROR  LOG->error() << "[" << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "]|"
#define LOG_INFO   LOG->info()  << "[" << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "]|"

#define LOG_DAY(x) FDLOG(x) << __FUNCTION__ << "|"

    //日志白名单列表
    class JFGameLogWhiteUin
    {
    public:
        bool isWrite(int iuin)
        {
            wbl::ReadLocker lock(m_rwlock);

            if(m_setUinList.find(iuin) != m_setUinList.end())
            {
                return true;
            }

            return false;
        }

        bool reloadWriteList(string &sUinOriStr)
        {
            vector<string> vsuin = TC_Common::sepstr<string>(sUinOriStr, ",");

            wbl::WriteLocker lock(m_rwlock);

            m_setUinList.clear();
            for(size_t i  = 0; i < vsuin.size(); i++)
            {
                m_setUinList.insert(TC_Common::strto<int>(vsuin[i]));
            }

            return true;
        }

    private:
        std::set<int> m_setUinList;
        wbl::ReadWriteLocker m_rwlock;
    };

    typedef TC_Singleton<JFGameLogWhiteUin, CreateStatic,  DefaultLifetime> SingleJFGameLogWhiteUin;

#ifdef AUTO_ROUTER_TEST
#define NEW_LOG_DEBUG(uin) LOG->debug()<< "[" << uin << ":"  << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "]|"
#else
#define NEW_LOG_DEBUG(uin) \
        if(SingleJFGameLogWhiteUin::getInstance()->isWrite(uin))\
            LOG->error()<< "["<< uin << ":" << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << "]|"
#endif

    // 记录异常日志
#define EXPLOG(e) \
	FDLOG("exception") << ServerConfig::LocalIp << "|" << __FILE__ << "|" \
		<< __FUNCTION__ << "|" << __LINE__ << "|" << e << endl; \
	LOG_ERROR << e << endl;

    //过程控制
#define __PROC_BEGIN__ do{
#define __PROC_END__   }while(0);

    // 捕捉异常
#define __TRY__ try\
	{

    // 处理异常
#define __CATCH__ }\
	catch (const TarsDecodeException& e)\
	{\
		EXPLOG(string("catch tars decode exception: ") + e.what());\
	}\
	catch (const TarsEncodeException& e)\
	{\
		EXPLOG(string("catch tars encode exception: ") + e.what());\
	}\
	catch (const TarsProtoException& e)\
	{\
		EXPLOG(string("catch proto exception: ") + e.what());\
	}\
	catch (const TC_Exception& e)\
	{\
		EXPLOG(string("catch tc exception: ") + e.what());\
	}\
	catch (const JFGameException& e)\
	{\
		EXPLOG(string("catch jfgame exception: ") + e.what());\
	}\
	catch (const std::exception& e)\
	{\
		EXPLOG(string("catch std exception: ") + e.what());\
	}\
	catch (...)\
	{\
		EXPLOG("catch unknown exception");\
	}

#define I2S(i) TC_Common::tostr<int>(i)
#define U2S(i) TC_Common::tostr<unsigned>(i)
#define L2S(i) TC_Common::tostr<tars::Int64>(i)
#define S2I(s) TC_Common::strto<int>(s)
#define S2U(s) TC_Common::strto<unsigned>(s)
#define S2L(s) TC_Common::strto<tars::Int64>(s)
#define S2B(s) ((s == "Y" || s == "y") ? true : false)
#define B2S(b) (b ? "Y" : "N")
#define SEPSTR(s1, s2) TC_Common::sepstr<string>(s1, s2)
#define SEPSTR_T(s1, s2) TC_Common::sepstr<string>(s1, s2, true)
#define TRIM(s) TC_Common::trim(s)
#define BIN2S(s) TC_Common::bin2str(s)
#define S2BIN(s) TC_Common::str2bin(s)
#define G2U8(s) TC_Encoder::gbk2utf8(s)
#define U82G(s) TC_Encoder::utf82gbk(s)
#define STR2T(s1, s2) Str2Timet(s1, s2)
#define IP2INT(s) strIp2Int(s)
#define STRHEX2INT(s) strHex2Int(s)

    /*
    *通用模板方法：pb对象T序列化
     * @param t 类型T的对象
     * @return 字符串
    */
    template<typename T> 
    std::string pbToString(const T &t)
    {
        string s;
        t.SerializeToString(&s);
        return s;
    }

    template<typename T> 
    void pbTobuffer(const T &t, vector<char> &buffer)
    {
        string s;
        t.SerializeToString(&s);
        buffer.insert(buffer.begin(), s.begin(), s.end());
        return;
    }

    template<typename T> 
    vector<char> pbTobuffer(const T &t)
    {
        string s;
        t.SerializeToString(&s);
        vector<char> buffer;
        buffer.insert(buffer.begin(), s.begin(), s.end());
        return buffer;
    }

    /**
     * 通用模板方法: pb对象T结构化
     * @param t 字符串
     * @return 类型T的对象
     */
    template<typename T> 
    T &pbToObj(const std::string &s, T &t)
    {
        t.ParseFromString(s);
        return t;
    }

    template<typename T> 
    T pbToObj(const vector<char> &vc)
    {
        T t;
        string s;
        s.assign(vc.begin(), vc.end());
        t.ParseFromString(s);
        return t;
    }

    template<typename T> 
    T pbToObj(const vector<char> &vc, T &t)
    {
        //T t;
        string s;
        s.assign(vc.begin(), vc.end());
        t.ParseFromString(s);
        return t;
    }

    template<typename T> 
    std::string logPb(const T &t)
    {
        std::string str;
        google::protobuf::TextFormat::PrintToString(t, &str);
        return str;
    }

    template<typename T> 
    void logPb(const T &t, std::string *str)
    {
        google::protobuf::TextFormat::PrintToString(t, str);
    }

    /**
     * 通用模板方法: tars对象T序列化
     * @param t 类型T的对象
     * @return 字符串
     */
    template<typename T> 
    std::string tostring(const T &t)
    {
        string s;
        tars::TarsOutputStream<BufferWriter> osk;
        t.writeTo(osk);
        s.assign(osk.getBuffer(), osk.getLength());

        return s;
    }

    // 更通用一点
    template<typename T> 
    void tobuffer(const T &t, vector<char> &buffer)
    {
        tars::TarsOutputStream<BufferWriter> osk;
        t.writeTo(osk);
        buffer = osk.getByteBuffer();

        return;
    }

    /**
     * 通用模板方法: tars对象T结构化
     * @param t 字符串
     * @return 类型T的对象
     */
    template<typename T> 
    T &toObj(const std::string &s, T &t)
    {
        tars::TarsInputStream<BufferReader> isk;
        isk.setBuffer(s.c_str(), s.length());
        t.readFrom(isk);

        return t;
    }

    // 通用一点
    template<typename T> 
    T toObj(const vector<char> &vc)
    {
        T t;
        tars::TarsInputStream<BufferReader> isk;
        isk.setBuffer((const char *)(&vc[0]), vc.size());
        t.readFrom(isk);

        return t;
    }

    // 更通用一点
    template<typename T, typename B> 
    T &toObj(const B &vc, T &t)
    {
        tars::TarsInputStream<BufferReader> isk;
        isk.setBuffer((const char *)(&vc[0]), vc.size());
        t.readFrom(isk);

        return t;
    }

    /**
     * 通用模板方法: tars对象T结构化打印
     * @param t 类型T的对象
     * @return 字符串
     */
    template<typename T> 
    std::string printTars(const T &t)
    {
        ostringstream os;
        t.display(os);

        return os.str();
    }

    /**
     * 通用模板方法: tars对象容器结构化打印
     * @param itbegin  容器的iterator begin
     * @param itend  容器的iterator end
     * @return 字符串
     */
    template <typename I> 
    std::string printTars(I itbegin, I itend)
    {
        std::ostringstream s;
        I it = itbegin;
        while(it != itend)
        {
            it->display(s);
            ++it;
        }

        return s.str();
    }

    /**
     * 通用模板方法: tars对象T单行日志化打印
     * @param t 类型T的对象
     * @return 字符串
     */
    template<typename T> 
    std::string logTars(const T &t)
    {
        ostringstream os;
        t.displaySimple(os);

        return os.str();
    }

    /**
     * 通用模板方法: Tars对象T单行日志化打印
     * @param t 类型T的对象
     * @param sep 替换分隔符
     * @return 字符串
     */
    template<typename T> 
    std::string logTars(const T &t, const string &sep)
    {
        ostringstream os;
        t.displaySimple(os);

        return TC_Common::replace(os.str(), "|", sep.c_str());
    }

    /**
     * 通用模板方法: tars对象容器日志化打印
     * @param itbegin  容器的iterator begin
     * @param itend  容器的iterator end
     * @return 字符串
     */
    template <typename I>
    std::string logTars(I itbegin, I itend)
    {
        std::ostringstream s;
        I it = itbegin;
        while(it != itend)
        {
            it->displaySimple(s);
            s << ";";
            ++it;
        }

        return s.str();
    }

    inline string tostring(const map<string, string> &mp, const string &sKVSep = ":", const string &sSep = "|")
    {
        ostringstream os;
        map<string, string>::const_iterator it;
        for(it = mp.begin(); it != mp.end();)
        {
            if (it->first == "misc_msg_id")
            {
                os << it->first << sKVSep << BIN2S(it->second);
            }
            else
            {
                os << it->first << sKVSep << it->second;
            }

            if (++it != mp.end())
            {
                os << sSep;
            }
        }

        return os.str();
    }

    inline void U2N(char *buffer, unsigned u)
    {
        unsigned tmp = htonl(u);
        memcpy(buffer, &tmp, sizeof(uint32_t));
    }

    inline unsigned N2U(const char *buffer)
    {
        return ntohl(*(uint32_t *)buffer);
    }

    inline void US2N(char *buffer, unsigned short us)
    {
        unsigned short tmp = htons(us);
        memcpy(buffer, &tmp, sizeof(uint16_t));
    }

    inline unsigned short N2US(const char *buffer)
    {
        return ntohs(*(uint16_t *)buffer);
    }

    inline unsigned int strIp2Int(const string &sIp)
    {
        struct in_addr inaddr;
        inet_aton(sIp.c_str(), &inaddr);
        return inaddr.s_addr;
    }

    inline unsigned int strHex2Int(string hex)
    {
        return strtoll(hex.c_str(), NULL, 16);
    }

    //根据错误码返回概要描述，使记录更直观
    inline string GetReturnStatus(int iECode)
    {
        if ( 0 == iECode )
        {
            return "OK: ";
        }
        else if ( iECode > 0 )
        {
            return "FAILED: ";
        }
        else
        {
            return "ERROR: ";
        }
    }

    inline int EncodeShort(char *pOutBuff, unsigned short usValue)
    {
        if( pOutBuff == NULL )
        {
            return 0;
        }

        //
        pOutBuff[0] = (char)((usValue & 0xFF00 ) >> 8);
        pOutBuff[1] = (char)(usValue & 0xFF);

        return 2;
    }

    template<typename K, typename V, typename D, typename A>
    void getMapValue(const map<K, V, D, A> &m, const K &key, V &value)
    {
        typename map<K, V, D, A>::const_iterator it = m.find(key);
        if (it != m.end())
        {
            value = it->second;
        }

        return;
    }

    inline void trim(vector<string> &v)
    {
        for (size_t i = 0; i < v.size(); ++i)
        {
            v[i] = TRIM(v[i]);
        }
    }

    //域名解析
    inline void getIp(char *domain, char *ip)
    {
        if(domain == NULL || ip == NULL)
        {
            return;
        }

        if(strlen(domain) == 0)
        {
            return;
        }

        //
        struct hostent host = *gethostbyname(domain);
        for(int i = 0; host.h_addr_list[i]; i++)
        {
            strcpy(ip, inet_ntoa(*(struct in_addr *)host.h_addr_list[i]));

            break;
        }
    }

    //域名解析
    inline string getIpStr(const string &domain)
    {
        if(domain.length() == 0)
        {
            return "";
        }

        //
        struct hostent host = *gethostbyname(domain.c_str());
        for(int i = 0; host.h_addr_list[i]; i++)
        {
            string ip = inet_ntoa(*(struct in_addr *)host.h_addr_list[i]);

            return ip;
        }

        return "";
    }

};

using namespace JFGame;

#endif


