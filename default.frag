#version 330 core
out vec4 FragColor;

in vec3 fPos;
in vec2 tPos;
in vec3 fNor;
in vec3 fCol;

uniform sampler2D tex0;
//uniform vec3 lDir;
uniform vec3 lPos;
uniform vec3 lCol;
uniform vec3 cPos;

void main()
{
    vec3 lDir = normalize(lPos - fPos);
    //vec3 lDir = normalize(vec3(-1.0f));
    //vec3 Nor = normalize(fNor);
    vec3 Nor = normalize(fPos-0.5f);
    float diffuse = max(dot(Nor, lDir), 0.0f);

    vec3 cDir = normalize(cPos-fPos);
    vec3 refl = normalize(reflect(-lDir,Nor));
    float glae = pow(max(dot(refl,cDir),0.0f),16)*0.5f;

    float ambi = 0.2f;
    //vec4 tCol = texture(tex0, tPos);
    vec4 tCol = vec4(1.0f);
    tCol.xyz = tCol.xyz * lCol * (diffuse + glae + ambi);
    FragColor = tCol;
}