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

#ifndef __OpcUaFileServer_FileObject_h__
#define __OpcUaFileServer_FileObject_h__

#include <filesystem>

#include "OpcUaStackServer/StandardObjectType/FileType.h"
#include "OpcUaStackServer/Application/ApplicationServiceIf.h"

#include "OpcUaFileServer/DataLayer/FileSystemIf.h"

namespace OpcUaFileServer
{

	class FileObject
	: public OpcUaStackServer::FileType
	, public boost::enable_shared_from_this<FileObject>
	{
	  public:
		using SPtr = boost::shared_ptr<FileObject>;
		using Map = std::map<OpcUaStackCore::OpcUaNodeId, SPtr>;
		using FileHandleSet = std::set<uint32_t>;

		FileObject(const std::filesystem::path& path);
		virtual ~FileObject(void);

		bool init(
			FileSystemIf::SPtr fileSystemIf,
			OpcUaStackServer::ApplicationServiceIf* applicationServiceIf,
			const std::string& namespaceName
		);
		const std::filesystem::path path(void);

		bool addToOpcUaModel(
			const std::string& displayName,
			const OpcUaStackCore::OpcUaNodeId& parentNodeId,
			const OpcUaStackCore::OpcUaNodeId& referenceTypeNodeId
		);
		bool deleteFromOpcUaModel(void);

	    bool create(void);
	    bool open(uint32_t& handle);
	    bool close(uint32_t handle);

	  private:
		std::filesystem::path path_ = {};
		FileSystemIf::SPtr fileSystemIf_ = nullptr;
		OpcUaStackServer::ApplicationServiceIf* applicationServiceIf_ = nullptr;
		std::string namespaceName_ = "";

		FileHandleSet fileHandleSet_;

        virtual void call_Close_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext) override;
        virtual void call_GetPosition_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext) override;
        virtual void call_Open_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext) override;
        virtual void call_Read_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext) override;
        virtual void call_SetPosition_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext) override;
        virtual void call_Write_Method(OpcUaStackCore::ApplicationMethodContext* applicationMethodContext) override;
	};

}

#endif
