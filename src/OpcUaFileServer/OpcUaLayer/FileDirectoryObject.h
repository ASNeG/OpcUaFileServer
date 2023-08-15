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

#ifndef __OpcUaFileServer_FileDirectoryObject_h__
#define __OpcUaFileServer_FileDirectoryObject_h__

#include "OpcUaStackServer/StandardObjectType/FileDirectoryType.h"
#include "OpcUaStackServer/Application/ApplicationServiceIf.h"

namespace OpcUaFileServer
{

	class FileDirectoryObject
	: public OpcUaStackServer::FileDirectoryType
	, public boost::enable_shared_from_this<FileDirectoryObject>
	{
	  public:
		using SPtr = boost::shared_ptr<FileDirectoryObject>;
		using Map = std::map<OpcUaStackCore::OpcUaNodeId, SPtr>;

		FileDirectoryObject(void);
		virtual ~FileDirectoryObject(void);

		bool init(
			OpcUaStackServer::ApplicationServiceIf* applicationServiceIf,
			const std::string& namespaceName
		);

		bool addToOpcUaModel(
			const std::string& displayName,
			const OpcUaStackCore::OpcUaNodeId& parentNodeId,
			const OpcUaStackCore::OpcUaNodeId& referenceTypeNodeId
		);
		bool deleteFromOpcUaModel(void);

        virtual void call_CreateDirectory_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext) override;
        virtual void call_CreateFile_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext) override;
        virtual void call_Delete_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext) override;
        virtual void call_MoveOrCopy_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext) override;

	  private:
		OpcUaStackServer::ApplicationServiceIf* applicationServiceIf_ = nullptr;
		std::string namespaceName_ = "";

		FileDirectoryObject::Map fileDirectoryMap_;
	};

}

#endif
