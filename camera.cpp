/*
* ファイル名	camera.cpp
* タイトル	カメラ制御
* 作成者		久保木幹太
* 作成日		10月23日
* 更新日		10月29日
*/

//================================================================
//	インクルード
//================================================================
#include"camera.h"
#include"keyboard.h"
#include"direct3d.h"
#include"ball.h"

//================================================================
//	グローバル変数
//================================================================
static CAMERA CameraObject;

XMFLOAT3 g_BallPosOld;

// 関数四天王
void Camera_Initialize()
{
	CameraObject.Position = XMFLOAT3(0.0f, 3.0f, -4.0f);
	CameraObject.AtPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	CameraObject.UpVector = XMFLOAT3(0.0f, 1.0f, 0.0f);

	CameraObject.Fov = 45.0f;
	float width = (float)Direct3D_GetBackBufferWidth();
	float height = (float)Direct3D_GetBackBufferHeight();
	CameraObject.Aspect = width / height;
	CameraObject.NearClip = 0.5f;
	CameraObject.FarClip = 1000.0f;

	g_BallPosOld = GetBallPosition();
}
void Camera_Finalize()
{
	return;
}
void Camera_Update()
{
	// ボールを注視点として設定
	XMFLOAT3 pos = g_BallPosOld;		// 前フレームのボールの座標
	g_BallPosOld = GetBallPosition();	// 現在のボールの座標

	// 前回のボールと現在のボールの座標の差分
	pos.x = g_BallPosOld.x - pos.x;
	pos.y = g_BallPosOld.y - pos.y;
	pos.z = g_BallPosOld.z - pos.z;

	// カメラの移動 ボールと同じ動きをする
	/*pos.x = CameraObject.AtPosition.x - CameraObject.Position.x;
	pos.y = CameraObject.AtPosition.y - CameraObject.Position.y;
	pos.z = CameraObject.AtPosition.z - CameraObject.Position.z;
	pos.x *= 1.0f / 60.0f;
	pos.y *= 1.0f / 60.0f;
	pos.z *= 1.0f / 60.0f;*/
	CameraObject.Position.x += pos.x;
	CameraObject.Position.y += pos.y;
	CameraObject.Position.z += pos.z;

	// 注視点としてセット
	CameraObject.AtPosition.x = g_BallPosOld.x;
	CameraObject.AtPosition.y = g_BallPosOld.y;
	CameraObject.AtPosition.z = g_BallPosOld.z;

	// Positionはカメラ自体が動いている
	// AtPositionはカメラの見ている場所が動いている

	// 基本的にPositionとAtPositionは一緒に動く
	// 俺たちが理解しやすい動きをするのはこれ

	// 注視点を中心にカメラの位置を回転(Y軸回転)
	float Rotation = 0.0f;
	if (Keyboard_IsKeyDown(KK_A))
	{
		// 今いる場所からこれだけ動くよ
		// +=じゃないから注意！！
		Rotation = 1.0f;
	}
	if (Keyboard_IsKeyDown(KK_D))
	{
		Rotation = -1.0f;
	}

	// FOVの変更
	if (Keyboard_IsKeyDown(KK_Q))
	{
		CameraObject.Fov += 0.3f;
		if (CameraObject.Fov > 160.0f)
		{
			CameraObject.Fov = 160.0f;
		}
	}
	if (Keyboard_IsKeyDown(KK_E))
	{
		CameraObject.Fov -= 0.3f;
		if (CameraObject.Fov < 5.0f)
		{
			CameraObject.Fov = 5.0f;
		}
	}

	// 注視点からカメラへのベクトル
	// 地面に対して平行にしか動かないからy軸計算はいらない
	XMFLOAT2 vec;
	vec.x = CameraObject.Position.x - CameraObject.AtPosition.x;
	// ややこしいけど、vec.yはy軸のことじゃないよ
	// vec.yはvec.x,vec.y,の流れでyなだけで、実際はz軸の計算をしているよ
	vec.y = CameraObject.Position.z - CameraObject.AtPosition.z;

	// ベクトルの回転
	// fがついている方が精度が悪いけど速いしデータが小さい
	float co = cosf(XMConvertToRadians(Rotation));
	float si = sinf(XMConvertToRadians(Rotation));

	// 回転する計算
	CameraObject.Position.x = (vec.x * co - vec.y * si);
	CameraObject.Position.z = (vec.x * si + vec.y * co);
	CameraObject.Position.x += CameraObject.AtPosition.x;
	CameraObject.Position.z += CameraObject.AtPosition.z;

	return;
}
void Camera_Draw()
{
//================================================================
//	プロジェクション行列作成
//================================================================

	CameraObject.Projection = XMMatrixPerspectiveFovLH
	(
		XMConvertToRadians(CameraObject.Fov),
		CameraObject.Aspect,
		CameraObject.NearClip,
		CameraObject.FarClip
	);

//================================================================
//	ビュー行列作成
//================================================================

	// カメラの座標
	XMVECTOR vPos = XMVectorSet
	(
		CameraObject.Position.x,
		CameraObject.Position.y,
		CameraObject.Position.z,
		0.0f
	);

	// カメラの注視点
	XMVECTOR vAt = XMVectorSet
	(
		CameraObject.AtPosition.x,
		CameraObject.AtPosition.y,
		CameraObject.AtPosition.z,
		0.0f
	);

	// 上方ベクトル
	XMVECTOR vUp = XMVectorSet
	(
		CameraObject.UpVector.x,
		CameraObject.UpVector.y,
		CameraObject.UpVector.z,
		0.0f
	);

	// ここからここを見ていますよという関数。
	// 行列のカメラ作成の基本
	// LHとRHを間違えないように注意
	CameraObject.View = XMMatrixLookAtLH
	(
		vPos,
		vAt,
		vUp
	);

	return;
}

// class内の変数はpublicなら直接いじれるが
// それはあまりよくないので関数を使う
void SetCameraFov(float fov)
{
	CameraObject.Fov = fov;
}
void SetCameraAspect(float asp)
{
	CameraObject.Aspect = asp;
}
void SetCameraClip(float n, float f)
{
	CameraObject.NearClip = n;
	CameraObject.FarClip = f;
}

void SetCameraPosition(XMFLOAT3 pos)
{
	CameraObject.Position = pos;
}
void SetCameraAtPosition(XMFLOAT3 at)
{
	CameraObject.AtPosition = at;
}
void SetCameraUpVector(XMFLOAT3 up)
{
	CameraObject.UpVector = up;
}

XMMATRIX GetViewMatrix()
{
	return CameraObject.View;
}
XMMATRIX GetProjectionMatrix()
{
	return CameraObject.Projection;
}

XMFLOAT3 GetCameraAtPosition()
{
	return CameraObject.AtPosition;
}
XMFLOAT3 GetCameraPosition()
{
	return CameraObject.Position;
}