#include "IOManager.h"

#include <filesystem>
#include <fstream>

// Namespace alias
namespace fs = std::filesystem;

namespace Dengine {
	bool IOManager::readFileToBuffer(std::vector<unsigned char>& buffer, std::string filePath) {
		//open file to get exact contents including newlines
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail()) {
			perror(filePath.c_str());
			return false;
		}

		//figure out the size of the file so we can resize our buffer
		file.seekg(0, std::ios::end);	//put file pointer to end of file
		int fileSize = file.tellg();	//how many bytes have we passed so far and since we're at end of file...
		file.seekg(0, std::ios::beg);	//go back to beginning of file so we can read the contents

		fileSize -= file.tellg();		//remove any file header content from the file size(not necessary)

		buffer.resize(fileSize);

		file.read((char*)&(buffer[0]), fileSize);

		file.close();

		return true;
	}

	bool IOManager::readFileToBuffer(std::string& buffer, std::string filePath)
	{
		//open file to get exact contents including newlines
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail()) {
			perror(filePath.c_str());
			return false;
		}

		//figure out the size of the file so we can resize our buffer
		file.seekg(0, std::ios::end);	//put file pointer to end of file
		int fileSize = file.tellg();	//how many bytes have we passed so far and since we're at end of file...
		file.seekg(0, std::ios::beg);	//go back to beginning of file so we can read the contents

		fileSize -= (unsigned int) file.tellg();		//remove any file header content from the file size(not necessary)

		buffer.resize(fileSize);

		file.read((char*)&(buffer[0]), fileSize);

		file.close();

		return true;
	}

	bool IOManager::getDirectoryEntries(const char* path, std::vector<DirEntry>& rvEntries) {
		auto dpath = fs::path(path);
		// Must be directory
		if (!fs::is_directory(dpath)) return false;

		for (auto it = fs::directory_iterator(dpath); it != fs::directory_iterator(); ++it) {
			rvEntries.emplace_back();
			rvEntries.back().path = it->path().string();
			if (is_directory(it->path())) {
				rvEntries.back().isDirectory = true;
			}
			else {
				rvEntries.back().isDirectory = false;
			}
		}
		return true;
	}

	bool IOManager::makeDirectory(const char* path) {
		return fs::create_directory(fs::path(path));
	}
}