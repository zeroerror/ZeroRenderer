#include "MaterialMetadata.h"
#include <sstream>
#include "FileHelper.h"
#include "Database.h"

void MaterialMetadata::SerializeTo(const std::string& path) {
	std::stringstream ss;
	ss << "guid: " << guid << std::endl;
	std::string result = ss.str();
	size_t len = result.length() + 1;
	unsigned char* charResult = new unsigned char[len];
	memcpy(charResult, result.c_str(), len);
	FileHelper::WriteCharsTo(path, charResult);

	ss << "MaterialMetadata::Serialize | guid: " << guid << std::endl;
}

void MaterialMetadata::DeserializeFrom(const std::string& path) {
	unsigned char* res = new unsigned char[1024];
	FileHelper::ReadCharsFrom(path, res);
	std::string str(reinterpret_cast<char*>(res));
	std::stringstream ss(str);
	std::string line;
	while (std::getline(ss, line)) {
		std::istringstream iss(line);
		std::string key;
		if (!(iss >> key)) {
			break;
		}
		if (key == "guid:") {
			iss >> guid;
		}
	}
}