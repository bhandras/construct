#pragma once


static const char* xyz_rgba_fs =
"varying lowp vec4 v_DestinationColor;								"
"																	"
"void main()														"
"{																	"
"	gl_FragColor = v_DestinationColor;								"
"}																	";


static const char* xyz_rgba_vs =
"attribute vec4 a_position;											"
"attribute vec4 a_source_color;										"
"uniform mat4 u_projection;											"
"																	"
"varying vec4 v_DestinationColor;									"
"																	"
"void main()														"
"{																	"
"	v_DestinationColor = a_source_color;							"
"	gl_Position = u_projection * a_position;						"
"}																	";


static const char* xyz_rgba_uv_fs =
"varying lowp vec4 v_frontColor;									"
"varying lowp vec2 v_txCoor;										"
"																	"
"uniform sampler2D u_texture;										"
"																	"
"void main()														"
"{																	"
"	gl_FragColor = texture2D(u_texture, v_txCoor) * v_frontColor;	"
"}																	";


static const char* xyz_rgba_uv_vs =
"attribute vec3 a_position;											"
"attribute vec4 a_source_color;										"
"attribute vec2 a_texture_coor;										"
"																	"
"uniform mat4 u_projection;											"
"																	"
"varying vec4 v_frontColor;											"
"varying vec2 v_txCoor;												"
"																	"
"void main()														"
"{																	"
"	gl_Position = u_projection * vec4(a_position, 1.);				"
"	v_frontColor = a_source_color;									"
"	v_txCoor = a_texture_coor;										"
"}																	";
