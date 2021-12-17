#pragma once
#include <DxLib.h>
#include "Common/Transform.h"
class SceneManager;
class ResourceManager;
class GravityManager;
class AnimationController;
class Capsule;
class Collider;

class Player
{

public:
	static constexpr float SPEED_MOVE = 5.0f;
	static constexpr float SPEED_RUN = 10.0f;

	//��]�����܂ł̎���
	static constexpr float TIME_ROT = 1.0f;

	//�W�����v��
	static constexpr float POW_JUMP = 35.0f;

	//�W�����v��t����
	static constexpr float TIME_JUMP_IN = 0.5f;

	// ���
	enum class STATE
	{
		NONE,
		PLAY,
		WARP_RESERVE,
		WARP_MOVE,
		DEAD,
		VICTORY,
		END
	};

	// �A�j���[�V�������
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		FAST_RUN,
		JUMP,
		WARP_PAUSE,
		FLY,
		FALLING,
		VICTORY
	};

	Player(SceneManager* manager);
	void Init(void);
	void InitAnimation(void);
	void Update(void);
	void UpdatePlay(void);
	void Draw(void);
	void DrawShadow(void);
	void DrawDebug(void);
	void Release(void);

	//����
	void ProcessMove(void);
	void ProcessJamp(void);

	//��]
	void SetGoalRotate(double rad);
	void Rotate(void);

	//�d�͂̋����v�Z
	void CalcGravityPow(void);

	Transform* GetTransform(void);

	//�Փ˔���ɗp������R���C�_
	void AddCollider(Collider* collider);
	void ClearCollider(void);

private:

	SceneManager* mSceneManager;
	ResourceManager* mResourceManager;
	GravityManager* mGravityManager;

	Transform mTransform;

	// �A�j���[�V����
	AnimationController* mAnimationController;

	// ���
	STATE mState;

	//�ړ��X�s�[�h
	float mSpeed;
	//�ړ�����
	VECTOR mMoveDir;
	//�ړ���
	VECTOR mMovePow;
	//�ړ���̍��W
	VECTOR mMovedPos;

	//��]
	Quaternion mPlayerRotY;
	Quaternion mGoalQuaRotY;
	float mStepRotTime;

	//�W�����v��
	VECTOR mJumpPow;

	bool mIsJump;

	//�W�����v��t����
	float mStepJump;

	//�Փ˔���ɗp������R���C�_
	Capsule* mCapsule;
	std::vector<Collider*>mColliders;

	VECTOR mGravHitDown;
	VECTOR mGravHitUp;


	//�ی`�摜
	int mImgShadow;

	//����
	int mEffectSmoke;
	int mStepFootSmoke;

	//�t���[�����Ƃ̈ړ���
	VECTOR mMoveDiff;

	// ��ԑJ��
	void ChangeState(STATE state);


	//�Փ˔���
	void Collision(void);
	void CollisionCapsule(void);

	void CollisionGravity(void);
	//���n���[�V�����I��
	bool IsEndLanding(void);

	void EffectFootSmoke(void);

};

