//  Set the fragment color

uniform sampler2D tex;

varying vec3 fragmentNormal ;
varying vec3 lightVector ;
varying vec3 cameraVector ;
uniform float time ;

void main()
{
    vec3 fragNormal = normalize( fragmentNormal ) ;

    float f = dot( fragNormal, normalize( lightVector  ) );
    float c = dot( fragNormal, normalize( cameraVector ) );
    c = (c - 0.5) * 2.0 ;

    vec4 color = texture2D(tex,gl_TexCoord[0].xy + vec2( -time, -0.2 ));

    float bluef2  = 0.0f;
    float redf2   = 0.0f;
    float greenf2 = 0.0f;

    if( f > 0.0 ) {
        bluef2  = (1.0f / (1.0f + exp(c * 20.0f))) * f;
        redf2   = 200.0 * (1.0f / (1.0f + exp(c * 20.0f))) * pow(f,10.0) * f;
        greenf2 = 200.0 * (1.0f / (1.0f + exp(c * 20.0f))) * pow(f,15.0) * f;
    }

    float alpha   = (1.0f / (1.0f + exp(c * 30.0f))) * f;
    
    /* Each color has its own drop off rate */
    float bluef  = (0.8f / (1.0f + exp(-f * 100.0f)));
    float greenf = (0.8f / (1.0f + exp(-f * 40.0f))) ;
    float redf   = (0.8f / (1.0f + exp(-f * 20.0f))) ;

    gl_FragColor = vec4(
        min(1.0,(redf*color[0] + 1000.0*redf2)),
        min(1.0,(greenf*color[1] + 1000.0*greenf2)),
        min(1.0,(bluef*color[2] + 1000.0*bluef2)),
        min(1.0,max(1.0-1000.0*alpha,0.0)) );
}
