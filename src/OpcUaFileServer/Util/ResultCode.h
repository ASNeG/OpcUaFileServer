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

#ifndef __OpcUaFileServer_ResultCode_h__
#define __OpcUaFileServer_ResultCode_h__

#include <string>
#include <map>

namespace OpcUaFileServer
{

	enum class ResultCode {
		OK = 0,
		BasePathNotExist = 1,
		CreatePathError = 2,
		PathExist = 3,
		CreateDirectoryError = 4,
		InvalidPathError = 5,
		FileExist = 6,
		CreateFileError = 7,
		FileNotExist = 8,
		FileAccessError = 9,
		FileRemoveError = 10,
		FileNotOpen = 11,
		FileOpenError = 12,
		FileCloseError = 13,
		FileWriteError = 14,
		FileReadError = 15
	};

	class ResultCodeMap
	{
	  public:
		static std::string resultCode2String(ResultCode resultCode);

	  private:
		static std::map<ResultCode, std::string> resultCodeMap_;
	};

}

#endif
