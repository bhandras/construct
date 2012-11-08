attribute vec4 a_position;
attribute vec4 a_source_color;
uniform mat4 u_projection;

varying vec4 v_DestinationColor;

void main()
{
    v_DestinationColor = a_source_color;
	gl_Position = u_projection * a_position;
}