// data.cpp
// by skip

#include "../include/data.h"


// konstruktory

CData::CData()
{
	x = y = z = 0; // rozmery dat
	dx = dy = dz = 1; // vzdialenosti medzi voxelmi
	posunx = posuny = posunz = 0.0f; // posun dat v priestore z default poz (0,0,0)
	data = NULL;
	homog = NULL;
	nastavObal();
	// sakas adr scheme
	Gaddr = Haddr = Iaddr = NULL;
	data2 = NULL;
	// lsm file
	imdirAkt = NULL;
	lsmfile = NULL;
	// distance volume
	bunky = NULL;
}




CData::CData(int x, int y, int z)
{
	this->x = x; this->y = y; this->z = z;
	dx = dy = dz = 1;
	posunx = posuny = posunz = 0.0f;
	data = NULL; 
	homog = NULL;
	nastavObal();
	// sakas adr scheme
	Gaddr = Haddr = Iaddr = NULL;
	data2 = NULL;
	// lsm file
	imdirAkt = NULL;
	lsmfile = NULL;
	// distance volume
	bunky = NULL;
}




CData::CData(const CVektor3& v)
{
	x=(UINT16)v.x; y=(UINT16)v.y; z=(UINT16)v.z;
	dx = dy = dz = 1;
	posunx = posuny = posunz = 0.0f;
	data = NULL; 
	homog = NULL;
	nastavObal();
	// sakas adr scheme
	Gaddr = Haddr = Iaddr = NULL;
	data2 = NULL;
	// lsm file
	imdirAkt = NULL;
	lsmfile = NULL;
	// distance volume
	bunky = NULL;
}	

	

	
// destruktor

CData::~CData()
{
	if (data) delete[] data;	data=NULL;
	if (homog) delete[] homog;	homog=NULL;
	if (Gaddr) delete[] Gaddr; Gaddr=NULL;
	if (Haddr) delete[] Haddr; Haddr=NULL;
	if (Iaddr) delete[] Iaddr; Iaddr=NULL;
	if (data2) delete[] data2; data2=NULL;
	if (bunky) delete[] bunky; bunky=NULL;
	if (lsmfile) delete lsmfile; lsmfile=NULL;
}




void CData::init()
{
	if (data) delete[] data;
	x=256; y=256; z=256;
	data = new UINT16[x*y*z];

	int i,j,k;
	// reset dat
	for (k=0; k<z; k++)
		for (j=0; j<y; j++)
			for (i=0; i<x; i++)
			{
				data[getIndex(i,j,k,*this)]=2000;
			}


	// mala kocka vo vnutri
	for (k=0; k<(int)diV((float)z,2.0f,0.0f); k++)
		for (j=0; j<(int)diV((float)y,2.0f,0.0f); j++)
			for (i=0; i<(int)diV((float)x,2.0f,0.0f); i++)
			{
				data[getIndex(15+i,15+j,15+k,*this)]=4000;
			}
/*

	for (k=0; k<z-2; k++)
		for (j=0; j<y-2; j++)
			for (i=0; i<x-2; i++)
			{
				data[getIndex(1+i,1+j,1+k,*this)]=4000;
			}
*/

	// posun dat v priestore
	posunx=x/-2.0f;
	posuny=y/-2.0f;
	posunz=z/-2.0f;
	// nastavenie rozmerov medzi voxelmi mriezky
	dx = 1;
	dy = 1;
	dz = 1;
	// nastavime obal
	nastavObal();
}



void CData::init2()
{
	if (data) delete[] data;
	x=1024; y=1024; z=2;
	data = new UINT16[x*y*z];

	int i,j,k;
	// reset dat
	for (k=0; k<z; k++)
		for (j=0; j<y; j++)
			for (i=0; i<x; i++)
			{
				data[getIndex(i,j,k,*this)]=4000;
			}

	for (k=0; k<z-1; k++)
		for (j=0; j<y; j++)
			for (i=0; i<x; i++)
			{
				data[getIndex(i,j,k,*this)]=2000;
			}



	posunx=-512.0f;
	posuny=-512.0f;
	posunz=0.0f;

	// nastavime obal
	nastavObal();
}




