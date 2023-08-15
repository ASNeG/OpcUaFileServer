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

#include "FileDirectoryObject.h"

using namespace OpcUaStackCore;
using namespace OpcUaStackServer;

namespace OpcUaFileServer
{

	FileDirectoryObject::FileDirectoryObject(void)
	: FileDirectoryType()
	{
	}

	FileDirectoryObject::~FileDirectoryObject(void)
	{
	}

	bool
	FileDirectoryObject::init(
		OpcUaStackServer::ApplicationServiceIf* applicationServiceIf,
		const std::string& namespaceName
	)
	{
		applicationServiceIf_ = applicationServiceIf;
		namespaceName_ = namespaceName;
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
		return true;
	}

    void
	FileDirectoryObject::call_CreateDirectory_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext)
    {
    	OpcUaVariant::SPtr variantInput;

		// Check number of input parameters
		if (applicationMethodContext->inputArguments_->size() != 1) {
			Log(Debug, "number of input arguments error in create directory method");
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}

		// Get input parameter [0]: DirectoryName
		applicationMethodContext->inputArguments_->get(0,variantInput);
		if (variantInput->variantType() != OpcUaBuildInType_OpcUaString) {
			Log(Debug, "get input argument [0] error in create directory method");
			applicationMethodContext->statusCode_ = BadInvalidArgument;
			return;
		}
		std::string directoryName = variantInput->getSPtr<OpcUaString>()->toStdString();

		// create new file directory object
		auto fileDirectoryObject = boost::make_shared<FileDirectoryObject>();
		bool rc = fileDirectoryObject->init(applicationServiceIf_, namespaceName_);
		if (rc == false) {
			Log(Error, "init directory error");
			applicationMethodContext->statusCode_ = BadInternalError;
			return;
		}
		rc  = fileDirectoryObject->addToOpcUaModel(
			directoryName,
			nodeId(),
			OpcUaNodeId((uint32_t)OpcUaId_HasComponent)
		);
		if (rc == false) {
			Log(Error, "init root directory error");
			applicationMethodContext->statusCode_ = BadInternalError;
			return;
		}

		// add new directory map
		fileDirectoryMap_.insert(std::make_pair(fileDirectoryObject->nodeId(), fileDirectoryObject));

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
    }

    void
	FileDirectoryObject::call_Delete_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext)
    {
    }

    void
	FileDirectoryObject::call_MoveOrCopy_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext)
    {
    	applicationMethodContext->statusCode_ = BadNotImplemented;
    }

}
