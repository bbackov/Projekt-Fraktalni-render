#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "Shader.h"
#include "common.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    AppState* state=static_cast<AppState*>(glfwGetWindowUserPointer(window));

    if(yoffset>0 && state->fractal.zoom*1.1f<=01e8){
        state->fractal.zoom*=1.1f;
    }
    else if(state->fractal.zoom*0.9f>=0.1){
        state->fractal.zoom*=0.9f;
    }

}

void mouse_callback(GLFWwindow* window, int button, int action, int mods){
    AppState* state=static_cast<AppState*>(glfwGetWindowUserPointer(window));

    if(button==GLFW_MOUSE_BUTTON_LEFT){
        if(action==GLFW_PRESS){
            glfwGetCursorPos(window, &state->mouse.lastPos.first, &state->mouse.lastPos.second);
            state->mouse.dragging=true;
        }
        else if(action==GLFW_RELEASE){
            state->mouse.dragging=false;
        }
    }
    else if(button==GLFW_MOUSE_BUTTON_RIGHT){
        if(action==GLFW_PRESS && !state->rightClickEngaged){
            state->mouse.secondJulija=!state->mouse.secondJulija;
            state->rightClickEngaged=true;
        }
        else if(action==GLFW_RELEASE){
            state->rightClickEngaged=false;
        }
    }
}

void cursor_callback(GLFWwindow* window,double xpos,double ypos){
    AppState* state=static_cast<AppState*>(glfwGetWindowUserPointer(window));

    double dx;
    double dy;
    double normalizationx= 1/(state->fractal.zoom * state->width);
    double normalizationy=1/(state->fractal.zoom * state->height);

    if(state->mouse.dragging==true){
        dx=xpos-state->mouse.lastPos.first;
        dy=ypos-state->mouse.lastPos.second;
        state->mouse.lastPos.first=xpos;
        state->mouse.lastPos.second=ypos;
        state->fractal.translation.first+=(-dx*normalizationx);
        state->fractal.translation.second+=(dy*normalizationy);
    }

}