void CData::initLSM()
{
	int prahovanie=500;

	if (data) delete[] data; data=NULL;
	cout << "Nahravam lsm subor. Toto moze trvat niekolko sekund..." << endl;
	cout.flush();
	
	int i=0; // error handling
	lsmfile=new CLSMFile("/home/hudak/code/klaster2/data/data.lsm",&i);
	if ((i==0) && (lsmfile!=NULL))
	{
		// rozmery pola musia byt mocniny dvojky
		int hlbka=0;
		for (int t=1; t<13; t++)
		{
			if (int(lsmfile->pocetdir/2)==(int)dvaNaX32(t)) 
			{
				hlbka=(lsmfile->pocetdir/2);
				break;
			}
			else 
			if ((int)dvaNaX32(t)>int(lsmfile->pocetdir/2))
			{
				hlbka=dvaNaX32(t);
				break;
			}
		}
		cout << "Rozmery : " << lsmfile->sirka << "x" << lsmfile->vyska << "x" << hlbka;
		// nastavenie rozmerov dat
		this->x=lsmfile->sirka;
		this->y=lsmfile->vyska;
		this->z=hlbka;
		// alokovanie pola
		int velkost = lsmfile->sirka*lsmfile->vyska*hlbka;
		data=new UINT16[velkost];

		// nacitanie dat
		imdirAkt=lsmfile->head;
		int pocetdir=0; // kolko adresarov sme uz presli
		int z2=0;
		// prvy v spajanom zozname ide *nahlad* ! (zoznam prechadzame od konca, realne v subore su nahlady az druhe)
	
		while (imdirAkt)
		{
			imdirAkt->LoadPixelData(lsmfile->f);
			// nacitame kazdy druhy - tam nie su nahlady
			if ((pocetdir%2)==1)
			{ 
				// nacitame 2D obrazok
				int tmp=0, celkovo=0;
				for (int y=0; y<imdirAkt->chead->vyska; y++)
					for (int x=0; x<imdirAkt->chead->sirka; x++)
					{
						celkovo++;
						if (imdirAkt->chead->pixeldata16[x][imdirAkt->chead->vyska-1-y]>prahovanie) tmp++;
						if (imdirAkt->chead->pixeldata16[x][imdirAkt->chead->vyska-1-y]<prahovanie) data[x + y*imdirAkt->chead->sirka + z2*imdirAkt->chead->sirka*imdirAkt->chead->vyska]=0;
						else data[x + y*imdirAkt->chead->sirka + z2*imdirAkt->chead->sirka*imdirAkt->chead->vyska]=imdirAkt->chead->pixeldata16[x][imdirAkt->chead->vyska-1-y];
					}
				printf("Pocet hodnot vacsich ako %d je (%d/%d).\n", prahovanie, tmp, celkovo);
				z2++;
			}
			// uvolnime 2D rez z pamati
			imdirAkt->DeletePixelData();
			// dalsi rez
			imdirAkt=imdirAkt->dalsi;
			pocetdir++;
		}
		// zvysok alokovanych dat doplnime nulami (ak nejaky zvysok je)
		while (z2<hlbka)
		{
				for (int y=0; y<(int)lsmfile->vyska; y++)
					for (int x=0; x<(int)lsmfile->sirka; x++)
					{
						data[x + y*lsmfile->sirka + z2*lsmfile->sirka*lsmfile->vyska]=0;
					}
				z2++;
		}
	}
	else 
	{
		switch(i)
		{
			default:printf("Chyba cislo %i\n",i);
				break;
			case 1:printf("Chyba: subor neexistuje.");
				break;
		}
		//char c=getchar();
		exit(2);
	}
	// upraceme
	if (lsmfile) delete lsmfile; lsmfile=NULL;

	posunx=-512.0f;
	posuny=-512.0f;
	posunz=0.0f;
	// nastavenie rozmerov medzi voxelmi mriezky
	dx = 1;
	dy = 1;
	dz = 1; //3.64f;
	// nastavime obal
	nastavObal();
}




