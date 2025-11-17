/*
* ファイル名	player.h
* タイトル	ボール制御
* 作成者		久保木幹太
* 作成日		11月5日
* 更新日		11月5日
*/

#ifndef PLAYER_H
#define PLAYER_H


//================================================================
//	マクロ定義
//================================================================
#define PLAYER_RADIUS (0.2f)

//================================================================
//	インクルード
//================================================================
#include<d3d11.h>
#include<DirectXMath.h>
#include"direct3d.h"
#include"sprite.h"
#include"shader.h"
using namespace DirectX;

#include"model.h"
#include"Collision.h"

void Player_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Player_Finalize(void);
void Player_Update(void);
void Player_Draw(void);

XMFLOAT3 GetPlayerPosition();

enum PLAYER_STATE
{
	PLAYER_STATE_IDLE = 0,	// 何もしない
	PLAYER_STATE_MOVE,		// 移動
	PLAYER_STATE_DIRECTION,	// 方向指示
	PLAYER_STATE_POWER,		// 威力指示
};

class PLAYER
{
public:
	XMFLOAT3 Position;		// 表示座標
	XMFLOAT3 Rotation;		// 回転角
	XMFLOAT3 Scaling;		//
	XMFLOAT3 Velocity;		// 速度
	XMFLOAT3 Acceleration;	// 落下速度

	PLAYER_STATE State;	// 状態
	MODEL* Model;		// モデルデータ
};

PLAYER* GetPlayer();

#endif // PLAYER_H