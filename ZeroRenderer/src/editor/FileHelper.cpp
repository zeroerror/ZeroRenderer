#include "FileHelper.h"
#include <fstream>
#include <iostream>

bool FileHelper::FileExist(const string& path) {
	std::ifstream is(path);
	return is.good();
}

void FileHelper::CreateFile(const string& path) {
	if(FileExist(path)) {
		return;
	}
}

void FileHelper::ReadCharsFrom(const string& path, unsigned char* chars) {
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

void FileHelper::WriteCharsTo(const string& path, const unsigned char* chars) {
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


void FileHelper::DeleteFile(const string& filePath) {
	if (!FileExist(filePath)) {
		return;
	}

	std::remove(filePath.c_str());
	std::cout << "Delete File: " << filePath << std::endl;
}