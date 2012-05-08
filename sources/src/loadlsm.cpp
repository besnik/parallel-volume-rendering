// loadlsm.cpp 
//
// trieda LSMFile - spajany zoznam image directories - kazdy adresar ukazuje na 2D pole pixel data.
// spajany zoznam ide odzadu(prvy adresar je posledny v stacku suboru)
//
// by skippo

#include "../include/loadlsm.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLASS CChannel
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


CChannel::CChannel(FILE *f,int tsirka, int tvyska, UINT32 offset, UINT32 BitsPerSample)					// konstruktor CChannel							
{
	pixeldata8 =NULL;
	pixeldata16=NULL;
	pixeldata32=NULL;
	
	this->dalsi=NULL;
	this->sirka=tsirka;
	this->vyska=tvyska;
	this->typ=(int)BitsPerSample;
	int i;
	switch(BitsPerSample)
	{
	case 8:
		pixeldata8 = new UINT8*[sirka];																	// vytvorenie 2D pola 8bit hodnot
		for (i=0;i<sirka;i++)
		{
			pixeldata8[i]=new UINT8[vyska];
		}
		break;  
	case 16:
		pixeldata16= new UINT16*[sirka];																// vytvorenie 2D pola 16bit hodnot
		for (i=0;i<sirka;i++)
		{
			pixeldata16[i]=new UINT16[vyska];
		}
		break;
	case 32:
		pixeldata32= new UINT32*[sirka];																// vytvorenie 2D pola 32bit hodnot
		for (i=0;i<sirka;i++)
		{
			pixeldata32[i]=new UINT32[vyska];
		}
		break;
	}

	fseek(f,offset,SEEK_SET);

	for (int j=0;j<vyska;j++)																			// nacitanie dat do prislusneho 2D pola
	{
		for (i=0;i<sirka;i++)
		{
			switch(BitsPerSample)
			{
			case 8:
				fread(&pixeldata8[i][j],sizeof(UINT8),1,f);
				break;
			case 16:
				fread(&pixeldata16[i][j],sizeof(UINT16),1,f);
				break;
			case 32:
				fread(&pixeldata32[i][j],sizeof(UINT32),1,f);
				break;
			}
		}
	}
}


