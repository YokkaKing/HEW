/*
* ファイル名	ball.cpp
* タイトル	ボール制御
* 作成者		久保木幹太
* 作成日		11月5日
* 更新日		11月5日
*/

//================================================================
//	マクロ定義
//================================================================
#define BALL_SPEED (1.0)

//================================================================
//	インクルード
//================================================================
#include"keyboard.h"
#include"ball.h"
#include"camera.h"
#include"shader.h"

//================================================================
//	グローバル変数
//================================================================
BALL g_Ball; // ボールオブジェクト

static ID3D11Device* g_pDevice = NULL;
static ID3D11DeviceContext* g_pContext = NULL;

float g_StopTime = 0.0f; // ボールが制止するまでの時間

//================================================================
//	プロトタイプ宣言
//================================================================
void Ball_Idle();
void Ball_Move();
void Ball_Power();
void Ball_Direction();

void Ball_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

	g_Ball.Model = ModelLoad("asset\\model\\test.fbx"); // デバッグ

	g_Ball.Position = XMFLOAT3(0.0f, 2.2f, 0.0f);
	g_Ball.Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Ball.Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_Ball.Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);

	g_Ball.State = BALL_STATE::BALL_STATE_MOVE;

	g_Ball.Acceleration = XMFLOAT3(0.0f, ((-9.8f / 600.0f) * 0.5f), 0.0f);

	g_StopTime = 0.0f; // 速度の初期化
}
void Ball_Finalize(void)
{
	ModelRelease(g_Ball.Model);
}
void Ball_Update(void)
{
	switch (g_Ball.State)
	{
	case BALL_STATE::BALL_STATE_IDLE:
		Ball_Idle();
		break;
	case BALL_STATE::BALL_STATE_MOVE:
		Ball_Move();
		break;
	case BALL_STATE::BALL_STATE_DIRECTION:
		Ball_Direction();
		break;
	case BALL_STATE::BALL_STATE_POWER:
		Ball_Power();
		break;
	default:
		break;
	}
}
void Ball_Draw(void)
{
	// スケーリング行列の作成
		// 基本ブロックの大きさは変わらないから1.0f固定
	XMMATRIX ScalingMatrix = XMMatrixScaling
	(
		g_Ball.Scaling.x,
		g_Ball.Scaling.y,
		g_Ball.Scaling.z
	);

	// 平行移動行列の作成
	XMMATRIX TranslationMatrix = XMMatrixTranslation
	(
		g_Ball.Position.x,
		g_Ball.Position.y,
		g_Ball.Position.z
	);

	// 回転行列の作成
	// 基本回転しないから0.0f固定
	XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw
	(
		// 間違えないと思うが、xyzの順番で
		XMConvertToRadians(g_Ball.Rotation.x),
		XMConvertToRadians(g_Ball.Rotation.y),
		XMConvertToRadians(g_Ball.Rotation.z)
	);

	// ワールド行列の作成
	XMMATRIX World = ScalingMatrix * RotationMatrix * TranslationMatrix;
	XMMATRIX Projection = GetProjectionMatrix();
	XMMATRIX View = GetViewMatrix();
	XMMATRIX WVP = World * View * Projection;

	Shader_SetWorldMatrix(World);
	Shader_SetMatrix(WVP);
	ModelDraw(g_Ball.Model);
}

XMFLOAT3 GetBallPosition()
{
	return g_Ball.Position;
}

void Ball_Idle()
{

}
void Ball_Move()
{
	g_Ball.Velocity.x += g_Ball.Acceleration.x;
	g_Ball.Velocity.y += g_Ball.Acceleration.y;
	g_Ball.Velocity.z += g_Ball.Acceleration.z;

	g_Ball.Position.x += g_Ball.Velocity.x;
	g_Ball.Position.y += g_Ball.Velocity.y;
	g_Ball.Position.z += g_Ball.Velocity.z;

	g_Ball.Velocity.x *= 0.98f; // 速度減衰
	// g_Ball.Velocity.y *= 0.98f;
	g_Ball.Velocity.z *= 0.98f;

	float len = (
		g_Ball.Velocity.x * g_Ball.Velocity.x +
		g_Ball.Velocity.y * g_Ball.Velocity.y +
		g_Ball.Velocity.z * g_Ball.Velocity.z
		);

	// 0.00001f以下だったら止まったと判定する
	if (len <= 0.0001f)
	{
		g_StopTime++;
		if (g_StopTime > (60.0f * 2)) // 2秒間続いてる
		{
			g_Ball.Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_Ball.State = BALL_STATE::BALL_STATE_DIRECTION;
			g_StopTime = 0.0f;
		}
	}

	float hit = BallField_Collision();
}
void Ball_Power()
{
	// 打ち出すパワーを決める
	float power = BALL_SPEED * 0.12f; // とりあえず固定値

	g_Ball.Velocity.x *= power;
	g_Ball.Velocity.y *= power;
	g_Ball.Velocity.z *= power;

	g_Ball.State = BALL_STATE::BALL_STATE_MOVE;
}
void Ball_Direction()
{
	// とりあえずカメラの向いている方向へ転がす
	// スペースキーを押したら転がる
	if (Keyboard_IsKeyDownTrigger(KK_SPACE)) // 押したら一回反応
	{
		// カメラの向きを取得
		XMFLOAT3 v1 = GetCameraAtPosition();
		XMFLOAT3 v2 = GetCameraPosition();
		XMFLOAT3 Direction;

		Direction.x = v1.x - v2.x;
		Direction.y = 0.0f; // 上向いてる下向いてるは無視
		Direction.z = v1.z - v2.z;

		// Directionを正規化する
		float len = sqrtf((
			Direction.x * Direction.x +
			Direction.y * Direction.y +
			Direction.z * Direction.z)
		);
		Direction.x /= len;
		Direction.z /= len;
		g_Ball.Velocity = Direction;

		g_Ball.State = BALL_STATE::BALL_STATE_POWER;
	}
}

BALL* GetBall()
{
	return &g_Ball;
}