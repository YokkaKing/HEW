/*
* ファイル名	camera.h
* タイトル	カメラ制御
* 作成者		久保木幹太
* 作成日		10月23日
* 更新日		10月23日
*/

#ifndef CAMERA_H
#define CAMERA_H

#include<d3d11.h>
#include<DirectXMath.h>
using namespace DirectX; // DirectXを省略することができる

// カメラの構造体 structでもいいけどclassで
class CAMERA
{
public:
	XMFLOAT3 Position;		// 座標
	XMFLOAT3 AtPosition;	// 注視点
	XMFLOAT3 UpVector;		// 上方ベクトル

	XMMATRIX View;			// ビュー行列
	XMMATRIX Projection;	// プロジェクション行列

	float Fov;				// 視野角(画角)
	float Aspect;			// 画面のアスペクト比
	float NearClip;			// 近面クリップ距離
	float FarClip;			// 遠目クリップ距離
};

// 関数四天王
void Camera_Initialize();
void Camera_Finalize();
void Camera_Update();
void Camera_Draw();

// class内の変数はpublicなら直接いじれるが
// それはあまりよくないので関数を使う
void SetCameraFov(float);
void SetCameraAspect(float);
void SetCameraClip(float, float);

void SetCameraPosition(XMFLOAT3);
void SetCameraAtPosition(XMFLOAT3);
void SetCameraUpVector(XMFLOAT3);

XMMATRIX GetViewMatrix();
XMMATRIX GetProjectionMatrix();

XMFLOAT3 GetCameraAtPosition();
XMFLOAT3 GetCameraPosition();

#endif // CAMERA_H