void CData::initHomog()
{
	// z nejakych dat musime vytvorit lookup data
	if (data==NULL) 
	{
		cout << "Chyba: neinicializovane data!" << endl;
		exit(1);
	}
	if (homog) delete[] homog;
	// "t"=pocet integerov aby sme dostali prislusny pocet bitov (x-1)*(y-1)*(z-1)=pocet kociek
	float t_ = (x-1)*(y-1)*(z-1)/32.0f;
	// ak treba este jeden integer tak ho pridame
	if ( float(t_-(UINT32)t_)!=0.0f ) t_=(float)(UINT32)t_+1; 
	int t = (int)t_;
	// vytvorime pole
	homog = new UINT32[t];
	// reset dat
	int i,j,k;
	for (i=0; i<t; i++) homog[i]=0;
	// zistenie homogennych blokov a ich oznacenie bitom v poli "homog"
	for (k=0; k<z-1; k++)
		for (j=0; j<y-1; j++)
			for (i=0; i<x-1; i++)
			{
				// ak je rovnakych vsetkych 8 intenzit v bloku
				if ((data[getIndex(i,j,k,*this)]==data[getIndex(i+1,j,k,*this)]) &&
					 (data[getIndex(i,j,k,*this)]==data[getIndex(i+1,j,k+1,*this)]) &&
					 (data[getIndex(i,j,k,*this)]==data[getIndex(i,j,k+1,*this)]) &&
					 (data[getIndex(i,j,k,*this)]==data[getIndex(i,j+1,k,*this)]) &&
					 (data[getIndex(i,j,k,*this)]==data[getIndex(i+1,j+1,k,*this)]) &&
					 (data[getIndex(i,j,k,*this)]==data[getIndex(i+1,j+1,k+1,*this)]) &&
					 (data[getIndex(i,j,k,*this)]==data[getIndex(i,j+1,k+1,*this)]))
				{
					// nastavime prislusny bit na 1
					setHBit(i,j,k,*this);
				}
			}
}



void CData::initDVolume() // alokuje a inicializuje distance volume
{
	// z nejakych dat musime vytvorit lookup data
	if (data==NULL) 
	{
		cout << "Chyba: neinicializovane data!" << endl;
		exit(1);
	}

	// alokovanie pamate
	bunky = new UINT8[(this->x-1) * (this->y-1) * (this->z-1)];

	// vystup zakl. algoritmu distance volume (vzdialenost od voxelov, nie buniek), tmp
	short *dvolume = NULL; 
	dvolume = new short[this->x * this->y * this->z];
	
	// vypocet distance volume pre bunky
	dv_init3x3x3(this->data, dvolume, this->x, this->y, this->z); // inicializuje pole dvolume 
	dv_compute3d(dvolume, this->x, this->y, this->z); // pre kazdy voxel vypocita vzdialenost od zaujimaveho objektu
	dv_celldist3d(dvolume, this->x, this->y, this->z, bunky); // vzdialenosti voxelov konvertujem na vzdialenosti medzi bunkami

	// upraceme
	delete[] dvolume; dvolume = NULL;
}



void CData::nastavObal()
{
	this->obal=CKvader(CVektor3(posunx,posuny,posunz),CVektor3(float((x-1)*dx)+posunx, float((y-1)*dy)+posuny, float((z-1)*dz)+posunz));
}




