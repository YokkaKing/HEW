/*
* ファイル名	Polygon3D.h
* タイトル	ポリゴン3D
* 作成者		久保木幹太
* 作成日		10月15日
* 更新日		10月15日
*/

#ifndef POLYGON3D_H
#define POLYGON3D_H

#include<d3d11.h>

void Polygon3D_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Polygon3D_Finalize();
void Polygon3D_Update();
void Polygon3D_Draw();

#endif // POLYGON3D_H