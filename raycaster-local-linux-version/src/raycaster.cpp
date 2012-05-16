// raycaster.cpp
// by skip
// hlavne funkcie pre renderovanie


#include "../include/raycaster.h"


// konstruktory

CRaycaster::CRaycaster()
{
	// defaultne nastavenie rozmerov obrazovky
	scr_x = scr_y = 0;
	// nastavenie kamery
	initCamRot();	
	//kamera = CKamera(CVektor3(0, 0, -1),CVektor3(0.0f,0.0f,0.0f),CVektor3(0.0f,1.0f,0.0f),
	//								 1.0f,0.1f,30.0f);
	// maximalny pocet odobratych vzoriek
	max_vz = 1024;
	// mnozstvo odobratych vzoriek pozdlz luca
	samplingRate = 1.0f;
	// typ dat
	typ_dat = 16;
	initPokrytie(800,4095,0.0f,0.3f); // nastavenie hodnot pre linearnu interpolaciu pokrytia
	pozadie = 0.0f;
	intenzita = -1.0f;
	acc_color = 0.0f;
	acc_opacity = 0.0f;
	// nastavenie svetla
	svetlo = CSvetlo(CVektor3(1024.0f,1024.0f,-1024.0f),CVektor3(0.0f,0.0f,0.0f),CFarba(1.0f,1.0f,1.0f,1.0f));
	// nastavenie tienovania
	shader = CShader(CFarba(), CFarba(), CFarba(), 40.0f, 0.5f, 0.5f, 0.5f);
	// data pre texturu
	textura.data=NULL; // textura je struct, takze nema konstruktor->treba ju na zaciatku manualne vynullovat
}




CRaycaster::CRaycaster(UINT16 scr_x, UINT16 scr_y)
{
	// nastavenie rozmerov obrazovky
	this->scr_x = scr_x; this->scr_y = scr_y;
	// nastavenie kamery
  initCamRot();
	//kamera = CKamera(CVektor3(-1.0*cam_vz*sin(cam_dx)*cos(cam_dy), cam_vz*sin(cam_dy), -1.0f*cam_vz*cos(cam_dx)*cos(cam_dy)),CVektor3(0.0f,0.0f,0.0f),CVektor3(0.0f,1.0f,0.0f),
	//kamera = CKamera(CVektor3(0, 0, -1),CVektor3(0.0f,0.0f,0.0f),CVektor3(0.0f,1.0f,0.0f),
	//kamera = CKamera(CVektor3(0, 0, -1),CVektor3(0.0f,0.0f,0.0f),CVektor3(0.0f,1.0f,0.0f),
	//								 1.0f,0.1f,30.0f);
	// nastavime UV ratio
	setRatioUV();
	// maximalny pocet vzoriek
	max_vz = 1024;
	// mnozstvo odobratych vzoriek pozdlz luca
	samplingRate = 1.0f;
	// typ dat	
	typ_dat = 16;
	initPokrytie(2000,4095,0.0f,0.1f);
	pozadie = 0.0f;
	intenzita = -1.0f;
	acc_color = 0.0f;
	acc_opacity = 0.0f;
	// nastavenie svetla
	svetlo = CSvetlo(kamera.from,kamera.to,CFarba(1.0f,1.0f,1.0f,1.0f));
	// nastavenie tienovania - prispevky jednotlivych svetiel
	shader = CShader(CFarba(), CFarba(), CFarba(), 40.0f, 0.9f, 0.5f, 1.0f);
	// data pre texturu
	textura.data=NULL; // textura je struct, takze nema konstruktor->treba ju na zaciatku manualne vynullovat
}




// destruktor

CRaycaster::~CRaycaster()
{
	freeTexture();
}
	



// linearna interpolacia
float CRaycaster::pokrytie(UINT16 i) // vypocet opacity
{
//	if (i>3000) return 0.0f; else return 0.25f;

	if ((i<oa) || (i>ob)) return 0.0f;
	else return linInt(oa,ob,foa,fob,i);
}




void CRaycaster::initOpacEdt(short edt_sir, short edt_vys, short edt_left, short edt_top)
{
	this->edt_sir=edt_sir;
	this->edt_vys=edt_vys;
	this->edt_left=edt_left;
	this->edt_top=edt_top;
}




