#include <DxLib.h>
#include "KeyCheck.h"


//変数の定義
//---------------------------
//  true : キーが押されている もしくは 状態が発生したら
bool keyOld[KEY_LIST_MAX];		// １フレーム前のキーの状態
bool keyNew[KEY_LIST_MAX];		// 現フレームのキーの状態
bool keyTrgDown[KEY_LIST_MAX];	// KeyDownのトリガー状態
bool keyTrgUp[KEY_LIST_MAX];	// KeyUpのトリガー状態


// キー情報の初期化
void KeyInit(void)
{
	for (int k = 0; k < KEY_LIST_MAX; k++)
	{
		keyOld[k] = false;
		keyNew[k] = false;
		keyTrgDown[k] = false;
		keyTrgUp[k] = false;
	}
}


// キー情報の作成
void CheckKeyList(void)
{
	// ①１フレーム前のデータをコピーする：keyOld
	// 　その他の変数は初期化
	for (int k = 0; k < KEY_LIST_MAX; k++)
	{
		keyOld[k] = keyNew[k];
		keyNew[k] = false;
		keyTrgDown[k] = false;
		keyTrgUp[k] = false;
	}

	// ②現フレームの状態を取得し格納：keyNew

	//システム関連
	keyNew[KEY_SYS_START] = CheckHitKey(KEY_INPUT_SPACE);

	//Player1関連
	keyNew[KEY_P1_UP] = CheckHitKey(KEY_INPUT_UP);
	keyNew[KEY_P1_DOWN] = CheckHitKey(KEY_INPUT_DOWN);
	keyNew[KEY_P1_LEFT] = CheckHitKey(KEY_INPUT_LEFT);
	keyNew[KEY_P1_RIGHT] = CheckHitKey(KEY_INPUT_RIGHT);
	keyNew[KEY_P1_A] = CheckHitKey(KEY_INPUT_N);
	keyNew[KEY_P1_B] = CheckHitKey(KEY_INPUT_M);

	//Player2関連
	keyNew[KEY_P2_UP] = CheckHitKey(KEY_INPUT_W);
	keyNew[KEY_P2_DOWN] = CheckHitKey(KEY_INPUT_S);
	keyNew[KEY_P2_LEFT] = CheckHitKey(KEY_INPUT_A);
	keyNew[KEY_P2_RIGHT] = CheckHitKey(KEY_INPUT_D);
	keyNew[KEY_P2_A] = CheckHitKey(KEY_INPUT_LCONTROL);
	keyNew[KEY_P2_B] = CheckHitKey(KEY_INPUT_LSHIFT);

	// ③トリガーの状態をチェックし格納：keyTrgDown、keyTrgUp

	for (int k = 0; k < KEY_LIST_MAX; k++)
	{
		keyTrgDown[k] = keyNew[k] && !keyOld[k];
		keyTrgUp[k] = !keyNew[k] && keyOld[k];
	}

}

