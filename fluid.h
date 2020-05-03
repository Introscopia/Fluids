#include <stdlib.h>
#include <math.h>


#define IX(i,j) ((i)+(N+2)*(j))
#define SWAP(x0,x) {float * tmp = x0; x0 = x; x = tmp;}
#define FOR_EACH_CELL for ( int i = 1; i<= N; i++ ) { for ( int j = 1; j<= N; j++ ) {
#define END_FOR }}

#define SOLVE_ITERATIONS 4


typedef struct FluidBox_struct {
    int size;
    float dt;
    float diff;
    float visc;
    
    float *s;
    float *density;
    
    float *Vx;
    float *Vy;

    float *Vx0;
    float *Vy0;
} FluidBox;



FluidBox *FluidBoxCreate(int size, float diffusion, float viscosity, float dt);

void FluidBoxFree(FluidBox *box);

void FluidBoxStep(FluidBox *box);

void FluidBoxAddDensity(FluidBox *box, int x, int y, float amount);

void FluidBoxAddVelocity(FluidBox *box, int x, int y, float amountX, float amountY);