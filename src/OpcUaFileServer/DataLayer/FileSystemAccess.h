/*
   Copyright 2023 Kai Huebl (kai@huebl-sgh.de)

   Lizenziert gemäß Apache Licence Version 2.0 (die „Lizenz“); Nutzung dieser
   Datei nur in Übereinstimmung mit der Lizenz erlaubt.
   Eine Kopie der Lizenz erhalten Sie auf http://www.apache.org/licenses/LICENSE-2.0.

   Sofern nicht gemäß geltendem Recht vorgeschrieben oder schriftlich vereinbart,
   erfolgt die Bereitstellung der im Rahmen der Lizenz verbreiteten Software OHNE
   GEWÄHR ODER VORBEHALTE – ganz gleich, ob ausdrücklich oder stillschweigend.

   Informationen über die jeweiligen Bedingungen für Genehmigungen und Einschränkungen
   im Rahmen der Lizenz finden Sie in der Lizenz.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#ifndef __OpcUaFileServer_FileSystemAccess_h__
#define __OpcUaFileServer_FileSystemAccess_h__

#include <map>
#include <map>
#include <string>
#include <filesystem>
#include <fstream>

#include <boost/shared_ptr.hpp>

#include "OpcUaFileServer/DataLayer/FileSystemIf.h"

namespace OpcUaFileServer
{

	class FileHandle
	{
	  public:
		using SPtr = boost::shared_ptr<FileHandle>;

		std::fstream fs_;
		time_t openTime_;
	};

	class FileSystemAccess
	: public FileSystemIf

	{
	  public:
		using SPtr = boost::shared_ptr<FileSystemAccess>;
		using FileHandleMap = std::map<uint32_t, FileHandle::SPtr>;
		using Vec = std::vector<SPtr>;

		FileSystemAccess(void);
		virtual ~FileSystemAccess(void);

		bool init(const std::filesystem::path& basePath);

		virtual void getChilds(
			const std::string& path,
			std::vector<std::string>& files,
			std::vector<std::string>& directories
		) override;

		virtual ResultCode createDirectory(
			const std::string& path,
			const std::string& directory
		) override;

		virtual ResultCode createFile(
			const std::string& path,
			const std::string& file
		) override;

		virtual ResultCode remove(
			const std::string& path
		) override;

		virtual ResultCode openFile(
			const std::string& path,
			const std::string& file,
			FileMode fileMode,
			uint32_t& fileHandle
		) override;

		virtual ResultCode closeFile(
			uint32_t& fileHandle
		) override;

		virtual ResultCode writeFile(
			uint32_t fileHandle,
			const std::string& data
		) override;

		virtual ResultCode readFile(
			uint32_t fileHandle,
			int32_t length,
			std::string& data
		) override;

		virtual bool isReadable(
			const std::string& path,
			const std::string& file
		) override;

		virtual bool isWriteable(
			const std::string& path,
			const std::string& file
		) override;

	  private:
		uint32_t fileHandle_ = 1;
		std::filesystem::path basePath_ = {};
		FileHandleMap fileHandleMap_;
	};

}

#endif
