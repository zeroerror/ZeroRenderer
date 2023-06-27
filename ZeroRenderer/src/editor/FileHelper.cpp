#include "FileHelper.h"
#include <fstream>
#include <iostream>

bool FileHelper::FileExist(const std::string& path) {
	std::ifstream is(path);
	return is.good();
}

void FileHelper::CreateFile(const std::string& path) {
	if(FileExist(path)) {
		return;
	}
}

void FileHelper::ReadCharsFrom(const std::string& path, unsigned char* chars) {
	std::ifstream is(path);
	if (!is.good()) {
		return;
	}

	char ch;
	int index = 0;
	while (is.get(ch)) {
		chars[index++] = static_cast<unsigned char>(ch);
	}
	is.close();
}

void FileHelper::WriteCharsTo(const std::string& path, const unsigned char* chars, const FileOperation& fileOperation) {
	std::ofstream os(path);
	if (!os.good()) {
		std::cout << " ****************** FileHelper::WriteCharsTo | file create failed! " <<std::endl;
		return;
	}

	int index = 0;
	unsigned char c = chars[index];
	while (c != '\0') {
		os << c;
		c = chars[++index];
	}
	os.close();
}
