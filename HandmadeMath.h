/*
  Author: Zak Strange
  
*/

#ifndef HANDMADE_MATH_H
#define HANDMADE_MATH_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HANDMADEMATH_STATIC
#define HMMDEF static
#else
#define HMMDEF extern
#endif

#if _MSC_VER && !__INTEL_COMPILER
#define HINLINE __inline 
#else
#define HINLLINE inline
#endif
    
typedef union vec2
{
    struct
    {
        float x, y;
    };
    
    struct
    {
        float u, v;
    };
    
    struct
    {
        float left, right;
    };

    float Elements[2];
} vec2;

typedef union vec3
{
    struct
    {
        float x, y, z;
    };
    
    struct
    {
        float u, v, w;
    };
    
    struct
    {
        float r, g, b;
    };
    
    struct
    {
        vec2 xy;
        float Ignored0_;
    };
    
    struct
    {
        float Ignored1_;
        vec2 yz;
    };
    
    struct
    {
        vec2 uv;
        float Ignored2_;
    };
    
    struct
    {
        float Ignored3_;
        vec2 vw;
    };

    float Elements[3];
} vec3;

typedef union vec4
{
    struct
    {
        union
        {
            vec3 xyz;
            struct
            {
                float x, y, z;
            };
        };
        
        float w;        
    };
    struct
    {
        union
        {
            vec3 rgb;
            struct
            {
                float r, g, b;
            };
        };
        
        float a;        
    };
    
    struct
    {
        vec2 xy;
        float Ignored0_;
        float Ignored1_;
    };
    
    struct
    {
        float Ignored2_;
        vec2 yz;
        float Ignored3_;
    };
    
    struct
    {
        float Ignored4_;
        float Ignored5_;
        vec2 zw;
    };
    
    float E[4];
} vec4;

    
HMMDEF HINLINE vec2 V2i(int X, int Y);
HMMDEF HINLINE vec2 V2(float X, float Y);

#ifdef __cplusplus
}
#endif

#endif /* HANDMADE_MATH_H */

#ifdef HANDMADE_MATH_IMPLEMENTATION

HINLINE vec2 V2i(int X, int Y)
{
    vec2 Result = {0};

    Result.x = (float)X;
    Result.y = (float)Y;

    return(Result);
}
    
HINLINE vec2 V2(float X, float Y)
{
    vec2 Result = {0};

    Result.x = X;
    Result.y = Y;

    return(Result);
}


HINLINE vec3 V3i(int X, int Y, int Z)
{
    vec3 Result = {0};

    Result.x = (float)X;
    Result.y = (float)Y;
    Result.z = (float)Z;
    
    return(Result);
}
    
HINLINE vec3 V3(float X, float Y, float Z)
{
    vec3 Result = {0};

    Result.x = X;
    Result.y = Y;
    Result.z = Z;

    return(Result);
}

HINLINE vec4 V4i(int X, int Y, int Z, int W)
{
    vec4 Result = {0};

    Result.x = (float)X;
    Result.y = (float)Y;
    Result.z = (float)Z;
    Result.w = (float)W;
    
    return(Result);
}
    
HINLINE vec4 V4(float X, float Y, float Z, float W)
{
    vec4 Result = {0};

    Result.x = X;
    Result.y = Y;
    Result.z = Z;
    Result.w = W;

    return(Result);
}

#endif


