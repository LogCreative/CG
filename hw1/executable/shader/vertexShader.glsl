#version 330 core
layout (location = 0) in vec2 aPos;
void main()
{
    gl_Position = vec4((aPos.x-40.0)/200.0, (aPos.y-40.0)/200.0, 0.0, 1.0);
}