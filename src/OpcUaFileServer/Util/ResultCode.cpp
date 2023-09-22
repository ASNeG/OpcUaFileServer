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

#include "OpcUaFileServer/Util/ResultCode.h"

namespace OpcUaFileServer
{

	std::map<ResultCode, std::string> ResultCodeMap::resultCodeMap_ {
		{ ResultCode::OK, "OK" },
		{ ResultCode::BasePathNotExist, "BasePathNotExist" },
		{ ResultCode::CreatePathError, "CreatePathError" },
		{ ResultCode::PathExist, "PathExist" },
		{ ResultCode::CreateDirectoryError, "CreateDirectoryError" },
		{ ResultCode::InvalidPathError, "InvalidPathError" },
		{ ResultCode::FileExist, "FileExist" },
		{ ResultCode::CreateFileError, "CreateFileError" },
		{ ResultCode::FileNotExist, "FileNotExist" },
		{ ResultCode::FileAccessError, "FileAccessError" },
		{ ResultCode::FileRemoveError, "FileRemoveError" },
		{ ResultCode::FileNotOpen, "FileNotOpen" },
		{ ResultCode::FileCloseError, "FileCloseError" },
		{ ResultCode::FileOpenError, "FileOpenError" },
		{ ResultCode::FileWriteError, "FileWriteError" },
		{ ResultCode::FileReadError, "FileReadError" }
	};

	std::string
	ResultCodeMap::resultCode2String(ResultCode resultCode)
	{
		std::string resultCodeString = "Unknown ResultCode";

		// Find result code in result code map
		auto it = resultCodeMap_.find(resultCode);
		if (it != resultCodeMap_.end()) {
			resultCodeString = it->second;
		}

		std::stringstream ss;
		ss << "("<< static_cast<std::underlying_type<ResultCode>::type>(resultCode) << ")" << " " << resultCodeString;
		return ss.str();
	}

}
