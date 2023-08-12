#pragma once
#include <unordered_map>
#include <Material.h>

using namespace std;

class MaterialRepo {

public:
	MaterialRepo();
	~MaterialRepo();

	bool TryAddMaterial(const string& guid, Material*& material);
	bool TryGetMaterialByGUID(const string& guid, Material*& material);

	Material* defaultMaterial;

private:
	unordered_map<string, Material*> allMaterials_sortedByPath;
	unordered_map<string, Material*> allMaterials_sortedByGUID;

};

