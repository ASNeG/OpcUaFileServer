/*
   Copyright 2015-2020 Kai Huebl (kai@huebl-sgh.de)

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

#include <iostream>
#include "BuildConfig.h"

#include "OpcUaStackCore/Base/os.h"
#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaStackCore/BuildInTypes/OpcUaIdentifier.h"
#include "OpcUaStackServer/ServiceSetApplication/ApplicationService.h"
#include "OpcUaStackServer/ServiceSetApplication/NodeReferenceApplication.h"

#include "OpcUaFileServer/Library/Library.h"
#include "OpcUaFileServer/DataLayer/FileSystemAccess.h"

using namespace OpcUaStackCore;
using namespace OpcUaStackServer;

namespace OpcUaFileServer
{

	Library::Library(void)
	: ApplicationIf()
	{
	}

	Library::~Library(void)
	{
	}

	bool
	Library::startup(void)
	{
		bool rc = true;

		Log(Debug, "Library::startup");

		std::string fileSystemName = "FileSystemRoot";
		std::string fileSystemDirectory = applicationInfo()->confDir();
		Log(Debug, "create file system")
			.parameter("FileSystemName", fileSystemName)
			.parameter("FileSystemDirectory", fileSystemDirectory);

		// Create file system access
		auto fileSystemAccess = boost::make_shared<FileSystemAccess>();
		fileSystemAccess->init((std::filesystem::path(fileSystemDirectory))); // FIXME: only example
		FileSystemIf::SPtr fileSystemIf = fileSystemAccess;

		Log(Debug, "");

		// Create root directory
		rootDir_ = boost::make_shared<FileDirectoryObject>(std::filesystem::path(""));
		rc = rootDir_->init(
			fileSystemIf,
			&service(),
			"http://ASNeG.com/OpcUaFileServer"
		);
		if (rc == false) {
			Log(Error, "init root directory error");
			return false;
		}
		rc  = rootDir_->addToOpcUaModel(
			fileSystemName,
			OpcUaNodeId((uint32_t)OpcUaId_ObjectsFolder),
			OpcUaNodeId((uint32_t)OpcUaId_HasComponent)
		);
		if (rc == false) {
			Log(Error, "init root directory error");
			return false;
		}

		// create file system objects in opc ua information model
		if (!rootDir_->addFileSystemChildsToOpcUaModel()) {
			Log(Error, "add file system objects to opc ua information model error");
			return false;
		}

		return true;
	}

	bool
	Library::shutdown(void)
	{
		Log(Debug, "Library::shutdown");
		return true;
	}

	std::string
	Library::version(void)
	{
		std::stringstream version;

		version << LIBRARY_VERSION_MAJOR << "." << LIBRARY_VERSION_MINOR << "." << LIBRARY_VERSION_PATCH;
		return version.str();
	}

	std::string
	Library::gitCommit(void)
	{
		return LIBRARY_GIT_COMMIT;
	}

	std::string
	Library::gitBranch(void)
	{
		return LIBRARY_GIT_BRANCH;
	}

}

extern "C" DLLEXPORT void  init(ApplicationIf** applicationIf) {
    *applicationIf = new OpcUaFileServer::Library();
}

