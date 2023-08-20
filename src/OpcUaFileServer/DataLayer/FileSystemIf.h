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

#include <vector>

#include <boost/shared_ptr.hpp>

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

		virtual bool createFile(
			const std::string& path,
			const std::string& file
		) = 0;

		virtual bool createDirectory(
			const std::string& path,
			const std::string& directory
		) = 0;

		virtual bool remove(
			const std::string& path
		) = 0;

		virtual bool openFile(
			const std::string& path,
			const std::string& file,
			uint32_t& fileHandle
		) = 0;

		virtual bool closeFile(
			uint32_t& fileHandle
		) = 0;

		virtual bool writeFile(
			uint32_t fileHandle,
			const std::string& data
		) = 0;

		virtual bool readFile(
			uint32_t fileHandle,
			int32_t length,
			std::string& data
		) = 0;

	  private:

	};

}

#endif
