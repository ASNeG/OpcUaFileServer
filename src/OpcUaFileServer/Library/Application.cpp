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

#include <boost/make_shared.hpp>

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaStackCore/BuildInTypes/OpcUaIdentifier.h"
#include "OpcUaStackServer/ServiceSetApplication/ApplicationService.h"
#include "OpcUaStackServer/ServiceSetApplication/NodeReferenceApplication.h"

#include "OpcUaFileServer/Library/Application.h"

using namespace OpcUaStackCore;
using namespace OpcUaStackServer;

namespace OpcUaFileServer
{

	Application::Application(void)
	{
	}

	Application::~Application(void)
	{
	}

	bool
	Application::startup(
		const std::string& configFileName,
		OpcUaStackServer::ApplicationIf* applicationIf
	)
	{
		bool rc = true;

		Log(Debug, "startup opc ua file server");

		// Read file server configuration file
		rc  = fileServerConfig_.parse(configFileName);
		if (rc == false) {
			Log(Error, "read file server configuration error")
				.parameter("ConfigFileName", configFileName);
			return false;
		}

		// Create file system and opc ua model
		for (auto fileSystemConfig : fileServerConfig_.fileSystemConfig()) {
			Log(Debug, "create file system")
				.parameter("FileSystemName", fileSystemConfig->fileSystemName())
				.parameter("FileSystemDirectrory", fileSystemConfig->fileSystemDirectory())
				.parameter("FileSystemNamesapce", fileSystemConfig->namespaceName())
				.parameter("FileSystemCreateFileMode", fileSystemConfig->fileMode())
				.parameter("FileSystemRWLock", fileSystemConfig->rwLock());

			// Create file system object
			auto fileSystemAccess = boost::make_shared<FileSystemAccess>();
			rc = fileSystemAccess->init(fileSystemConfig);
			if (rc == false) {
				Log(Error, "create file system object error")
					.parameter("FileSystemName", fileSystemConfig->fileSystemName())
					.parameter("FileSystemDirectrory", fileSystemConfig->fileSystemDirectory());
				return false;
			}
			FileSystemIf::SPtr fileSystemIf = fileSystemAccess;


			// Create opc ua model
			auto fileDirectoryObject = boost::make_shared<FileDirectoryObject>(std::filesystem::path(""));
			rc = fileDirectoryObject->init(
				fileSystemIf,
				&applicationIf->service(),
				fileSystemConfig->namespaceName()
			);
			if (rc == false) {
				Log(Error, "init root directory error")
					.parameter("FileSystemName", fileSystemConfig->fileSystemName())
					.parameter("FileSystemDirectrory", fileSystemConfig->fileSystemDirectory());
				return false;
			}

			rc  = fileDirectoryObject->addToOpcUaModel(
				fileSystemConfig->fileSystemName(),
				OpcUaNodeId((uint32_t)OpcUaId_ObjectsFolder),
				OpcUaNodeId((uint32_t)OpcUaId_HasComponent)
			);
			if (rc == false) {
				Log(Error, "add root directory to opc ua model error")
					.parameter("FileSystemName", fileSystemConfig->fileSystemName())
					.parameter("FileSystemDirectrory", fileSystemConfig->fileSystemDirectory());
				return false;
			}

			// create file system objects in opc ua information model
			if (!fileDirectoryObject->addFileSystemChildsToOpcUaModel()) {
				Log(Error, "add file system objects to opc ua information model error")
					.parameter("FileSystemName", fileSystemConfig->fileSystemName())
					.parameter("FileSystemDirectrory", fileSystemConfig->fileSystemDirectory());
				return false;
			}

			// Save new objects
			fileSystemAccessVec_.push_back(fileSystemAccess);
			fileDirectoryObjectVec_.push_back(fileDirectoryObject);
		}

		return true;
	}

	bool
	Application::shutdown(void)
	{
		Log(Debug, "shutdown opc ua file server");

		// Cleanup file directory objects
		for (auto fileDirectoryObject : fileDirectoryObjectVec_) {
			fileDirectoryObject->deleteFromOpcUaModel();
		}
		fileDirectoryObjectVec_.clear();

		// Cleanup file systems
		for (auto fileSystemAccess : fileSystemAccessVec_) {
			// Nothing to do
		}
		fileSystemAccessVec_.clear();

		return true;
	}

}

