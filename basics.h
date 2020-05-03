#ifndef BASICS_H_INCLUDED
#define BASICS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

#include <SDL.h>


#define PHI         (double) 1.618033988749894848204586834365638117720309179805762862135
#define TWO_PI      (double) 6.283185307179586476925286766559005768394338798750211641949
#define PI          (double) 3.141592653589793238462643383279502884197169399375105820974
#define HALF_PI     (double) 1.570796326794896619231321691639751442098584699687552910487
#define THIRD_PI    (double) 1.047197551196597746154214461093167628065723133125035273658
#define QUARTER_PI  (double) 0.785398163397448309615660845819875721049292349843776455243
#define FIFTH_PI    (double) 0.628318530717958647692528676655900576839433879875021164194
#define SIXTH_PI    (double) 0.523598775598298873077107230546583814032861566562517636829
#define EIGTH_PI    (double) 0.392699081698724154807830422909937860524646174921888227621
#define TWELFTH_PI  (double) 0.261799387799149436538553615273291907016430783281258818414
#define ONE_OVER_PI (double) 0.318309886183790671537767526745028724068919291480912897495

//const SDL_Color black = {0, 0, 0, 255};
//const SDL_Color white = {255, 255, 255, 255};

Uint32 rmask, gmask, bmask, amask;

typedef char bool;

double sq( double a );

/* RANDOM: min inclusive, max not inclusive
	int co  [7];
    for (int i = 0; i < 7; ++i) co[i] = 0;
    for (int i = 0; i < 1000; ++i){
       int x = random(1, 8);
       co[x-1]++;
    }
    for (int i = 0; i < 7; ++i) printf("%d: %d, ", i+1, co[i] );
    puts(".");
*/
int random( int min, int max );

double lerp(double start, double stop, double amt);

double map(double value, double start1, double stop1,  double start2, double stop2);

   int constrain (    int a,    int min,    int max );
 float constrainf(  float a,  float min,  float max );
double constrainD( double a, double min, double max );


double degrees( double radians );
double radians( double degrees );

double rectify_angle( double a );

// String split
void strspl( char *string, const char *delimiters, char ***list, int *size );
// String Count character
int strcchr( char *string, char C );
// sub-string
char * substr( char *string, int start, int stop );

bool fseek_lines( FILE* f, int N );
bool fseek_string( FILE *f, char *str );

bool cursor_in_rect( SDL_MouseButtonEvent button, SDL_Rect R );
bool coordinates_in_Rect( float x, float y, SDL_Rect *R );
bool coordinates_in_FRect( float x, float y, SDL_FRect *R );
bool rect_overlap( SDL_FRect *A, SDL_FRect *B );

bool alphanumeric( char c );
bool lower_case( char c );
bool upper_case( char c );
bool numeral( char c );

typedef struct {
    int i, j;
} index2d;

#endif