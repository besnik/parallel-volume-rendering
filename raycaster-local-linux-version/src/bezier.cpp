// bezier.cpp
// by skip


#include "../include/bezier.h"


// konstruktory

CBezKrivka::CBezKrivka()
{
	c=0;
	presnost=1;
	presnost_min=0; presnost_max=200;
	pocet=0;
	suradnice=NULL;
	sindex=-1;
}




// destruktor

CBezKrivka::~CBezKrivka()
{
	if (suradnice) delete[] suradnice; suradnice=NULL;
}




void initBKriv(int c, CBezKrivka& bk)
{
	bk.c=(UINT8)c;
	if (bk.suradnice==NULL) bk.suradnice=new BOD[c];
}




// ostatne funckie

float bsPol(int n, int i, float t)
{
	if ((i<0) || (i>n)) return 0.0f;
	if ((i==0) && (t==0.0f)) return 1.0f;
	if ((t==0.0f) && (i>0)) return 0.0f;
	if ((t==1.0f) && (i<n)) return 0.0f;
	if ((i==n) && (t==1.0f)) return 1.0f;
	return ((float)komCis(n,i))*powf(t,i)*powf(1.0f-t,n-i);
}




BOD bKrivka(float t, int p, const CBezKrivka& bk)
{
	// vysledny bod v krivky   
	float vx=0.0f,vy=0.0f;
	if (p>0)
	{
		float polynom;
		for (int i=0;i<p;i++)
		{
			polynom=bsPol(p-1,i,t);
			vx+=bk.suradnice[i].x*polynom;
			vy+=bk.suradnice[i].y*polynom;
		}
	}
	BOD vystup;
	vystup.x=vx;
	vystup.y=vy;
	return vystup;
}




void kresliBody(const float polomer, const CBezKrivka& bk)
{
   if (bk.pocet>0)
   {
     glColor3f(0.0f,1.0f,0.0f);   
     for (UINT8 i=0;i<=bk.pocet-1;i++)
     {   
      glBegin(GL_POLYGON); 
        glVertex2f((float)bk.suradnice[i].x,(float)bk.suradnice[i].y+polomer);
        glVertex2f((float)bk.suradnice[i].x+polomer,(float)bk.suradnice[i].y);
        glVertex2f((float)bk.suradnice[i].x,(float)bk.suradnice[i].y-polomer);
        glVertex2f((float)bk.suradnice[i].x-polomer,(float)bk.suradnice[i].y);
      glEnd();
     }
   }
}




void kresliSpojnice(const CBezKrivka& bk)
{
   glColor3f(0.3f,0.3f,0.3f);   
   glBegin(GL_LINE_STRIP);
   if (bk.pocet>0) for (int i=0;i<=bk.pocet-1;i++)
   {   
     glVertex3f((float)bk.suradnice[i].x,(float)bk.suradnice[i].y,0.0f);  
   }
   glEnd();
}



void kresliKrivku(const CBezKrivka& bk)
{
  if (bk.pocet>0) 
  {
   BOD krivka;
   float t=0.0f,posun;
   posun=1.0f/(float)bk.presnost;
   glColor3f(1.0f,1.0f,1.0f);
   glBegin(GL_LINE_STRIP);
   for (int i=0;i<=bk.presnost;i++)  //rovnost aby sa vykreslila dokonca - teda skutocna presnost je var presnost+1
   {   
      krivka=bKrivka(t,bk.pocet,bk);
      glVertex3f(krivka.x,krivka.y,0.0f);  
      t+=posun;
   }
   glEnd();
  }
}




BOD vratKrivku(float t, const CBezKrivka& bk)
{
  if (bk.pocet>0) return bKrivka(t,bk.pocet,bk);
	// len aby nebol warning
	BOD krivka; krivka.x=-1.0f; krivka.y=-1.0f;
	return krivka; 
}




short vratBod(float x, float y, const CBezKrivka& bk)
{
	if (bk.pocet>0) 
	{
		float dx,dy; //rozdiel dx - x, dy - y
		float vzd; //vzialenost
		const float max = 13.0f; // okolie bodu na ktore mozeme kliknut
		for (UINT8 i=0;i<bk.pocet;i++)
		{
			dx=fabs(bk.suradnice[i].x-x);
			dy=fabs(bk.suradnice[i].y-y);
			vzd=sqrtf(dx*dx + dy*dy);
			if (vzd<=max) return i;
		}
	}
	return -1;
}
