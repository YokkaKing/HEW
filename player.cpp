/*
* ファイル名	player.cpp
* タイトル	ボール制御
* 作成者		久保木幹太
* 作成日		11月5日
* 更新日		11月5日
*/

//================================================================
//	マクロ定義
//================================================================
#define PLAYER_SPEED (1.0)

//================================================================
//	インクルード
//================================================================
#include"keyboard.h"
#include"player.h"
#include"camera.h"
#include"shader.h"

//================================================================
//	グローバル変数
//================================================================
PLAYER g_Player; // ボールオブジェクト

static ID3D11Device* g_pDevice = NULL;
static ID3D11DeviceContext* g_pContext = NULL;

float g_StopTime = 0.0f; // ボールが制止するまでの時間

//================================================================
//	プロトタイプ宣言
//================================================================
void Player_Idle();
void Player_Move();
void Player_Power();
void Player_Direction();

void Player_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

	g_Player.Model = ModelLoad("asset\\model\\test.fbx"); // デバッグ

	g_Player.Position = XMFLOAT3(0.0f, 2.2f, 0.0f);
	g_Player.Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Player.Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_Player.Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);

	g_Player.State = PLAYER_STATE::PLAYER_STATE_MOVE;

	g_Player.Acceleration = XMFLOAT3(0.0f, ((-9.8f / 600.0f) * 0.5f), 0.0f);

	g_StopTime = 0.0f; // 速度の初期化
}
void Player_Finalize(void)
{
	ModelRelease(g_Player.Model);
}
void Player_Update(void)
{
	switch (g_Player.State)
	{
	case PLAYER_STATE::PLAYER_STATE_IDLE:
		Player_Idle();
		break;
	case PLAYER_STATE::PLAYER_STATE_MOVE:
		Player_Move();
		break;
	case PLAYER_STATE::PLAYER_STATE_DIRECTION:
		Player_Direction();
		break;
	case PLAYER_STATE::PLAYER_STATE_POWER:
		Player_Power();
		break;
	default:
		break;
	}
}
void Player_Draw(void)
{
	// スケーリング行列の作成
		// 基本ブロックの大きさは変わらないから1.0f固定
	XMMATRIX ScalingMatrix = XMMatrixScaling
	(
		g_Player.Scaling.x,
		g_Player.Scaling.y,
		g_Player.Scaling.z
	);

	// 平行移動行列の作成
	XMMATRIX TranslationMatrix = XMMatrixTranslation
	(
		g_Player.Position.x,
		g_Player.Position.y,
		g_Player.Position.z
	);

	// 回転行列の作成
	// 基本回転しないから0.0f固定
	XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw
	(
		// 間違えないと思うが、xyzの順番で
		XMConvertToRadians(g_Player.Rotation.x),
		XMConvertToRadians(g_Player.Rotation.y),
		XMConvertToRadians(g_Player.Rotation.z)
	);

	// ワールド行列の作成
	XMMATRIX World = ScalingMatrix * RotationMatrix * TranslationMatrix;
	XMMATRIX Projection = GetProjectionMatrix();
	XMMATRIX View = GetViewMatrix();
	XMMATRIX WVP = World * View * Projection;

	Shader_SetWorldMatrix(World);
	Shader_SetMatrix(WVP);
	ModelDraw(g_Player.Model);
}

XMFLOAT3 GetPlayerPosition()
{
	return g_Player.Position;
}

void Player_Idle()
{

}
void Player_Move()
{
	//g_Player.Velocity.x += g_Player.Acceleration.x;
	//g_Player.Velocity.y += g_Player.Acceleration.y;
	//g_Player.Velocity.z += g_Player.Acceleration.z;

	g_Player.Position.x += g_Player.Velocity.x;
	g_Player.Position.y += g_Player.Velocity.y;
	g_Player.Position.z += g_Player.Velocity.z;

	g_Player.Velocity.x *= 0.98f; // 速度減衰
	// g_Player.Velocity.y *= 0.98f;
	g_Player.Velocity.z *= 0.98f;

	float len = (
		g_Player.Velocity.x * g_Player.Velocity.x +
		g_Player.Velocity.y * g_Player.Velocity.y +
		g_Player.Velocity.z * g_Player.Velocity.z
		);

	// 0.00001f以下だったら止まったと判定する
	if (len <= 0.0001f)
	{
		g_StopTime++;
		if (g_StopTime > (60.0f * 2)) // 2秒間続いてる
		{
			g_Player.Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_Player.State = PLAYER_STATE::PLAYER_STATE_DIRECTION;
			g_StopTime = 0.0f;
		}
	}

	float hit = PlayerField_Collision();
}
void Player_Power()
{
	// 打ち出すパワーを決める
	float power = PLAYER_SPEED * 0.12f; // とりあえず固定値

	g_Player.Velocity.x *= power;
	g_Player.Velocity.y *= power;
	g_Player.Velocity.z *= power;

	g_Player.State = PLAYER_STATE::PLAYER_STATE_MOVE;
}
void Player_Direction()
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
		g_Player.Velocity = Direction;

		g_Player.State = PLAYER_STATE::PLAYER_STATE_POWER;
	}
}

PLAYER* GetPlayer()
{
	return &g_Player;
}