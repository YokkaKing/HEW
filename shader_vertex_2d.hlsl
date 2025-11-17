/*==============================================================================

   2D描画用頂点シェーダー [shader_vertex_2d.hlsl]
--------------------------------------------------------------------------------

==============================================================================*/

// 定数バッファ
cbuffer Buffer0 : register(b0)
{
    float4x4 mtx; //C言語から渡されたデータが入っている
}
cbuffer Buffer1 : register(b1)
{
    float4x4 World; //C言語から渡されたデータが入っている
}
struct LIGHT
{
    // C++側のLIGHT構造体と全く同じ順番で書け
    // 全く同じようにコピーされるから違ってたらイかれる
    bool Enable;
    bool3 dummy;
    float4 Direction;
    float4 Diffuse;
    float Ambient;
};
cbuffer Buffer2 : register(b2)
{
    LIGHT Light; // C言語から受け取る器を作る
}

//入力用頂点構造体
struct VS_INPUT
{ //              V コロン！
    float4 posL : POSITION0;    //頂点座標 オーでなくゼロ！
    float4 normal : NORMAL0;    // 法線
    float4  color : COLOR0;     //頂点カラー（R,G,B,A）
    float2 texcoord : TEXCOORD0;
};

//出力用頂点構造体
struct VS_OUTPUT
{
    float4  posH : SV_POSITION;     //変換済頂点座標
    float4  color : COLOR0;         //頂点カラー
    float2 texcoord : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT vs_in)
{
    VS_OUTPUT vs_out;   //出力用構造体変数
    
    //頂点を行列で変換
    vs_out.posH = mul(vs_in.posL, mtx);
    //頂点カラーはそのまま出力
    vs_out.color = vs_in.color;

    vs_out.texcoord = vs_in.texcoord;

    // ライティング
    if (Light.Enable)
    {
        // 全部ベクトルだと思え
        // 法線をワールド変換
        float4 normal = float4(vs_in.normal.xyz, 0.0f); // 法線をコピー
        normal = mul(normal, World);    // normalをWorld行列で変換
        normal = normalize(normal);     // normalを正規化する
        
        // ライティング
        float light = -dot(normal.xyz, Light.Direction.xyz);
        light = saturate(light);
        vs_out.color.rgb *= light; // 頂点の明るさを調節(最高で元の色、最低で真っ黒)
        vs_out.color.rgb += Light.Ambient;
    }
    
    //結果を出力する
    return vs_out;
}