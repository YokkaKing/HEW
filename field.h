/*
* ファイル名	Field.h
* タイトル	フィールド
* 作成者		久保木幹太
* 作成日		10月15日
* 更新日		10月29日
*/

#ifndef FIELD_H
#define FIELD_H

//================================================================
//	マクロ定義
//================================================================
#define BOX_RADIUS (0.5f)

//================================================================
//	インクルード
//================================================================
#include<d3d11.h>
#include<DirectXMath.h>
#include"direct3d.h"
#include"sprite.h"
#include"shader.h"
using namespace DirectX;

// MAP構成ブロックの種類
enum FIELD
{
	FIELD_BOX = 0,			// 手作りBOX

	FIELD_GINGER_DOCTOR,	// 障害

	FIELD_MAX
};

class MAPDATA
{
public:
	XMFLOAT3 pos;	// ブロックの座標
	FIELD type;		// ブロックの種類
	// その他必要なものは追加する
};

void Field_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Field_Finalize(void);
void Field_Update(void);
void Field_Draw(void);
void CreateBox(); // BOX作成関数

MAPDATA* GetFieldMap();

#endif // FIELD_H