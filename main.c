#include "basics.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "SDL2_gfx/SDL2_gfxPrimitives.h"
#include "SDL2_gfx/SDL2_framerate.h"

#ifdef WIN32
#define _WIN32_WINNT 0x0500
#include <windows.h>
#endif

#include "fluid.h"


Uint32 time_passed = 0;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~O~~~~~~~~~~| M A I N |~~~~~~~~~~~O~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main(int argc, char *argv[]){

    //HWND hwnd_win = GetConsoleWindow();
    //ShowWindow(hwnd_win,SW_HIDE);
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    int width = 600;
    int height = 600;
    int cx, cy;
    bool loop = 1;
    int zoomI = 0;
    double zoom = 1;
    double tx = 0, ty = 0;
    int mouseX, mouseY, pmouseX, pmouseY;

    Uint32 then, now, frames;
    FPSmanager fpsm;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }
    if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }
    //SDL_MaximizeWindow( window );
    //SDL_GetWindowSize( window, &width, &height );
    cx = width / 2;
    cy = height / 2;


    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    #else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    #endif


    int N = 200;
    //                  int size, float diffusion, float viscosity, float dt
    FluidBox *FB = FluidBoxCreate( N, 0.00001, 0.000015, 0.4 );
    N += 2;
    int length = N * N;
    SDL_Surface *surface = SDL_CreateRGBSurface( 0, N, N, 32, rmask, gmask, bmask, amask );
    float scale = 3;
    float inv_scale = 1 / scale;
    SDL_Rect dstrct = { (width - scale*N)/2, (height - scale*N)/2, scale*N, scale*N };
    
    SDL_initFramerate(&fpsm);
    SDL_setFramerate(&fpsm, 60);
    int frc = 0;
    puts("<<Entering Loop>>");
    while ( loop ) { //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> L O O P <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

        while( SDL_PollEvent(&event) ){

            switch (event.type) {
                case SDL_QUIT:
                    loop = 0;
                    break;
                case SDL_KEYDOWN:
                    //if( event.key.keysym.sym == 'w' || event.key.keysym.sym == SDLK_UP )    
                    break;
                case SDL_KEYUP:
                         
                    break;
                case SDL_MOUSEMOTION:
                	pmouseX = mouseX;
        			pmouseY = mouseY;
                    mouseX = event.motion.x;
                    mouseY = event.motion.y;
                    if( event.motion.state & SDL_BUTTON_LMASK ){
                    	int X = inv_scale*(pmouseX - dstrct.x);
                    	int Y = inv_scale*(pmouseY - dstrct.y);
                    	float K = 1;
                        FluidBoxAddVelocity( FB, X, Y, K*(mouseX - pmouseX), K*(mouseY - pmouseY) );
                    }
                    else if( event.motion.state & SDL_BUTTON_RMASK ){
                    	int X = inv_scale*(pmouseX - dstrct.x);
                    	int Y = inv_scale*(pmouseY - dstrct.y);
                    	FluidBoxAddDensity( FB, X, Y, 200 );
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    break;
                case SDL_MOUSEBUTTONUP:
                    break;
                case SDL_MOUSEWHEEL:;
                    
                    break;
            }
        }

        SDL_RenderClear( renderer );
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255);
        SDL_RenderFillRect( renderer, NULL );


        FluidBoxAddDensity( FB, 100, 146, 600 );
        FluidBoxAddVelocity( FB, 100, 150, 0, -10 );
        FluidBoxStep( FB );

        int *pix = (int*) surface->pixels;
        for (int i = 0; i < length; ++i){
        	int v = (Uint8)constrainf( 64 * FB->density[ i ], 0, 255);
			pix[ i ] = SDL_MapRGBA( surface->format, v, v, v, 255);
			FB->density[ i ] *= 0.98;
			//if( i % 111 == 0 ) printf("%d: %f, %d.\n", i, FB->density[ i ], v );
        }
        SDL_Texture* T =  SDL_CreateTextureFromSurface( renderer, surface );
        SDL_RenderCopy( renderer, T, NULL, &dstrct );


        SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect( renderer, &dstrct );


        SDL_RenderPresent(renderer);

        SDL_DestroyTexture( T );

        time_passed = SDL_framerateDelay(&fpsm);
        /*
        if( frc == 80 ){
        	printf("%f\n", 1000.0/time_passed );
        	frc = 0;
        }
        frc++;
        */

    }//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> / L O O P <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    puts("\n<<exiting>>");

    FluidBoxFree(FB);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

