#ifndef __COMMON_OBSERVEABLE_H__
#define __COMMON_OBSERVEABLE_H__

#include "Common/CommonGlobal.h"
#include "Common/Thread/Mutex.h"
#include <list>
#include <algorithm>

namespace Common
{
    // 观察者模式中的可观察者
    template <class TObserver>
    class CObservable
    {
        typedef typename std::list<TObserver*>::iterator ObserverIterator;
    public:
        CObservable();
        ~CObservable();

    public:
        // 注册
        bool RegisterObserver(TObserver* pObserver);
        // 销毁
        bool UnRegisterObserver(TObserver* pObserver);

    public:
        // FunctionPointer fn为函数指针
        template <class FunctionPointer, class Param>
        void NotifyObservers(FunctionPointer fn, const Param& param);

		// FunctionPointer fn为函数指针
		template <class FunctionPointer, class Param>
		void NotifyObservers(FunctionPointer fn, const Param* pParam);
    private:
        std::list<TObserver*> m_lstObservers;            // 观察者列表

    private:
        DSMutex m_mutex;                       // 锁Q
    };

    template <class TObserver>
    inline CObservable<TObserver>::CObservable()
    {
    }

    template <class TObserver>
    inline CObservable<TObserver>::~CObservable()
    {
    }

    // 注册
    template <class TObserver>
    inline bool CObservable<TObserver>::RegisterObserver(TObserver* pObserver)
    {
        if (pObserver == NULL)
        {
            return false;
        }

        DSMutexLock lock(m_mutex);
        m_lstObservers.push_front(pObserver);

#ifdef _DEBUG
		pObserver->IncreaseCount();
#endif 

        return true;
    }

    // 销毁
    template <class TObserver>
    inline bool CObservable<TObserver>::UnRegisterObserver(TObserver* pObserver)
    {
        DSMutexLock lock(m_mutex);
        ObserverIterator it = find(m_lstObservers.begin(), m_lstObservers.end(), pObserver);
        if (it != m_lstObservers.end())
        {
#ifdef _DEBUG
			(*it)->DecreaseCount();
#endif 
            m_lstObservers.erase(it);
        }

        return true;
    }

    // 向所有观察者分发消息
    template <class TObserver> template <class FunctionPointer, class Param>
    inline void CObservable<TObserver>::NotifyObservers(FunctionPointer fn, const Param& param)
    {
        DSMutexLock lock(m_mutex);
        for (ObserverIterator it = m_lstObservers.begin(); it != m_lstObservers.end(); ++it)
        {
            TObserver* pTObserver = *it;
            (pTObserver->*fn)(param);
        }
    }

	// 向所有观察者分发消息
	template <class TObserver> template <class FunctionPointer, class Param>
	inline void CObservable<TObserver>::NotifyObservers(FunctionPointer fn, const Param* pParam)
	{
		DSMutexLock lock(m_mutex);
		for (ObserverIterator it = m_lstObservers.begin(); it != m_lstObservers.end(); ++it)
		{
			TObserver* pTObserver = *it;
			(pTObserver->*fn)(pParam);
		}
	}
}

#endif
