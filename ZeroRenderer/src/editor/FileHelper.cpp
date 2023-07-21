#include "FileHelper.h"
#include "FileSuffix.h"
#include <fstream>
#include <iostream>
#include <filesystem>
using namespace filesystem;
namespace fs = filesystem;

unsigned int FileHelper::GetFileCharSize(const string& path) {
	ifstream file(path, ios::ate | ios::binary);
	if (!file.is_open()) {
		return 0;
	}
	streampos fileSize = file.tellg();
	return static_cast<unsigned int>(fileSize);
}


bool FileHelper::FileExist(const string& path) {
	return filesystem::exists(path);
}

void FileHelper::CreateFile(const string& path) {
}

bool FileHelper::ReadCharsFrom(const string& path, char* chars) {
	ifstream is(path);
	if (!is.good()) {
		is.close();
		cout << " ****************** FileHelper::ReadCharsFrom | file open failed! " << endl;
		return false;
	}

	char ch;
	int index = 0;
	while (is.get(ch)) {
		chars[index++] = ch;
	}
	chars[index] = '\0'; 
	is.close();

	return true;
}

void FileHelper::WriteCharsTo(const string& path, const char* chars) {
	ofstream os(path);
	if (!os.good()) {
		os.close();
		cout << " ****************** FileHelper::WriteCharsTo | file create failed! " << endl;
		return;
	}

	int index = 0;
	char c = chars[index];
	while (c != '\0') {
		os << c;
		c = chars[++index];
	}
	os.close();
}


void FileHelper::DeleteFile(const string& filePath) {
	if (!FileExist(filePath))return;

	remove(filePath.c_str());
	cout << "Delete File: " << filePath << endl;
}

bool FileHelper::PathEquals(const string& path1, const string& path2) {
	string::size_type p1 = 0;
	string::size_type p2 = 0;

	string::size_type p1Len = path1.length();
	string::size_type p2Len = path2.length();
	if (p1Len != p2Len) {
		return false;
	}

	while (p1 < p1Len && p2 < p2Len) {
		char c1 = path1[p1];
		char c2 = path2[p2];
		if (c1 == '\\') c1 = '/';
		if (c2 == '\\') c2 = '/';
		if (c1 != c2) {
			return false;
		}

		p1++;
		p2++;
	}

	return true;
}

void FileHelper::NormalizePath(string& path) {
	for (char& c : path) {
		if (c == '\\') {
			c = '/';
		}
	}
}

string FileHelper::NormalizedPath(const string& path) {
	string norPath = path;
	for (char& c : norPath) {
		if (c == '\\') {
			c = '/';
		}
	}
	return norPath;
}

void FileHelper::ForeachFilePath(const string& path, void(*callback)(const string& path)) {
	if (!FileExist(path))return;

	directory_iterator dirIt = directory_iterator(path);
	for (const auto& entry : dirIt) {
		if (entry.is_directory()) {
			fs::path path = entry.path();
			string pathStr = path.string();
			ForeachFilePath(pathStr, callback);
		}
		else {
			fs::path path = entry.path();
			string pathStr = path.string();
			callback(pathStr);
		}
	}
}

void FileHelper::ForeachFilePath(const string& path, const unsigned int& suffixFlag, void(*callback)(const string& path)) {
	if (!fs::is_directory(path)) {
		string suffix = path.substr(path.find_last_of('.'));
		if (FileSuffix::FileSuffix_Contains(suffixFlag, suffix)) {
			callback(path);
		}
		return;
	}

	directory_iterator dirIt = directory_iterator(path);
	for (const auto& entry : dirIt) {
		if (entry.is_directory()) {
			fs::path path = entry.path();
			string pathStr = path.string();
			ForeachFilePath(pathStr, suffixFlag, callback);
		}
		else {
			fs::path path = entry.path();
			string pathStr = path.string();
			size_t pos = pathStr.find_last_of('.');
			if (pos == string::npos) continue;
			string suffix = pathStr.substr(pos);
			if (FileSuffix::FileSuffix_Contains(suffixFlag, suffix)) {
				callback(pathStr);
			}
		}
	}
}

void FileHelper::GetFilePaths(const string& path, const unsigned int& suffixFlag, vector<string>& filePaths) {
	if (!fs::is_directory(path)) {
		string suffix = path.substr(path.find_last_of('.'));
		if (FileSuffix::FileSuffix_Contains(suffixFlag, suffix)) {
			filePaths.push_back(path);
		}
		return;
	}

	directory_iterator dirIt = directory_iterator(path);
	for (const auto& entry : dirIt) {
		if (entry.is_directory()) {
			fs::path path = entry.path();
			string pathStr = path.string();
			GetFilePaths(pathStr, suffixFlag, filePaths);
		}
		else {
			fs::path path = entry.path();
			string pathStr = path.string();
			size_t pos = pathStr.find_last_of('.');
			if (pos == string::npos) continue;
			string suffix = pathStr.substr(pos);
			if (FileSuffix::FileSuffix_Contains(suffixFlag, suffix)) {
				filePaths.push_back(pathStr);
			}
		}
	}
}