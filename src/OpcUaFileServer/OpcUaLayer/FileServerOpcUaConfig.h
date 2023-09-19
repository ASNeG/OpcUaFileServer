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

#ifndef __OpcUaFileServer_FileServerOpcUaConfig_h__
#define __OpcUaFileServer_FileServerOpcUaConfig_h__

#include <string>

#include <boost/shared_ptr.hpp>

namespace OpcUaFileServer
{

	class FileServerOpcUaConfig
	{
	  public:
		using SPtr = boost::shared_ptr<FileServerOpcUaConfig>;

		FileServerOpcUaConfig(void);
		virtual ~FileServerOpcUaConfig(void);

	  private:
		std::string namespace_ = "http://ASNeG.com/OpcUaFileServer";
		std::string fileSystemName_ = "FileSystemRoot";
	};

}

#endif
