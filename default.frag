#version 330 core
out vec4 FragColor;

in float lVal;
in vec3 fPos;
in vec2 tPos;
in vec3 fNor;
//in int Data;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform vec3 lDir;
uniform vec3 lPos;
uniform vec3 lCol;
uniform vec3 cPos;

vec3 cDir,Nor;
vec4 tCol,fSpe;

vec4 PointLight(vec3 nlPos)
{
    vec3 lVec = nlPos - fPos;
    float lDis = length(lVec);
    float lSte = 1.0f/(0.1f*pow(lDis,2)+0.05f*lDis+1.0f);
    vec3 lDir = normalize(lVec);
    //vec3 lDir = normalize(vec3(-1.0f));
    //vec3 Nor = normalize(fPos-0.5f);
    float diffuse = max(dot(Nor, lDir), 0.0f)*lSte;

    vec3 refl = normalize(reflect(-lDir,Nor));
    float glae = pow(max(dot(refl,cDir),0.0f),8)*0.5f;

    return vec4(lCol * (diffuse + fSpe.r * glae),1.0f);
}

vec4 SkyLight()
{
    float ambi = 0.1f;
    vec3 lDir = normalize(lDir);
    float diffuse = max(dot(Nor, lDir), 0.0f);

    vec3 refl = normalize(reflect(-lDir,Nor));
    float glae = pow(max(dot(refl,cDir),0.0f),8)*0.5f;

    return vec4(lCol * (diffuse + ambi + fSpe.r * glae),1.0f);
}

void main()
{
    Nor = normalize(fNor);
    cDir = normalize(cPos-fPos);
    //int k = Data;
    //if (k == 0)
    tCol = texture(tex0, tPos);
    //else tCol = texture(tex1, tPos);
    //fSpe = texture(tex1, tPos);
    //vec3 tCol = fCol;
    FragColor = vec4(tCol.xyz*lVal,1.0f);
}