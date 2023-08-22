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

#include "OpcUaFileServer/OpcUaLayer/FileDirectoryObject.h"

using namespace OpcUaStackCore;
using namespace OpcUaStackServer;

namespace OpcUaFileServer
{

	FileDirectoryObject::FileDirectoryObject(const std::filesystem::path& path)
	: FileDirectoryType()
	, path_(path)
	{
	}

	FileDirectoryObject::~FileDirectoryObject(void)
	{
	}

	bool
	FileDirectoryObject::init(
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
	FileDirectoryObject::path(void)
	{
		return path_;
	}

	bool
	FileDirectoryObject::addFileSystemChildsToOpcUaModel(void)
	{
		// get files and directory names from file system interface
		std::vector<std::string> files;
		std::vector<std::string> directories;
		fileSystemIf_->getChilds(path_.string(), files, directories);

		// create files
		for (auto fileName : files) {
			if (!createOpcUaFileObject(fileName)) {
				return false;
			}
		}

		// create directories
		for (auto directoryName : directories) {
			auto fileDirectoryObject = createOpcUADirectoryObject(directoryName);
			if (!fileDirectoryObject) return false;

			if (!fileDirectoryObject->addFileSystemChildsToOpcUaModel()) {
				return false;
			}
		}

		return true;
	}

	bool
	FileDirectoryObject::addToOpcUaModel(
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
	FileDirectoryObject::deleteFromOpcUaModel(void)
	{
		// remove all file directory objects from opc ua information model
		while (!fileDirectoryMap_.empty()) {
			auto fileDirectoryObject = fileDirectoryMap_.begin()->second;
			if (!deleteOpcUaDirectoryObject(fileDirectoryObject)) {
				Log(Error, "remove directory object error");
				return false;
			}
		}

		// remove all file objects from opc ua information model
		while (!fileMap_.empty()) {
			auto fileObject = fileMap_.begin()->second;
			if (!deleteOpcUaFileObject(fileObject)) {
				Log(Error, "remove file object error");
				return false;
			}
		}

		return true;
	}

    void
	FileDirectoryObject::call_CreateDirectory_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext)
    {
    	bool rc = true;

    	OpcUaVariant::SPtr variantInput;

		// Check number of input parameters
		if (applicationMethodContext->inputArguments_->size() != 1) {
			Log(Error, "number of input arguments error in create directory method");
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}

		// Get input parameter [0]: DirectoryName
		applicationMethodContext->inputArguments_->get(0,variantInput);
		if (variantInput->variantType() != OpcUaBuildInType_OpcUaString) {
			Log(Error, "get input argument [0] error in create directory method");
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}
		std::string directoryName = variantInput->getSPtr<OpcUaString>()->toStdString();

		// create new directory instance
		rc = fileSystemIf_->createDirectory(path_.string(), directoryName);
		if (!rc) {
			Log(Error, "create directory instance error")
				.parameter("Path", path_.string())
				.parameter("DirectoryName", directoryName);
			applicationMethodContext->statusCode_ = BadInternalError;
			return;
		}

		// create new file directory object
		auto fileDirectoryObject = createOpcUADirectoryObject(directoryName);
		if (!fileDirectoryObject) {
			Log(Error, "create opc ua directory object error")
				.parameter("Path", path_.string())
				.parameter("DirectoryName", directoryName);
			applicationMethodContext->statusCode_ = BadInternalError;
			return;
		}

		// add node id to result
		applicationMethodContext->outputArguments_->resize(1);

		// Set result code
		OpcUaVariant::SPtr variant = boost::make_shared<OpcUaVariant>();
		variant->variant(boost::make_shared<OpcUaNodeId>(fileDirectoryObject->nodeId()));
		applicationMethodContext->outputArguments_->push_back(variant);
		applicationMethodContext->statusCode_ = Success;
    }

    void
	FileDirectoryObject::call_CreateFile_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext)
    {
    	bool rc  = true;
    	OpcUaVariant::SPtr variantInput;

		// Check number of input parameters
		if (applicationMethodContext->inputArguments_->size() != 2) {
			Log(Error, "number of input arguments error in create file method");
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}

		// Get input parameter [0]: FileName
		applicationMethodContext->inputArguments_->get(0,variantInput);
		if (variantInput->variantType() != OpcUaBuildInType_OpcUaString) {
			Log(Error, "get input argument [0] error in create file method");
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}
		std::string fileName = variantInput->getSPtr<OpcUaString>()->toStdString();

		// Get input parameter [1]: RequestFileOpen
		applicationMethodContext->inputArguments_->get(1,variantInput);
		if (variantInput->variantType() != OpcUaBuildInType_OpcUaBoolean) {
			Log(Error, "get input argument [1] error in create file method");
				applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
			}
		bool requestFileOpen = variantInput->get<OpcUaBoolean>();

		// create new file object
		auto fileObject = createOpcUaFileObject(fileName);
		if (!fileObject) {
			Log(Error, "create opc ua file object error")
				.parameter("Path", path_.string())
				.parameter("FileName", fileName);
			applicationMethodContext->statusCode_ = BadInternalError;
			return;
		}

		// create new file instance
		rc = fileObject->create();
		if (!rc) {
			Log(Error, "create file instance error")
				.parameter("Path", path_.string())
				.parameter("FileName", fileName);
			applicationMethodContext->statusCode_ = BadInternalError;
			return;
		}

		// open file instance
		uint32_t fileHandle = 0;
		if (requestFileOpen) {
			rc = fileObject->open(fileHandle);
			if (!rc) {
				Log(Error, "open file instance error")
					.parameter("Path", path_.string())
					.parameter("FileName", fileName);
				applicationMethodContext->statusCode_ = BadInternalError;
				return;
			}
		}

		// add node id to result
		applicationMethodContext->outputArguments_->resize(2);

		// Set result code
		OpcUaVariant::SPtr variant = boost::make_shared<OpcUaVariant>();
		variant->variant(boost::make_shared<OpcUaNodeId>(fileObject->nodeId()));
		applicationMethodContext->outputArguments_->push_back(variant);

		// Set file handle
		variant = boost::make_shared<OpcUaVariant>();
		variant->variant(fileHandle);
		applicationMethodContext->outputArguments_->push_back(variant);

		applicationMethodContext->statusCode_ = Success;
    }

