#version 110

// The MIT License
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// Color Wave Effect 
// @MoebiusMeow

uniform vec2      iResolution;           // viewport resolution (in pixels)
uniform float     iTime;                 // shader playback time (in seconds)
uniform float     iRoundRadius;          // round-rect radius (discarding round corner)
varying vec2 fragCoord;

// -----------------------------------------------
// Noise Function © 2013 Inigo Quilez
// https://www.shadertoy.com/view/Msf3WH

vec2 hash( vec2 p ) // replace this by something better
{
	p = vec2( dot(p,vec2(127.1,311.7)), dot(p,vec2(269.5,183.3)) );
	return -1.0 + 2.0*fract(sin(p)*43758.5453123);
}

float noise( in vec2 p )
{
    const float K1 = 0.366025404; // (sqrt(3)-1)/2;
    const float K2 = 0.211324865; // (3-sqrt(3))/6;

	vec2  i = floor( p + (p.x+p.y)*K1 );
    vec2  a = p - i + (i.x+i.y)*K2;
    float m = step(a.y,a.x); 
    vec2  o = vec2(m,1.0-m);
    vec2  b = a - o + K2;
	vec2  c = a - 1.0 + 2.0*K2;
    vec3  h = max( 0.5-vec3(dot(a,a), dot(b,b), dot(c,c) ), 0.0 );
	vec3  n = h*h*h*h*vec3( dot(a,hash(i+0.0)), dot(b,hash(i+o)), dot(c,hash(i+1.0)));
    return dot( n, vec3(70.0) );
}

// -----------------------------------------------

float basic_tex( vec2 p )
{
    return (cos(p.x)+sin(p.x*0.27)+cos(p.y)+sin(p.y*0.37)+4.0)*(1.0 + noise(p*0.1+iTime*0.1))*0.125;
    mat2 m = mat2( 1.6,  1.2, -1.2,  1.6 );
    float f  = 0.5*noise( p ); 
    p = m*p;
	f += 0.25*noise( p ); 
    return f;
}

float zag( float f )
{
    float p = sign(fract(f*0.5)-0.5);
    return fract(f) * p + max(-p, 0.0);
}

float calc_block( vec2 p , vec2 block_size)
{
    vec2 ij = (p + vec2(0.20, 0.0) * iTime) / block_size;
    vec2 front = p - fract(ij) * block_size;
    ij = floor(ij);
    if (fract((ij.y) * 0.5) >= 0.5) return 0.0;
    if (front.y <= 0.6 || front.y >= 0.9 - block_size.y) return 0.0;
    float r = noise(ij * vec2(200.0, 10.0));
    if (r > -0.005) return 0.0;
    //r = noise(ij * 2.0);
    float s = front.x + r*0.1;
    float ex = iResolution.x / iResolution.y - 1.0;
    float res = smoothstep(-0.1,0.20,s) * (1.0 - smoothstep(0.80-block_size.x,1.1-block_size.x,s-ex));
    return res;
}

void main(void)
{
    vec2 cor = (-fragCoord + vec2(0.0, 1.0)) * iResolution;
    if (abs(cor.x) <= iRoundRadius && abs(cor.y) <= iRoundRadius && length(cor + vec2(iRoundRadius, -iRoundRadius)) > iRoundRadius) discard;
    cor = (-fragCoord + vec2(1.0, 1.0)) * iResolution;
    if (abs(cor.x) <= iRoundRadius && abs(cor.y) <= iRoundRadius && length(cor + vec2(-iRoundRadius, -iRoundRadius)) > iRoundRadius) discard;
    // discard round corner

    vec2 p = fragCoord;
    p.x *= iResolution.x / iResolution.y;
    vec2 uv = p;
    uv += vec2(0.01, -0.05) * iTime;
    uv += noise(uv*1.0+iTime*0.1) * 0.13;

    float f1 = smoothstep(0.1,1.0,zag(iTime*0.3+basic_tex(uv*3.5)));
    float f2 = smoothstep(0.1,0.4,zag(iTime*0.1+basic_tex(uv*1.5)));
    vec3 col = vec3(0.0, mix(0.3,0.9,f1), mix(1.0,0.8,f1));
    //col.x = clamp(1.0 - 1.0/(col.r*10.0+0.001), 0.0, 0.6);
    //col.y += clamp(1.0 - 0.01/(col.r*10.0+0.01), 0.0, 0.2);
    //col.x += clamp(1.0 - 1.0/(smoothstep(0.1,0.5,col.r)*5.0+1.0), 0.0, 0.3);
    col = max(col, vec3(1.0, 0.0, 0.0) * mix(0.9,0.0,f2) * p.y * p.y);

    col = max(col, calc_block(p, vec2(0.4, 0.04)));
    
    gl_FragColor = vec4(col,1.0);
    //if(col.y < 0.3) discard;
}
