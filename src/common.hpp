
#pragma once

#include <utility>  
    
enum class FractalType{
    Mandelbrot=0,Julia,Burning_Ship,Multibrot
};

struct FractalState{
    int iter;
    double zoom;
    std::pair<double,double> translation;
    FractalType type;
    std::pair<double,double> julijaconstant;
    float multibrotPower;       
};

struct MouseState{
    bool dragging;
    bool secondJulija;
    std::pair<double,double> lastPos;
};

struct AppState{
    FractalState fractal;
    MouseState mouse;
    int width;
    int height;
    bool resetEngaged;
    bool selectorEngaged;
    bool GEngaged;
    bool FEngaged;
    bool rightClickEngaged;
};

