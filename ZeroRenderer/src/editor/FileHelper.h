#pragma once
#include <string>
using namespace std;

class FileHelper {

public:
	static bool FileExist(const string& path);
	static void CreateFile(const string& path);
	static void ReadCharsFrom(const string& path, unsigned char* chars);
	static void WriteCharsTo(const string& path, const unsigned char* chars);
	static void DeleteFile(const string& filePath);
	static bool PathEquals(const string& path1, const string& path2);
	static void NormalizePath(string &path);

};

