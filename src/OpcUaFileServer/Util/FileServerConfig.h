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

#ifndef __OpcUaFileServer_FileSystemConfig_h__
#define __OpcUaFileServer_FileSystemConfig_h__

#include <string>
#include <vector>

#include "OpcUaStackCore/Base/Config.h"

#include "OpcUaFileServer/Util/FileMode.h"

namespace OpcUaFileServer
{

	class FileSystemConfig
	{
	  public:
		using SPtr = std::shared_ptr<FileSystemConfig>;
		using Vec = std::vector<SPtr>;

		FileSystemConfig(void);
		~FileSystemConfig(void);

		bool parse(OpcUaStackCore::Config& config);
		std::string fileSystemName(void);
		std::string fileSystemDirectory(void);
		std::string namespaceName(void);
		bool rwLock(void);
		FileMode& fileMode(void);

	  private:
		std::string fileSystemName_ = "";
		std::string fileSystemDirectory_ = "";
		std::string namespaceName_ = "";
		bool rwLock_ = false;
		FileMode fileMode_;
	};


	class FileServerConfig
	{
	  public:
		FileServerConfig(void);
		virtual ~FileServerConfig(void);

		bool parse(const std::string& fileName);
		FileSystemConfig::Vec& fileSystemConfig(void);

	  private:
		FileSystemConfig::Vec fileSystemConfigVec_;
	};

}

#endif
