#shader vertex
#version 330 core
		
layout(location = 0) in vec4 position;
layout(location = 1) in float texNumber;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec4 vertexColor;

out float v_TexNumber;
out vec2 v_TexCoord; //v´ú±ívarying
out vec4 v_VertexColor;


uniform mat4 u_MVP;


void main()
{
	v_TexNumber = texNumber;
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
	v_VertexColor = vertexColor;
}

#shader fragment
#version 330 core



layout(location = 0) out vec4 color;

in float v_TexNumber;
in vec2 v_TexCoord;
in vec4 v_VertexColor;
uniform vec4 u_Color;
uniform sampler2D u_Textures[2];


void main()
{
	int index = int(v_TexNumber);
	vec4 texColor = texture(u_Textures[index], v_TexCoord);
	color = texColor;
	//color = v_VertexColor;
}