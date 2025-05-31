#include "DyncObject.h"
#include <iostream>

using namespace std;

//执行期对象全局链表初始节点
RuntimeObject DyncObject::classDyncObject = {"DyncObject", NULL, NULL};

static RuntimeObject* __pFirstCmdClass = &DyncObject::classDyncObject; 

ObjectList::ObjectList(RuntimeObject* pNewClass)
{
	assert(pNewClass);

	pNewClass->m_pNextClass = __pFirstCmdClass;

	__pFirstCmdClass = pNewClass;
}

DyncObject* TCreateObject(const char* class_name)
{
	static TC_ThreadMutex _mutex_create_obj;

	TC_LockT<TC_ThreadMutex> lock(_mutex_create_obj); 

	RuntimeObject *pHead = __pFirstCmdClass;

	LOG->debug() << ">>" << class_name << std::endl;

	while(pHead)
	{
		LOG->debug()  << "->" << pHead->m_szClassName << std::endl;

		if (!strcmp(pHead->m_szClassName, class_name))
		{
			if (pHead->m_pfnCreateObject)
			{
				return (*(pHead->m_pfnCreateObject))();
			}
		}

		pHead = pHead->m_pNextClass;
	}
	
	return NULL;
}


