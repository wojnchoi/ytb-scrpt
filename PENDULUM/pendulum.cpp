#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include<iostream>
#include<string>
#include<cmath>

using namespace std;

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 480

#define OFFSETX 150
#define OFFSETY 150

#define PI 3.14

int frameCount, timerFPS, lastFrame, fps;

struct Pendulum {
    float x;
    float y;
    float length = 250;
    float theta = PI/6;

    float angleV = 0;
    float angleA = 0;
};

void SwapValue(float& a, float&b) {
    float tmp = a;
    a = b;
    b = tmp;
}

void DrawFilledCircle(SDL_Renderer *renderer, int x0, int y0, int radius);
void DrawString(SDL_Renderer *renderer, float startx, float starty, float endx, float endy);
void Update(Pendulum &p);
int main(int argc, char *argv[]) {
    Pendulum p;
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout<<"SDL_Init error"<<endl;
        return 1;
    }
    if(TTF_Init() < 0) {
        cout<<"TTF_Init error"<<endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Pendulum", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,SCREEN_HEIGHT,0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface *surface;
    SDL_Texture *texture;
    TTF_Font *font;

    bool running = true;
    static int lastTime = 0;
    while(running) {
        font = TTF_OpenFont("arial,ttf", 25);
        surface = TTF_RenderText_Solid(font, "hello world", {0,0,0});
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        
        lastFrame=SDL_GetTicks();
        if(lastFrame>=(lastTime+1000)) {
        lastTime=lastFrame;
        fps=frameCount;
        frameCount=0;
        cout<< fps << endl; // PRINT FPS
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

        Update(p);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        DrawFilledCircle(renderer,p.x + OFFSETX, p.y + OFFSETY, 10);
        DrawString(renderer, p.x + OFFSETX, p.y + OFFSETY, OFFSETX, OFFSETY);
        SDL_RenderPresent(renderer);
        
        frameCount++;
        timerFPS = SDL_GetTicks()-lastFrame;
        if(timerFPS<(1000/60)) {
        SDL_Delay((1000/60)-timerFPS);
        }
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

float gravity = 1;
void Update(Pendulum &p) {
    float force = gravity * sin(p.theta);
    p.angleA = (-1 *force) / p.length;
   
    p.angleV += p.angleA;
    p.theta += p.angleV;

    // angleV *= 0.099; //제동력
    p.x = p.length * sin(p.theta);
    p.y = p.length * cos(p.theta);
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