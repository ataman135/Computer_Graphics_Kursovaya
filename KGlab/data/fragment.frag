#version 120

uniform sampler2D tex; // Наша основная текстура (слот 0)

varying vec2 TexCoord;
varying vec4 VertColor;
varying vec3 Normal;
varying vec3 ViewPos;

void main()
{
    vec4 texColor = texture2D(tex, TexCoord) * VertColor;
    
    vec3 viewDir = normalize(-ViewPos);
    vec3 N = normalize(Normal);
    
    vec3 lightDir = normalize(vec3(0.2, 0.8, 0.4)); 

    float diffuseFactor = max(dot(N, lightDir), 0.0);
    
    vec3 ambient = vec3(0.4, 0.4, 0.4);
    vec3 diffuse = vec3(0.6, 0.6, 0.6) * diffuseFactor;

    vec3 halfDir = normalize(lightDir + viewDir);
    float specFactor = pow(max(dot(N, halfDir), 0.0), 32.0);
    vec3 specular = vec3(0.4, 0.4, 0.4) * specFactor;

    float isShiny = max(texColor.r, texColor.b) - texColor.g * 0.3;
    isShiny = clamp(isShiny * 2.0, 0.0, 1.0);

    vec3 finalRGB = texColor.rgb * (ambient + diffuse) + (specular * isShiny);
    
    gl_FragColor = vec4(finalRGB, texColor.a);
}