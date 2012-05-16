// par uzitocnych matematickych cisielok
// ver 1.1
// by skip

#undef PI
#undef INV_PI
#undef PI_TIMES2
#undef PI_OVER2
#undef DEG_RAD
#undef DEG_RAD_PRESNE
#undef RAD_DEG
#undef SQRT2
#undef INV_SQRT2
#undef SQRT3
#undef INV_SQRT3
#undef LOG2
#undef INV_LOG2
#undef LN2
#undef INV_LN2
#undef INFINITY
#undef EPSILON
#undef EULER_NUMBER
#undef NULL


#define NULL 0
#define PI 3.141592653589793238462643383279502884197169399375105820975
#define INV_PI ( 1.0/PI )
#define PI_TIMES2 ( 2.0*PI )
#define PI_OVER2 ( 0.5*PI )
#define DEG_RAD ( PI/180.0 )
#define DEG_RAD_PRESNE 0.01745329251
#define RAD_DEG ( 180.0/PI )
#define SQRT2 pow( 2.0, 0.5 )
#define INV_SQRT2 ( 1.0/SQRT2 )
#define SQRT3 pow( 3.0, 0.5 )
#define INV_SQRT3 ( 1.0/SQRT3 )
#define LOG2 log10( 2.0 )
#define INV_LOG2 ( 1.0/LOG2 )
#define LN2 log( 2.0 )
#define INV_LN2 ( 1.0/LN2 )
#define INFINITY 10000000.0
#define EPSILON 0.0001
#define EULER_NUMBER 2.7182818284590452353602874713527

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define ABSF(x) ( ((x)<0.0) ? -(x) : (x) ) 
#define MIN(a,b) ((a<b)?(a):(b))
#define MAX(a,b) ((a>b)?(a):(b))
#define MIN3(a,b,c) ((((a)<(b))&&((a)<(c))) ? (a) : (((b)<(c)) ? (b) : (c)))
#define MAX3(a,b,c) ((((a)>(b))&&((a)>(c))) ? (a) : (((b)>(c)) ? (b) : (c))) 
#define TIME(x) { struct timeval start, end;\
                gettimeofday(&start, NULL); \
                x;                          \
 	        gettimeofday(&end, NULL);   \
	        cerr << "Time: "  << delay(start, end) << "ms" << endl; }

		
typedef unsigned int   UINT32;
typedef unsigned short UINT16;
typedef unsigned char  UINT8;



