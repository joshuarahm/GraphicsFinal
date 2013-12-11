//  Set the fragment color

uniform sampler2D tex;

varying vec3 fragmentNormal ;
varying vec3 lightVector ;
varying vec3 cameraVector ;

void main()
{
    vec3 fragNormal = normalize( fragmentNormal ) ;

    float f = dot( fragNormal, normalize( lightVector  ) );
    float c = dot( fragNormal, normalize( cameraVector ) );
    c = (c - 0.5) * 2.0 ;

    vec4 color = texture2D(tex,gl_TexCoord[0].xy + vec2( 0.5, -0.2 ));

    float bluef = (1.0f / (1.0f + exp(-f * 150.0f)));

    float bluef2  = (1.0f / (1.0f + exp(c * 20.0f))) * f;
    float redf2   = (1.0f / (1.0f + exp(c * 20.0f))) * pow(f,2) * f;
    float greenf2 = redf2;

    float alpha   = (1.0f / (1.0f + exp(c * 30.0f))) * f;
    
    /* Each color has its own drop off rate */
    float greenf = (1.0f / (1.0f + exp(-f * 60.0f)));
    float redf   = (1.0f / (1.0f + exp(-f * 30.0f)));

    gl_FragColor = vec4(
        redf*color[0] + 1000.0*redf2,
        greenf*color[1] + 1000.0*greenf2,
        bluef*color[2] + 1000.0*bluef2,
        1.0-1000.0*alpha );
}
