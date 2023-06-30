#include "Mat.h"
#include <sstream>
#include "FileHelper.h"

void Mat::SerializeTo(const string& path){
	std::stringstream ss;
	ss << "shaderGUID: " << shaderGUID << std::endl;
	ss << "diffuseTextureGUID: " << diffuseTextureGUID << std::endl;
	ss << "specularTextureGUID: " << specularTextureGUID << std::endl;
	ss << "diffuseColor: " << diffuseColor.x << " " << diffuseColor.y << " " << diffuseColor.z << " " << diffuseColor.w << std::endl;
	ss << "specularIntensity: " << specularIntensity << std::endl;
	ss << "shininess: " << shininess << std::endl;
	std::string result = ss.str();
	size_t len = result.length() + 1;
	unsigned char* charResult = new unsigned char[len];
	memcpy(charResult, result.c_str(), len);
	FileHelper::WriteCharsTo(path, charResult);
}

void Mat::DeserializeFrom(const string& path){
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
		 if (key == "shaderGUID:") {
			iss >> shaderGUID;
		}
		else if (key == "diffuseTextureGUID:") {
			iss >> diffuseTextureGUID;
		}
		else if (key == "specularTextureGUID:") {
			iss >> specularTextureGUID;
		}
		else if (key == "diffuseColor:") {
			iss >> diffuseColor.x >> diffuseColor.y >> diffuseColor.z >> diffuseColor.w;
		}
		else if (key == "specularIntensity:") {
			iss >> specularIntensity;
		}
		else if (key == "shininess:") {
			iss >> shininess;
		}
	}
}