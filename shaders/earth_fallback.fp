//  Set the fragment color

uniform sampler2D tex;

varying vec3 fragmentNormal ;
varying vec3 lightVector ;
varying vec3 cameraVector ;

void main()
{
    vec3 fragNormal = normalize( fragmentNormal ) ;

    float light = dot( fragNormal, normalize( lightVector  ) );
    float c = dot( fragNormal, normalize( cameraVector ) );
    c = (c - 0.5) * 2.0 ;

    vec4 color = texture2D(tex,gl_TexCoord[0].xy);

    float bluef2  = (1.0f / (1.0f + exp(c * 20.0f))) * light;
    float redf2   = (1.0f / (1.0f + exp(c * 20.0f))) * pow(light,2.0) * light;
    float greenf2 = redf2;

    /* Each color has its own drop off rate */

    float bluef  = pow(light, 3.0) ;
    float greenf = pow(light, 2.0) ;
    float redf   = pow(light, 1.0) ;

    if( bluef < 0.0 ) bluef = 0.0 ; 
    if( greenf < 0.0 ) greenf = 0.0 ; 
    if( redf < 0.0 ) redf = 0.0 ; 

    gl_FragColor = vec4(
        redf*color[0]   + 1000.0*redf2,
        greenf*color[1] + 1000.0*greenf2,
        bluef*color[2]  + 1000.0*bluef2,
        1.0 );
}
