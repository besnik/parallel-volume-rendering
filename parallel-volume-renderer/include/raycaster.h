// raycaster.h
// by skip
// class CRaycaster - intialize and validates settings of the world+camera. Renders the scene.
// ver 1.06, jul 2006

#ifndef __RAYCASTER_H
#define __RAYCASTER_H


#include "kamera.h"
#include "data.h"
#include "funkcie.h"
#include "svetlo.h"
#include "shader.h"
#include "bezier.h"


#define MAX_DISTANCE 255 // maximalna preskocena vzdialenost luca pri distanceVolume


// TEXTURY ////////////////////////////////////////////////////


typedef struct // tuto strukturu(char*) naplnime datami, ktore potom priradime do textury
{
	unsigned long sirka;
	unsigned long vyska;
	UINT8 *data; // velkost: obrSirka*obrVyska*4 (pre kazdy pixel 3 byte RGBA) ak berieme intenzitu tak *1
} Image;


///////////////////////////////////////////////////////////////



class CRaycaster
{
public:
	UINT16 scr_x, scr_y;	// rozmery okna kam renderujeme
	CKamera kamera;	// pozorovatel v priestore
	CData data;
	
	// premenne pouzivane pri renderovani
	float cam_u, cam_v; // smer luca v camera space
	CVektor3 ray_start;	// pociatocna startovacia pozicia luca
	///CVektor3 d; // ake vzdialenosti pouzit na odoberanie vzoriek pozdlz luca?
	CVektor3 delta; // posun na dalsiu vzorku, vypocita sa z kamera.dir*d
	
	float samplingRate; // kolko vzoriek odoberame pozdlz luca, default 1
	
	UINT16 poc_vz, max_vz; // odobrate vzorky, maximalny pocet odobratych vzoriek
	UINT8 typ_dat; // 8,16,32 bitove?
	CRay r; // luc, ktory renderuje jeden pixel
	float tmin, tmax; // kde luc vstupil a vystupil z dat
	UINT16 mi, mj, mk; // index bloku v mriezke 
	CVektor3 offset; // offset vzorky v ramci bloku mriezky - pre trilinearnu interpolaciu
	CVektor3 vystup; // vystup z dat (ak luc trafil)
	UINT32 v[8]; // indexy do datovych poli pre interpolaciu a vypocet gradientu
	CBezKrivka kriv; // bezierova opacity krivka

	// svetlo
	CSvetlo svetlo;

	// color
	float intenzita; // naakumulovana intenzita
	float opacity; // naakumulovane pokrytie
	float pozadie; // intenzita pozadia
	float farba; // farba ziskana trilinearnou interpolaciou
	float acc_color; // naakumulovana farba

	// shader
	CShader shader; // phongovo tienovanie
	CVektor3 L,V,N; // light, view, normal vector
	CFarba c_out; // vystupna farba z phong shadera
	
	// opacity
	UINT16 oa, ob; // prahove hodnoty na ciselnej osi
	float foa, fob; // ich pokrytie, zvysne hodnoty medzi oA a oB sa interpoluju
	float acc_opacity; // naakumulovane pokrytie

	// opacity editor
	short edt_sir, edt_vys, edt_left, edt_top;
	float edt_lut[4096];

	// distance volume
	CVektor3 incrTable[MAX_DISTANCE];
	int incr[MAX_DISTANCE];

	// textura
	Image textura;

	// rotacia kamery
	float cam_dx,cam_dy; // o kolko sa kamera posunula - mapujeme 2D zmenu a 3D pohyb kamery
	float cam_old_dx,cam_old_dy;
	float cam_vz; // vzdialenost kamery
	float cam_old_vz;

	// konstruktory
	CRaycaster();
	CRaycaster(UINT16 scr_x, UINT16 scr_y);
	// destruktor
	~CRaycaster();
	
	// funkcie
	void initCamRot(int rozm_x, int rozm_y, int rozm_z); // nastavi parametre pre rotaciu kamery
	void setRatioUV(); // nastavi kamere uv ratio podla scr_x/scr_y
	void initTexture();
	void nastavKameru(const CVektor3 from, const CVektor3 to, const CVektor3 up, float zoom, float n, float f); // nastavi kameru - vlastne iba vola konstruktor s tymi parametrami
	void nastavZoom(float zoom); // nastavi zoom kamery
	void freeData(); // uvolni data z pamate
	void freeTexture(); // uvolni data pre texturu z pamate
	bool overNastavenia(); // overi ci su spravne nastavene parametre (kamery, atd.) Ak false tak exit(1) a musime uvolnit vsetky alokovane struktury!
	// renderovanie
	void renderuj(); // bez tienovania
	void renderujPhong(); // phong shading
	void renderujOld(); // plne starucicka prva verzia (pomala;)
	// blokove funkcie
	bool zistiBlok(const CVektor3& v, UINT16& x, UINT16& y, UINT16& z); // vrati v ktorom bloku mriezky sa vzorka pozdlz luca nachadza, data su ulozene v lavotocivej sur. sustave, ak je bod mimo dat tak vrati FALSE 
	void vratBlok(const CVektor3& v, UINT16& x, UINT16& y, UINT16& z); // vrati v ktorom bloku mriezky sa vzorka pozdlz luca nachadza, data su ulozene v lavotocivej sur. sustave 
	void vratOffsetBloku(const CVektor3& v, UINT16 mi, UINT16 mj, UINT16 mk, CVektor3 &offset);		// do premennej offset ulozi offset pozicie vzorky vzhladom na kocku
	// funkcie opacity
	float pokrytie(UINT16 i); // vypocet opacity - linearna interpolacia
	float pokrytieKriv(UINT16 i, short edt_sir, short edt_vys, short edt_left, short edt_top); // cez bezierovu krivku
	void initPokrytie(UINT16 oa, UINT16 ob, float foa, float fob);	
	// rotacia kamery
	void setCam(); // rotuje kameru o dx,dy okolo stredu 0,0,0
	// bezierova krivka
	void initBKriv(); // inicializuje bezierovu krivku
	void initOpacEdt(short edt_sir, short edt_vys, short edt_left, short edt_top); // nastavi rozmery a poziciu edt
	void vytvorEdtLut(); // nastavi hodnoty v opacity lookup table
	// distance volume RADC
	void initRadcTables(); // naplni lookup tables pre RADC (incr, incrTable)
};


#endif



/*

	History version:
	1.06 [jul 2006]
	- RGBA
	
	1.05 [11.4.2006]
	- some bugs fixed
	- faster ray-leave-volume check
	- distance volume accel structure was added

	1.04
	- fast implementation of "out of data check". fps+=0.3-0.4

	1.03
	- two serious bugs fixed -> access violation by camera rotation is no more
	- camera rotation around origin(0,0,0) and camera distance change via mouse was added
	- camera rotation in arbitrary direction(left,right,up,down) still supported via keys

	1.02
	-	5.1.2006
	- homog lookup data accel

	1.01
	- 4.1.2006
	- opengl and texture support
	- rendering to texture
	- major bugs fixed
	
	1.00
	- 2.1.2006
	- set and check the camera and scene
	- not optimized raycasting

*/

