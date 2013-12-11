uniform sampler2D tex;

varying vec3 fragmentNormal ;
varying vec3 lightVector ;

void main() {
	float f = dot( normalize( fragmentNormal ), normalize( lightVector ) );
	vec4 color = texture2D( tex, gl_TexCoord[0].xy );

	f = ( 1.0f / (1.0f + exp( -f * 150.0f ) ) ) ;

	gl_FragColor = f * color;
}
