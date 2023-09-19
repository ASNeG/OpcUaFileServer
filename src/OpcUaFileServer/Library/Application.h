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

#ifndef __OpcUaFileServer_Application_h__
#define __OpcUaFileServer_Application_h__

#include <string>

#include "OpcUaStackServer/Application/ApplicationIf.h"

#include "OpcUaFileServer/Util/FileServerConfig.h"
#include "OpcUaFileServer/DataLayer/FileSystemAccess.h"
#include "OpcUaFileServer/OpcUaLayer/FileDirectoryObject.h"

namespace OpcUaFileServer
{

	class Application
	{
	  public:
		Application(void);
		~Application(void);

		bool startup(
			const std::string& configFileName,
			OpcUaStackServer::ApplicationIf* applicationIf
		);
		bool shutdown(void);

	  private:
		OpcUaStackServer::ApplicationIf* applicationIf_ = nullptr;
		FileServerConfig fileServerConfig_;
		FileSystemAccess::Vec fileSystemAccessVec_;
		FileDirectoryObject::Vec fileDirectoryObjectVec_;
	};

}

#endif
