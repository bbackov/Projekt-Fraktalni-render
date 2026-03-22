#version 330 core

out vec4 FragColor;
uniform vec2 u_resolution;
uniform int maxIter;
uniform float zoom;
uniform vec2 translation;
uniform int type;
uniform float power;
uniform vec2 julijaConst;

void main()
{
    // 1. normalizirane koordinate [0,1]
    vec2 uv = gl_FragCoord.xy / u_resolution;
    float aspect = u_resolution.x / u_resolution.y;

    float R;
    float G;
    float B;
    float pi=3.1415;
    float t;
    float smoothIter;

    float centerX=translation.x;
    float centerY=translation.y;

    // 2. mapiranje u kompleksnu ravninu
    float x = centerX + (uv.x-0.5)*aspect/zoom;      
    float y = centerY + (uv.y-0.5)/zoom;      

    int iter=0;
    vec2 c;
    vec2 z;

    if(type==0 || type==2 || type==3){
        c =vec2(x, y);
        z = vec2(0.0);
    }
    else if(type==1){
        c=julijaConst;
        z=vec2(x,y);
    }

    if(type==0 || type==1){
        for (iter = 0; iter < maxIter; iter++) {
            float zx = z.x*z.x - z.y*z.y + c.x;
            float zy = 2.0*z.x*z.y + c.y;
            z = vec2(zx, zy);

            if (length(z) > 2.0){
                break;
            }
        }
    }
    else if(type==2){
        for (iter = 0; iter < maxIter; iter++) {
            z.x=abs(z.x);
            z.y=abs(z.y);
            float zx = z.x*z.x - z.y*z.y + c.x;
            float zy = 2.0*z.x*z.y + c.y;
            z = vec2(zx, zy);

            if (length(z) > 2.0){
                break;
            }
        }
    }
    else if(type==3){
        for (iter = 0; iter < maxIter; iter++) {
            float r = length(z);
            float theta = atan(z.y, z.x);
            
            r = pow(r, power);
            theta = theta * power;

            float zx = r*cos(theta) + c.x;
            float zy= r*sin(theta)+c.y;

            z = vec2(zx, zy);

            if (length(z) > pow(2.0, 1.0 / power)){
                break;
            }
        }
    }
    // 4. boja
    if(iter!=maxIter){
        smoothIter=iter+1-log(log(length(z)))/log(2.0);
        t=smoothIter*0.051874;
        R=0.5*sin(2*pi*t+20)+0.5;
        G=0.5*sin(2*pi*t-9)+0.5;
        B=0.5*sin(2*pi*t-17.2)+0.5;
    }
    else{
        R=0;
        G=0;
        B=0;
    }
    
    FragColor = vec4(R,G,B, 1.0);
}