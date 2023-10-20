#include "peraPolygon.hlsli"
Texture2D<float4> gTexture : register(t0); // 通常テクスチャ
SamplerState gSampler : register(s0); // サンプラー

cbuffer EffectParameters : register(b0) {
    float2 centerPosition;
    float parameterRate;
    int type;
}

//  ピクセルシェーダー
float4 main(Output input) : SV_Target{
    float4 textureColor = gTexture.Sample(gSampler, input.uv);
    float4 result;
    float Y; float4 monochro;
    float4 retoro;
    
    switch(type) {
    case 0:
        Y = (textureColor.x + textureColor.y + textureColor.z) / 3.0f;
        monochro = float4(Y,Y,Y,textureColor.a);
        result = monochro;
        break;
    case 1:
        retoro = float4(textureColor.rgb - fmod(textureColor.rgb, 0.25f),textureColor.a);
        result = retoro;
        break;
    default:
        result = textureColor;
        break;
    }

    //float len = centerPosition.x - input.position.x;
    //len = clamp(len, 0.0f, 1.0f);

    float v = pow(input.position.x - centerPosition.x,2) + 
    pow(input.position.y - centerPosition.y,2);
    if (v <= pow(parameterRate,2)) {
        return result;
    }
/*
    float2 posCenter = input.position.xy - centerPosition;
    length(posCenter) - parameterRate
    result.rgb = lerp(textureColor.rgb, result.rgb, );
*/
    return textureColor;
}