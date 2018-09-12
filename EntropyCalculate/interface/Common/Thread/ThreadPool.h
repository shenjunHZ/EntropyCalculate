#ifndef __COMMON_THREADPOOL_H__
#define __COMMON_THREADPOOL_H__

#include "Common/CommonGlobal.h"
#include "Common/Pattern/Singleton.h"

namespace Common
{
    class CTask;
	// 多线程消息循环。无顺序、定时执行。类似登录心跳等。
	class CScheduleThreadPoolPrivate;
	class CScheduleThreadPool
	{
	public:
		// 塞入队列
		bool PostTask(CTask* pTask);
	protected:
		// 启动消息队列
		bool Start();
		// 停止消息队列
		void Stop();

		// 消息队列循环
		void Run();

	private:
		CScheduleThreadPool(int iMaxThreadNum);
		~CScheduleThreadPool();

	private:
		DS_DECLARE_PRIVATE(CScheduleThreadPool);
		CScheduleThreadPoolPrivate* d_ptr;
	};

	//////////////////////////////////////////////////////////////////////////
	class CCachedThreadPoolPrivate;
	// 可缓存的线程池。适用于预览、回放等操作相关。 
	class COMMON_EXPORT CCachedThreadPool
	{
		friend class CMessageLoopPoolForSDK;
	public:
		/** 塞入任务。
		 * @param[in] const std::string& strID 线程池队列标识码，相同队列需保证唯一。
		 * @param[in] CTask* pTask 任务。
		 * @return 
		 */
		bool PostTask(const std::string& strID, CTask* pTask);        /** 置为空闲且清除队列中的任务。
         *  等待当前正在执行任务完毕
		 * @param[in] const std::string& strID 线程池队列标识码，相同队列需保证唯一。
		 * @return 
		 */        void ClearTask(const std::string& strID);        /** 异步置为空闲且清除队列中的任务。
         *  不清除当前正在执行任务
		 * @param[in] const std::string& strID 线程池队列标识码，相同队列需保证唯一。
		 * @return 
		 */        void AsyncClearTask(const std::string& strID);
	public:
		// 启动
		bool Start();
		// 停止
		void Stop();

	private:
		CCachedThreadPool();
		~CCachedThreadPool();

	private:
		DS_DECLARE_PRIVATE(CCachedThreadPool);
		CCachedThreadPoolPrivate* d_ptr;
	};

	// SDK同步调用队列，对于需持续使用的，例如登录心跳等，不使用此线程池
	class COMMON_EXPORT CMessageLoopPoolForSDK : public CCachedThreadPool,
		public CSingleton<CMessageLoopPoolForSDK>
	{
		friend class CSingleton<CMessageLoopPoolForSDK>;
	};
	//////////////////////////////////////////////////////////////////////////
}

#endif // __COMMON_THREADPOOL_H__