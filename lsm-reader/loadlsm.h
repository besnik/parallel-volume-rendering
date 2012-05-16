// loadlsm.h version 1.02
//
// trieda LSMFile - spajany zoznam image directories - kazdy adresar ukazuje na 2D pole pixel data.
// spajany zoznam ide odzadu(prvy adresar je posledny v stacku suboru)
//
// by skippo, marec 2005

#ifndef LOADLSM_H
#define LOADLSM_H

#include <iostream>
#include <tchar.h>

typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned char UINT8;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CZaznam
{
public:
	UINT16 tag;																// tag zaznamu
	UINT16 typ;																// typ - byte, ascii, short, long, rational
	UINT32 pocethod;														// pocet hodnot(vacsinou pocet kanalov)
	UINT8*   hodnota8;														// !! hodnot v jednom zazname moze byt viac
	UINT16* hodnota16;														// !! tiez mozu byt rozneho typu
	UINT32* hodnota32;														// !! jedna hodnota iba ak sa zmesti do 4BYTE, inak offset	
	UINT32* offset;															// ak sa hodnota nezmesti do 4Byte tak tu bude ulozeny offset
	void UpravZaznam(FILE *f, UINT16 tagg, UINT16 typp, UINT32 pocethodd, int *chyba);	// nacita hodnoty zaznamu do prislusneho pola hodnotaX
	void Vypis();															// vypise svoj typ a hodnotu(y)
	CZaznam();																// nastavi vsetkym pointerom hodnotu NULL
   ~CZaznam();																// destruktor 
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CChannel																// uchovava povodne nacitane hodnoty vsetkych kanalov pre jednotlive rezy
{
public:
    int sirka,vyska,typ;

	UINT8  **pixeldata8;													// pocet bit/per/pixel moze byt rozny: 8,16,32bits. 32 iba pre scanmode "Mean-of-ROIs"
	UINT16 **pixeldata16;
	UINT32 **pixeldata32;
	CChannel *dalsi;														// pointer na dalsi objekt spajaneho zoznau

	CChannel(FILE *f, int tsirka, int tvyska, UINT32 offset, UINT32 BitsPerSample);
   ~CChannel();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CVoxel																// voxel - najmensie objemove data
{
public:
	float red,green,blue;													// farba voxelu
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CPlane																// rez voxelov, ktore su zrekonstruovane z prislusnych channelov
{
protected:
	int sirka, vyska;														// rozmery plochy
	CVoxel **plocha;														// 2D pole dat
public:
	CPlane(int tsirka, int tvyska, CChannel *kanal, UINT16 pockanalov);		// konstruktor triedy CPlocha
   ~CPlane();																// destruktor
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CImageDirectory														// LSM(TIFF) Image Directory
{
protected:
	UINT16 pocetzaz;														// pocet zaznamov v image directory
	CZaznam *zaznam;														// zaznamy v image directory
	CPlane *rez;															// rez voxelov, ktore su zrekonstruovane z prislusnych channelov
public:
	CChannel *chead;														// kanaly zoscanovaneho rezu - head spajaneho zoznamu
	CChannel *cakt;															// pointer na aktivny objekt spajaneho zoznamu

	CImageDirectory *dalsi;													// pointer na dalsi adresar(spajany zoznam)
	UINT32 nextdir;															// offset! dalsieho image directory

	CImageDirectory(FILE *f, UINT32 offset);								// konstruktor CAdresarObrazku
   ~CImageDirectory();														// destruktor
	void VypisZaznamy();													// vypise zaznamy s hodnotami image dir
	void VratInfo(UINT16 *pk, UINT32 *s, UINT32 *v);						// vrati Pocet Kanalov, sirku a vysku
	void DeletePixelData();													// uvolni spajany zoznam s channelmi
	int LoadPixelData(FILE *f);												// konstruktor nenacita samotne pixel data, treba ich nacitat dodatocne touto fciou

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CLSMFile																// LSM subor
{
public:
	int pocetdir;															// pocet image directories
	CImageDirectory *head;													// pointer na zaciatok spajaneho zoznamu adresarov
	CImageDirectory *akt;													// pointer na aktivny objekt spajaneho zoznamu

	CLSMFile(char *subor,int* test);										// konstruktor LSM objektu
   ~CLSMFile();																// destruktor
    void VypisAdresare(bool d);												// vypise zoznam adresarov. ak d==TRUE tak aj detaily adresarov
};




#endif																		// LOADLSM_H