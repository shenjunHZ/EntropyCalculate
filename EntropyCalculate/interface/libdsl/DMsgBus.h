/* Copyright (c) 2013, 浙江大华技术股份有限公司, All rights reserved.
* 2013-10
*
* DMsgBus.h : 消息总线对象
*/

#ifndef	INCLUDED_LIBDSL_DMSGBUS_H
#define	INCLUDED_LIBDSL_DMSGBUS_H

#include <libdsl/DRefObj.h>
#include <libdsl/DThreadRunner.h>
#include <libdsl/DEvent.h>
#include <libdsl/DStr.h>
#include <deque>
#include <vector>

BEGIN_NAMESPACE_DSL
/////////////////////////////////////////////////

class DMsgBus;

typedef unsigned int DMHID;  // 标识MsgHandler身份的ID

#define DMSG_INVALID_ID		((unsigned int)-1)

#define DMSG_TYPE_UNKNOWN			0     // 不应该出现的类型
#define DMSG_TYPE_DST_NOT_FOUND		1	  // 接收者不存在，系统退回给发送者
#define DMSG_TYPE_DST_NOT_ACCEPT 	2     // 接收者不处理此消息，退回给发送者
#define DMSG_TYPE_USER_BEGIN 		1000  // 用户自定义消息应从1000开始

class LIBDSL_API DMsg : virtual public DRefObj
{
public:
	DMsg( int type = DMSG_TYPE_UNKNOWN, DMHID src = DMSG_INVALID_ID, DMHID dst = DMSG_INVALID_ID ) 
		: m_type( type ), m_src( src ), m_dst( dst ) {}

	inline void SetType( int type ) { m_type = type; return; }
	inline int GetType() const { return m_type; }

	inline void SetID( DMHID src, DMHID dst ) { m_src = src; m_dst = dst; return; }
	inline void SetSrcID( DMHID src ) { m_src = src; return; }
	inline void SetDstID( DMHID dst ) { m_dst = dst; return; }
	inline DMHID GetSrcID() const { return m_src; }
	inline DMHID GetDstID() const { return m_dst; }

protected:
	virtual ~DMsg() {}

protected:
	int m_type;  // 消息类型 0 ~ 999 是系统保留值
	DMHID m_src;
	DMHID m_dst;
};

// 退回给用户的消息，里面存放的是原始消息
class LIBDSL_API DMsgWrap : public DMsg
{
public:
	DMsgWrap( const DRef<DMsg> & msg, int type = DMSG_TYPE_UNKNOWN, DMHID src = DMSG_INVALID_ID, DMHID dst = DMSG_INVALID_ID ) 
		: DMsg( type, src, dst ), m_msg( msg ) {}

	inline const DRef< DMsg > & GetMsg() const { return m_msg; }
	inline void SetMsg( const DRef<DMsg> & msg ) { m_msg = msg; return; }

protected:
	DRef<DMsg> m_msg;
};

// 消息处理器的基类，用户继承此类实现具体的消息处理
// 系统会保证同一个DMsgHandler收到的消息是串行化进行回调
class LIBDSL_API DMsgHandler : virtual public DRefObj
{
public:
	DMsgHandler() : m_id( DMSG_INVALID_ID ) {}

	inline DMHID GetID() { return m_id; }
	virtual const char * GetName() { return NULL; }
	virtual int OnMsg( const DRef<DMsg> & msg ) = 0;
	
protected:
	virtual ~DMsgHandler() {}

private:
	friend class DMsgBus;
	DMHID m_id; // 加入时设置，退出时清除，由DMsgBus负责维护
};

// NOTE ：在OnMsg()的实现中，往往需要msg转换成具体的子类，可以使用DRef::Cast()函数来实现
// 例如 DRef<DMsg> msg = new DMsgWrap(); DRef< DMsgWrap > msgwrap = msg.Cast<DMsgWrap>();

#define DMSG_FLAG_MSG_URGENCY	(1 << 0)	// 紧急消息，放在队列前头尽快处理

class LIBDSL_API DMsgBus
{
public:
	DMsgBus( unsigned int nThreadNum = 1, unsigned int nMaximumQueue = 4096 );
	virtual ~DMsgBus();

	int Start();
	int SignalStop();
	int Stop();

	// NOTE : 添加的handler数不可超过65535个
	DMHID AddHandler( const DRef<DMsgHandler> & handler );
	int DelHandler( const DRef<DMsgHandler> & handler );

	DMHID FindHandler( const char * name );

	int PushMsg( const DRef< DMsg > & msg, int flag = 0 );

	int QueueSize( void );
	bool IsRunning( void ) const { return m_isRunning; }
	void SetThreadName(const char * name); // 设置线程名称 最长15字节 必须在Start前调用

protected:
	int driver_engine( DThread * th, int index );
	static int driver_func( void * arg, void * th );

protected:
	class  PerThreadInfo
	{
	public:
		int id;
		DMsgBus* pMsgBus;
	};

	unsigned int m_maximumQueue;
	unsigned int m_threadNum;
	int m_handlerCount;          // 防止MsgHandler的ID重复的全局计数
	
	dsl::DEvent* m_evt;
	std::vector< std::deque< DRef<DMsg> > > m_vecMsg;

	std::vector< PerThreadInfo > m_vecArgInfo;

	dsl::DMutex m_handlerMutex;
	std::vector< DRef<DMsgHandler> > m_vecHandler;
	
	std::vector< DRunner<void> > m_vecThread;
	bool m_isRunning;
};

/////////////////////////////////////////////////
END_NAMESPACE_DSL

#endif // INCLUDED_LIBDSL_DMSGBUS_H