// bezierovka
float CRaycaster::pokrytieKriv(UINT16 i, short edt_sir, short edt_vys, short edt_left, short edt_top)
{
	// aspon 2 body definuju nejaku krivku
	if (kriv.pocet>1)
	{
		// normalizovana hodnota x na interval <0,1>
		float norm_i = (float)i/4095.0f;
		// normalizovane krajne x-ove suradnice riadiacich bodov
		float norm_oa = (kriv.suradnice[0].x-(float)edt_left)/(float)edt_sir;
		float norm_ob = (kriv.suradnice[kriv.pocet-1].x-(float)edt_left)/edt_sir;
		// normalizovane i v suradniciach krivky
		float norm_i_kriv = (norm_i-norm_oa)/(norm_ob-norm_oa); // t do krivky
		// ak je intenzita mimo krivky tak je uplne priehladna
		if ((norm_i<norm_oa) || (norm_i>norm_ob)) return 0.0f;
		else 
		{
			BOD krivka=vratKrivku(norm_i_kriv,kriv);
			return 1.0f-((krivka.y-(float)edt_top)/(float)edt_vys);
		}
	}
	else return 0.0f;
}




// rotacia kamery

void CRaycaster::setCam()
{
	float up_vektor;
	// ak je kamera rotovana okolo X tak, ze hore je dole tak menime up-vektor
	if (cam_vz*cos(cam_dy)>=0)	up_vektor=1.0f;					
	else												up_vektor=-1.0f;
	// alebo ak sa pozerame zozadu tak vymenime up_vektor
	if (cam_vz<0) up_vektor*=-1.0f;
	
	kamera=CKamera(CVektor3(-1.0f*cam_vz*sin(cam_dx)*cos(cam_dy), cam_vz*sin(cam_dy), -1.0f*cam_vz*cos(cam_dx)*cos(cam_dy)),CVektor3(0.0f, 0.0f, 0.0f), CVektor3(0.0f, up_vektor, 0.0f),kamera.zoom,0.1f,30.0f);
	svetlo.pos=kamera.from;
}




// init camera rotation settings
void CRaycaster::initCamRot()
{
	cam_old_dx = cam_old_dy = 0.0f;
	cam_dy = 0.0f;
	cam_dx = 0.0f;
	// ak su nacitane data tak prisposobime vzdialenost kamery podla nich
	if (data.data!=NULL) cam_old_vz = cam_vz = odmocnina(mocnina(data.x)+mocnina(data.y)+mocnina(data.z));
	else                 cam_old_vz = cam_vz = 1800.0f;
	setCam(); // nastavime kameru + svetlo na poziciu kamery
}




void CRaycaster::setRatioUV()
{
  if (scr_y!=0) kamera.setRatioUV((float)scr_x/(float)scr_y);
}




void CRaycaster::initPokrytie(UINT16 oa, UINT16 ob, float foa, float fob)
{
  this->oa=oa; this->ob=ob; this->foa=foa; this->fob=fob;
}



void CRaycaster::initTexture()
{
	unsigned long velkost; // velkost(pocet bytov-charov) Image->data
	textura.sirka=scr_x;
	textura.vyska=scr_y;
	velkost = textura.sirka * textura.vyska * 3; // *3=RGB, intenzitu= *1
	if (textura.data==NULL) textura.data = new UINT8[velkost]; // alokovanie pamate
}




// bezierova krivka

void CRaycaster::initBKriv()
{
	::initBKriv(20,this->kriv);
}




// ostatne funkcie

void CRaycaster::nastavKameru(const CVektor3 from, const CVektor3 to, const CVektor3 up, float zoom, float n, float f)
{
	kamera=CKamera(from,to,up, zoom, n,f);
	setRatioUV();
}




void CRaycaster::nastavZoom(float zoom)
{
	kamera.zoom=zoom;
}




void CRaycaster::freeData()
{
	if (data.data) delete[] data.data;
	data.data=NULL;
}




void CRaycaster::freeTexture()
{
	if (textura.data) delete[] textura.data;
	textura.data=NULL;
}




