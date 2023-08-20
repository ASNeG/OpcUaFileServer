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

#include <iostream>

#include <boost/make_shared.hpp>

#include "OpcUaStackCore/Base/Log.h"

#include "OpcUaFileServer/DataLayer/FileSystemAccess.h"

using namespace OpcUaStackCore;

namespace OpcUaFileServer
{

	FileSystemAccess::FileSystemAccess(void)
	: FileSystemIf()
	{
	}

	FileSystemAccess::~FileSystemAccess(void)
	{
	}

	bool
	FileSystemAccess::init(const std::filesystem::path& basePath)
	{
		basePath_ = basePath;

		// check if base path exist
		if (!std::filesystem::exists(basePath)) {
			Log(Error, "base file system path do not exist")
				.parameter("BasePath", basePath);
			return false;
		}
		if (!std::filesystem::is_directory(basePath)) {
			Log(Error, "base file system path do not exist")
				.parameter("BasePath", basePath);
			return false;
		}

		return true;
	}

	void
	FileSystemAccess::getChilds(
		const std::string& path,
		std::vector<std::string>& files,
		std::vector<std::string>& directories
	)
	{
		std::filesystem::path absPath = basePath_;
		absPath.append(path);

		// Check path
		if (!std::filesystem::exists(std::filesystem::path(absPath))) {
			Log(Error, "get childs error, because base path not exist")
				.parameter("Path", absPath);
			return;
		}
		if (!std::filesystem::is_directory(std::filesystem::path(absPath))) {
			Log(Error, "get childs error, because base path is not a directory")
				.parameter("Path", absPath);
			return;
		}

		for (const auto & entry : std::filesystem::directory_iterator(absPath)) {
			if (std::filesystem::is_directory(entry.path())) {
				directories.push_back(entry.path().filename().string());
			}
			else if (std::filesystem::is_regular_file(entry.path())) {
				files.push_back(entry.path().filename().string());
			}
		}
	}

	bool
	FileSystemAccess::createDirectory(
		const std::string& path,
		const std::string& directory
	)
	{
		// Create access path
		std::filesystem::path absPath(basePath_);
		absPath.append(path);

		// Check path
		if (!std::filesystem::exists(std::filesystem::path(absPath))) {
			Log(Error, "cannot create directory, because base path not exist")
				.parameter("Path", absPath)
				.parameter("Directory", directory);
			return false;
		}
		if (!std::filesystem::is_directory(std::filesystem::path(absPath))) {
			Log(Error, "cannot create directory, because base path is not a directory")
				.parameter("Path", absPath)
				.parameter("Directory", directory);
			return false;
		}

		std::filesystem::path newDirectory(absPath);
		newDirectory.append(directory);

		if (std::filesystem::exists(std::filesystem::path(newDirectory))) {
			Log(Error, "cannot create directory, because target directory already exist")
				.parameter("NewDirectory", newDirectory);
			return false;
		}

		// Create new directory
		if (!std::filesystem::create_directory(newDirectory)) {
			Log(Error, "cannot create directory")
				.parameter("Path", absPath)
				.parameter("Directory", directory);
			return false;
		}

		return true;
	}

	bool
	FileSystemAccess::createFile(
		const std::string& path,
		const std::string& file
	)
	{
		// Create access path
		std::filesystem::path absPath(basePath_);
		absPath.append(path);

		// Check path
		if (!std::filesystem::exists(std::filesystem::path(absPath))) {
			Log(Error, "cannot create file, because base path not exist")
				.parameter("Path", absPath)
				.parameter("File", file);
			return false;
		}
		if (!std::filesystem::is_directory(std::filesystem::path(absPath))) {
			Log(Error, "cannot create file, because base path is not a directory")
				.parameter("Path", absPath)
				.parameter("File", file);
			return false;
		}

		std::filesystem::path newFile(absPath);
		newFile.append(file);

		if (std::filesystem::exists(std::filesystem::path(newFile))) {
			Log(Error, "cannot create file, because target file already exist")
				.parameter("NewFile", newFile);
			return false;
		}

		// Create new file
		std::fstream fs;
		fs.open(newFile.string(), std::ios_base::out);
		if (!fs.is_open()) {
			Log(Error, "create file failed")
				.parameter("NewFile", newFile);
			return false;
		}
		fs.close();

		return true;
	}

	bool
	FileSystemAccess::remove(
		const std::string& path
	)
	{
		// Create access path
		std::filesystem::path absPath(basePath_);
		absPath.append(path);

		// Check path
		if (!std::filesystem::exists(std::filesystem::path(absPath))) {
			Log(Error, "cannot delete directory/file, because path not exist")
				.parameter("Path", absPath);
			return false;
		}
		if (!std::filesystem::is_directory(std::filesystem::path(absPath)) &&
			!std::filesystem::is_regular_file(std::filesystem::path(absPath))) {
			Log(Error, "cannot delete directory/file, because path is not a directory or regular file")
				.parameter("Path", absPath);
			return false;
		}

		// Delete directory/file
		if (!std::filesystem::remove_all(absPath)) {
			Log(Error, "cannot remove directory/file")
				.parameter("Path", absPath);
			return false;
		}

		return true;
	}

	bool
	FileSystemAccess::openFile(
		const std::string& path,
		const std::string& file,
		uint32_t& fileHandle
	)
	{
		// Create access path
		std::filesystem::path absPath(basePath_);
		absPath.append(path);

		// Check path
		if (!std::filesystem::exists(std::filesystem::path(absPath))) {
			Log(Error, "cannot open file, because base path not exist")
				.parameter("Path", absPath)
				.parameter("File", file);
			return false;
		}
		if (!std::filesystem::is_directory(std::filesystem::path(absPath))) {
			Log(Error, "cannot open file, because base path is not a directory")
				.parameter("Path", absPath)
				.parameter("File", file);
			return false;
		}

		std::filesystem::path newFile(absPath);
		newFile.append(file);

		if (!std::filesystem::exists(std::filesystem::path(newFile))) {
			Log(Error, "cannot open file, because target file not exist")
				.parameter("NewFile", newFile);
			return false;
		}
		if (!std::filesystem::is_regular_file(newFile)) {
			Log(Error, "cannot open file, because target file is not a regular file")
				.parameter("NewFile", newFile);
			return false;
		}

		// Open file
		auto fh = boost::make_shared<FileHandle>();
		fh->fs_.open(newFile.string(), std::ios_base::out | std::ios_base::in);
		if (!fh->fs_.is_open()) {
			Log(Error, "open file failed")
				.parameter("NewFile", newFile);
			return false;
		}

		fileHandle = fileHandle_;
		fileHandle_++;
		fileHandleMap_.insert(std::make_pair(fileHandle, fh));

		return true;
	}

	bool
	FileSystemAccess::closeFile(
		uint32_t& fileHandle
	)
	{
		// Get file handle from file handle map
		auto it = fileHandleMap_.find(fileHandle);
		if (it == fileHandleMap_.end()) {
			Log(Error, "close file failed, because file handle unknown")
				.parameter("FileHandle", fileHandle);
			return false;
		}

		// Close file
		it->second->fs_.close();

		// Remove file handle from file handle map
		fileHandleMap_.erase(fileHandle);

		return true;
	}
}
