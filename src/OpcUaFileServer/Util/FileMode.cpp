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

#include <sstream>

#include "OpcUaFileServer/Util/FileMode.h"

namespace OpcUaFileServer
{

	FileMode::FileMode(void)
	{
	}

	FileMode::FileMode(const FileMode& fileMode)
	{
		mode_ = fileMode.mode_;
	}

	FileMode::~FileMode(void)
	{
	}

	bool
	FileMode::validate(uint32_t mode)
	{
		return (mode & 0xFFFFFFF0) == 0;
	}

	bool
	FileMode::setMode(uint32_t mode)
	{
		if (!validate(mode)) return false;
		if ((mode & Read) == Read) mode_[0] = 1; else mode_[0] = 0;
		if ((mode & Write) == Write) mode_[1] = 1; else mode_[1] = 0;
		if ((mode & EraseExisting) == EraseExisting) mode_[2] = 1; else mode_[2] = 0;
		if ((mode & Append) == Append) mode_[3] = 1; else mode_[3] = 0;
		return true;
	}

	bool
	FileMode::setMode(const std::string& modeStr)
	{
		uint32_t mode;
		std::stringstream ss(modeStr);

		if (!ss >> mode) return false;

		setMode(mode);
		return true;
	}

	bool
	FileMode::isMode(uint32_t mode)
	{
		if ((mode & Read) == Read) return mode_[0] == 1;
		if ((mode & Write) == Write) return mode_[1] == 1;
		if ((mode & EraseExisting) == EraseExisting) return mode_[2] == 1;
		if ((mode & Append) == Append) return mode_[3] == 1;
		return false;
	}

}