    void
	FileDirectoryObject::call_Delete_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext)
    {
    	OpcUaVariant::SPtr variantInput;

		// Check number of input parameters
		if (applicationMethodContext->inputArguments_->size() != 1) {
			Log(Error, "number of input arguments error in delete method");
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}

		// Get input parameter [0]: ObjectToDelete
		applicationMethodContext->inputArguments_->get(0,variantInput);
		if (variantInput->variantType() != OpcUaBuildInType_OpcUaNodeId) {
			Log(Error, "get input argument [0] error in delete method");
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}
		auto objectToDelete = variantInput->getSPtr<OpcUaNodeId>();

		auto itDirectory = fileDirectoryMap_.find(*objectToDelete.get());
		auto itFile = fileMap_.find(*objectToDelete.get());

		// Check if node id is a directory object
		if (itDirectory  != fileDirectoryMap_.end()) {
			auto fileDirectoryObject = itDirectory->second;
			bool rc = deleteOpcUaDirectoryObject(fileDirectoryObject);
			if (!rc) {
				Log(Error, "delete method error")
					.parameter("NodeId", *objectToDelete.get());
				applicationMethodContext->statusCode_ = BadInternalError;
				return;
			}
		}

		// Check if node id is a file object
		else if (itFile != fileMap_.end()) {
			auto fileObject = itFile->second;
			bool rc = deleteOpcUaFileObject(fileObject);
			if (!rc) {
				Log(Error, "delete method error")
					.parameter("NodeId", *objectToDelete.get());
				applicationMethodContext->statusCode_ = BadInternalError;
				return;
			}
		}

		else {
			applicationMethodContext->statusCode_ = BadNotFound;
			return;
		}

		applicationMethodContext->statusCode_ = Success;
    }

    void
	FileDirectoryObject::call_MoveOrCopy_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext)
    {
    	applicationMethodContext->statusCode_ = BadNotImplemented;
    }

	FileDirectoryObject::SPtr
	FileDirectoryObject::createOpcUADirectoryObject(
		const std::string& directoryName
	)
	{
		// create new file directory object
		std::filesystem::path newPath = path_;
		newPath.append(directoryName);
		auto fileDirectoryObject = boost::make_shared<FileDirectoryObject>(newPath);
		bool rc = fileDirectoryObject->init(
			fileSystemIf_,
			applicationServiceIf_,
			namespaceName_
		);
		if (rc == false) {
			Log(Error, "init directory error")
				.parameter("Path", newPath);
			return nullptr;
		}
		rc  = fileDirectoryObject->addToOpcUaModel(
			directoryName,
			nodeId(),
			OpcUaNodeId((uint32_t)OpcUaId_HasComponent)
		);
		if (rc == false) {
			Log(Error, "add directory to opc ua model error")
				.parameter("Path", newPath);
			return nullptr;
		}

		// add new directory map
		fileDirectoryMap_.insert(std::make_pair(fileDirectoryObject->nodeId(), fileDirectoryObject));
		return fileDirectoryObject;
	}

	bool
	FileDirectoryObject::deleteOpcUaDirectoryObject(
			FileDirectoryObject::SPtr& fileDirectoryObject
	)
	{
		// remove all childs in directory
		if (!fileDirectoryObject->deleteFromOpcUaModel()) {
			Log(Error, "cannot delete child directory")
				.parameter("DirectoryNodeId", fileDirectoryObject->nodeId());
			return false;
		}

		// remove object from opc ua model
		DeleteNodeInstance deleteNodeInstance;
		deleteNodeInstance.node(fileDirectoryObject->nodeId());

		if (!deleteNodeInstance.query(applicationServiceIf_)) {
			Log(Error, "delete node instance error")
				.parameter("DirectoryNodeId", fileDirectoryObject->nodeId());
			return false;
		}

		// remove object from file directory map
		fileDirectoryMap_.erase(fileDirectoryObject->nodeId());

		// Delete directory from filesystem
		bool rc = fileSystemIf_->remove(fileDirectoryObject->path().string());
		if (!rc) {
			Log(Error, "delete directory from filesystem error")
				.parameter("Path", fileDirectoryObject->path().string());
			return false;
		}

		return true;
	}

	FileObject::SPtr
	FileDirectoryObject::createOpcUaFileObject(
		const std::string& fileName
	)
	{
		// create new file object
		std::filesystem::path newPath = path_;
		newPath.append(fileName);
		auto fileObject = boost::make_shared<FileObject>(newPath);
		bool rc = fileObject->init(fileSystemIf_, applicationServiceIf_, namespaceName_);
		if (rc == false) {
			Log(Error, "init file error")
				.parameter("Path", newPath);
			return nullptr;
		}
		rc  = fileObject->addToOpcUaModel(
			fileName,
			nodeId(),
			OpcUaNodeId((uint32_t)OpcUaId_HasComponent)
		);
		if (rc == false) {
			Log(Error, "add file to opc ua model error")
				.parameter("Path", newPath);
			return nullptr;
		}

		// add new directory map
		fileMap_.insert(std::make_pair(fileObject->nodeId(), fileObject));

		return fileObject;
	}

	bool
	FileDirectoryObject::deleteOpcUaFileObject(
		FileObject::SPtr& fileObject
	)
	{
		// remove object from opc ua model
		if (!fileObject->deleteFromOpcUaModel()) {
			Log(Error, "delete file from opc ua model");
			return false;
		}

		// remove object from file map
		fileMap_.erase(fileObject->nodeId());

		// Delete file from filesystem
		bool rc = fileSystemIf_->remove(fileObject->path().string());
		if (!rc) {
			Log(Error, "delete file from filesystem error")
				.parameter("Path", fileObject->path().string());
			return false;
		}

		return true;
	}

}
