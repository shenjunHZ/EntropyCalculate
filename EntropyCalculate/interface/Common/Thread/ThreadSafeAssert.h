#ifndef __COMMON_THREADSAFEASSERT_H__
#define __COMMON_THREADSAFEASSERT_H__

#include "Common/CommonGlobal.h"

namespace Common
{
#ifdef _DEBUG
	class CThreadSafeAssertPrivate;
	class COMMON_EXPORT CThreadSafeAssert
	{
	public:
	    CThreadSafeAssert();
	    ~CThreadSafeAssert();

    protected:
		// 非线程安全接口，会出断言
		void CallOnNonThreadSafe();

    private:
	    DS_DECLARE_PRIVATE(CThreadSafeAssert);
	    CThreadSafeAssertPrivate* d_ptr;
    };

#else
	class COMMON_EXPORT CThreadSafeAssert
	{
	public:
		CThreadSafeAssert();
		~CThreadSafeAssert();

	protected:
		// 非线程安全接口，会出断言
		void CallOnNonThreadSafe();
	};
#endif
}

#endif // __COMMON_THREADSAFEASSERT_H__
