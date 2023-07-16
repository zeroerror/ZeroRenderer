#pragma once
#include <string>
#include <unordered_map>
using namespace std;

class FileSuffix {

public:
	inline static string SUFFIX_META = ".meta";
	inline static string SUFFIX_PNG = ".png";
	inline static string SUFFIX_SHADER = ".shader";
	inline static string SUFFIX_OBJ = ".obj";
	inline static string SUFFIX_FBX = ".fbx";
	inline static string SUFFIX_MAT = ".mat";
	inline static string SUFFIX_SCENE = ".scene";
	inline static string SUFFIX_PREFAB = ".prefab";

	inline static bool FileSuffix_Contains(const unsigned int& flag, const string& suffix) {
		if (Suffix2FlagMap.find(suffix) == Suffix2FlagMap.end()) {
			return false;
		}

		return (flag & Suffix2FlagMap.at(suffix)) != 0;
	}

	inline static unsigned int ToFileSuffixFlag(vector<string>& suffixes) {
		unsigned int flag = 0;
		for (int i = 0; i < suffixes.size(); i++) {
			if (Suffix2FlagMap.find(suffixes[i]) == Suffix2FlagMap.end()) {
				continue;
			}

			flag |= Suffix2FlagMap.at(suffixes[i]);
		}

		return flag;
	}

private:
	inline static unordered_map<string, unsigned int> Suffix2FlagMap = {
		{ SUFFIX_META, 1 << 0 },
		{ SUFFIX_PNG, 1 << 1 },
		{ SUFFIX_SHADER, 1 << 2 },
		{ SUFFIX_OBJ, 1 << 3 },
		{ SUFFIX_FBX, 1 << 4 },
		{ SUFFIX_MAT, 1 << 5 },
		{ SUFFIX_SCENE, 1 << 6 },
		{ SUFFIX_PREFAB, 1 << 7 },
	};

};