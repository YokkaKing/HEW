/*
* ファイル名	ball.h
* タイトル	ボール制御
* 作成者		久保木幹太
* 作成日		11月5日
* 更新日		11月5日
*/

#ifndef BALL_H
#define BALL_H


//================================================================
//	マクロ定義
//================================================================
#define BALL_RADIUS (0.2f)

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

void Ball_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Ball_Finalize(void);
void Ball_Update(void);
void Ball_Draw(void);

XMFLOAT3 GetBallPosition();

enum BALL_STATE
{
	BALL_STATE_IDLE = 0,	// 何もしない
	BALL_STATE_MOVE,		// 移動
	BALL_STATE_DIRECTION,	// 方向指示
	BALL_STATE_POWER,		// 威力指示
};

class BALL
{
public:
	XMFLOAT3 Position;		// 表示座標
	XMFLOAT3 Rotation;		// 回転角
	XMFLOAT3 Scaling;		//
	XMFLOAT3 Velocity;		// 速度
	XMFLOAT3 Acceleration;	// 落下速度

	BALL_STATE State;	// 状態
	MODEL* Model;		// モデルデータ
};

BALL* GetBall();

#endif // BALL_H