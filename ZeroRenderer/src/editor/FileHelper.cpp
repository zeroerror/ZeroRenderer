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

bool FileHelper::PathEquals(const std::string& path1, const std::string& path2) {
	string::size_type p1 = 0;
	string::size_type p2 = 0;

	string::size_type p1Len = path1.length();
	string::size_type p2Len = path2.length();
	if(p1Len != p2Len) {
		return false;
	}

	while (p1 < p1Len && p2 < p2Len) {
		char c1 = path1[p1];
		char c2 = path2[p2];
		if(c1 == '\\') c1 = '/';
		if(c2 == '\\') c2 = '/';
		if (c1 != c2) {
			return false;
		}

		p1++;
		p2++;
	}

	return true;
}