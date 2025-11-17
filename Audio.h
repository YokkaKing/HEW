/*
* ファイル名　Audio.h
* タイトル　　オーディオ
* 作成者		久保木幹太
* 作成日		10月15日
* 更新日
*/

#ifndef AUDIO_H
#define AUDIO_H

#include <xaudio2.h>

void InitAudio();		//サウンドの初期化
void UninitAudio();		//サウンドの終了


int LoadAudio(const char* FileName);		//サウンドデータ読み込み
void UnloadAudio(int Index);				//サウンドデータ解放（停止）
void PlayAudio(int Index, bool Loop = false);//サウンドデータ再生

#endif // AUDIO_H