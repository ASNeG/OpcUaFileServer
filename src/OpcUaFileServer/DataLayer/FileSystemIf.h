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

#ifndef __OpcUaFileServer_FileSystemIf_h__
#define __OpcUaFileServer_FileSystemIf_h__

#include <optional>
#include <vector>
#include <bitset>

#include <boost/shared_ptr.hpp>

#include "OpcUaFileServer/Util/FileMode.h"
#include "OpcUaFileServer/Util/ResultCode.h"

namespace OpcUaFileServer
{

	class FileSystemIf
	{
	  public:
		using SPtr = boost::shared_ptr<FileSystemIf>;

		FileSystemIf(void);
		virtual ~FileSystemIf(void);

		virtual void getChilds(
			const std::string& path,
			std::vector<std::string>& files,
			std::vector<std::string>& directorys
		) = 0;

		virtual ResultCode createFile(
			const std::string& path,
			const std::string& file
		) = 0;

		virtual ResultCode createDirectory(
			const std::string& path,
			const std::string& directory
		) = 0;

		virtual ResultCode remove(
			const std::string& path
		) = 0;

		virtual ResultCode openFile(
			const std::string& path,
			const std::string& file,
			std::optional<FileMode> fileMode,
			uint32_t& fileHandle
		) = 0;

		virtual ResultCode closeFile(
			uint32_t& fileHandle
		) = 0;

		virtual ResultCode writeFile(
			uint32_t fileHandle,
			const std::string& data
		) = 0;

		virtual ResultCode readFile(
			uint32_t fileHandle,
			int32_t length,
			std::string& data
		) = 0;

		virtual bool isReadable(
			const std::string& path,
			const std::string& file
		) = 0;

		virtual bool isWriteable(
			const std::string& path,
			const std::string& file
		) = 0;

	  private:

	};

}

#endif
