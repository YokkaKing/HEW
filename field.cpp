/*
* ファイル名	field.cpp
* タイトル	フィールド
* 作成者		久保木幹太
* 作成日		10月15日
* 更新日
*/

//================================================================
//	インクルード
//================================================================
#include"field.h"
#include"sprite.h"//スプライト機能を追加
#include"shader.h"
#include"camera.h"
#include"model.h"

//================================================================
//	マクロ定義
//================================================================
#define BOX_NUM_VERTEX (24)

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

MODEL* Model[FIELD_MAX] = { NULL };

// BOXの頂点データ
static Vertex3D boxVertexData[BOX_NUM_VERTEX] =
{
//================================================================
//	-Z面
//================================================================
	{	// 頂点0 LEFT-TOP 左上
		XMFLOAT3(-0.5f, 0.5f, -0.5f),		// 座標
		XMFLOAT3(0.0f, 0.0f, 1.0f),			// 法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(0.0f, 0.0f)				// テクスチャ座標
	},
	{	// 頂点1 RIGHT-TOP 右上
		XMFLOAT3(0.5f, 0.5f, -0.5f),			// 座標
		XMFLOAT3(0.0f, 0.0f, 1.0f),			// 法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(1.0f, 0.0f)				// テクスチャ座標
	},
	{	// 頂点2 LEFT-BOTTOM 左下
		XMFLOAT3(-0.5f, -0.5f, -0.5f),		// 座標
		XMFLOAT3(0.0f, 0.0f, 1.0f),			// 法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(0.0f, 1.0f)				// テクスチャ座標
	},
	{	// 頂点3 RIGHT-TOP 右下
		XMFLOAT3(0.5f, -0.5f, -0.5f),			// 座標
		XMFLOAT3(0.0f, 0.0f, 1.0f),			// 法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(1.0f, 1.0f)				// テクスチャ座標
	},

//================================================================
//	+X面
//================================================================
	{	// 頂点4 左上
		XMFLOAT3(0.5f, 0.5f, -0.5f),			// 座標
		XMFLOAT3(-1.0f, 0.0f, 0.0f),			// 法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(0.0f, 0.0f)				// テクスチャ座標
	},
	{	// 頂点5 右上
		XMFLOAT3(0.5f, 0.5f, 0.5f),			// 座標
		XMFLOAT3(-1.0f, 0.0f, 0.0f),			// 法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(1.0f, 0.0f)				// テクスチャ座標
	},
	{	// 頂点6 左下
		XMFLOAT3(0.5f, -0.5f, -0.5f),			// 座標
		XMFLOAT3(-1.0f, 0.0f, 0.0f),			// 法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(0.0f, 1.0f)				// テクスチャ座標
	},
	{	// 頂点7 右下
		XMFLOAT3(0.5f, -0.5f, 0.5f),			// 座標
		XMFLOAT3(-1.0f, 0.0f, 0.0f),			// 法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(1.0f, 1.0f)				// テクスチャ座標
	},

//================================================================
//	+Z面
//================================================================
	{	// 頂点8 左上
		XMFLOAT3(0.5f, 0.5f, 0.5f),			// 座標
		XMFLOAT3(0.0f, 0.0f, -1.0f),			// 法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(0.0f, 0.0f)				// テクスチャ座標
	},
	{	// 頂点9 右上
		XMFLOAT3(-0.5f, 0.5f, 0.5f),		// 座標
		XMFLOAT3(0.0f, 0.0f, -1.0f),			// 法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(1.0f, 0.0f)				// テクスチャ座標
	},
	{	// 頂点10 左下
		XMFLOAT3(0.5f, -0.5f, 0.5f),			// 座標
		XMFLOAT3(0.0f, 0.0f, -1.0f),			// 法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(0.0f, 1.0f)				// テクスチャ座標
	},
	{	// 頂点11 右下
		XMFLOAT3(-0.5f, -0.5f, 0.5f),		// 座標
		XMFLOAT3(0.0f, 0.0f, -1.0f),			// 法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(1.0f, 1.0f)				// テクスチャ座標
	},

//================================================================
//	-X面
//================================================================
	{	// 頂点12 左上
		XMFLOAT3(-0.5f, 0.5f, 0.5f),		// 座標
		XMFLOAT3(1.0f, 0.0f, 0.0f),			// 法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(0.0f, 0.0f)				// テクスチャ座標
	},
	{	// 頂点13 右上
		XMFLOAT3(-0.5f, 0.5f, -0.5f),		// 座標
		XMFLOAT3(1.0f, 0.0f, 0.0f),			// 法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(1.0f, 0.0f)				// テクスチャ座標
	},
	{	// 頂点14 左下
		XMFLOAT3(-0.5f, -0.5f, 0.5f),		// 座標
		XMFLOAT3(1.0f, 0.0f, 0.0f),			// 法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(0.0f, 1.0f)				// テクスチャ座標
	},
	{	// 頂点15 右下
		XMFLOAT3(-0.5f, -0.5f, -0.5f),		// 座標
		XMFLOAT3(1.0f, 0.0f, 0.0f),			// 法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(1.0f, 1.0f)				// テクスチャ座標
	},

//================================================================
//	+Y面
//================================================================
	{	// 頂点16 左上
		XMFLOAT3(-0.5f, 0.5f, 0.5f),		// 座標
		XMFLOAT3(0.0f, -1.0f, 0.0f),			// 法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(0.0f, 0.0f)				// テクスチャ座標
	},
	{	// 頂点17 右上
		XMFLOAT3(0.5f, 0.5f, 0.5f),			// 座標
		XMFLOAT3(0.0f, -1.0f, 0.0f),			// 法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(1.0f, 0.0f)				// テクスチャ座標
	},
	{	// 頂点18 左下
		XMFLOAT3(-0.5f, 0.5f, -0.5f),		// 座標
		XMFLOAT3(0.0f, -1.0f, 0.0f),			// 法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(0.0f, 0.25f)				// テクスチャ座標
	},
	{	// 頂点19 右下
		XMFLOAT3(0.5f, 0.5f, -0.5f),		// 座標
		XMFLOAT3(0.0f, -1.0f, 0.0f),			// 法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(1.0f, 0.25f)				// テクスチャ座標
	},

//================================================================
//	-Y面
//================================================================
	{	// 頂点20 左上
		XMFLOAT3(-0.5f, -0.5f, -0.5f),		// 座標
		XMFLOAT3(0.0f, 1.0f, 0.0f),			// 法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(0.0f, 0.75f)				// テクスチャ座標
	},
	{	// 頂点21 右上
		XMFLOAT3(0.5f, -0.5f, -0.5f),		// 座標
		XMFLOAT3(0.0f, 1.0f, 0.0f),			// 法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(1.0f, 0.75f)				// テクスチャ座標
	},
	{	// 頂点22 左下
	XMFLOAT3(-0.5f, -0.5f, 0.5f),		// 座標
		XMFLOAT3(0.0f, 1.0f, 0.0f),			// 法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(0.0f, 1.0f)				// テクスチャ座標
	},
	{	// 頂点23 右下
		XMFLOAT3(0.5f, -0.5f, 0.5f),			// 座標
		XMFLOAT3(0.0f, 1.0f, 0.0f),			// 法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	// 色
		XMFLOAT2(1.0f, 1.0f)				// テクスチャ座標
	},
};

