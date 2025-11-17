/*
* ファイル名	Game.cpp
* タイトル	ゲーム
* 作成者		久保木幹太
* 作成日		10月15日
* 更新日		10月23日
*/

//================================================================
//	インクルード
//================================================================
#include"Manager.h"
#include"sprite.h"
#include"Game.h"
#include"keyboard.h"

#include"field.h"
#include"Effect.h"
#include"Audio.h"

#include"camera.h"
#include"player.h"

#include"direct3d.h"

//================================================================
//	グローバル変数
//================================================================
LIGHT_OBJECT Light;	// ライト管理オブジェクト

static	int		g_BgmID = NULL;	//サウンド管理ID

void Game_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Player_Initialize(pDevice, pContext);
	Field_Initialize(pDevice, pContext);		// フィールドの初期化

	Camera_Initialize();						// カメラの初期化
	
	//ライト初期化
	XMFLOAT4	para;

	para = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);//環境光の色
	Light.SetAmbient(para);

	para = XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);//光の色
	Light.SetDiffuse(para);

	para = XMFLOAT4(0.5f, -1.0f, 0.0f, 1.0f);//光方向
	float	len = sqrtf(para.x * para.x + para.y * para.y + para.z * para.z);
	para.x /= len;
	para.y /= len;
	para.z /= len;
	Light.SetDirection(para);//光の方向（正規化済）
}

void Game_Finalize()
{
	Player_Finalize();
	Field_Finalize();	// フィールドの終了処理
	Camera_Finalize();
}

void Game_Update()
{
	Field_Update();
	Player_Update();
	Camera_Update();
}

void Game_Draw()
{ 
//================================================================
//	3Dを表示
//================================================================
	Light.SetEnable(TRUE);			//ライティングON
	Shader_SetLight(Light.Light);	//ライト構造体をシェーダーへセット
	SetDepthTest(TRUE);

	Camera_Draw();		//Drawの最初で呼ぶ！
	Field_Draw();
	Player_Draw();

	//2D描画
	Light.SetEnable(FALSE);			//ライティングOFF
	Shader_SetLight(Light.Light);	//ライト構造体をシェーダーへセット
	SetDepthTest(FALSE);

//================================================================
//	2Dを表示
//================================================================

	// SetDepthTestをONにしたままにすると
	// このままリザルトとかに行っちゃったりして変になっちゃうから
	// ちゃんと戻す
}