CChannel::~CChannel()																					// destruktor
{
	int i;
	switch(this->typ)
	{
	case 8:
		for (i=0;i<sirka;i++)
		{
			if (pixeldata8[i]) { delete[] pixeldata8[i]; pixeldata8[i]=NULL; }
		}
		if (pixeldata8) { delete[] pixeldata8; pixeldata8=NULL; }
		break;

	case 16:
		for (i=0;i<sirka;i++)
		{
			if (pixeldata16[i]) { delete[] pixeldata16[i]; pixeldata16[i]=NULL; }
		}
		if (pixeldata16) { delete[] pixeldata16; pixeldata16=NULL; }
		break;

	case 32:
		for (i=0;i<sirka;i++)
		{
			if (pixeldata32[i]) { delete[] pixeldata32[i]; pixeldata32[i]=NULL; }
		}
		if (pixeldata32) { delete[] pixeldata32; pixeldata32=NULL; }
		break;
	}
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLASS CPlane
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


CPlane::CPlane(int tsirka, int tvyska, CChannel *kanal, UINT16 pockanalov)							// konstruktor CPlane							
{
	CChannel *akt;
	akt=kanal;

	this->sirka=tsirka;
	this->vyska=tvyska;
	int i;
	plocha = new CVoxel*[sirka];																	// vytvorime 2D pole voxelov
	for (i=0;i<sirka;i++)
	{
		plocha[i]=new CVoxel[vyska];
	}
	
	switch (pockanalov)																				// podla poctu kanalov sa rozhodneme co budeme robit																			
	{
	case 3:																							// ak su 3 channels
		if (akt->pixeldata8!=NULL)																	// mame 3x8bit data -> zlozime do RGB
		{
			for (int j=0;j<vyska;j++)
			{
				for (i=0;i<sirka;i++)
				{
					akt=kanal;
					plocha[i][j].blue=(akt->pixeldata8[i][j])/255.0f;									// blue
					akt=akt->dalsi;
					plocha[i][j].green=(akt->pixeldata8[i][j])/255.0f;									// green
					akt=akt->dalsi;
					plocha[i][j].red=(akt->pixeldata8[i][j])/255.0f;
				}
			}
		}  // if
		break;
	} // switch
}


CPlane::~CPlane()																					// destruktor CPlane
{
	for (int i=0;i<sirka;i++)																		// vymazeme 2D pole voxelov
	{
		if (plocha[i]) delete[] plocha[i];
		plocha[i]=NULL;
	}
	if (plocha) delete[] plocha;
	plocha=NULL;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLASS CZaznam
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


CZaznam::CZaznam()
{
	hodnota8=NULL;
	hodnota16=NULL;
	hodnota32=NULL;
	offset=NULL;
}


CZaznam::~CZaznam()														// destruktor CZaznam
{
	if (hodnota8) { delete[] hodnota8; hodnota8=NULL; }
	if (hodnota16) { delete[] hodnota16; hodnota16=NULL; }
	if (hodnota32) { delete[] hodnota32; hodnota32=NULL; }
}

void CZaznam::UpravZaznam(FILE *f, UINT16 tagg, UINT16 typp, UINT32 pocethodd, int *chyba)					// nacita hodnotu pre zaznam
{
	*chyba=0;
	this->tag=tagg;
	this->typ=typp;
	this->pocethod=pocethodd;
	int rozdiel;														// kolko bytov musim docitat do 4 byte
	long oldpoz;														// v pripade, ze v hodnote je ulozeny offset, tu sa uchova pozicia odkial sa bude pokracovat po nacitani hodnot z offsetu

	if (pocethod>0)														// musi byt v zazname ulozena aspon 1 hodnota, ak nie chyba
	{
		switch(typ)														// podla typu(velkosti) zaznamu naplnime polia hodnotaX(hodnotami alebo do hodnotaX=offset
		{
		case 1:
		case 2: //////////////////////////////////////////////////////////
			if (pocethod<=4)											// 8 bit - ZMESTI SA
			{
				hodnota8=new UINT8[pocethod];
				for (UINT32 i=0;i<pocethod;i++) fread(&hodnota8[i],sizeof(UINT8),1,f);
				rozdiel=4-pocethod;										// kolko bytov musim docitat do 4 byte
				if (rozdiel!=0) fseek(f,rozdiel,SEEK_CUR);				// posunieme sa na koniec 4 byte vyhradenych pre hodnotu
			} else														
			{															// 8 bit - NEZMESTI SA
				offset=new UINT32;									
				fread(offset,sizeof(UINT32),1,f);						// nacitaj
				oldpoz=ftell(f);										// zapamataj si staru poziciu
				fseek(f,offset[0],SEEK_SET);							// najdi poziciu hodnot
				
				hodnota8=new UINT8[pocethod];
				for (UINT32 i=0;i<pocethod;i++) fread(&hodnota8[i],sizeof(UINT8),1,f);		
				fseek(f,oldpoz,SEEK_SET);								// po nacitani sa vrati na povodnu poziciu
			}
			break;
		case 3: //////////////////////////////////////////////////////////
			if (pocethod<=2)											// 16 bit - ZMESTI SA
			{
				hodnota16=new UINT16[pocethod];
				for (UINT32 i=0;i<pocethod;i++) fread(&hodnota16[i],sizeof(UINT16),1,f);
				if (pocethod==1) fseek(f,2,SEEK_CUR);					// posunieme sa o 2B na koniec 4 byte vyhradenych pre hodnotu
			} else														
			{
				offset=new UINT32;									
				fread(offset,sizeof(UINT32),1,f);						// nacitaj
				oldpoz=ftell(f);										// zapamataj si staru poziciu
				fseek(f,offset[0],SEEK_SET);							// najdi poziciu hodnot
				
				hodnota16=new UINT16[pocethod];
				for (UINT32 i=0;i<pocethod;i++) fread(&hodnota16[i],sizeof(UINT16),1,f);		
				fseek(f,oldpoz,SEEK_SET);								// po nacitani sa vrati na povodnu poziciu
			}
			break;
		case 4: //////////////////////////////////////////////////////////
			if (pocethod==1)											// 32 bit - ZMESTI SA
			{
				hodnota32=new UINT32[pocethod];
				fread(&hodnota32[pocethod-1],sizeof(UINT32),1,f);
			} else
			{
				offset=new UINT32;									
				fread(offset,sizeof(UINT32),1,f);						// nacitaj
				oldpoz=ftell(f);										// zapamataj si staru poziciu
				fseek(f,offset[0],SEEK_SET);							// najdi poziciu hodnot
				
				hodnota32=new UINT32[pocethod];
				for (UINT32 i=0;i<pocethod;i++) fread(&hodnota32[i],sizeof(UINT32),1,f);		
				fseek(f,oldpoz,SEEK_SET);								// po nacitani sa vrati na povodnu poziciu
			}
			break;
		default:
			fseek(f,4,SEEK_CUR);
			break;
		}  //end of switch

	} else																// pocet hodnot v zazname < 1 (chyba)
	{
		*chyba=1;													
		return;
	}
	if (offset) { delete offset; offset=NULL; }
}


void CZaznam::Vypis()
{
	UINT32 j;
	switch(tag)
	{
	case 254:																	// 0 = images, 1 = thumbnails
		if (hodnota32[0]) printf("Nahlad obrazku\n");
		else			  printf("Obrazok\n");
		break;

	case 256:																	// sirka
		if (typ==3) printf("Sirka(16bit): %i\n",(int)hodnota16[0]);
		else		printf("Sirka: %i\n",(int)hodnota32[0]);
		break;

	case 257:																	// vyska
		if (typ==3) printf("Vyska(16bit): %i\n",(int)hodnota16[0]);
		else		printf("Vyska: %i\n",(int)hodnota32[0]);
		break;

	case 258:																	// hodnota = 8,16,32; pocet = pocet kanalov,
		for (j=0;j<pocethod;j++) printf("BitsPerSample: %i\tKanal: %i\n",(int)hodnota16[j], j);
		break;

	case 259:
		if (hodnota16[0]==1) printf("Nekomprimovany\n");
		else				 printf("Komprimovany\n");
		break;

	case 262:																	// hodnota = 0(WhiteIsZero) 1(BlackIsZero) 2(RGB24) 3(Palette)
		printf("PhotometricInterpretation\tHodnota: %i\n",(int)hodnota16[0]);
		break;

	case 273:																	// hodnota = tabulka s offsetmi na pixeldata, pocet = pocet kanalov
		if (typ==3)																// short 16bit
		{
			for (j=0;j<pocethod;j++) printf("StripOffsets(16bit): %i\tKanal: %i\n",(int)hodnota16[j],j);
		} else																	// long 32bit
		{
			for (j=0;j<pocethod;j++) printf("StripOffsets: %i\tKanal: %i\n",(int)hodnota32[j],j);
		}
		break;

	case 277:																	// hodnota = pocet kanalov
		printf("SamplesPerPixel\tPocet kanalov: %i\n",(int)hodnota16[0]);
		break;

	case 279:																	// hodnota = pre strip pocet bytes(sirka*vysla*n kde n=1(8bit),2(12bit kanal); pocet = pocet kanalov
		if (typ==3)
		{
			for (j=0;j<pocethod;j++) printf("StripByteCounts\tMnozstvo bytes(16bit): %i\tKanal: %i\n",(int)hodnota16[j],j);
		} else
		{
			for (j=0;j<pocethod;j++) printf("StripByteCounts\tMnozstvo bytes: %i\tKanal: %i\n",(int)hodnota32[j],j);
		}
		break;

	case 284:																	// hodnota = 2(kanaly sa uchovavaju osobitne)
		printf("PlanarConfiguration: %i\n",(int)hodnota16[0]);
		break;

	case 34412:																	// struktura s LSM specifickymi datami(iba v prvom image directory)
		printf("CZ_LSMInfo\tOffset: %i\n",(int)hodnota8[0]);
		break;

	default:
		printf("TAG: %i\tTYP: %i\tPOCET HODNOT: %i\n",(int)tag,(int)typ,(int)pocethod);
	} // end of switch
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLASS CImageDirectory
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




CImageDirectory::CImageDirectory(FILE *f, UINT32 offset)				// konstruktor CImageDirectory
{
	int chyba;															// chyba ktoru moze vratit fcia UpravZaznam pri nacitavani hodnoty
	UINT16 ttag;														// 4 pomocne premenne na nacitanie zaznamu
	UINT16 ttyp;							
	UINT32 tpocethod;						

	fseek(f,offset,SEEK_SET);
	fread(&this->pocetzaz,sizeof(UINT16),1,f);
	this->zaznam=new CZaznam[this->pocetzaz];

	for (int i=0;i<(this->pocetzaz);i++)								// nacitanie n zaznamov v image directory
	{
		fread(&ttag,sizeof(UINT16),1,f);
		fread(&ttyp,sizeof(UINT16),1,f);
		fread(&tpocethod,sizeof(UINT32),1,f);
		zaznam[i].UpravZaznam(f,ttag,ttyp,tpocethod,&chyba);
	}

	fread(&this->nextdir,sizeof(UINT32),1,f);							// offset na dalsi image directory, ak je 0 tak tento bol posledny
	this->dalsi=NULL;													

	this->rez=NULL;														// vynulovanie zatial nenacitanych poli
	this->chead=NULL;
	this->cakt=NULL;
	
	//this->LoadPixelData(f);												// nacitanie pixeldat do spajaneho zoznamu *head
	
	UINT16 pockan;														// vytvorenie obrazka z channelov
	UINT32 sir,vys;
	this->VratInfo(&pockan,&sir,&vys);									// zistenie poctu kanalov, sirky, vysky
//	this->rez=new CPlane(sir,vys,this->chead,pockan);					// vytvorenie obrazku - silene narocne na pamat!!!!
}




CImageDirectory::~CImageDirectory()										// destruktor pre CImageDirectory		
{
	if (rez) { delete rez; rez=NULL; }									// vymazanie obrazku, musi ist pred deletepixeldata, kvoli *chead
	if (chead) DeletePixelData();										// vymazanie channelov(pixeldata)
	if (zaznam) { delete[] zaznam; zaznam=NULL; }						// vymazanie zaznamov o obrazku					
}



void CImageDirectory::VypisZaznamy()									// do konzoly vypise zaznamy a ich hodnotu
{
	for (int i=0;i<this->pocetzaz;i++) zaznam[i].Vypis();
}




void CImageDirectory::VratInfo(UINT16 *pk, UINT32 *s, UINT32 *v)        // vrati pocet kanalov, sirku a vysku
{
	int pocetkan,vys,sir;												// pocet kanalov, vyska, sirka
	for (int i=0;i<pocetzaz;i++)
	{
		if (zaznam[i].tag==277) pocetkan=zaznam[i].hodnota16[0];		// nacitanie pocetkan

		else if (zaznam[i].tag==256)									// nacitanie sirky
		{
			if (zaznam[i].typ==3) sir=zaznam[i].hodnota16[0];
			else				  sir=zaznam[i].hodnota32[0];
		} else

		if (zaznam[i].tag==257)											// nacitanie vysky
		{
			if (zaznam[i].typ==3) vys=zaznam[i].hodnota16[0];
			else				  vys=zaznam[i].hodnota32[0];
		}	
	}
	*pk=pocetkan;
	*s=sir;
	*v=vys;
}




int CImageDirectory::LoadPixelData(FILE *f)								// konstruktor nenacita samotne pixel data, treba ich nacitat dodatocne touto fciou
{
	UINT16 pocetkan;													// pocet kanalov
	UINT32 sir,vys;														// vyska, sirka

	UINT16 *farby;														// farebna hlbka pre kazdy kanal
	UINT32 *offsety;													// offsety pre prislusny kanal
	UINT32 *velkosti;													// strip bytes count - kvoli kontrole

	this->VratInfo(&pocetkan,&sir,&vys);

  farby   = new UINT16[pocetkan];										// najprv zo zaznamov vytiahnem tieto udaje - bude ich treba do konstruktorov CChannel
	offsety = new UINT32[pocetkan];
	velkosti= new UINT32[pocetkan];
	int i;

	for (i=0;i<pocetzaz;i++)
	{
		if (zaznam[i].tag==279)											// strip bytes count
		{
			for (int j=0;j<(int)zaznam[i].pocethod;j++)
			{
				if (zaznam[i].typ==3) velkosti[j]=zaznam[i].hodnota16[j];
				else				  velkosti[j]=zaznam[i].hodnota32[j];
			}
		} else
		if (zaznam[i].tag==273)											// offsety stripov
		{
			for (int j=0;j<(int)zaznam[i].pocethod;j++)
			{
				if (zaznam[i].typ==3) offsety[j]=zaznam[i].hodnota16[j];
				else				  offsety[j]=zaznam[i].hodnota32[j];
			}
		} else
		if (zaznam[i].tag==258)											// bits per sample
			for (int j=0;j<(int)zaznam[i].pocethod;j++) 
				farby[j]=zaznam[i].hodnota16[j];
	}

	for (i=0;i<pocetkan;i++)										// vytvorenie spajaneho zoznamu
	{
		// tu by este mohol byt check na 'strip byte counts'
		cakt=new CChannel(f,sir,vys,offsety[i],farby[i]);
		
		cakt->dalsi=chead;
		chead=cakt;
	}

	if (farby) { delete[] farby; farby=NULL; }
	if (offsety) { delete[] offsety; offsety=NULL; }
	if (velkosti) { delete[] velkosti; velkosti=NULL; }

	return 1;
}


void CImageDirectory::DeletePixelData()
{
	while (chead)
	{
		cakt=chead;
		chead=chead->dalsi;
		delete cakt;
	}
	chead=NULL;
	cakt=NULL;
}





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLASS CLSMFile
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




CLSMFile::CLSMFile(char *subor, int* test)
{
	*test=0;																			// na zaciatku je vsetko ok=0
	UINT16 data16;																		// pomocne premenne
	UINT32 data32;
	sirka = vyska = 0; // 2D rozmery dat
	UINT16 pk; // temp, netreba nam to

	head=NULL; akt=NULL;																// vynulovanie pointrov na spajany zoznam
	pocetdir=0; 
	f=fopen(subor,"rb");																// otvorime subor na binarne citanie
  if (f!=NULL)
 	{
		fseek(f,0,SEEK_SET);															// nacitanie hlavicky - 8 byte, 2+2+4(byte order, identifikator, offset na image dir)

		fread(&data16,sizeof(UINT16),1,f);												// nacitame 0-1 byte
		if (data16!=18761) { *test=1; fclose(f); return; }								// error1: zla hlavicka	
		fread(&data16,sizeof(UINT16),1,f); // nacitame 2-3 byte
		if (data16!=42) { *test=1; fclose(f); return; }	// error1: zla hlavicka
		fread(&data32,sizeof(UINT32),1,f); // nacitame 4-7 byte - offset na image adresar

		while ((data32!=0) && (pocetdir<22)) // ak uz nie je dalsi image dir tak je offset rovny nule
		{
			akt=new CImageDirectory(f,data32); // vytvori sa adresar
			akt->dalsi=head; // nastavi mu Pointer next na prvy v zozname
			head=akt; // nastavi jeho ako prveho 
			data32=akt->nextdir; // ziska sa Offset na dalsi image dir
			if (pocetdir==0) head->VratInfo(&pk, &sirka, &vyska); // prvy obrazok udava 2D rozmery objemovych dat
			pocetdir++;
		}
//		fclose(f); // kvoli tomu,ze neskor budeme nacitavat data tak nemozeme zatvorit stream !!!
		
		// vypocet hlbky 3D dat z premennej <pocetdir>
		this->hlbka=0;
		for (int t=1; t<13; t++)
		{
			if ((this->pocetdir/2)==(int)powf(2.0f, (float)t))
			{
				this->hlbka=(this->pocetdir/2);
				break;
			}
			else 
			if ((int)powf(2.0f,(float)t)>(this->pocetdir/2))
			{
				this->hlbka=(int)powf(2.0f,(float)t);
				break;
			}
		}

	} else *test=1; // subor neexistuje = chyba c.1
}




CLSMFile::~CLSMFile()
{
	while (head)
	{
		akt=head;
		head=head->dalsi;
		delete akt;
	}
	head=NULL;
	akt=NULL;
	if (f!=NULL) fclose(f);
}




void CLSMFile::VypisAdresare(bool d)									// vypise zoznam adresarov. ak d==TRUE tak aj detaily adresarov
{
	printf("Celkovy pocet Image Directory: %i\n\n",pocetdir);
	int i=0;
	akt=head;
	while (akt)
	{
		printf("\nImage Directory c.%i\n",i);
		i++;
		if (d) akt->VypisZaznamy();
		akt=akt->dalsi;
	}
}




void CLSMFile::nacitajData()
{
	akt=head;
	while (akt)
	{
		akt->LoadPixelData(f);
		akt=akt->dalsi;
	}
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Other Code
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#ifdef _LSM_MAIN


int main(int argc, char* argv[]) 
{
	int i=0;

	printf("Loading .LSM file. Wait few sec...\n\n");
	CLSMFile* lsmfile=new CLSMFile("data.lsm",&i);
	if ((i==0) && (lsmfile!=NULL)) lsmfile->VypisAdresare(true);
	else switch(i)
	{
		default:printf("Chyba cislo %i\n",i);
			break;
		case 1:printf("Chyba: subor neexistuje.");
			break;
	}

	i=getchar();
	delete lsmfile;
	return 0;
}

#endif