// 頂点の数字だけを指定して使いまわすことができる
static UINT boxIndexData[6 * 6] =
{
	0, 1, 2, 2, 1, 3,		// -Z面
	4, 5, 6, 6, 5, 7,		// +X面
	8, 9, 10, 10, 9, 11,	// +Z面
	12, 13, 14, 14, 13, 15,	// -X面
	16, 17, 18, 18, 17, 19,	// +Y面
	20, 21, 22, 22, 21, 23,	// -Y面
};

// マップデータ配列
MAPDATA Map[] =
{
	// 座標とその座標に表示したい座標の種類
	{XMFLOAT3(-1.0f, -0.5f, 1.0f), FIELD_BOX},
	{XMFLOAT3(-1.0f, -0.5f, 2.0f), FIELD_BOX},
	{XMFLOAT3(-1.0f, -0.5f, 3.0f), FIELD_BOX},
	{XMFLOAT3(0.0f, -0.5f, 1.0f), FIELD_BOX},
	{XMFLOAT3(0.0f, -0.5f, 2.0f), FIELD_BOX},
	{XMFLOAT3(0.0f, 0.5f, 2.0f), FIELD_GINGER_DOCTOR},
	{XMFLOAT3(0.0f, -0.5f, 3.0f), FIELD_BOX},
	{XMFLOAT3(1.0f, -0.5f, 1.0f), FIELD_BOX},
	{XMFLOAT3(1.0f, -0.5f, 2.0f), FIELD_BOX},
	{XMFLOAT3(1.0f, -0.5f, 3.0f), FIELD_BOX},
	{XMFLOAT3(2.0f, -0.5f, 1.0f), FIELD_BOX},
	{XMFLOAT3(2.0f, -0.5f, 2.0f), FIELD_BOX},
	{XMFLOAT3(2.0f, -0.5f, 3.0f), FIELD_BOX},

	{XMFLOAT3(2.0f, -1.0f, 5.0f), FIELD_MAX} // MAPデータ終了
};

