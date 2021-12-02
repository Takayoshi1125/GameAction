#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Resource.h"

Resource::Resource(void)
{
	mType = TYPE::NONE;
	mPath = "";

	mNumX = -1;
	mNumY = -1;
	mSizeX = -1;
	mSizeY = -1;

	mHandleId = -1;
	mHandleIds = nullptr;
}

Resource::Resource(TYPE type, std::string path)
{
	mType = type;
	mPath = path;

	mNumX = -1;
	mNumY = -1;
	mSizeX = -1;
	mSizeY = -1;

	mHandleId = -1;
	mHandleIds = nullptr;
}

Resource::Resource(TYPE type, std::string path, int numX, int numY, int sizeX, int sizeY)
{
	mType = type;
	mPath = path;
	mNumX = numX;
	mNumY = numY;
	mSizeX = sizeX;
	mSizeY = sizeY;

	mHandleId = -1;
	mHandleIds = nullptr;
}

Resource::Resource(Resource* base)
{
	mType = base->mType;
	mPath = base->mPath;
	mNumX = base->mNumX;
	mNumY = base->mNumY;
	mSizeX = base->mSizeX;
	mSizeY = base->mSizeY;

	mHandleId = base->mHandleId;
	mHandleIds = base->mHandleIds;
}

void Resource::Load(void)
{

	switch (mType)
	{
	case Resource::TYPE::IMG:

		mHandleId = LoadGraph(mPath.c_str());
		break;

	case Resource::TYPE::IMGS:

		mHandleIds = new int[(size_t)mNumX * mNumY];
		LoadDivGraph(
			mPath.c_str(),
			mNumX * mNumY,
			mNumX, mNumY,
			mSizeX, mSizeY,
			&mHandleIds[0]);
			//0, 0, 0);
		break;

	case Resource::TYPE::MODEL:

		mHandleId = MV1LoadModel(mPath.c_str());
		break;

	case Resource::TYPE::EFFEKSEER:

		mHandleId = LoadEffekseerEffect(mPath.c_str());
		break;

	}

}

void Resource::Release(void)
{

	switch (mType)
	{
	case Resource::TYPE::IMG:

		DeleteGraph(mHandleId);
		break;

	case Resource::TYPE::IMGS:
	{
		int num = mNumX * mNumY;
		for (int i = 0; i < num; i++)
		{
			DeleteGraph(mHandleIds[i]);
		}
		delete[] mHandleIds;
		break;
	}
	case Resource::TYPE::MODEL:
	{
		MV1DeleteModel(mHandleId);
		auto ids = mDuplicateModelIds;
		for (auto id : ids)
		{
			MV1DeleteModel(id);
		}
		break;
	}
	case Resource::TYPE::EFFEKSEER:
		DeleteEffekseerEffect(mHandleId);
		break;

	}

}

void Resource::CopyHandle(int* imgs)
{

	if (mHandleIds == nullptr)
	{
		return;
	}

	int num = mNumX * mNumY;
	for (int i = 0; i < num; i++)
	{
		imgs[i] = mHandleIds[i];
	}

}
