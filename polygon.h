/*
* ファイル名	polygon.h
* タイトル	ポリゴン
* 作成者		久保木幹太
* 作成日		10月15日
* 更新日
*/

#ifndef POLYGON_H
#define POLYGON_H

#include <d3d11.h>

void Polygon_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Polygon_Finalize(void);
void Polygon_Draw(void);

#endif // POLYGON_H
