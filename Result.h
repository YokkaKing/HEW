/*
* ファイル名	Result.h
* タイトル	リザルト
* 作成者		久保木幹太
* 作成日		10月15日
* 更新日
*/

#ifndef RESULT_H
#define RESULT_H

#include "direct3d.h"


void Result_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Result_Finalize();
void Result_Update();
void Result_Draw();

#endif // RESULT_H