bool CRaycaster::overNastavenia()
{
	// pociatok
	CVektor3 origin = CVektor3(0.0f,0.0f,0.0f);
	// body "from" a "to" su rovnake?
	if (kamera.from==kamera.to) return false;
	// je spravne definovany up-vektor?
	if (kamera.from==kamera.up) return false;
	// je spravne nastaveny vektor 'vpravo' a 'hore' definujuce camera plane?
	if (kamera.u==origin) return false;
	if (kamera.v==origin) return false;
	// kontrola zoomu
	if (kamera.zoom<EPSILON) return false;
	// su orezavacie roviny ok?
	if ((kamera.n<0.0f) || (kamera.f<0.0f) || (kamera.f<kamera.n)) return false;
	// kontrola kamery ok!
	//
	// su nacitane data?
	if (data.data==NULL) return false;
	// su rozmery okna ok?
	if ((scr_x<=0) || (scr_y<=0)) return false;
	// kontrola nastaveni ok!
	return true;
}



bool CRaycaster::zistiBlok(const CVektor3& v, UINT16& x, UINT16& y, UINT16& z) // vrati v ktorom bloku mriezky sa vzorka pozdlz luca nachadza, data su ulozene v lavotocivej sur. sustave 
{
	// sme vobec v kocke? - treba nam vobec toto testovanie ? :-)
	if ((v.x - data.posunx >= 0.0f) &&
      (v.x - data.posunx <= float(data.x-1)*data.dx) &&
      (v.y - data.posuny >= 0.0f) &&
      (v.y - data.posuny <= float(data.y-1)*data.dy) &&
      (v.z - data.posunz >= 0.0f) &&	
      (v.z - data.posunz <= float(data.z-1)*data.dz)) 
	{
		// vypocet indexu mriezky
		if (data.dx==1.0f) x=UINT16(v.x-data.posunx); else x=UINT16((v.x-data.posunx)/data.dx);
		if (data.dy==1.0f) y=UINT16(v.y-data.posuny); else y=UINT16((v.y-data.posuny)/data.dy);
		if (data.dz==1.0f) z=UINT16(v.z-data.posunz); else z=UINT16((v.z-data.posunz)/data.dz);
		
		// osetrenie na prave okrajove hrany mriezky
		if (v.x - data.posunx == float(data.x - 1)*data.dx) x = data.x - 2;	// preco 2? lebo kociek je o 1 menej ako hranicnych vzoriek(-1) + cisluje sa od nuly(dalsia -1)
		if (v.y - data.posuny == float(data.y - 1)*data.dy) y = data.y - 2;
		if (v.z - data.posunz == float(data.z - 1)*data.dz) z = data.z - 2;
		
		return true; 
  } 	 
  return false; //vzorka mimo 
}




void CRaycaster::vratBlok(const CVektor3& v, UINT16& x, UINT16& y, UINT16& z) // vrati v ktorom bloku mriezky sa vzorka pozdlz luca nachadza, data su ulozene v lavotocivej sur. sustave 
{
		// vypocet indexu mriezky
		if (data.dx==1) x=UINT16(v.x-data.posunx); else x=UINT16((v.x-data.posunx)/(float)data.dx);
		if (data.dy==1) y=UINT16(v.y-data.posuny); else y=UINT16((v.y-data.posuny)/(float)data.dy);
		if (data.dz==1) z=UINT16(v.z-data.posunz); else z=UINT16((v.z-data.posunz)/(float)data.dz);
		/* riesi nasledujuci IF
		// osetrenie na prave okrajove hrany mriezky
		if (v.x - data.posunx == float(data.x-1)*data.dx) x = data.x - 2;	// preco 2? lebo kociek je o 1 menej ako hranicnych vzoriek(-1) + cisluje sa od nuly(dalsia -1)
		if (v.y - data.posuny == float(data.y-1)*data.dy) y = data.y - 2;
		if (v.z - data.posunz == float(data.z-1)*data.dz) z = data.z - 2;
		*/

		// osetrenie problemov s nepresnostami na poslednom desatinnom mieste floatu (6)
		if (x>data.x-2) x = data.x-2;
		if (y>data.y-2) y = data.y-2;
		if (z>data.z-2) z = data.z-2;
}




