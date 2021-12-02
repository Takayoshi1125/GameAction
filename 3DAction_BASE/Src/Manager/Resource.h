#pragma once
#include <string>
#include <vector>

class Resource
{

public:

	enum class TYPE
	{
		NONE,
		IMG,
		IMGS,
		MODEL,
		EFFEKSEER
	};

	Resource(void);
	Resource(TYPE type, std::string path);
	Resource(TYPE type, std::string path, int numX, int numY, int sizeX, int sizeY);
	Resource(Resource* base);

	void Load(void);
	void Release(void);

	void CopyHandle(int* imgs);

	TYPE mType;
	std::string mPath;

	// IMGS::LoadDivGraphóp
	int mNumX;
	int mNumY;
	int mSizeX;
	int mSizeY;

	int mHandleId;
	int* mHandleIds;

	// ÉÇÉfÉãï°êªóp
	std::vector<int> mDuplicateModelIds;

};

