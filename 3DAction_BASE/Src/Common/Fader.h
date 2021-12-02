#pragma once

class Fader
{

public:

	static constexpr float SPEED_ALPHA = 5;

	enum class FADE_STATE
	{
		NONE
		, FADE_OUT		// èôÅXÇ…à√ì]
		, FADE_IN		// èôÅXÇ…ñæì]
	};

	FADE_STATE GetState(void);
	bool IsEnd(void);
	void SetFade(FADE_STATE state);
	
	void Init(void);
	void Update(void);
	void Draw(void);

private:

	FADE_STATE mState;

	// ìßñæìx
	float mAlpha;

	bool mIsPreEnd;
	bool mIsEnd;

};

