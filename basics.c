#include "basics.h"

double sq( double a ){
	return a * a;
}

int random( int min, int max ){
    return (rand() % (max-min)) + min;
}

double lerp(double start, double stop, double amt) {
    return start + (stop-start) * amt;
}

double map(double value, double start1, double stop1,  double start2, double stop2) {
	return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}

int constrain( int a, int min, int max ){
	if( a < min ) return min;
	else if( a > max ) return max;
	else return a;
}
float constrainf( float a, float min, float max ){
	if( a < min ) return min;
	else if( a > max ) return max;
	else return a;
}
double constrainD( double a, double min, double max ){
	if( a < min ) return min;
	else if( a > max ) return max;
	else return a;
}

double degrees( double radians ){
	return radians * (double)57.29577951308232087679815481410517033240547246656432154916;//ONE_OVER_PI * 180;
}
double radians( double degrees ){
    return degrees * 0.017453292519943295769236907684886127134428718885417254560;// PI over 180
}

double rectify_angle( double a ){
	if( a < 0 ){
		//printf("++ %f, %f, %f, %f.\n", a, abs(a), abs(a)/TWO_PI, ceil( abs(a) / TWO_PI ) );
		if( a >= -TWO_PI ) return TWO_PI + a;
		else return (ceil( abs(a) / TWO_PI ) * TWO_PI) + a;
	}
	else{
		if( a < TWO_PI ) return a;
		else{
			return a - (floor( a / TWO_PI ) * TWO_PI);
		}
	}
}


void strspl( char *string, const char *delimiters, char ***list, int *size ){
	int ss = strlen( string );
	*list = (char**) malloc( ceil(0.5*ss) * sizeof(char*) );
	int sd = strlen( delimiters );
	*size = 0;

	bool *checks = (bool*) malloc( ss * sizeof(bool) );
	for( int i = 0; i < ss; ++i ){
		checks[i] = 0;
		for (int j = 0; j < sd; ++j){
			if( string[i] == delimiters[j] ){
				checks[i] = 1;
				break;
			}
		}
	}

	bool looking_for_first = 1;
	for( int i = 0; i < ss; ++i ){
		if( looking_for_first ){
			if( checks[i] ){
				string[i] = '\0';
			}
			else{
				(*list)[0] = string+i;
				(*size)++;
				looking_for_first = 0;
			}
		}
		else{
			if( checks[i] ){
				string[i] = '\0';
				if( i < ss-1 ){
					if( !checks[i+1] ){
						(*list)[*size] = string+i+1;
						(*size)++;
					}
				}
			}
		}
	}
	*list = (char**) realloc( *list, (*size) * sizeof(char*) );
	//apparently for strtok the string MUST be declared as "char string[]" in the calling function
	// it can't be a literal and it can't be "char *string"...
	/* TEST:
	char **list;
	int size = 0;
	char string[] = "split me baby one more time";
	split_string( string, " ", &list, &size );
	for (int i = 0; i < size; ++i){
	    printf("%s\n", list[i] );
	}
	*/
	// char * p = strtok (string, delimiters);
	// int i = 0;
	// while (p != NULL){
	// 	(*list)[i] = p;
	// 	p = strtok (NULL, delimiters);
	// 	++i;
	// }
}

int strcchr( char *string, char C ){ // String Count character
	int i = 0;
	int count = 0;
	while( string[i] != '\0' ){
		if( string[i] == C ) ++count;
	}
	return count;
}

// sub-string
char * substr( char *string, int start, int stop ){
	char *sub = (char*) calloc( stop-start +1, sizeof(char) );
	for (int i = start; i < stop; ++i){
		sub[i-start] = string[i];
	}
	sub[ stop-start ] = '\0';
	return sub;
}

bool fseek_lines( FILE* f, int N ){
	char c = getc( f );
	while( c != EOF ){
		if( c == '\n' ){
			N -= 1;
			if( N == 0 ) return 1;
		}
		c = getc( f );
	}
	return 0;
}

bool fseek_string( FILE *f, char *str ){
	char c = getc( f );
	int i = 0;
	while( c != EOF ){
		if( c == str[i] ){
			i++;
			if( str[i] == '\0' ) return 1;
		}
		else{
			i = 0;
		}
		c = getc( f );
	}
	return 0;
}

bool cursor_in_rect( SDL_MouseButtonEvent button, SDL_Rect R ){
	return ( button.x > R.x && button.x < R.x + R.w ) && ( button.y > R.y && button.y < R.y + R.h );
}

bool coordinates_in_Rect( float x, float y, SDL_Rect *R ){
	return ( x > R->x && x < R->x + R->w ) && ( y > R->y && y < R->y + R->h );
}
bool coordinates_in_FRect( float x, float y, SDL_FRect *R ){
	return ( x > R->x && x < R->x + R->w ) && ( y > R->y && y < R->y + R->h );
}
bool rect_overlap( SDL_FRect *A, SDL_FRect *B ){
	return ( ( A->x + A->w > B->x ) && ( B->x + B->w > A->x ) ) && ( ( A->y + A->h > B->y ) && ( B->y + B->h > A->y ) );
}


bool alphanumeric( char c ){
  if( lower_case( c ) || upper_case( c ) || numeral( c ) || c == '-' ) return 1;// (int)c == 32 || // 32 is ' '
  else return 0;
}
bool lower_case( char c ){
  int n = (int)c;
  if( n >= 97 && n <= 122) return 1;
  else return 0;
}
bool upper_case( char c ){
  int n = (int)c;
  if( n >= 65 && n <= 90) return 1;
  else return 0;
}
bool numeral( char c ){
  int n = (int)c;
  if( (n >= 48 && n <= 57) || c == '.' ) return 1; // 46 is '.'
  else return 0;
}