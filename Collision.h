/*
* ファイル名	Collision.h
* タイトル	コリジョン
* 作成者		久保木幹太
* 作成日		11月12日
* 更新日		11月12日
*/

#ifndef COLLISION_H
#define COLLISION_H

//================================================================
//	マクロ定義
//================================================================
#define COE (0.5f)	// 反発係数
//================================================================
//	インクルード
//================================================================
#include<d3d11.h>
#include<DirectXMath.h>
#include"direct3d.h"
#include"sprite.h"
#include"shader.h"
using namespace DirectX;

#include"field.h"
#include"ball.h"

enum COLLISION_HIT
{
	HIT_NONE = 0,	// 当たってない
	HIT_GROUND,		// 地面に当たった
	HIT_WALL_0,		// 壁に-Zから当たった
	HIT_WALL_1,		// 壁に+Xから当たった
	HIT_WALL_2,		// 壁に+Zから当たった
	HIT_WALL_3,		// 壁に-Xから当たった
};

FLOAT BallField_Collision();

#endif // COLLISION_H