#ifndef _OUTER_FACTORY_H_
#define _OUTER_FACTORY_H_

#include <string>
#include "OuterProxyFactory.h"

using namespace std;
using namespace tars;

/**
 * 外部接口工厂接口规约
 */
class OuterFactory : public TC_HandleBase
{
public:
	OuterFactory() {}

	virtual ~OuterFactory() {}

public:
	virtual const OuterProxyFactoryPtr& getProxyFactory() const = 0;

	virtual const TC_Config& getConfig() const = 0;
};
////////////////////////////////////////////////////////////////////

typedef TC_AutoPtr<OuterFactory> OuterFactoryPtr;

#endif


