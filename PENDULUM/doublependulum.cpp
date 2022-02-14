#include<SDL2/SDL.h>
#include<iostream>
#include<cmath>

using namespace std;

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 480

#define OFFSETX 350
#define OFFSETY 150

#define PI 3.14

int frameCount, timerFPS, lastFrame, fps;


void DrawFilledCircle(SDL_Renderer *renderer, int x0, int y0, int radius);
void DrawString(SDL_Renderer *renderer, float startx, float starty, float endx, float endy);
void SwapValue(float &a, float &b) {
    float tmp = a;
    a = b;
    b = tmp;
}
float r1 = 100;
float r2 = 100;

float m1 = 10;
float m2 = 10;

float theta1 = PI/2;
float theta2 = PI/2;

float v1 = 0;
float v2 = 0;

float g = 1;
float x2, y2;

void DoublePen(SDL_Renderer *renderer) {
   float a1 = 
   ((-g * (2 *m1 + m2) * sin(theta1)) + (-m2 * g * sin(theta1-2*theta2)) + (-2*sin(theta1-theta2)*m2) *(pow(v2,2)*r2+pow(v1,2)*r1*cos(theta1-theta2))) 
                                    / (r1 * (2*m1+m2-m2*cos(2*theta1-2*theta2))); 
    
    float a2 = 
    ((2 * sin(theta1-theta2))*(((pow(v1,2)*r1*(m1+m2))+(g * (m1 + m2) * cos(theta1))+(pow(v2,2)*r2*m2*cos(theta1-theta2)))) 
                                    / (r2 * (2*m1+m2-m2*cos(2*theta1-2*theta2))));
    
    
    float x1 = r1 *sin(theta1) + OFFSETX;
    float y1 = r1 *cos(theta1) + OFFSETY;

    x2 = x1 + r2 * sin(theta2);
    y2 = y1 + r2 * cos(theta2);

    
     DrawString(renderer, OFFSETX,OFFSETY, x1, y1 );
     DrawFilledCircle(renderer,x1,y1 ,m1);
     DrawString(renderer, x1 ,y1 , x2, y2);
     DrawFilledCircle(renderer,x2,y2,m2);
   
    v1 += a1;
    v2 += a2;
    theta1 += v1;
    theta2 += v2;


}

int main(int argc, char *argv[]) {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout<<"SDL_Init error"<<endl;
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("Pendulum", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,SCREEN_HEIGHT,0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture * my_layer = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetTextureBlendMode(my_layer, SDL_BLENDMODE_BLEND);

    bool running = true;
    static int lastTime = 0;
    while(running) {
        lastFrame=SDL_GetTicks();
        if(lastFrame>=(lastTime+1000)) {
        lastTime=lastFrame;
        fps=frameCount;
        frameCount=0;
        //cout<< fps << endl; // PRINT FPS
        }

        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            
            default:
                break;
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        DoublePen(renderer);
        
        SDL_SetRenderTarget(renderer,my_layer);
        SDL_RenderDrawPoint(renderer,x2,y2);
       
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, my_layer, NULL, NULL);
        SDL_RenderPresent(renderer);
        
        frameCount++;
        timerFPS = SDL_GetTicks()-lastFrame;
        if(timerFPS<(1000/60)) {
        SDL_Delay((1000/60)-timerFPS);
        }
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void DrawString(SDL_Renderer *renderer, float startx, float starty, float endx, float endy) {
    bool step = abs(endx-startx) < abs(endy-starty);
    if(step) {
        SwapValue(startx,starty);
        SwapValue(endx,endy);
    }
    if(endx < startx) {
        SwapValue(startx,endx);
        SwapValue(starty,endy);
    }
    float error = 0.0;
    float roundError = (float)abs(endy-starty)/(endx-startx);

    int y = starty;
    int ystep = (endy>starty ? 1 : -1);
    for(int i = startx; i < endx; i++) {
        if(step) {
            SDL_RenderDrawPoint(renderer, y, i);
        }
        else {
            SDL_RenderDrawPoint(renderer, i, y);
            }
            error += roundError;
            if(error>=0.5) {
                y+= ystep;
                error -= 1.0;
                }
        }
}

void DrawFilledCircle(SDL_Renderer *renderer, int x0, int y0, int radius)
{
    int x = radius;
    int y = 0;
    int xChange = 1 - (radius * 2);
    int yChange = 0;
    int radiusError = 0;

    while (x >= y)
    {
        for (int i = x0 - x; i <= x0 + x; i++)
        {
            SDL_RenderDrawPoint(renderer, i, y0 + y);
            SDL_RenderDrawPoint(renderer, i, y0 - y);
        }
        for (int i = x0 - y; i <= x0 + y; i++)
        {
            SDL_RenderDrawPoint(renderer, i, y0 + x);
            SDL_RenderDrawPoint(renderer, i, y0 - x);
        }

        y++;
        radiusError += yChange;
        yChange += 2;
        if (((radiusError << 1) + xChange) > 0)
        {
            x--;
            radiusError += xChange;
            xChange += 2;
        }
    }
}