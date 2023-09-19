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

#ifndef __OpcUaFileServer_FileMode_h__
#define __OpcUaFileServer_FileMode_h__

#include <bitset>
#include <iostream>

namespace OpcUaFileServer
{

	class FileMode
	{
	  public:
		const uint32_t Read = 1;
		const uint32_t Write = 2;
		const uint32_t EraseExisting = 4;
		const uint32_t Append = 8;

		FileMode(void);
		FileMode(const FileMode& fileMode);
		~FileMode(void);

		bool validate(uint32_t mode);
		bool setMode(uint32_t mode);
		bool setMode(const std::string& modeStr);
		bool isMode(uint32_t mode);
		void out(std::ostream& os) const;

		friend std::ostream& operator<<(std::ostream& os, const FileMode& fileMode) {
			fileMode.out(os);
			return os;
		}

	  private:
		std::bitset<4> mode_ = std::bitset<4>(std::string("0000"));
	};

}

#endif
