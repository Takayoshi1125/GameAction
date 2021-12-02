#pragma once
class Collider
{
public :

	enum class TYPE
	{
		STAGE,
	};

	Collider(TYPE type, int modelId);

	TYPE mType;

	int mModelId;

};

