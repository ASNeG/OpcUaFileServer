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

#include "OpcUaStackCore/BuildInTypes/OpcUaIdentifier.h"
#include "OpcUaStackServer/ServiceSetApplication/CreateObjectInstance.h"
#include "OpcUaStackServer/ServiceSetApplication/DeleteNodeInstance.h"

#include "OpcUaFileServer/OpcUaLayer/FileObject.h"

using namespace OpcUaStackCore;
using namespace OpcUaStackServer;

namespace OpcUaFileServer
{

	FileObject::FileObject(const std::filesystem::path& path)
	: FileType()
	, path_(path)
	{
	}

	FileObject::~FileObject(void)
	{
		for (auto fileHandle: fileHandleSet_) {
			close(fileHandle);
		}
		fileHandleSet_.clear();
	}

	bool
	FileObject::init(
		FileSystemIf::SPtr fileSystemIf,
		OpcUaStackServer::ApplicationServiceIf* applicationServiceIf,
		const std::string& namespaceName
	)
	{
		fileSystemIf_ = fileSystemIf;
		applicationServiceIf_ = applicationServiceIf;
		namespaceName_ = namespaceName;
		return true;
	}

	const std::filesystem::path
	FileObject::path(void)
	{
		return path_;
	}

	bool
	FileObject::addToOpcUaModel(
		const std::string& displayName,
		const OpcUaNodeId& parentNodeId,
		const OpcUaNodeId& referenceTypeNodeId
	)
	{
		// create a new object instance in opc ua information model
		Object::SPtr obj = shared_from_this();
		CreateObjectInstance createObjectInstance(
			namespaceName_,									// namespace name of the object instance
			OpcUaLocalizedText("", displayName),			// display name of the object instance
			parentNodeId,									// parent node of the object instance
			referenceTypeNodeId,							// reference type between object and variable instance
			obj
		);
		if (!createObjectInstance.query(applicationServiceIf_)) {
			Log(Error, "create object error")
				.parameter("DisplayName", displayName);
			return false;
		}

		return true;
	}

	bool
	FileObject::deleteFromOpcUaModel(void)
	{
		// close all files
		for (auto fileHandle: fileHandleSet_) {
			close(fileHandle);
		}
		fileHandleSet_.clear();

		// remove object from opc ua model
		DeleteNodeInstance deleteNodeInstance;
		deleteNodeInstance.node(nodeId());

		if (!deleteNodeInstance.query(applicationServiceIf_)) {
			Log(Error, "delete node from opc ua model error")
				.parameter("DirectoryNodeId", nodeId());
			return false;
		}

		return true;
	}

    void
	FileObject::call_Close_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext)
    {
    	bool rc  = true;
    	OpcUaVariant::SPtr variantInput;

		// Check number of input parameters
		if (applicationMethodContext->inputArguments_->size() != 1) {
			Log(Error, "number of input arguments error in close file method");
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}

		// Get input parameter [0]: FileHandle
		applicationMethodContext->inputArguments_->get(0,variantInput);
		if (variantInput->variantType() != OpcUaBuildInType_OpcUaUInt32) {
			Log(Error, "get input argument [0] error in close file method");
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}
		uint32_t fileHandle = variantInput->get<uint32_t>();

		// Check if file handle exist
		if (fileHandleSet_.find(fileHandle) == fileHandleSet_.end()) {
			Log(Error, "close file failed, because file handle not exist")
				.parameter("FileHandle", fileHandle);
			applicationMethodContext->statusCode_ = BadNoEntryExists;
			return;
		}

		// Close file
		if (!close(fileHandle)) {
			Log(Error, "close file failed")
				.parameter("FileHandle", fileHandle);
			applicationMethodContext->statusCode_ = BadInternalError;
			return;
		}

		fileHandleSet_.erase(fileHandle);
		applicationMethodContext->statusCode_ = Success;
    }

    void
	FileObject::call_GetPosition_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext)
    {
    }

    void
	FileObject::call_Open_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext)
    {
    }

    void
	FileObject::call_Read_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext)
    {
    }

    void
	FileObject::call_SetPosition_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext)
    {
    }

    void
	FileObject::call_Write_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext)
    {
    }

    bool
	FileObject::create(void)
    {
		// create new file instance
		bool rc = fileSystemIf_->createFile(path_.parent_path().string(), path_.filename().string());
		if (!rc) {
			Log(Error, "create file instance error")
				.parameter("Path", path_.string());
			return false;
		}
    	return true;
    }

    bool
	FileObject::open(uint32_t& handle)
    {
    	uint32_t fileHandle;

    	// open file instance
    	bool rc  = fileSystemIf_->openFile(path_.parent_path().string(), path_.filename().string(), fileHandle);
		if (!rc) {
			Log(Error, "open file instance error")
				.parameter("Path", path_.string());
			return false;
		}

		// Add file handle to file handle set
		fileHandleSet_.insert(fileHandle);

    	handle = fileHandle;
    	return true;
    }

    bool
	FileObject::close(uint32_t fileHandle)
    {
    	// Check file handle in file handle set
    	if (fileHandleSet_.find(fileHandle) == fileHandleSet_.end()) {
			Log(Error, "close file instance error, because file handle unknown")
				.parameter("Path", path_.string())
				.parameter("FileHandle", fileHandle);
			return 0;
    	}

    	// close file instance
    	bool rc = fileSystemIf_->closeFile(fileHandle);
		if (!rc) {
			Log(Error, "close file instance error")
				.parameter("Path", path_.string())
				.parameter("FileHandle", fileHandle);
			return 0;
		}

		// remove file handle from file handle set
		fileHandleSet_.erase(fileHandle);

    	return  true;
    }

}
