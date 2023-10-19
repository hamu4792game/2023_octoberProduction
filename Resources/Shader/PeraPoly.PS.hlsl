#include "peraPolygon.hlsli"
Texture2D<float4> gTexture : register(t0); // 通常テクスチャ
SamplerState gSampler : register(s0); // サンプラー

/*
cbuffer EffectParameters : register(b0) {
    float2 centerPosition;
    float parameterRate;
    int type;
}*/

//  ピクセルシェーダー
float4 main(Output input) : SV_Target{
    float4 textureColor = gTexture.Sample(gSampler, input.uv);
    //float4 result;
    
    //float4 retoro = float4(textureColor.rgb - fmod(textureColor.rgb, 0.25f),textureColor.a);
    /*if (type == 0) {
        float Y = (textureColor.x + textureColor.y + textureColor.z) / 3.0f;
        float4 monochro = float4(Y,Y,Y,textureColor.a);
        result = monochro;
    } 
    else {
        result = textureColor;
    }*/

    return textureColor;
}