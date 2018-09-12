#ifndef __COMMON_THREAD_H__
#define __COMMON_THREAD_H__

#include "Common/CommonGlobal.h"

namespace Common
{
	class CThreadPrivate;
	// 单线程消息循环
	class COMMON_EXPORT CThread
	{
	protected:
		CThread();
		~CThread();

	public:
		// 启动线程，单位毫秒
		bool StartThread(unsigned int uWaitTime);
		// 停止线程
		void StopThread();

		// 唤醒线程，取消等待
		void WakeUpOnce();

		// 线程是否运行
		bool IsRunning() const;
	public:
		// 线程睡眠
		static void Sleep(unsigned long iSecs);
		static void MSleep(unsigned long iMSecs);
		static void USleep(unsigned long iUSecs);

	protected:
		// 线程执行。返回值：false：停止线程函数 true：正常执行
		virtual bool Run() = 0;

	private:
		// 启动线程
		void CallThread();

	private:
		DS_DECLARE_PRIVATE(CThread);
		CThreadPrivate* d_ptr;
	};
}

#endif // __COMMON_THREAD_H__