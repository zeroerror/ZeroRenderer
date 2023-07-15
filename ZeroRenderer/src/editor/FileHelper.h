#pragma once
#include <string>
#include <vector>
using namespace std;

class FileHelper {

public:
	static unsigned int GetFileCharSize(const string& path);
	static bool FileExist(const string& path);
	static void CreateFile(const string& path);
	static bool ReadCharsFrom(const string& path, unsigned char* chars);
	static void WriteCharsTo(const string& path, const unsigned char* chars);
	static void DeleteFile(const string& filePath);
	static bool PathEquals(const string& path1, const string& path2);
	static string NormalizedPath(const string& path);
	static void NormalizePath(string& path);
	static void ForeachFilePath(const string& path, void(*callback)(const string& path));
	static void ForeachFilePath(const string& path, const unsigned int& suffixFlag, void(*callback)(const string& path));
	static void GetFilePaths(const string& path, const unsigned int& suffixFlag, vector<string>& filePaths);

};

