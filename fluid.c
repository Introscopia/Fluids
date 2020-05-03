/*
BASED ON: 
Fluid Simulation for Dummies by Mike Ash:
    https://mikeash.com/pyblog/fluid-simulation-for-dummies.html

Real-Time Fluid Dynamics for Games by Jos Stam:
    https://pdfs.semanticscholar.org/847f/819a4ea14bd789aca8bc88e85e906cfc657c.pdf
    https://github.com/CFusion-zz/Real-Time-Fluid-Dynamics-for-Games/blob/master/code
*/

#include "fluid.h"



FluidBox *FluidBoxCreate(int size, float diffusion, float viscosity, float dt){

    FluidBox *box = (FluidBox*) malloc( sizeof(FluidBox) );
    
    box->size = size;
    box->dt = dt;
    box->diff = diffusion;
    box->visc = viscosity;

    int N = (size+2)*(size+2);

    box->s = (float*) calloc( N, sizeof(float) );

    box->density = (float*) calloc( N, sizeof(float) );

    box->Vx = (float*) calloc( N, sizeof(float) );
    box->Vy = (float*) calloc( N, sizeof(float) );
    
    box->Vx0 = (float*) calloc( N, sizeof(float) );
    box->Vy0 = (float*) calloc( N, sizeof(float) );
    
    return box;
}

void FluidBoxFree(FluidBox *box){

    free(box->s);
    free(box->density);
    
    free(box->Vx);
    free(box->Vy);
    //free(box->Vz);
    
    free(box->Vx0);
    free(box->Vy0);
    //free(box->Vz0);
    
    free(box);
}


static void set_bnd(int N, int b, float *x){

    for ( int i = 1 ; i <= N ; i++ ) {
        x[IX(0  ,i)] = b==1 ? -x[IX(1,i)] : x[IX(1,i)];
        x[IX(N+1,i)] = b==1 ? -x[IX(N,i)] : x[IX(N,i)];
        x[IX(i,0  )] = b==2 ? -x[IX(i,1)] : x[IX(i,1)];
        x[IX(i,N+1)] = b==2 ? -x[IX(i,N)] : x[IX(i,N)];
    }
    x[IX(0  ,0  )] = 0.5f*(x[IX(1,0  )]+x[IX(0  ,1)]);
    x[IX(0  ,N+1)] = 0.5f*(x[IX(1,N+1)]+x[IX(0  ,N)]);
    x[IX(N+1,0  )] = 0.5f*(x[IX(N,0  )]+x[IX(N+1,1)]);
    x[IX(N+1,N+1)] = 0.5f*(x[IX(N,N+1)]+x[IX(N+1,N)]);
}

static void lin_solve ( int N, int b, float * x, float * x0, float a, float c ) {

    for ( int k = 0; k < SOLVE_ITERATIONS; k++ ) {
        FOR_EACH_CELL
            x[IX(i,j)] = (x0[IX(i,j)] + a*(x[IX(i-1,j)]+x[IX(i+1,j)]+x[IX(i,j-1)]+x[IX(i,j+1)]))/c;
        END_FOR

        set_bnd ( N, b, x );
    }
}

static void diffuse ( int N, int b, float * x, float * x0, float diff, float dt ) {
    float a = dt * diff * N * N;
    lin_solve ( N, b, x, x0, a, 1+4*a );
}

static void advect ( int N, int b, float * d, float * d0, float * u, float * v, float dt ) {
    int i0, j0, i1, j1;
    float x, y, s0, t0, s1, t1, dt0;

    dt0 = dt * N;
    FOR_EACH_CELL
        x = i - dt0 * u[IX(i,j)]; 
        y = j - dt0 * v[IX(i,j)];
        if (x<0.5f) x=0.5f; if (x>N+0.5f) x=N+0.5f; i0=(int)x; i1=i0+1;
        if (y<0.5f) y=0.5f; if (y>N+0.5f) y=N+0.5f; j0=(int)y; j1=j0+1;
        s1 = x-i0; s0 = 1-s1; t1 = y-j0; t0 = 1-t1;
        d[IX(i,j)] = s0*(t0*d0[IX(i0,j0)]+t1*d0[IX(i0,j1)])+
                     s1*(t0*d0[IX(i1,j0)]+t1*d0[IX(i1,j1)]);
    END_FOR

    set_bnd ( N, b, d );
}

static void project ( int N, float * u, float * v, float * p, float * div ) {

    FOR_EACH_CELL
        div[IX(i,j)] = -0.5f*(u[IX(i+1,j)]-u[IX(i-1,j)]+v[IX(i,j+1)]-v[IX(i,j-1)])/N;
        p[IX(i,j)] = 0;
    END_FOR 

    set_bnd ( N, 0, div ); 
    set_bnd ( N, 0, p );

    lin_solve ( N, 0, p, div, 1, 4 );

    FOR_EACH_CELL
        u[IX(i,j)] -= 0.5f*N*(p[IX(i+1,j)]-p[IX(i-1,j)]);
        v[IX(i,j)] -= 0.5f*N*(p[IX(i,j+1)]-p[IX(i,j-1)]);
    END_FOR

    set_bnd ( N, 1, u ); set_bnd ( N, 2, v );
}

void FluidBoxStep(FluidBox *box){

    int N          = box->size;
    float visc     = box->visc;
    float diff     = box->diff;
    float dt       = box->dt;
    float *Vx      = box->Vx;
    float *Vy      = box->Vy;
    float *Vx0     = box->Vx0;
    float *Vy0     = box->Vy0;
    float *s       = box->s;
    float *density = box->density;
    
    diffuse( N, 1, Vx0, Vx, visc, dt);
    diffuse( N, 2, Vy0, Vy, visc, dt);
    
    project( N, Vx0, Vy0, Vx, Vy);
    
    advect( N, 1, Vx, Vx0, Vx0, Vy0, dt);
    advect( N, 2, Vy, Vy0, Vx0, Vy0, dt);
    
    project( N, Vx, Vy, Vx0, Vy0);
    
    diffuse( N, 0, s, density, diff, dt);
    advect( N, 0, density, s, Vx, Vy, dt);
}

void FluidBoxAddDensity(FluidBox *box, int x, int y, float amount) {

    int N = box->size;
    box->density[IX(x, y)] += amount;
}

void FluidBoxAddVelocity(FluidBox *box, int x, int y,  float amountX, float amountY ) {
    int N = box->size;
    int index = IX(x, y);
    
    box->Vx[index] += amountX;
    box->Vy[index] += amountY;
}

