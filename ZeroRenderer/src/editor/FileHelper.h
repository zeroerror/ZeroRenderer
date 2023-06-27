#pragma once
#include <string>

enum FileType {
	Shader,
	Texture,
	Material,
	Model,
	Scene
};

enum FileOperation {
	None,
	CreateWhenNotExist,
};

class FileHelper {

public:
	static bool FileExist(const std::string& path);
	static void CreateFile(const std::string& path);
	static void ReadCharsFrom(const std::string& path, unsigned char* chars);
	static void WriteCharsTo(const std::string& path, const unsigned char* chars, const FileOperation& fileOperation);

};

