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

#ifndef __FSFileServer_FileServerFSConfig_h__
#define __FSFileServer_FileServerFSConfig_h__

#include <string>

#include <boost/shared_ptr.hpp>

namespace FSFileServer
{

	class FileServerFSConfig
	{
	  public:
		using SPtr = boost::shared_ptr<FileServerFSConfig>;

		FileServerFSConfig(void);
		virtual ~FileServerFSConfig(void);

	  private:
		std::string fileSystemPath_ = "Unknown";
		// FIXME: Default Open File Mode
	};

}

#endif
