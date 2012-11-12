varying lowp vec4 v_frontColor;
varying lowp vec2 v_txCoor;

uniform sampler2D u_texture;

void main() 
{
	gl_FragColor = texture2D(u_texture, v_txCoor) * v_frontColor;
}
