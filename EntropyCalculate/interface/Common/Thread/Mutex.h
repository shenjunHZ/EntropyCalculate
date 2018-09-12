#ifndef __COMMON_MUTEX_H__
#define __COMMON_MUTEX_H__

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

namespace Common
{
    //////////////////////////////////////////////////////////////////////////
    //独占锁
    typedef boost::mutex DSMutex;                            // 独占锁
    typedef boost::unique_lock<boost::mutex> DSMutexLock;    // 独占锁Lock

    //////////////////////////////////////////////////////////////////////////
    //递归锁
    typedef boost::recursive_mutex DSRecursiveMutex;                   // 递归锁(不建议使用，建议使用DSMutex)
    typedef boost::recursive_mutex::scoped_lock DSRecursiveMutexLock;  // 递归锁Lock

    //////////////////////////////////////////////////////////////////////////
    //读写锁
    // 读写锁共享互斥量（会创建句柄，不建议大量对象使用，建议使用独占锁DSMutex）
    typedef boost::shared_mutex DSRWMutex; 
    // 读锁
    typedef boost::shared_lock<DSRWMutex> DSReadLock; 
    // 写锁
    typedef boost::unique_lock <DSRWMutex> DSWriteLock; 
}

#endif // __COMMON_MUTEX_H__
