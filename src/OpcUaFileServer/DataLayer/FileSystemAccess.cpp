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
		bool rc = true;
		basePath_ = basePath;

		// check if base path exist
		try {
			rc  = std::filesystem::exists(basePath);
		} catch(std::filesystem::filesystem_error const& ex) {
			Log(Error, "filesystem exists call error")
				.parameter("What", ex.what())
				.parameter("CodeValue", ex.code().value())
				.parameter("CodeMessage", ex.code().message());
			rc = false;
		}
		if (!rc) {
			Log(Error, "base file system path do not exist")
				.parameter("BasePath", basePath);
			return false;
		}

		// Check if base path is a directory
		try {
			rc = std::filesystem::is_directory(basePath);
		} catch(std::filesystem::filesystem_error const& ex) {
			Log(Error, "filesystem is_directory call error")
				.parameter("What", ex.what())
				.parameter("CodeValue", ex.code().value())
				.parameter("CodeMessage", ex.code().message());
			rc = false;
		}
		if (!rc) {
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
		bool rc = true;
		std::filesystem::path absPath = basePath_;
		absPath.append(path);

		// Check if path exist
		try {
			rc = std::filesystem::exists(std::filesystem::path(absPath));
		} catch(std::filesystem::filesystem_error const& ex) {
			Log(Error, "filesystem exists call error")
				.parameter("What", ex.what())
				.parameter("CodeValue", ex.code().value())
				.parameter("CodeMessage", ex.code().message());
			rc = false;
		}
		if (!rc) {
			Log(Error, "get childs error, because base path not exist")
				.parameter("Path", absPath);
			return;
		}

		// Check if path is a directory
		try {
			rc  = std::filesystem::is_directory(std::filesystem::path(absPath));
		} catch(std::filesystem::filesystem_error const& ex) {
			Log(Error, "filesystem is_directory call error")
				.parameter("What", ex.what())
				.parameter("CodeValue", ex.code().value())
				.parameter("CodeMessage", ex.code().message());
			rc = false;
		}
		if (!rc) {
			Log(Error, "get childs error, because base path is not a directory")
				.parameter("Path", absPath);
			return;
		}

		// Get all directory childs and file childs from directory
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
		bool rc = true;

		// Create access path
		std::filesystem::path absPath(basePath_);
		absPath.append(path);

		// Check if path exist
		try {
			rc = std::filesystem::exists(std::filesystem::path(absPath));
		} catch(std::filesystem::filesystem_error const& ex) {
			Log(Error, "filesystem exists call error")
				.parameter("What", ex.what())
				.parameter("CodeValue", ex.code().value())
				.parameter("CodeMessage", ex.code().message());
			rc = false;
		}
		if (!rc) {
			Log(Error, "cannot create directory, because base path not exist")
				.parameter("Path", absPath)
				.parameter("Directory", directory);
			return false;
		}

		// check if path is a directory
		try {
			rc = std::filesystem::is_directory(std::filesystem::path(absPath));
		} catch(std::filesystem::filesystem_error const& ex) {
			Log(Error, "filesystem is_directory call error")
				.parameter("What", ex.what())
				.parameter("CodeValue", ex.code().value())
				.parameter("CodeMessage", ex.code().message());
			rc = false;
		}
		if (!rc) {
			Log(Error, "cannot create directory, because base path is not a directory")
				.parameter("Path", absPath)
				.parameter("Directory", directory);
			return false;
		}

		std::filesystem::path newDirectory(absPath);
		newDirectory.append(directory);

		// Check if new directory already exist
		try {
			rc  = std::filesystem::exists(std::filesystem::path(newDirectory));
		} catch(std::filesystem::filesystem_error const& ex) {
			Log(Error, "filesystem exists call error")
				.parameter("What", ex.what())
				.parameter("CodeValue", ex.code().value())
				.parameter("CodeMessage", ex.code().message());
			rc = false;
		}
		if (rc) {
			Log(Error, "cannot create directory, because target directory already exist")
				.parameter("NewDirectory", newDirectory);
			return false;
		}

		// Create new directory
		try {
			rc  = std::filesystem::create_directory(newDirectory);
		} catch(std::filesystem::filesystem_error const& ex) {
			Log(Error, "filesystem create_directory call error")
				.parameter("What", ex.what())
				.parameter("CodeValue", ex.code().value())
				.parameter("CodeMessage", ex.code().message());
			rc = false;
		}
		if (!rc) {
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
		bool rc = true;

		// Create access path
		std::filesystem::path absPath(basePath_);
		absPath.append(path);

		// Check if path exist
		try {
			rc = std::filesystem::exists(std::filesystem::path(absPath));
		} catch(std::filesystem::filesystem_error const& ex) {
			Log(Error, "filesystem exists call error")
				.parameter("What", ex.what())
				.parameter("CodeValue", ex.code().value())
				.parameter("CodeMessage", ex.code().message());
			rc = false;
		}
		if (!rc) {
			Log(Error, "cannot create file, because base path not exist")
				.parameter("Path", absPath)
				.parameter("File", file);
			return false;
		}

		// Check if path is a directory
		try {
			rc = std::filesystem::is_directory(std::filesystem::path(absPath));
		} catch(std::filesystem::filesystem_error const& ex) {
			Log(Error, "filesystem is_directory call error")
				.parameter("What", ex.what())
				.parameter("CodeValue", ex.code().value())
				.parameter("CodeMessage", ex.code().message());
			rc = false;
		}
		if (!rc) {
			Log(Error, "cannot create file, because base path is not a directory")
				.parameter("Path", absPath)
				.parameter("File", file);
			return false;
		}

		std::filesystem::path newFile(absPath);
		newFile.append(file);

		// Check if new file already exist
		try {
			rc = std::filesystem::exists(std::filesystem::path(newFile));
		} catch(std::filesystem::filesystem_error const& ex) {
			Log(Error, "filesystem exists call error")
				.parameter("What", ex.what())
				.parameter("CodeValue", ex.code().value())
				.parameter("CodeMessage", ex.code().message());
			rc = false;
		}
		if (rc) {
			Log(Error, "cannot create file, because target file already exist")
				.parameter("NewFile", newFile);
			return false;
		}
		rc = true;

		// Create new file
		std::fstream fs;
		try {
			fs.open(newFile.string(), std::ios_base::out | std::ios_base::in | std::ios_base::app);
			if (!fs.is_open()) {
				Log(Error, "create file failed")
					.parameter("NewFile", newFile);
				return false;
			}
			fs.close();
		} catch (const std::exception& e) {
			Log(Error, "fstream operation error")
				.parameter("What", e.what());
			rc  = false;
		}

		return rc;
	}

	bool
	FileSystemAccess::remove(
		const std::string& path
	)
	{
		bool rc = true;

		// Create access path
		std::filesystem::path absPath(basePath_);
		absPath.append(path);

		// Check ic path exist
		try {
			rc = std::filesystem::exists(std::filesystem::path(absPath));
		} catch(std::filesystem::filesystem_error const& ex) {
			Log(Error, "filesystem exists call error")
				.parameter("What", ex.what())
				.parameter("CodeValue", ex.code().value())
				.parameter("CodeMessage", ex.code().message());
			rc = false;
		}
		if (!rc) {
			Log(Error, "cannot delete directory/file, because path not exist")
				.parameter("Path", absPath);
			return false;
		}

		// Check if path is a directory or a regular file
		try {
			rc  = !std::filesystem::is_directory(std::filesystem::path(absPath)) &&
				  !std::filesystem::is_regular_file(std::filesystem::path(absPath));
		} catch(std::filesystem::filesystem_error const& ex) {
			Log(Error, "filesystem is_directory or is_regular_file call error")
				.parameter("What", ex.what())
				.parameter("CodeValue", ex.code().value())
				.parameter("CodeMessage", ex.code().message());
			rc = false;
		}
		if (rc) {
			Log(Error, "cannot delete directory/file, because path is not a directory or regular file")
				.parameter("Path", absPath);
			return false;
		}

		// Delete directory/file
		try {
			rc = std::filesystem::remove_all(absPath);
		} catch(std::filesystem::filesystem_error const& ex) {
			Log(Error, "filesystem remove_all call error")
				.parameter("What", ex.what())
				.parameter("CodeValue", ex.code().value())
				.parameter("CodeMessage", ex.code().message());
			rc = false;
		}
		if (!rc) {
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
		bool rc = true;

		// Create access path
		std::filesystem::path absPath(basePath_);
		absPath.append(path);

		// Check if path exist
		try {
			rc = std::filesystem::exists(std::filesystem::path(absPath));
		} catch(std::filesystem::filesystem_error const& ex) {
			Log(Error, "filesystem exists call error")
				.parameter("What", ex.what())
				.parameter("CodeValue", ex.code().value())
				.parameter("CodeMessage", ex.code().message());
			rc = false;
		}
		if (!rc) {
			Log(Error, "cannot open file, because base path not exist")
				.parameter("Path", absPath)
				.parameter("File", file);
			return false;
		}

		// Check if path is a directory
		try {
			rc  = std::filesystem::is_directory(std::filesystem::path(absPath));
		} catch(std::filesystem::filesystem_error const& ex) {
			Log(Error, "filesystem is_directory call error")
				.parameter("What", ex.what())
				.parameter("CodeValue", ex.code().value())
				.parameter("CodeMessage", ex.code().message());
			rc = false;
		}
		if (!rc) {
			Log(Error, "cannot open file, because base path is not a directory")
				.parameter("Path", absPath)
				.parameter("File", file);
			return false;
		}

		std::filesystem::path newFile(absPath);
		newFile.append(file);

		// Check if file exist
		try {
			rc = std::filesystem::exists(std::filesystem::path(newFile));
		} catch(std::filesystem::filesystem_error const& ex) {
			Log(Error, "filesystem exists call error")
				.parameter("What", ex.what())
				.parameter("CodeValue", ex.code().value())
				.parameter("CodeMessage", ex.code().message());
			rc = false;
		}
		if (!rc) {
			Log(Error, "cannot open file, because target file not exist")
				.parameter("NewFile", newFile);
			return false;
		}

		// Check if file is a regular file
		try {
			rc  = std::filesystem::is_regular_file(newFile);
		} catch(std::filesystem::filesystem_error const& ex) {
			Log(Error, "filesystem is_regular call error")
				.parameter("What", ex.what())
				.parameter("CodeValue", ex.code().value())
				.parameter("CodeMessage", ex.code().message());
			rc = false;
		}
		if (!rc) {
			Log(Error, "cannot open file, because target file is not a regular file")
				.parameter("NewFile", newFile);
			return false;
		}

		// Open file
		auto fh = boost::make_shared<FileHandle>();
		try {
			fh->fs_.open(newFile.string(), std::ios_base::out | std::ios_base::in /*| std::ios_base::app*/);
			if (!fh->fs_.is_open()) {
				Log(Error, "open file failed")
					.parameter("NewFile", newFile);
				return false;
			}
		} catch (const std::exception& e) {
			Log(Error, "fstream operation error")
				.parameter("What", e.what());
			rc = false;
		}
		if (!rc) {
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
		try {
			it->second->fs_.close();
		} catch (const std::exception& e) {
			Log(Error, "fstream operation error")
				.parameter("What", e.what());
			return false;
		}

		// Remove file handle from file handle map
		fileHandleMap_.erase(fileHandle);

		return true;
	}

	bool
	FileSystemAccess::writeFile(
		uint32_t fileHandle,
		const std::string& data
	)
	{
		// Check file handle
		auto it = fileHandleMap_.find(fileHandle);
		if (it == fileHandleMap_.end()) {
			Log(Error, "file handle not exist")
				.parameter("FileHanlde", fileHandle);
		}
		auto fh = it->second;

		// Check file
		if (!fh->fs_.is_open()) {
			Log(Error, "file write failed, because file not open")
				.parameter("FileHandle", fileHandle);
			return false;
		}

		// Write data to file
		try {
			fh->fs_ << data;
		} catch (const std::exception& e) {
			Log(Error, "fstream write data error")
				.parameter("What", e.what())
				.parameter("FileHandle", fileHandle);
			return false;
		}

		return true;
	}

	bool
	FileSystemAccess::readFile(
		uint32_t fileHandle,
		int32_t length,
		std::string& data
	)
	{
		std::stringstream ss;

		// Check file handle
		auto it = fileHandleMap_.find(fileHandle);
		if (it == fileHandleMap_.end()) {
			Log(Error, "file handle not exist")
				.parameter("FileHanlde", fileHandle);
		}
		auto fh = it->second;

		// Read data from file
		try {
			char c;
			while (!fh->fs_.eof()) {
				fh->fs_.get(c);
				ss << c;
			}
			data = ss.str();
		} catch (const std::exception& e) {
			Log(Error, "fstream read data error")
				.parameter("What", e.what())
				.parameter("FileHandle", fileHandle);
			return false;
		}

		return true;
	}

	bool
	FileSystemAccess::isReadable(
		const std::string& path,
		const std::string& file
	)
	{
		// FIXME: TODO
		return true;
	}

	bool
	FileSystemAccess::isWriteable(
		const std::string& path,
		const std::string& file
	)
	{
		// FIXME: TODO
		return true;
	}

}
