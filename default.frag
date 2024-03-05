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
    vec3 lVec = nlPos - round(fPos*16.0f)/16.0f;
    float lDis = length(lVec);
    float lSte = 1.0f/(0.1f*pow(lDis,2)+0.05f*lDis+1.0f);
    vec3 lDir = normalize(lVec);
    //vec3 lDir = normalize(vec3(-1.0f));
    //vec3 Nor = normalize(fPos-0.5f);
    float diffuse = max(dot(Nor, lDir), 0.0f)*lSte;

    vec3 refl = normalize(reflect(-lDir,Nor));
    float glae = pow(max(dot(refl,cDir),0.0f),8)*0.5f;

    return vec4(lCol * round((diffuse + fSpe.r * glae)*16.0f)/16.0f,1.0f);
}

vec4 SkyLight()
{
    float ambi = 0.1f;
    vec3 lDir = normalize(lDir);
    float diffuse = max(dot(Nor, lDir), 0.0f);

    vec3 refl = normalize(reflect(-lDir,Nor));
    float glae = pow(max(dot(refl,cDir),0.0f),8)*0.5f;

    return vec4(lCol * (diffuse + ambi + round(glae*8.0f)/8.0f),1.0f);
}

void main()
{
    Nor = normalize(fNor);
    cDir = normalize(cPos-round(fPos*8.0f)/8.0f);
    //int k = Data;
    //tCol = texture(tex0, tPos).x*vec4(100.0f/255.0f,198.0f/255.0f,67.0f/255.0f,1.0f);
    tCol = texture(tex0, tPos);
    //fSpe = texture(tex1, tPos);
    //vec3 tCol = fCol;
    FragColor = vec4(vec3(tCol.x*lVal),1.0f);
    //FragColor = tCol*(SkyLight()+PointLight(vec3(10.0f)));
}