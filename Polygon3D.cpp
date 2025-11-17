/*
* ファイル名	Polygon3D.cpp
* タイトル	ポリゴン3D
* 作成者		久保木幹太
* 作成日		10月15日
* 更新日		10月15日
*/

//================================================================
//	マクロ定義
//================================================================
#define NUM_VERTEX (100) // 頂点の数

//================================================================
//	インクルード
//================================================================
// #include"d3d11.h"
#include"DirectXMath.h"
using namespace DirectX;
#include"direct3d.h"
#include"shader.h"
#include"keyboard.h"
#include"sprite.h"
#include"camera.h"

//================================================================
//	グローバル変数
//================================================================
static ID3D11Device* g_pDevice = NULL;
static ID3D11DeviceContext* g_pContext = NULL;

// 頂点バッファ
static ID3D11Buffer* g_VertexBuffer = NULL;
static ID3D11Buffer* g_IndexBuffer = NULL;

// テクスチャ変数
static ID3D11ShaderResourceView* g_Texture;

// ポリゴン表示座標
static XMFLOAT3 Position;
// ポリゴン回転角度
static XMFLOAT3 Rotation;
// ポリゴン拡大率
static XMFLOAT3 Scaling;


// テスト用にポリゴンのデータを作成
static Vertex vdata[NUM_VERTEX] =
{
//================================================================
//	-Z面
//================================================================
	{	// 頂点0 LEFT-TOP 左上
		XMFLOAT3(-0.5f, 0.5f, -0.5f),		// 座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(0.0f, 0.0f)				// テクスチャ座標
	},
	{	// 頂点1 RIGHT-TOP 右上
		XMFLOAT3(0.5f, 0.5f, -0.5f),			// 座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(1.0f, 0.0f)				// テクスチャ座標
	},
	{	// 頂点2 LEFT-BOTTOM 左下
		XMFLOAT3(-0.5f, -0.5f, -0.5f),		// 座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(0.0f, 1.0f)				// テクスチャ座標
	},
	//{	// 頂点3 LEFT-BOTTOM 左下
	//	XMFLOAT3(-0.5f, -0.5f, -0.5f),		// 座標
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
	//	XMFLOAT2(0.0f, 1.0f)				// テクスチャ座標
	//},
	//{	// 頂点4 RIGHT-TOP 右上
	//	XMFLOAT3(0.5f, 0.5f, -0.5f),			// 座標
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
	//	XMFLOAT2(1.0f, 0.0f)				// テクスチャ座標
	//},
	{	// 頂点3 RIGHT-TOP 右下
		XMFLOAT3(0.5f, -0.5f, -0.5f),			// 座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(1.0f, 1.0f)				// テクスチャ座標
	},

//================================================================
//	+X面
//================================================================
	{	// 頂点4 左上
		XMFLOAT3(0.5f, 0.5f, -0.5f),			// 座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(0.0f, 0.0f)				// テクスチャ座標
	},
	{	// 頂点5 右上
		XMFLOAT3(0.5f, 0.5f, 0.5f),			// 座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(1.0f, 0.0f)				// テクスチャ座標
	},
	{	// 頂点6 左下
		XMFLOAT3(0.5f, -0.5f, -0.5f),			// 座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(0.0f, 1.0f)				// テクスチャ座標
	},
	//{	// 頂点4 左下
	//	XMFLOAT3(0.5f, -0.5f, -0.5f),			// 座標
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
	//	XMFLOAT2(0.0f, 1.0f)				// テクスチャ座標
	//},
	//{	// 頂点5 右上
	//	XMFLOAT3(0.5f, 0.5f, 0.5f),			// 座標
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
	//	XMFLOAT2(1.0f, 0.0f)				// テクスチャ座標
	//},
	{	// 頂点7 右下
		XMFLOAT3(0.5f, -0.5f, 0.5f),			// 座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(1.0f, 1.0f)				// テクスチャ座標
	},

//================================================================
//	+Z面
//================================================================
	{	// 頂点8 左上
		XMFLOAT3(0.5f, 0.5f, 0.5f),			// 座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(0.0f, 0.0f)				// テクスチャ座標
	},
	{	// 頂点9 右上
		XMFLOAT3(-0.5f, 0.5f, 0.5f),		// 座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(1.0f, 0.0f)				// テクスチャ座標
	},
	{	// 頂点10 左下
		XMFLOAT3(0.5f, -0.5f, 0.5f),			// 座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(0.0f, 1.0f)				// テクスチャ座標
	},
	//{	// 頂点3 左下
	//	XMFLOAT3(0.5f, -0.5f, 0.5f),			// 座標
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
	//	XMFLOAT2(0.0f, 1.0f)				// テクスチャ座標
	//},
	//{	// 頂点4 右上
	//	XMFLOAT3(-0.5f, 0.5f, 0.5f),		// 座標
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
	//	XMFLOAT2(1.0f, 0.0f)				// テクスチャ座標
	//},
	{	// 頂点11 右下
		XMFLOAT3(-0.5f, -0.5f, 0.5f),		// 座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(1.0f, 1.0f)				// テクスチャ座標
	},

//================================================================
//	-X面
//================================================================
	{	// 頂点12 左上
		XMFLOAT3(-0.5f, 0.5f, 0.5f),		// 座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(0.0f, 0.0f)				// テクスチャ座標
	},
	{	// 頂点13 右上
		XMFLOAT3(-0.5f, 0.5f, -0.5f),		// 座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(1.0f, 0.0f)				// テクスチャ座標
	},
	{	// 頂点14 左下
		XMFLOAT3(-0.5f, -0.5f, 0.5f),		// 座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(0.0f, 1.0f)				// テクスチャ座標
	},
	//{	// 頂点3 左下
	//	XMFLOAT3(-0.5f, -0.5f, 0.5f),		// 座標
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
	//	XMFLOAT2(0.0f, 1.0f)				// テクスチャ座標
	//},
	//{	// 頂点4 右上
	//	XMFLOAT3(-0.5f, 0.5f, -0.5f),		// 座標
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
	//	XMFLOAT2(1.0f, 0.0f)				// テクスチャ座標
	//},
	{	// 頂点15 右下
		XMFLOAT3(-0.5f, -0.5f, -0.5f),		// 座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(1.0f, 1.0f)				// テクスチャ座標
	},

//================================================================
//	+Y面
//================================================================
	{	// 頂点16 左上
		XMFLOAT3(-0.5f, 0.5f, 0.5f),		// 座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(0.0f, 0.0f)				// テクスチャ座標
	},
	{	// 頂点17 右上
		XMFLOAT3(0.5f, 0.5f, 0.5f),			// 座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(1.0f, 0.0f)				// テクスチャ座標
	},
	{	// 頂点18 左下
		XMFLOAT3(-0.5f, 0.5f, -0.5f),		// 座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(0.0f, 1.0f)				// テクスチャ座標
	},
	//{	// 頂点3 左下
	//	XMFLOAT3(-0.5f, 0.5f, -0.5f),		// 座標
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
	//	XMFLOAT2(0.0f, 1.0f)				// テクスチャ座標
	//},
	//{	// 頂点4 右上
	//	XMFLOAT3(0.5f, 0.5f, 0.5f),			// 座標
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
	//	XMFLOAT2(1.0f, 0.0f)				// テクスチャ座標
	//},
	{	// 頂点19 右下
		XMFLOAT3(0.5f, 0.5f, -0.5f),		// 座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(1.0f, 1.0f)				// テクスチャ座標
	},

//================================================================
//	-Y面
//================================================================
	{	// 頂点20 左上
		XMFLOAT3(-0.5f, -0.5f, -0.5f),		// 座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(0.0f, 0.0f)				// テクスチャ座標
	},
	{	// 頂点21 右上
		XMFLOAT3(0.5f, -0.5f, -0.5f),		// 座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(1.0f, 0.0f)				// テクスチャ座標
	},
	{	// 頂点22 左下
		XMFLOAT3(-0.5f, -0.5f, 0.5f),		// 座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(0.0f, 1.0f)				// テクスチャ座標
	},
	//{	// 頂点3 左下
	//	XMFLOAT3(-0.5f, -0.5f, 0.5f),		// 座標
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
	//	XMFLOAT2(0.0f, 1.0f)				// テクスチャ座標
	//},
	//{	// 頂点4 右上
	//	XMFLOAT3(0.5f, -0.5f, -0.5f),			// 座標
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
	//	XMFLOAT2(1.0f, 0.0f)				// テクスチャ座標
	//},
	{	// 頂点23 右下
		XMFLOAT3(0.5f, -0.5f, 0.5f),			// 座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(1.0f, 1.0f)				// テクスチャ座標
	},

	// 後で増える
};

