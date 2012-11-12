attribute vec3 a_position;
attribute vec4 a_source_color;
attribute vec2 a_texture_coor;

uniform mat4 u_projection;

varying vec4 v_frontColor;
varying vec2 v_txCoor;

void main() 
{
	gl_Position = u_projection * vec4(a_position, 1.);
	v_frontColor = a_source_color;
	v_txCoor = a_texture_coor;
}