// sakas addressing scheme
void CData::initSakas()
{
	Caddr=4;	

	if (Gaddr) delete[] Gaddr; Gaddr=new UINT32[x];
	if (Haddr) delete[] Haddr; Haddr=new UINT32[y];
	if (Iaddr) delete[] Iaddr; Iaddr=new UINT32[z];
	if (data2) delete[] data2; data2=new UINT16[x*y*z];

	int i,j,k;
	for (i=0; i<x; i++) Gaddr[i]=(i/Caddr)*Caddr*Caddr*Caddr+(i%Caddr);
	for (j=0; j<y; j++) Haddr[j]=(j/Caddr)*(x/Caddr)*Caddr*Caddr*Caddr+(j%Caddr)*Caddr;
	for (k=0; k<z; k++) Iaddr[k]=(k/Caddr)*(x/Caddr)*(y/Caddr)*Caddr*Caddr*Caddr+(k%Caddr)*Caddr*Caddr;

	for (k=0; k<z; k++)
		for (j=0; j<y; j++)
			for (i=0; i<x; i++)
			{
				data2[getIndexSak(i,j,k,*this)]=data[getIndex(i,j,k,*this)];
			}
}



void CData::initBrick()
{
	// dimenzie musia byt rovnake!! posuvame ich potom nech su po sebe iduce ako mocniny 2
	bd_x = bd_y = bd_z = 2; // pri 16bit datach je to 64kB
  bvd_x=this->x/bd_x;
	bvd_y=this->y/bd_y;
	bvd_z=this->z/bd_z;

	//cout << x << "," << y << "," << z << "...";
	if (data2) delete[] data2; data2=new UINT16[this->x*this->y*this->z];

	int i,j,k;
	for (k=0; k<this->z; k++)
		for (j=0; j<this->y; j++)
			for (i=0; i<this->x; i++)
			{
				data2[getIndexBrick(i,j,k,*this)]=data[getIndex(i,j,k,*this)];
			}

	int _i, _j, _k; // pozicia vzorky
	int di, dj, dk; // delta-tato sa pricita k pozicie vzorky-dostaneme druhu vzorku
	di = dj = dk = 0; // rozdiel druhej a prvej vzorky bude offset do "lut"

	for (int y=0; y<7; y++)
		for (int x=0; x<8; x++)
		{
			// 8 roznych poloh vzorky v bricku			
			switch (x)
			{
				case 0:	_i=0; _j=0; _k=0; break;
				case 1:	_i=0; _j=0; _k=bd_z-1; break;
				case 2: _i=0; _j=bd_y-1; _k=0; break;
				case 3: _i=0; _j=bd_y-1; _k=bd_z-1; break;
				case 4: _i=bd_x-1; _j=0; _k=0; break;
				case 5: _i=bd_x-1; _j=0; _k=bd_z-1; break;
				case 6: _i=bd_x-1; _j=bd_y-1; _k=0; break;
				case 7: _i=bd_x-1; _j=bd_y-1; _k=bd_z-1; break;
			}
			// 7 roznych susedov
			switch (y)
			{
				case 0: di=1; dj=0; dk=0; break;
				case 1: di=0; dj=1; dk=0; break;
				case 2: di=1; dj=1; dk=0; break;
				case 3: di=0; dj=0; dk=1; break;
				case 4: di=1; dj=0; dk=1; break;
				case 5: di=0; dj=1; dk=1; break;
				case 6: di=1; dj=1; dk=1; break;
			}
			lut[x][y]=getIndexBrick(_i+di,_j+dj,_k+dk,*this)-getIndexBrick(_i,_j,_k,*this);
		}
}




// dalsie funkcie

int getIndex(int i, int j, int k, const CData& d)
{
	return i + j*(d.x) + k*(d.x*d.y);
}




int getIndexSak(int i, int j, int k, const CData& d)
{
	return d.Gaddr[i] + d.Haddr[j] + d.Iaddr[k];
}