void keyControl(GLFWwindow* window){
    AppState* state=static_cast<AppState*>(glfwGetWindowUserPointer(window));

    if(state->fractal.iter+5<=2000 && (glfwGetKey(window,GLFW_KEY_KP_ADD)==GLFW_PRESS || glfwGetKey(window,GLFW_KEY_EQUAL)==GLFW_PRESS)){
        state->fractal.iter+=5;
    }
    else if(state->fractal.iter-5>=100 && (glfwGetKey(window,GLFW_KEY_KP_SUBTRACT)==GLFW_PRESS || glfwGetKey(window,GLFW_KEY_MINUS)==GLFW_PRESS)){
        state->fractal.iter-=5;
    }
    else if(glfwGetKey(window,GLFW_KEY_R)==GLFW_PRESS && !state->resetEngaged){
        state->fractal.iter=100;
        state->fractal.translation.first=0;
        state->fractal.translation.second=0;
        state->fractal.zoom=1;
        state->fractal.julijaconstant={0,0};
        state->fractal.multibrotPower=3;
        state->resetEngaged=true;
    }else if(glfwGetKey(window,GLFW_KEY_1)==GLFW_PRESS && !state->selectorEngaged){
        state->fractal.iter=100;
        state->fractal.translation.first=0;
        state->fractal.translation.second=0;
        state->fractal.zoom=1;
        state->fractal.type=FractalType::Mandelbrot;
        state->selectorEngaged=true;
    }else if(glfwGetKey(window,GLFW_KEY_2)==GLFW_PRESS && !state->selectorEngaged){
        state->fractal.iter=100;
        state->fractal.translation.first=0;
        state->fractal.translation.second=0;
        state->fractal.zoom=1;
        state->fractal.julijaconstant={0,0};
        state->fractal.type=FractalType::Julia;
        state->selectorEngaged=true;
    }else if(glfwGetKey(window,GLFW_KEY_3)==GLFW_PRESS && !state->selectorEngaged){
        state->fractal.iter=100;
        state->fractal.translation.first=0;
        state->fractal.translation.second=0;
        state->fractal.zoom=1;
        state->fractal.type=FractalType::Burning_Ship;
        state->selectorEngaged=true;
    }else if(glfwGetKey(window,GLFW_KEY_4)==GLFW_PRESS && !state->selectorEngaged){
        state->fractal.iter=100;
        state->fractal.translation.first=0;
        state->fractal.translation.second=0;
        state->fractal.zoom=1;
        state->fractal.type=FractalType::Multibrot;
        state->fractal.multibrotPower=3;
        state->selectorEngaged=true;
    }else if(glfwGetKey(window,GLFW_KEY_G)==GLFW_PRESS && !state->selectorEngaged){
        if(state->fractal.type==FractalType::Julia){
            if(state->mouse.secondJulija && state->fractal.julijaconstant.second+0.0005<1.5){
                state->fractal.julijaconstant.second+=0.0005;
            }
            else if(state->fractal.julijaconstant.first+0.0005<1.5){
                state->fractal.julijaconstant.first+=0.0005;
            }
        }
        else if(state->fractal.type==FractalType::Multibrot && !state->GEngaged){
            if(state->fractal.multibrotPower+1<=10){
                state->fractal.multibrotPower+=1;
                state->GEngaged=true;
            }
        }
    }else if(glfwGetKey(window,GLFW_KEY_F)==GLFW_PRESS){
        if(state->fractal.type==FractalType::Julia){
            if(state->mouse.secondJulija && state->fractal.julijaconstant.second-0.0005>-1.5){
                state->fractal.julijaconstant.second-=0.0005;
            }
            else if(state->fractal.julijaconstant.first-0.0005>-1.5){
                state->fractal.julijaconstant.first-=0.0005;
            }
        }
        else if(state->fractal.type==FractalType::Multibrot && !state->FEngaged){
            if(state->fractal.multibrotPower-1>=3){
                state->fractal.multibrotPower-=1;
                state->FEngaged=true;
            }
        }
    }

    if(glfwGetKey(window,GLFW_KEY_1)==GLFW_RELEASE || glfwGetKey(window,GLFW_KEY_2)==GLFW_RELEASE || glfwGetKey(window,GLFW_KEY_3)==GLFW_RELEASE || glfwGetKey(window,GLFW_KEY_4)==GLFW_RELEASE){
        state->selectorEngaged=false;
    }
    
    if(glfwGetKey(window,GLFW_KEY_R)==GLFW_RELEASE){
        state->resetEngaged=false;
    }
    
    if(glfwGetKey(window,GLFW_KEY_G)==GLFW_RELEASE)
        state->GEngaged=false;

    if(glfwGetKey(window,GLFW_KEY_F)==GLFW_RELEASE)
        state->FEngaged=false;
    }

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    AppState* state = static_cast<AppState*>(glfwGetWindowUserPointer(window));

    glViewport(0, 0, width, height);

    state->width = width;
    state->height = height;
}

