/*
* ファイル名	Collision.cpp
* タイトル	コリジョン
* 作成者		久保木幹太
* 作成日		11月12日
* 更新日		11月12日
*/

//================================================================
//	インクルード
//================================================================
#include"Collision.h"

FLOAT BallField_Collision()
{
	FLOAT hit = false;
	BALL* Ball = GetBall();
	MAPDATA* Map = GetFieldMap();
	int i = 0;

	// 全てのブロックをチェック
	while (Map[i].type != FIELD_MAX)
	{
		float BoxTop;

		switch (Map[i].type)
		{

		default:
			BoxTop = Map[i].pos.y + BOX_RADIUS;	// 普通のBOX
			break;
		}

		// 壁としての判定処理
		if ((Map[i].pos.y - BOX_RADIUS < Ball->Position.y) && (Ball->Position.y < BoxTop - 0.1f))
		{
			if ((Map[i].pos.z - BOX_RADIUS < Ball->Position.z) && Ball->Position.z < Map[i].pos.z + BOX_RADIUS)
			{
				if ((Map[i].pos.x - BOX_RADIUS < Ball->Position.x + BALL_RADIUS) && (Ball->Position.x < Map[i].pos.x - BOX_RADIUS))
				{
					// BOXの-X面にぶつかったので座標の補正
					Ball->Position.x += ((Map[i].pos.x - BOX_RADIUS) - (Ball->Position.x + BALL_RADIUS));
					Ball->Velocity.x *= -COE; // 移動ベクトルの反転
					hit = COLLISION_HIT::HIT_WALL_3;
				}
				else if ((Map[i].pos.x + BOX_RADIUS > Ball->Position.x - BALL_RADIUS) && (Ball->Position.x > Map[i].pos.x + BOX_RADIUS))
				{
					Ball->Position.x += (Map[i].pos.x + BOX_RADIUS) - (Ball->Position.x - BOX_RADIUS);
					Ball->Velocity.x *= -COE;
					hit = COLLISION_HIT::HIT_WALL_1;
				}
			}
			else if ((Map[i].pos.x - BOX_RADIUS < Ball->Position.x) && (Ball->Position.x < Map[i].pos.x + BOX_RADIUS))
			{
				if ((Map[i].pos.z - BOX_RADIUS < Ball->Position.z + BALL_RADIUS) && (Ball->Position.z < Map[i].pos.z - BOX_RADIUS))
				{
					// BOXの-X面にぶつかったので座標の補正
					Ball->Position.z += ((Map[i].pos.z - BOX_RADIUS) - (Ball->Position.z + BALL_RADIUS));
					Ball->Velocity.z *= -COE; // 移動ベクトルの反転
					hit = COLLISION_HIT::HIT_WALL_0;
				}
				else if ((Map[i].pos.z + BOX_RADIUS > Ball->Position.z - BALL_RADIUS) && (Ball->Position.z > Map[i].pos.z + BOX_RADIUS))
				{
					Ball->Position.z += (Map[i].pos.z + BOX_RADIUS) - (Ball->Position.z - BOX_RADIUS);
					Ball->Velocity.z *= -COE;
					hit = COLLISION_HIT::HIT_WALL_2;
				}
			}
		}
		else // 地面としての判定処理
		{
			if ((Map[i].pos.z - BOX_RADIUS < Ball->Position.z) && (Ball->Position.z < Map[i].pos.z + BOX_RADIUS))
			{

			}
		}

		i++;
	}

	return hit;
}