// bloku dat priradi index v poli homog
void setHBit(int i, int j, int k, const CData& d)
{
	// pozicia v integeroch, nie bitoch
	UINT32 byte_pos=i+j*(d.x-1) + k*((d.x-1)*(d.y-1)); 
  // div, kam do pola sa ulozi vysledne cislo
	UINT32 kam=byte_pos/32;
	// na ktoru poziciu sa bit zapise
	UINT8 poz=byte_pos%32;
	// vytvorenie bitu
	UINT32 co = dvaNaX32(poz);
	d.homog[kam]=d.homog[kam] | co;
}



bool overHBit(int i, int j, int k, const CData& d)
{
	// pozicia v integeroch, nie bitoch
	UINT32 byte_pos=i+j*(d.x-1) + k*((d.x-1)*(d.y-1)); 
  // div, kam do pola sa ulozi vysledne cislo
	UINT32 kam=byte_pos/32;
	// na ktoru poziciu sa bit zapise
	UINT8 poz=byte_pos%32;
	// cislo, ktore ideme testovat
	UINT32 test=d.homog[kam];
	// shift, aby bol testovany bit na prvom mieste
	if (poz!=0) test = test >> poz;
	// ak je tam 1 tak true inak false
	if (test&1) return true; else return false;
}



// bricking

void nastavIcka(int i, int j, int k, CData& d)
{
	d.i1=i % d.bd_x;
	d.j1=j % d.bd_y;
	d.k1=k % d.bd_z;
	
	d.i2=i / d.bd_x;
	d.j2=j / d.bd_y;
	d.k2=k / d.bd_z;
}



int blkOffset(int i, int j, int k, const CData& d)
{
	return d.i2 + d.j2*d.bvd_x + d.k2*(d.bvd_x*d.bvd_y);
}




int offsetInBlk(int i, int j, int k, const CData& d)
{
  return d.i1 + d.j1*d.bd_x + d.k1*(d.bd_x*d.bd_y);
}




int getIndexBrick(int i, int j, int k, CData& d)
{
	nastavIcka(i,j,k,d);
	return blkOffset(i,j,k,d)*(d.bd_x*d.bd_y*d.bd_z)+offsetInBlk(i,j,k,d);
}




void getIndexLut(int i, int j, int k, CData& d, UINT32* v)
{
	short index=-1; // index do look-up table

	// vstup do vypoctu "index" je v dim bricku nie v dim dat
	int _i=(i % d.bd_x) << 2;
	int _j=(j % d.bd_y) << 1; 
	int _k=(k % d.bd_z);

	/*
	// univerzalny vypocet indexu
	int _bd_x = d.bd_x << 2;
	int _bd_y = d.bd_y << 1;

	// delime najmensou dimenziou bricku - vypocitame o kolko bitov treba shiftovat
	UINT8 posun = 0;						
  int m = MIN(d.bd_z,MIN(_bd_x,_bd_y));
  for (register int l = 0; l<13; l++) if (powf(2,l)==m) { posun = l; break;}
   
	// o ktory pripad z 8 moznych sa jedna?
	index=((((_i&( _bd_x-1))+4)&(256- _bd_x))+
		     (((_j&( _bd_y-1))+2)&(256- _bd_y))+
				 ((( k&(d.bd_z-1))+1)&(256-d.bd_z))) >> posun;
	
  */
	// kvoli rychlosti ho nastavime manualne
	index=((((_i&0x7F)+4)&0x80)+(((_j&0x3F)+2)&0xC0)+(((_k&0x1F)+1)&0xE0)) >> 5;

	// vypocet hodnot
	v[0]=getIndexBrick(i,j,k,d);
	v[1]=v[0]+d.lut[index][0]; // 100
	v[2]=v[0]+d.lut[index][1]; // 010
	v[3]=v[0]+d.lut[index][2]; // 110
	v[4]=v[0]+d.lut[index][3]; // 001
	v[5]=v[0]+d.lut[index][4]; // 101
	v[6]=v[0]+d.lut[index][5]; // 011
	v[7]=v[0]+d.lut[index][6]; // 111
}