// vstup: pozicia vzorky v scene a cislo kocky mriezky v datach (vystup funkcie vratBlok)
void CRaycaster::vratOffsetBloku(const CVektor3& v, UINT16 mi, UINT16 mj, UINT16 mk, CVektor3 &offset)		// do premennej offset ulozi offset pozicie vzorky vzhladom na kocku
{
	offset = CVektor3(v.x, v.y, v.z);

	offset.x = offset.x - data.posunx - float(mi * data.dy);
	offset.y = offset.y - data.posuny - float(mj * data.dy);
	offset.z = offset.z - data.posunz - float(mk * data.dz);

	// kvoli nepresnostiam pri float
	if (offset.x<0.0f) offset.x=0.0f; if (offset.x>1.0f) offset.x=1.0f;
	if (offset.y<0.0f) offset.y=0.0f; if (offset.y>1.0f) offset.y=1.0f;
	if (offset.z<0.0f) offset.z=0.0f; if (offset.z>1.0f) offset.z=1.0f;
} 




void CRaycaster::vytvorEdtLut()
{
	for (UINT16 i=0; i<4096; i++)
	{
		// ak je oversamling tak musime zmenit aj pokrytie
		if (samplingRate==1.0f) edt_lut[i]=pokrytieKriv(i, edt_sir, edt_vys, edt_left, edt_top);
		else edt_lut[i]=1.0f - powf(1.0f - pokrytieKriv(i, edt_sir, edt_vys, edt_left, edt_top),1/samplingRate);
	}
}



void CRaycaster::initRadcTables()
{


}



