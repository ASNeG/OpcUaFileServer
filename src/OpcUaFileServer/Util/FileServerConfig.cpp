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

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaStackCore/Base/ConfigXml.h"

#include "OpcUaFileServer/Util/FileServerConfig.h"

using namespace OpcUaStackCore;

namespace OpcUaFileServer
{

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// class FileSystemConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	FileSystemConfig::FileSystemConfig(void)
	{
	}

	FileSystemConfig::~FileSystemConfig(void)
	{
	}

	bool
	FileSystemConfig::parse(Config& config)
	{
		bool rc = true;

		// Get file system name attribute from configuration
		rc = config.getConfigParameter("<xmlattr>.FileSystemName", fileSystemName_);
		if (rc == false) {
			Log(Error, "attribute not found in file system configuration")
				.parameter("Attribute", "FileSystemName");
			return false;
		}

		// Get file system directory attribute from configuration
		rc = config.getConfigParameter("FileSystemDirectory", fileSystemDirectory_);
		if (rc == false) {
			Log(Error, "attribute not found in file system configuration")
				.parameter("FileSystemName", fileSystemName_)
				.parameter("Attribute", "FileSystemDirectory");
			return false;
		}

		// Get namespace name attribute from configuration
		rc = config.getConfigParameter("NamespaceName", namespaceName_);
		if (rc == false) {
			Log(Error, "attribute not found in file system configuration")
				.parameter("FileSystemName", fileSystemName_)
				.parameter("Attribute", "NamespaceName");
			return false;
		}

		// Get create file mode attribute from configuration
		uint32_t mode = 0;
		rc = config.getConfigParameter("CreateFileMode", mode);
		if (rc == false) {
			Log(Error, "attribute not found in file system configuration")
				.parameter("FileSystemName", fileSystemName_)
				.parameter("Attribute", "CreateFileMode");
			return false;
		}
		if (!fileMode_.setMode(mode)) {
			Log(Error, "attribute invalid in file system configuration")
				.parameter("FileSystemName", fileSystemName_)
				.parameter("Attribute", "CreateFileMode")
				.parameter("Value", mode);
			return false;
		}

		// Get read/write lock
		uint32_t rwLock = 0;
		rc = config.getConfigParameter("RWLock", rwLock);
		if (rc == false) {
			Log(Error, "attribute not found in file system configuration")
				.parameter("FileSystemName", fileSystemName_)
				.parameter("Attribute", "RWLock");
			return false;
		}
		rwLock_ = rwLock > 0;

		return rc;
	}

	std::string
	FileSystemConfig::fileSystemName(void)
	{
		return fileSystemName_;
	}

	std::string
	FileSystemConfig::fileSystemDirectory(void)
	{
		return fileSystemDirectory_;
	}

	std::string
	FileSystemConfig::namespaceName(void)
	{
		return namespaceName_;
	}

	bool
	FileSystemConfig::rwLock(void)
	{
		return rwLock_;
	}

	FileMode&
	FileSystemConfig::fileMode(void)
	{
		return fileMode_;
	}


	// ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    //
    // class FileServerConfig
    //
    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
	FileServerConfig::FileServerConfig(void)
	{
	}

	FileServerConfig::~FileServerConfig(void)
	{
	}

	bool
	FileServerConfig::parse(const std::string& fileName)
	{
		bool rc = true;

		// Read configuration file
		Config config;
		ConfigXml configXml;
		rc = configXml.parse(fileName, &config);
		if (rc == false) {
			Log(Error, "read configuration file error")
				.parameter("ConfigurationFile", fileName)
				.parameter("Reason", configXml.errorMessage());
			return false;
		}

		// Find file system entries in configuration
		std::vector<Config> configVec;
		config.getChilds("OpcUaFileServer.Filesystem", configVec);
		if (configVec.size() == 0) {
			Log(Error, "file system configuration not found in configuration file")
				.parameter("ConfigurationFile", fileName)
				.parameter("Attribute", "OpcUaFileServer.Filesystem");
			return false;
		}

		// parse file system entries
		for (auto configEntry: configVec) {
			auto fileSystemConfig = std::make_shared<FileSystemConfig>();

			// parse file system entry
			rc = fileSystemConfig->parse(configEntry);
			if (rc == false) {
				Log(Error, "parse file system entry error")
					.parameter("ConfigurationFile", fileName)
					.parameter("Attribute", "OpcUaFileServer.Filesystem");
				return false;
			}

			// add file system entry to file system entry vector
			fileSystemConfigVec_.push_back(fileSystemConfig);
		}

		return true;
	}

	FileSystemConfig::Vec&
	FileServerConfig::fileSystemConfig(void)
	{
		return fileSystemConfigVec_;
	}

}
