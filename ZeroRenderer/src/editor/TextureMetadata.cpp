#include "TextureMetadata.h"
#include "FileHelper.h"
#include <sstream>
#include "Database.h"

void TextureMetadata::SerializeTo(const string& path) {
	stringstream ss;
	ss << "guid: " << guid << endl;
	ss << "width: " << width << endl;
	ss << "height: " << height << endl;
	string result = ss.str();
	size_t len = result.length() + 1;
	unsigned char* charResult = new unsigned char[len];
	memcpy(charResult, result.c_str(), len);
	FileHelper::WriteCharsTo(path, charResult);
}

void TextureMetadata::DeserializeFrom(const string& path) {
	unsigned char* res = new unsigned char[1024];
	FileHelper::ReadCharsFrom(path, res);
	string str(reinterpret_cast<char*>(res));
	stringstream ss(str);
	string line;
	while (getline(ss, line)) {
		istringstream iss(line);
		string key;
		if (!(iss >> key)) {
			break;
		}
		if (key == "guid:") {
			iss >> guid;
		}
		else if (key == "width:") {
			iss >> width;
		}
		else if (key == "height:") {
			iss >> height;
		}
	}
}