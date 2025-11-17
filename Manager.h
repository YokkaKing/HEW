/*
* ファイル名	Manager.h
* タイトル	マネージャー
* 作成者		久保木幹太
* 作成日		10月15日
* 更新日
*/

#ifndef MANAGER_H
#define MANAGER_H

enum SCENE
{
	SCENE_NONE = 0,		//何もなし
	SCENE_TITLE,		//タイトルシーン
	SCENE_GAME,			//ゲームシーン
	SCENE_RESULT,		//リザルトルシーン

};

void	Manager_Initialize();
void	Manager_Finalize();
void	Manager_Update();
void	Manager_Draw();

void	SetScene(SCENE scene);

#endif // MANAGER_H