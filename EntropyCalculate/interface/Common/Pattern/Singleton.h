/*
 *@file   Singleton.h
 *@brief  单件类
 * 
 *@author 
 *@date   
 *
 */

#ifndef __COMMON_SINGLETON_H__
#define __COMMON_SINGLETON_H__

#include "Common/Thread/Mutex.h"

namespace Common
{
	/**  类名：CSingleton
	  *  说明：单件类
	 **/
	template <class T>
	class CSingleton
	{
	public:
		// 获取单件
		static T& GetInstance();

	protected:
		// 构造
		CSingleton() {}
		// 析够
		virtual ~CSingleton() {}
	};

	/** @fn	    inline T& CSingleton<T>::GetInstance()
	 *  @brief	获取单件
	 *  @param	void
	 *  @return	T 返回单件
	 */
	template <class T>
	inline T& CSingleton<T>::GetInstance()
	{
        static std::auto_ptr<T> s_ptrInstance;
		static DSMutex m_mutexInstance;

		if (s_ptrInstance.get() == NULL)
		{
            // 避免创建两个
			DSMutexLock locker(m_mutexInstance); 
            // 多线程保护
			if (s_ptrInstance.get() == NULL)
			{
				s_ptrInstance = std::auto_ptr<T>(new T);
			}
		}

		return *s_ptrInstance.get();
	}
}

#endif // __COMMON_SINGLETON_H__