void CRaycaster::renderuj() // kolme premietanie
{
	// konverzia z camera space start point na world start point
	float nu = this->scr_x/(2.0f*dlzka(kamera.u))*kamera.zoom*(256.0f/scr_x);
	float nv = this->scr_y/(2.0f*dlzka(kamera.v))*kamera.zoom*(256.0f/scr_y);
	
	// prechadzame vsetkymi pixelmi obrazovky
	register UINT16 ray_u, ray_v; // pixel na obrazovke [ray_u, ray_v]
	register UINT32 tex=0; // index do datoveho pola textury
	static float alpha; // intenzite sa priradi pokrytie
	
	static CVektor3 luc; // vektor vstup-vystup
	static int poc_vzr; // dlzka vektora luc, tj. pocet odobranych vzoriek
	
	// spolocne nastavenia pre luce
	// smer lucov
	r.dir=(kamera.dir);
	// o aky vektor sa posuvame
	delta = r.dir/samplingRate;
	
	// lookup tabulky pre preskakovanie prazdnych miest
	int toSkip; // kolko vzoriek pre danu vzdialenost preskocime
	// pre kazdu vzdialenost urcime, vektor a pocet vzoriek na preskocenie
	for (int i=0; i<MAX_DISTANCE; i++)
	{
		toSkip = int(float(i)/samplingRate);
		incrTable[i]=(toSkip+1)*delta; // vektor posunu
		incr[i]=toSkip+1;	// pocet preskocenych vzoriek
	}


	// raycasting

	for (ray_v=scr_y; ray_v>0; ray_v--) // iba kvoli texture sa renderuje od scr_y - aby nebol obrazok hore nohami
		for (ray_u=0; ray_u<scr_x; ray_u++)
		{
			// zistenie startovacej pozicie v cam space. scr_x, scr_y namapovane na <-1,1>
      cam_u=2.0f*(float)ray_u/(float)(scr_x-1)-1.0f;			
			cam_v=2.0f*(float)(ray_v-1)/(float)(scr_y-1)-1.0f; // -2 lebo ideme od scr_y po 1 a to cele treba o -1 znizit
			// vypocet startovacej pozicie luca v world space - vektor o ktory posunieme "from"
			ray_start.x=nu*kamera.u.x*cam_u + nv*kamera.v.x*cam_v;
			ray_start.y=nu*kamera.u.y*cam_u + nv*kamera.v.y*cam_v;
			ray_start.z=nu*kamera.u.z*cam_u + nv*kamera.v.z*cam_v;
			// nastavenie startovacej pozicie luca
			r.pos=kamera.from + ray_start; 
			// trafil luc mriezku dat?
			if (kvaderRay(data.obal,r,&tmin,&tmax))
			{
				// sme vo vnutri dat? ak ano, tak zaciname od *near* camera plane
				if (tmin<EPSILON) tmin=kamera.n;
				// pokial koniec luca je pred "prednou orezavacou rovinou" tak orezene=>nic nevykreslime
				if (tmin<tmax) 
				{
					// zistime vystup aby sme vedeli overit, ci sme uz von z dat
					vystup = r.pos + r.dir*tmax;
					// pozicia vstupu do dat
					r.pos=r.pos + r.dir*tmin;
					// kolko vzoriek musime odobrat
					luc = vystup - r.pos;
					poc_vzr = 1 + (int)dlzka(luc);
					// reset poctu odobratych vzoriek
					poc_vz=0;
					acc_color=0.0f;
					acc_opacity=0.0f;
					
					// prechadzame pozdlz luca dokial sme neodobrali dostatok vzoriek a tieto vzorky este prispievaju do vyslednej farby
					
					do {
						// zistime v ktorom bloku mriezky sme -> i,j,k
						vratBlok(r.pos, mi,mj,mk);
						// je blok homogenny?
						//static UINT32 v0,v1,v2,v3,v4,v5,v6,v7;

						if (overHBit(mi,mj,mk,data)) // data ako cela trieda !! nie pole!
						{
							// priradime lubovolny z 8 vrcholov 
							//farba=data.data[getIndex(mi,mj,mk,data)];
							farba=data.data2[getIndexBrick(mi,mj,mk,data)];
						} else // ak nie je, tak interpolacia farby
						{
							vratOffsetBloku(r.pos,mi,mj,mk,offset);
							
							// trilinearna interpolacia intenzit
							
							// bricking lut
							getIndexLut(mi,mj,mk,data,v);
																
							farba=triIntAnySize(data.data2[v[0]],data.data2[v[1]],
	   							 						    data.data2[v[5]],data.data2[v[4]],
											  					data.data2[v[2]],data.data2[v[3]],
										 							data.data2[v[7]],data.data2[v[6]],
											  					offset.x,offset.y,offset.z,
																	data.dx,data.dy,data.dz);						
								
							/*
							// linear layout, premmenne su definove o par riadkov vyssie
							this->v[0]=getIndex(mi,mj,mk,data);
							v[1]=v[0]+1;
							v[3]=v[0]+data.x*data.y;
							v[2]=v[3]+1;
							v[4]=v[0]+data.x;
							v[5]=v[4]+1;
							v[7]=v[3]+data.x;
							v[6]=1+v[7];

							farba=triInt(data.data[v[0]],data.data[v[1]],
													 data.data[v[2]],data.data[v[3]],
													 data.data[v[4]],data.data[v[5]],
													 data.data[v[6]],data.data[v[7]],
													 offset.x,offset.y,offset.z);
							*/

						} 
						// kompozicia
						//alpha = pokrytie(farba);
						alpha=edt_lut[(UINT16)farba];
						//alpha = pokrytieKriv(farba, edt_sir, edt_vys, edt_left, edt_top);
						if (alpha>0.05f)
						{
							farba=farba/4095.0f; // ak neshadujeme tak musime farbu namapovat na <0,1>
							acc_color=acc_color+(farba)*alpha*(1.0f-acc_opacity); 
							acc_opacity=acc_opacity+alpha*(1.0f-acc_opacity);
						}

						// preskocime prazdne miesto
						static int dist; // vzdialenost k zaujimavemu objektu
						dist = data.bunky[dv_getIndex(mi,mj,mk, data.x-1,data.y-1)]; // ziskame distance value
						// prekocime prazdne miesto
						r.pos=r.pos+incrTable[dist];
						// aktualizujeme pocet vzoriek
						poc_vzr-=incr[dist];
						// zvysime pocet odobranych vzoriek
						poc_vz++;
					} while ((poc_vzr>0) && (acc_opacity<0.95f) && (poc_vz<max_vz));

					// vratime naakumulovanu farbu
					intenzita=acc_color;
				} // end of 2if
			}
			else // luc netrafil data
			{
				intenzita=pozadie;
			}
			// zapiseme RGB hodnoty do textury
			if (intenzita>1.0f) intenzita=1.0f;
			textura.data[tex]=0;
			textura.data[tex+1]=(UINT8)(intenzita*255.0f);
			textura.data[tex+2]=0;

			// dalsi generovany pixel v texture
			tex=tex+3;
			// end of for cycles
		} 
}



