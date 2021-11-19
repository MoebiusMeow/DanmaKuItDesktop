#version 110

attribute vec3 position;
varying vec2 fragCoord;

void main(void)
{
    gl_Position = vec4(position, 1.0);
    fragCoord = vec2(position.x*0.5+0.5, position.y*0.5+0.5);
}

