#pragma once

#include <vector>
#include <string>




namespace Dengine {
	struct DirEntry {
		std::string path;
		bool isDirectory;
	};

	class IOManager
	{
	public:
		//improvements: add an int with a default value specifying the number of lines to read
		static bool readFileToBuffer(std::vector<unsigned char>& buffer, std::string filePath);
		static bool readFileToBuffer(std::string& buffer, std::string filePath);
		//gets all director entries in a path specified and returns false if there are no directories
		static bool getDirectoryEntries(const char* path, std::vector<DirEntry>& rvEntries);
		static bool makeDirectory(const char* path);
	};

}