void CRaycaster::renderujPhong() // kolme premietanie
{
	// konverzia z camera space start point na world start point
	float nu = this->scr_x/(2.0f*dlzka(kamera.u))*kamera.zoom*(256.0f/scr_x);
	float nv = this->scr_y/(2.0f*dlzka(kamera.v))*kamera.zoom*(256.0f/scr_y);
	
	// prechadzame vsetkymi pixelmi obrazovky
	register UINT16 ray_u, ray_v; // pixel na obrazovke [ray_u, ray_v]
	register UINT32 tex=0; // index do datoveho pola textury
	static float alpha; // intenzite sa priradi pokrytie
	
	static CVektor3 luc; // vektor vstup-vystup
	static int poc_vzr; // dlzka vektora luc, tj. pocet odobranych vzoriek
	
	// spolocne nastavenia pre luce
	// smer lucov
	r.dir=kamera.dir;
	// o aky vektor sa posuvame
	delta = r.dir/samplingRate;
	
	// lookup tabulky pre preskakovanie prazdnych miest
	int toSkip; // kolko vzoriek pre danu vzdialenost preskocime
	// pre kazdu vzdialenost urcime, vektor a pocet vzoriek na preskocenie
	for (int i=0; i<MAX_DISTANCE; i++)
	{
		toSkip = int(float(i)/samplingRate);
		incrTable[i]=(toSkip+1)*delta; // vektor posunu
		incr[i]=toSkip+1;	// pocet preskocenych vzoriek
	}

	static bool hom; // ci sme v homogennom bloku, aby sme nemuseli robit tienovanie


	// raycasting

	for (ray_v=scr_y; ray_v>0; ray_v--)
		for (ray_u=0; ray_u<scr_x; ray_u++)
		{
			// zistenie startovacej pozicie v cam space. scr_x, scr_y namapovane na <-1,1>
      cam_u=2.0f*(float)ray_u/(float)(scr_x-1)-1.0f;			
			cam_v=2.0f*(float)(ray_v-1)/(float)(scr_y-1)-1.0f; // -2 lebo ideme od scr_y po 1 a to cele treba o -1 znizit
			// vypocet startovacej pozicie luca v world space - vektor o ktory posunieme "from"
			ray_start.x=nu*kamera.u.x*cam_u + nv*kamera.v.x*cam_v;
			ray_start.y=nu*kamera.u.y*cam_u + nv*kamera.v.y*cam_v;
			ray_start.z=nu*kamera.u.z*cam_u + nv*kamera.v.z*cam_v;
			// nastavenie startovacej pozicie luca
			r.pos=kamera.from + ray_start;
			// trafil luc mriezku dat?
			if (kvaderRay(data.obal,r,&tmin,&tmax))
			{
				// sme vo vnutri dat? ak ano, tak zaciname od *near* camera plane
				if (tmin<EPSILON) tmin=kamera.n;
				// pokial koniec luca je pred "prednou orezavacou rovinou" tak orezene=>nic nevykreslime
				if (tmin<tmax) 
				{
					// zistime vystup aby sme vedeli overit, ci sme uz von z dat
					vystup = r.pos + r.dir*tmax;
					// pozicia vstupu do dat
					r.pos=r.pos + r.dir*tmin;
					// kolko vzoriek musime odobrat
					luc = vystup - r.pos;
					poc_vzr = 1 + (int)dlzka(luc);
					// reset poctu odobratych vzoriek
					poc_vz=0;
					acc_color=0.0f;
					acc_opacity=0.0f;
					// prechadzame pozdlz luca dokial sme neodobrali dostatok vzoriek a tieto vzorky este prispievaju do vyslednej farby
					while ((poc_vz<max_vz) && (acc_opacity<0.95f))
					{
						// zistime v ktorom bloku mriezky sme -> i,j,k
						vratBlok(r.pos, mi,mj,mk);
						// je blok homogenny?
						hom = false;
						if (overHBit(mi,mj,mk,data))
						{
							// priradime lubovolny z 8 vrcholov 
							//farba=data.data[getIndex(mi,mj,mk,data)];
							farba=data.data2[getIndexBrick(mi,mj,mk,data)];
							//farba=data.data2[getIndexSak(mi,mj,mk,data)];
							hom = true;
						} else // ak nie je, tak interpolacia farby
						{
						
							vratOffsetBloku(r.pos,mi,mj,mk,offset);
		
							// bricking lut
							getIndexLut(mi,mj,mk,data,v);
							farba=triIntAnySize(data.data2[v[0]],data.data2[v[1]],  // 0 1
											 		 data.data2[v[5]],data.data2[v[4]],  // 5 4
											  	 data.data2[v[2]],data.data2[v[3]],  // 2 3
										 		   data.data2[v[7]],data.data2[v[6]],  // 7 6
											  	 offset.x,offset.y,offset.z,
													 data.dx,data.dy,data.dz);	
							
							/*
							// rychlejsia verzia
							v[0]=getIndex(mi,mj,mk,data);
							v[1]=v[0]+1;
							v[3]=v[0]+data.x*data.y;
							v[2]=v[3]+1;
							v[4]=v[0]+data.x;
							v[5]=v[4]+1;
							v[7]=v[3]+data.x;
							v[6]=1+v[7];

							farba=triInt(data.data[v[0]],data.data[v[1]],
							data.data[v[2]],data.data[v[3]],
							data.data[v[4]],data.data[v[5]],
							data.data[v[6]],data.data[v[7]],
							offset.x,offset.y,offset.z);
							*/
							
						} 
						// Kompozicia
						alpha=edt_lut[(UINT16)farba];						
						//alpha = pokrytie((int)farba);
						//alpha = pokrytieKriv(farba, edt_sir, edt_vys, edt_left, edt_top);
						
						// Tienovanie
						if (!hom)
						{
							// tienovanie
							L=normalizuj(CVektor3(svetlo.pos-r.pos));
							V=normalizuj(CVektor3(kamera.from-r.pos));

							// 6 7 - horo kocky
							// 2 3
							// ===						
							// 4 5 - dolo kocky
							// 0 1

							
							// bricking layout shading
							N=getAnalGrad(data.data2[v[0]],data.data2[v[1]],  // 0 1
											 		 data.data2[v[2]],data.data2[v[3]],  // 5 4
													 data.data2[v[4]],data.data2[v[5]],  // 2 3
										 			 data.data2[v[6]],data.data2[v[7]],  // 7 6
													 offset);							
							
							/*
							// linear layout shading
							N=getAnalGrad(data.data[v[0]],data.data[v[1]],
														 data.data[v[2]],data.data[v[3]],
														 data.data[v[4]],data.data[v[5]],
														 data.data[v[6]],data.data[v[7]],
														 offset);
							*/

							N=normalizuj(N);
							N=N*-1;
							//N=CVektor3(0,0,-1);

							c_out=tienujPhong(CFarba(0.0f,farba/4095.0f,0.0f,1.0f),shader,L,V,N);
							farba=c_out.g;
						} else 
						farba=farba/4095.0f*shader.Fa;
						// kompozicia
						acc_color=acc_color+(farba)*alpha*(1.0f-acc_opacity); 
						acc_opacity=acc_opacity+alpha*(1.0f-acc_opacity);
						// zvysime pocet odobranych vzoriek
						poc_vz++;
						// presunieme sa na poziciu dalsej vzorky
						r.pos=r.pos+delta;
						// testujeme, ci sme uz opustili mriezku, tj skalarny sucin je zaporny
						//if (skalarsucin(r.dir,vystup-r.pos)<0.0f) break;
						if (!(--poc_vzr)) break;
					}
					// vratime naakumulovanu farbu
					intenzita=acc_color;
				} // end of 2if
			}
			else // luc netrafil data
			{
				intenzita=pozadie;
			}
			// zapiseme RGB hodnoty do textury
			if (intenzita>1.0f) intenzita=1.0f;
			textura.data[tex]=0;
			textura.data[tex+1]=0;
			textura.data[tex+2]=(UINT8)(intenzita*255.0f);
			
			// dalsi generovany pixel v texture
			tex=tex+3;
			// end of for cycles
		} 
}



