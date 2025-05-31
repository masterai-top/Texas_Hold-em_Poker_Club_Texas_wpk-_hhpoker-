
#ifndef  __ITABLEGAME_H__
#define  __ITABLEGAME_H__

#include "RoomSoProto.h"
#include "util/tc_logger.h"
#include "/usr/local/tars/cpp/include/servant/TarsLogger.h"

using namespace std;

class IGame
{
public:
    virtual ~IGame() {};

public:
    /**
    * Table -> Game 的通知事件
    */
    //游戏消息数据到达
    virtual int onGameSoMessage(const long lPlayerID, const vector<char> &vecMsgData) = 0;
    //给SO数据
    virtual long toSoData(const RoomSo::E_ROOM_TO_SO eMsgType, void *p) = 0;
};

class ITable
{
public:
    ITable()
    {

    }

    virtual ~ITable()
    {

    }

public:
    /**
    * Game -> Table 的动作请求
    */
    //向该桌玩家发送游戏数据 @到达客户端不保证先后顺序
    virtual int doSendGameData(const long lPlayerID, const vector<char> &vecMsgData) = 0;
    //向该桌玩家发送游戏数据 @到达客户端不保证先后顺序
    virtual int doSendGameData(const long lPlayerID, const vector<vector<char> > &vecMsgData) = 0;
    //向该桌所有玩家发送游戏数据 @到达客户端不保证先后顺序
    virtual int doSendAllGameData(const vector<char> &vecMsgData) = 0;
    //向该桌所有玩家发送游戏数据 @到达客户端不保证先后顺序
    virtual int doSendAllGameData(const vector<vector<char> > &vecMsgData) = 0;
    //给房间数据
    virtual int toRoomData(const RoomSo::E_SO_TO_ROOM eMsgType, void *p) = 0;
    //向该桌所有观看玩家发送游戏数据 @到达客户端不保证先后顺序
    virtual int doSendWatchGameData(const vector<char> &vecMsgData) = 0;
    //向该桌所有观看玩家发送游戏数据 @到达客户端不保证先后顺序
    virtual int doSendWatchGameData(const vector<vector<char> > &vecMsgData) = 0;
    //请求AI操作
    virtual int doAIGameDeing(const long lPlayerID, const int round, const std::vector<char> &vesMsgData) = 0;
    //请求AI操作
    virtual int doAIGameAction(const long lPlayerID, const unsigned int actionType, const unsigned int jetton) = 0;
    //请求AI操作
    virtual int doAIGameBanker(const long lPlayerID, const unsigned int chairId) = 0;
    //请求AI操作
    virtual int doAIGameBetToken(const long lPlayerID) = 0;
    //请求AI操作
    virtual int doAIGameBetList(const long lPlayerID) = 0;
};

struct GameInitParam
{
    std::string strRoomTableId;
    ITable *pTable;
    std::string strConfigParam;
    std::string strConfigPath;
    int iTableType; //0-默认类型，1-人工智能桌子
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 创建游戏的入口
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C" {
    typedef tars::TarsRollLogger *(*GetInsFunc)();
    typedef void (*SetGetInsFunc) (GetInsFunc pFunc);
    typedef IGame *(*CreateGameProc)(int iInitMode, const char *szConfigPath, const char *szConfigParam, ITable *pTable);

#ifndef __SO_TICK_TOCK_FUNC__
#define __SO_TICK_TOCK_FUNC__
    typedef int (*soTickTockFunc)();
    typedef soTickTockFunc (*getSoTickTockFunc)();
#endif

    //via puremvc version
    typedef IGame *(*CreateGameFunc)(int iInitMode, const GameInitParam &gameInitParam);

    typedef tars::TarsRollLogger *(*GetRollLoggerFunc)();
    typedef tars::TarsTimeLogger::TimeLogger *(*GetTimeLoggerFunc)(const string &sFile);
    typedef void (*initLoggerGetterFunc) (GetRollLoggerFunc pRollLog, GetTimeLoggerFunc pTimeLog);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SO版本查询处理
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#if __pic__ || __PIC__

#if __MAKE_GAME_SO__
extern "C"
#if __x86_64__
const char __invoke_dynamic_linker__[] __attribute__((section(".interp"))) = "/lib64/ld-linux-x86-64.so.2";
#else
const char __invoke_dynamic_linker__[] __attribute__((section(".interp"))) = "/lib/ld-linux.so.2";
#endif

#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 游戏配置结构
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagGameCfgItem
{
    short nGameID;				//游戏类型编号
    short nMaxMember;			//游戏最大参与人数
    int   iGameVer;				//游戏的版本号

    std::string sSoFileName;	//游戏对象的动态链结库的全路径名
    CreateGameProc pfnInitGame;	//创建游戏的函数

    void *pGameHandle;			//游戏动态库的指针
} TGameCfgItem;

#endif