void Field_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	// TEST = ModelLoad("asset\\model\\test.fbx"); // デバッグ

	g_pDevice = pDevice;
	g_pContext = pContext;

	// テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"Asset\\Texture\\block_field.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(),
		image.GetImageCount(), metadata, &g_Texture);
	assert(g_Texture);

	// ブロック作成
	for (int i = 0; i < FIELD_MAX; i++)
	{
		switch (i)
		{
		case FIELD_BOX:
			CreateBox();
			break;

		case FIELD_GINGER_DOCTOR:
			Model[FIELD_GINGER_DOCTOR] = ModelLoad("asset\\model\\tree.fbx");
			break;
			
		case FIELD_MAX:
			break;

		default:
			break;
		}
	}
}
void Field_Finalize(void)
{
	for (int i = 0; i < FIELD_MAX; i++)
	{
		if (Model[i] != NULL)
		{
			ModelRelease(Model[i]);
			Model[i] = NULL;
		}
	}

	SAFE_RELEASE(g_VertexBuffer);
	SAFE_RELEASE(g_IndexBuffer);
	SAFE_RELEASE(g_Texture);
}
void Field_Update(void)
{

}
void Field_Draw(void)
{
	// シェーダーを描画パイプラインへ設定
	Shader_Begin();

	// プロジェクション行列作成
	XMMATRIX Projection = GetProjectionMatrix();
	// ビュー行列作成
	XMMATRIX View = GetViewMatrix();
	//先にVP変換行列を作っておく
	// 何回も繰り返し使う
	XMMATRIX VP = View * Projection;

	// MAPの表示
	int i = 0;
	static float rot = 0.0f;
	rot -= 0.5f;
	while (Map[i].type != FIELD_MAX)
	{
		// スケーリング行列の作成
		// 基本ブロックの大きさは変わらないから1.0f固定
		XMMATRIX ScalingMatrix = XMMatrixScaling
		(
			1.0f,
			1.0f,
			1.0f
		);

		// 平行移動行列の作成
		XMMATRIX TranslationMatrix = XMMatrixTranslation
		(
			Map[i].pos.x,
			Map[i].pos.y,
			Map[i].pos.z
		);

		// 回転行列の作成
		// 基本回転しないから0.0f固定
		XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw
		(
			// 間違えないと思うが、xyzの順番で
			// XMConvertToRadians(rot),
			XMConvertToRadians(0.0f),
			XMConvertToRadians(0.0f),
			XMConvertToRadians(0.0f)
		);

		// ワールド行列の作成
		XMMATRIX World = ScalingMatrix * RotationMatrix * TranslationMatrix;
		// 最終的な変換行列を作成
		XMMATRIX WVP = World * VP; // VP = (View * Projection)
		// DirectXへ行列をセット
		Shader_SetWorldMatrix(World);
		Shader_SetMatrix(WVP);

		// テクスチャをセット
		g_pContext->PSSetShaderResources(0, 1, &g_Texture);

		// 頂点バッファをセット
		UINT stride = sizeof(Vertex3D);	// 頂点1個のデータサイズ
		UINT offset = 0;
		g_pContext->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

		// インデックスバッファをセット
		g_pContext->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R32_UINT, 0); // 何の型(UINT)を使っているかを教えないといけない

		// 描画するポリゴンの種類をセット
		g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // TRIANGLELISTは3頂点で1ポリゴン


		if (Map[i].type == FIELD_BOX)
		{
			g_pContext->DrawIndexed(6 * 6, 0, 0);
		}
		else
		{
			ModelDraw(Model[Map[i].type]);
		}

		// 描画リクエスト
		// DrawIndexed(表示に使いたいインデックスの数, 0, 0);
		// g_pContext->DrawIndexed((6 * 6), 0, 0);

		i++;
	}
}

// BOX作成関数
void CreateBox()
{
	// 頂点バッファ作成
	D3D11_BUFFER_DESC bd; // 構造体
	ZeroMemory(&bd, sizeof(bd)); // 0でクリア
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex3D) * BOX_NUM_VERTEX; // 格納できる丁点数 * 頂点サイズ
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	g_pDevice->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	Vertex3D* vertex = (Vertex3D*)msr.pData;

	// インデックスデータをバッファへコピー
	CopyMemory(&vertex[0], &boxVertexData[0], sizeof(Vertex3D) * BOX_NUM_VERTEX);
	g_pContext->Unmap(g_VertexBuffer, 0);


	// インデックスバッファ作成
	{
		// 頂点バッファ作成
		D3D11_BUFFER_DESC bd; // 構造体
		ZeroMemory(&bd, sizeof(bd)); // 0でクリア
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(UINT) * (6 * 6); // 格納できる丁点数 * 頂点サイズ
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		g_pDevice->CreateBuffer(&bd, NULL, &g_IndexBuffer);

		// インデックスバッファへ書き込み
		D3D11_MAPPED_SUBRESOURCE msr;
		g_pContext->Map(g_IndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		UINT* index = (UINT*)msr.pData;

		// インデックスデータをバッファへコピー
		CopyMemory(&index[0], &boxIndexData[0], sizeof(UINT) * (6 * 6));
		g_pContext->Unmap(g_IndexBuffer, 0);
	}
}

MAPDATA* GetFieldMap()
{
	return &Map[0];

	// こう書いてもいい
	// return Map;
}