#ifndef _DYNC_OBJECT_H_
#define _DYNC_OBJECT_H_

#include <iostream>
#include <assert.h>
#include <string>
#include "util/tc_autoptr.h"
#include "util/tc_thread_mutex.h"
#include "servant/Application.h"

using namespace std;
using namespace tars;

/**
 * 精简版动态创建:
 *
 */
//.虚类声明
#define DECLARE_CMDDYNAMIC(class_name) \
public: \
	static RuntimeObject class##class_name;

//.虚类定义
#define IMPLEMENT_CMDDYNAMIC(class_name) \
	RuntimeObject class_name::class##class_name = {#class_name, NULL, NULL}; \
	ObjectList __initcmd_##class_name(&class_name::class##class_name);

//.实类声明
#define DECLARE_CMDDYNCREATE(class_name) \
public: \
	static RuntimeObject class##class_name; \
	static DyncObject* CreateObject();

//.实类定义
#define IMPLEMENT_CMDDYNCREATE(class_name) \
	RuntimeObject class_name::class##class_name = { #class_name, class_name::CreateObject, NULL}; \
	ObjectList __initcmd_##class_name(&class_name::class##class_name); \
	DyncObject* class_name::CreateObject(void) { return new class_name; }

struct DyncObject;

/**
 * 执行期对象结构
 */
struct RuntimeObject
{
	const char* m_szClassName;

	DyncObject* (*m_pfnCreateObject)(void);

	RuntimeObject* m_pNextClass;
};

/**
 * 执行期对象
 */
struct DyncObject : public tars::TC_HandleBase
				  , public TC_ThreadMutex
{
public:
	static RuntimeObject classDyncObject;

	void setName(const string& name) { sThisName = name; }

	const std::string& name() const { return sThisName; }

private:
	std::string sThisName;
};
typedef tars::TC_AutoPtr<DyncObject> DyncObjectPtr;

/**
 * 构建执行期对象全局链
 */
struct ObjectList
{
	ObjectList(RuntimeObject *pNewClass);
};

/**
 * 执行期对象创建接口
 */
DyncObject* TCreateObject(const char* class_name);
/////////////////////////////////////////////////////////
#endif

