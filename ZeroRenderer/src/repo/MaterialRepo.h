#pragma once
#include <unordered_map>
#include <Material.h>

class MaterialRepo {

public:
	MaterialRepo();
	~MaterialRepo();

	std::string LoadMaterial(const char* path);
	Material* GetMaterialByGUID(const std::string& guid);

private:
	std::unordered_map<std::string, Material*> allMaterials;

};

