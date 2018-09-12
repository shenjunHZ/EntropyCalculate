/**
 * Copyright (c) 
 * All rights reserved.
 *
 * 文件名称：GlobalDataCenter.h
 * 文件标识：
 * 摘　　要：
 *
 * version：1.0
*/

#pragma once

#include "Common/CommonGlobal.h"
#include "Common/Pattern/Singleton.h"

namespace Common
{
	class CGlobalDataCenterPrivate;
	class COMMON_EXPORT CGlobalDataCenter : public CSingleton<CGlobalDataCenter>
	{
		friend class CSingleton<CGlobalDataCenter>;

	public:
		/**
		 * @brief	设置可执行程序路径
		 * @param	[IN] strDirPath	可执行程序路径
		 * @return
		 * @note	需在程序初始化时调用
		 */
		void SetCurrentDirPath(const std::string& strDirPath);

		/**
		 * @brief	获取可执行文件路径
		 * @return
		 */
		std::string GetCurrentDirPath();

		/**
		 * @brief	设置可执行程序路径
		 * @param	[IN] strDirPath 可执行程序路径	utf8编码
		 * @return
		 * @note	sqlite需要utf8的编码
		 * @see
		 */
		void SetCurrentDirPath_UTF8(const std::string& strDirPath);

		/**
		 * @brief	获取可执行文件路径
		 * @return
		 * @note	获取的路径是utf8编码
		 */
		std::string GetCurrentDirPath_UTF8();

	public:
		CGlobalDataCenter();
		~CGlobalDataCenter();
	private:
		DS_DECLARE_PRIVATE(CGlobalDataCenter);
		CGlobalDataCenterPrivate* d_ptr;
	};

} // end namespace common
