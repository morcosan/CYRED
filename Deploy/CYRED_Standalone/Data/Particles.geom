#version 430

layout(points) in;
layout(triangle_strip, max_vertices = 6) out;

uniform mat4 	DEFAULT_view;
uniform mat4 	DEFAULT_projection;
uniform float 	lifetime;

in float 	INTER_age[];
in float 	INTER_size[];
in vec4 	INTER_color[];

out vec2 INTER_fragUV;
out vec4 INTER_fragColor;


void main() 
{
	if ( INTER_age[0] >= 0 && INTER_age[0] < lifetime )
	{
		float halfSize = INTER_size[0] / 2;

		mat4 MV = DEFAULT_view;
		mat4 VP = DEFAULT_projection * DEFAULT_view;
 
		vec3 right = vec3( MV[0][0], 
		                   MV[1][0], 
		                   MV[2][0] );

		vec3 up = vec3( MV[0][1], 
		                MV[1][1], 
		                MV[2][1] );

		vec3 pos = gl_in[0].gl_Position.xyz;

		{
			INTER_fragUV = vec2( 0, 0 );
			INTER_fragColor = INTER_color[0];
			gl_Position = VP * vec4( pos - (right + up) * halfSize, 1 );
			EmitVertex();
		}
		{
			INTER_fragUV = vec2( 1, 0 );
			INTER_fragColor = INTER_color[0];
			gl_Position = VP * vec4( pos + (right - up) * halfSize, 1 );
			EmitVertex();
		}
		{
			INTER_fragUV = vec2( 1, 1 );
			INTER_fragColor = INTER_color[0];
			gl_Position = VP * vec4( pos + (right + up) * halfSize, 1 );
			EmitVertex();
		}

		{
			INTER_fragUV = vec2( 1, 1 );
			INTER_fragColor = INTER_color[0];
			gl_Position = VP * vec4( pos + (right + up) * halfSize, 1 );
			EmitVertex();
		}
		{
			INTER_fragUV = vec2( 0, 0 );
			INTER_fragColor = INTER_color[0];
			gl_Position = VP * vec4( pos - (right + up) * halfSize, 1 );
			EmitVertex();
		}
		{
			INTER_fragUV = vec2( 0, 1 );
			INTER_fragColor = INTER_color[0];
			gl_Position = VP * vec4( pos - (right - up) * halfSize, 1 );
			EmitVertex();
		}
	}
}