// 頂点の数字だけを指定して使いまわすことができる
static UINT idxdata[6 * 6] =
{
	0, 1, 2, 2, 1, 3,		// -Z面
	4, 5, 6, 6, 5, 7,		// +X面
	8, 9, 10, 10, 9, 11,	// +Z面
	12, 13, 14, 14, 13, 15,	// -X面
	16, 17, 18, 18, 17, 19,	// +Y面
	20, 21, 22, 22, 21, 23,	// -Y面
};


void Polygon3D_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)

{
	Position = XMFLOAT3(0.0f, 0.0f, 0.0f); // 原点
	Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f); // 回転なし
	Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f); // 0.0fは入れちゃだめ

	// 頂点バッファ作成
	D3D11_BUFFER_DESC bd; // 構造体
	ZeroMemory(&bd, sizeof(bd)); // 0でクリア
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex) * NUM_VERTEX; // 格納できる丁点数 * 頂点サイズ
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pDevice->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	g_pDevice = pDevice;
	g_pContext = pContext;

	// テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"Asset\\Texture\\texture.jpg", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(),
		image.GetImageCount(), metadata, &g_Texture);
	assert(g_Texture);


	// インデックスバッファ作成
	{
		// 頂点バッファ作成
		D3D11_BUFFER_DESC bd; // 構造体
		ZeroMemory(&bd, sizeof(bd)); // 0でクリア
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(UINT) * (6 * 6); // 格納できる丁点数 * 頂点サイズ
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		pDevice->CreateBuffer(&bd, NULL, &g_IndexBuffer);

		// インデックスバッファへ書き込み
		D3D11_MAPPED_SUBRESOURCE msr;
		pContext->Map(g_IndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		UINT* index = (UINT*)msr.pData;

		// インデックスデータをバッファへコピー
		CopyMemory(&index[0], &idxdata[0], sizeof(UINT) * (6 * 6));
		pContext->Unmap(g_IndexBuffer, 0);
	}
}
void Polygon3D_Finalize()
{
	if (g_IndexBuffer != NULL)
	{
		g_IndexBuffer->Release();
		g_IndexBuffer = NULL;
	}


	if (g_VertexBuffer != NULL)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	if (g_Texture != NULL)
	{
		g_Texture->Release();
		g_Texture = NULL;
	}
}
void Polygon3D_Update()
{
	/*if (Keyboard_IsKeyDown(KK_UP))
	{
		if (Keyboard_IsKeyDown(KK_LEFTSHIFT))
		{
			Rotation.x += 1.0f;
		}
		else
		{
			Position.z += 0.01f;
		}
	}
	if (Keyboard_IsKeyDown(KK_DOWN))
	{
		if (Keyboard_IsKeyDown(KK_LEFTSHIFT))
		{
			Rotation.x -= 1.0f;
		}
		else
		{
			Position.z -= 0.01f;
		}
	}
	if (Keyboard_IsKeyDown(KK_LEFT))
	{
		if (Keyboard_IsKeyDown(KK_LEFTSHIFT))
		{
			Rotation.y += 1.0f;
		}
		else
		{
			Position.x -= 0.01f;
		}
	}
	if (Keyboard_IsKeyDown(KK_RIGHT))
	{
		if (Keyboard_IsKeyDown(KK_LEFTSHIFT))
		{
			Rotation.y -= 1.0f;
		}
		else
		{
			Position.x += 0.01f;
		}
	}*/
}
void Polygon3D_Draw()
{
//================================================================
// ワールド行列の作成
//================================================================

	// XXMATRIX <-行列
	// 行列を作る順番は関係ない。ただ、行列を乗算する順番は変えちゃダメ

	// スケーリング行列の作成
	XMMATRIX ScalingMatrix = XMMatrixScaling
	(
		Scaling.x,
		Scaling.y,
		Scaling.z
	);

	// 平行移動行列の作成
	XMMATRIX TranslationMatrix = XMMatrixTranslation
	(
		Position.x,
		Position.y,
		Position.z
	);

	// 回転行列の作成
	XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw
	(
		// 間違えないと思うが、xyzの順番で
		XMConvertToRadians(Rotation.x),
		XMConvertToRadians(Rotation.y),
		XMConvertToRadians(Rotation.z)
	);

	// 順番大事！！！！　SRTの順番！！！
	XMMATRIX WorldMatrix = ScalingMatrix * RotationMatrix * TranslationMatrix;


//================================================================
//	プロジェクション行列の作成　カメラの性能
//================================================================
	
	// まずは解像度(アスペクト比)を取得する
	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();

	// 3次元ではXMMatrixPerspective～を使う
	// Fovは視界のこと、LHは左手系の軸のこと。RHだと右手系になる
	//XMMATRIX Projection = XMMatrixPerspectiveFovLH
	//(
	//	XMConvertToRadians(45.0f),		// 視野角45度
	//	SCREEN_WIDTH / SCREEN_HEIGHT,	// アスペクト比(横÷縦)
	//	1.0f,							// 近くの表示限界の距離(近面クリップ)
	//	10.0f							// 遠くの表示限界の距離(遠面クリップ)
	//);

	XMMATRIX Projection = GetProjectionMatrix();


//================================================================
//	カメラのビュー行列の作成　カメラの物理的な状態
//================================================================

	//// カメラの座標
	//XMVECTOR vPos = XMVectorSet(0.0f, 0.0f, -4.0f, 0.0f);	// x,y,zの順
	//// カメラの注視点(見ている場所)
	//XMVECTOR vAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//// カメラの向いている方向
	//XMVECTOR vUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);		// 上方ベクトル、固定値
	//XMMATRIX View = XMMatrixLookAtLH(vPos, vAt, vUp);
	
	XMMATRIX View = GetViewMatrix();

//================================================================
//	最終的な変換行列を作成
//================================================================

	// WorldViewProjection
	// 乗算の順番が決まってる！！！！ WVPの順番！！！！
	XMMATRIX WVP = WorldMatrix * View * Projection;

	// 変換行列を頂点シェーダーへセット
	Shader_SetMatrix(WVP);

	// シェーダーを描画パイプラインへ設定
	Shader_Begin();

	// 頂点データを頂点バッファにコピーする
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	Vertex* vertex = (Vertex*)msr.pData;
	// 頂点データをコピーする
	CopyMemory(&vertex[0], &vdata[0], sizeof(Vertex) * NUM_VERTEX); // コピーのアクセス権をもらう
	// コピー完了
	g_pContext->Unmap(g_VertexBuffer, 0); // コピーが終わったことを伝える、アクセス権を返す

	// テクスチャをセット
	g_pContext->PSSetShaderResources(0, 1, &g_Texture);

	// 頂点バッファをセット
	UINT stride = sizeof(Vertex);	// 頂点1個のデータサイズ
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// インデックスバッファをセット
	g_pContext->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R32_UINT, 0); // 何の型(UINT)を使っているかを教えないといけない

	// 描画するポリゴンの種類をセット
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // TRIANGLELISTは3頂点で1ポリゴン

	// 描画リクエスト
	// g_pContext->Draw(NUM_VERTEX, 0);

	// DrawIndexed(表示に使いたいインデックスの数, 0, 0);
	g_pContext->DrawIndexed((6 * 6), 0, 0);
}