int main(){

    //1. init GLFW
    if(!glfwInit()){
        std::cout<<"GLFW init failed\n";
        return -1;
    }

    //2. OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    //3. Stvaranje prozora
    GLFWwindow* window=glfwCreateWindow(800,600,"Fraktalni render",nullptr,nullptr);

    if(!window){
        std::cout<<"Window creation failed"<<std::endl;
        glfwTerminate();
        return -1;
    }

    //4. Primjenjujemo trenutni kontekst
    
    glfwMakeContextCurrent(window);

    //5.Init GLAD

   if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){

        std::cout<<"GLAD init failed\n";
        return -1;
    }

    // === IMGUI INIT ===
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();
    
    // GLFW + OpenGL3 backend
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    //inicijalizacija shdera
    unsigned int shaderProgram =
        LoadShaderProgram(
            "res/shaders/simple.vert",
            "res/shaders/simple.frag"
    );

    //kvadrat

    float quadVertices[] = {
        // prvi trokut
        -1.0f,  1.0f,
        -1.0f, -1.0f,
        1.0f, -1.0f,

        // drugi trokut
        -1.0f,  1.0f,
        1.0f, -1.0f,
        1.0f,  1.0f
    };


    //generiranje VAO i VBO

    unsigned int VAO,VBO;

    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);

    glBufferData(GL_ARRAY_BUFFER,sizeof(quadVertices),quadVertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    FractalState fractalState;
    fractalState.iter=100;
    fractalState.zoom=1;
    fractalState.translation={0,0};
    fractalState.type=FractalType::Mandelbrot;
    fractalState.julijaconstant={0,0};
    fractalState.multibrotPower=3;

    MouseState mouseState;
    mouseState.dragging=false;
    mouseState.secondJulija=false;
    mouseState.lastPos={0,0};

    AppState state;
    state.fractal=fractalState;
    state.mouse=mouseState;
    glfwGetFramebufferSize(window, &state.width, &state.height);
    state.GEngaged=false;
    state.FEngaged=false;
    state.resetEngaged=false;
    state.rightClickEngaged=false;
    state.selectorEngaged=false;


    //state i prozorom
    glfwSetWindowUserPointer(window, &state);

    //registracija velicine prozora
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // registriranje callback funkcije
    glfwSetScrollCallback(window, scroll_callback);

    // registriranje callback ffunkcije za pritisak misa
    glfwSetMouseButtonCallback(window,mouse_callback);

    //registriranje callback funkcije za pomicanje pokazivača
    glfwSetCursorPosCallback(window,cursor_callback);

    


    //Render loop
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.8f);

        ImGui::Begin(
            "Fractal",
            nullptr,
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_AlwaysAutoResize
        );

        const char* typeName = "Unknown";
        switch(state.fractal.type){
            case FractalType::Mandelbrot: typeName = "Mandelbrot"; break;
            case FractalType::Julia: typeName = "Julia"; break;
            case FractalType::Burning_Ship: typeName = "Burning Ship"; break;
            case FractalType::Multibrot: typeName = "Multibrot"; break;
        }

        ImGui::Text("Fractal Info:");
        ImGui::BulletText("Current fractal: %s", typeName);
        ImGui::BulletText("Iterations: %d", state.fractal.iter);
        ImGui::BulletText("Zoom: %.6f", state.fractal.zoom);
        ImGui::BulletText("X Position: %.6f",state.fractal.translation.first);
        ImGui::BulletText("Y Position: %.6f",state.fractal.translation.second);
        if(state.fractal.type==FractalType::Julia){
            ImGui::BulletText("Julia Constant X: %.6f", state.fractal.julijaconstant.first);
            ImGui::BulletText("Julia Constant Y: %.6f", state.fractal.julijaconstant.second);
        }
        if(state.fractal.type==FractalType::Multibrot){
            ImGui::BulletText("Multibrot Power: %d", state.fractal.multibrotPower);
        }
        ImGui::Separator();

        ImGui::Text("Controls:");
        ImGui::BulletText("1-4  -> Select fractal");
        ImGui::BulletText("+/-  -> Change iterations");
        if(state.fractal.type==FractalType::Julia||state.fractal.type==FractalType::Multibrot){
            ImGui::BulletText("G/F  -> Adjust parameters");
        }
        ImGui::BulletText("R    -> Reset");
        ImGui::BulletText("Scroll -> Zoom");
        ImGui::BulletText("LMB drag -> Pan");
        if(state.fractal.type==FractalType::Julia){
            ImGui::BulletText("RMB -> Switch Julia constant");
        }
        ImGui::End();


        glClear(GL_COLOR_BUFFER_BIT);
        
        //dohvaćanje varijabla
        int resLoc = glGetUniformLocation(shaderProgram, "u_resolution");
        int maxIterLoc= glGetUniformLocation(shaderProgram, "maxIter");
        int zoomLoc= glGetUniformLocation(shaderProgram, "zoom");
        int translationLoc= glGetUniformLocation(shaderProgram, "translation");
        int typeLoc= glGetUniformLocation(shaderProgram, "type");
        int julijaconstLoc=glGetUniformLocation(shaderProgram,"julijaConst");
        int powerLoc=glGetUniformLocation(shaderProgram,"power");


        glUseProgram(shaderProgram);

        keyControl(window);

        //slanje varijablaa
        glUniform1i(maxIterLoc,state.fractal.iter);
        glUniform1f(zoomLoc,state.fractal.zoom);
        glUniform2f(translationLoc,state.fractal.translation.first,state.fractal.translation.second);
        glUniform1i(typeLoc,static_cast<int>(state.fractal.type));
        glUniform2f(resLoc, state.width, state.height);
        glUniform2f(julijaconstLoc,state.fractal.julijaconstant.first,state.fractal.julijaconstant.second);
        glUniform1f(powerLoc,state.fractal.multibrotPower);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,6);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}