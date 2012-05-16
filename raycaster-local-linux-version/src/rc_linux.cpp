// rc_linux.cpp
// raycaster verzia pre Linux
// by skippo 
// port dna 13.07.2006

#include "../include/rc_linux.h" // main program

using namespace std;

// rozmery okna na ktoru sa bude mapovat textura
#define obr_sirka 512
#define obr_vyska 512

// rozmery camera plane vo world coord., tiez rozmery vyslednej textury
#define cam_plane_x 512
#define cam_plane_y 512

// opacity editor - rozmery a pozicia
#define edt_sir 200
#define edt_vys 100

#define edt_left obr_sirka-210 
#define edt_top obr_vyska-110


// mys
bool lb=false, rb=false; // left/right button down?
int left_mouse_x,left_mouse_y, right_mouse_y;	// kde bolo kliknute

bool tienuj=false;

CRaycaster renderer;


// OPACITY EDITOR DEFAULT /////////////////////////////////////

void setOpacEdt()
{
	renderer.kriv.pocet=2;
	renderer.kriv.suradnice[0].x=edt_left+39;
	renderer.kriv.suradnice[0].y=edt_top+edt_vys-1;
	renderer.kriv.suradnice[1].x=edt_left+edt_sir-1;
	renderer.kriv.suradnice[1].y=edt_top+70;
}



// TEXTURY ////////////////////////////////////////////////////

void InitTextury()
{
	glGenTextures(1, &texture[0]); // vygenerovanie textury - 1kus
	glBindTexture(GL_TEXTURE_2D, texture[0]); // nastavenie aktivnej 2D textury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //skaluj linearne ked obrazok je vacsi ako textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //skaluj linearne ked obrazok je mensi ako textura
	glPixelTransferf(GL_RED_SCALE, 1.0f);
	glPixelTransferf(GL_GREEN_SCALE, 1.0f); // 1=prepusti iba zelenu farbu
	glPixelTransferf(GL_BLUE_SCALE, 1.0f); // 0=neprepusti
}


// GLUT OPENGL SETTINGS //////////////////////////////////////



void onKeyboard(unsigned char key, int x, int y) // procedura obsluhujuca stlacenie klavesy
{
	switch(key)
	{
		case 27:					// ESC
			exit(0);				// ak bolo stlacene Esc tak ukoncime program
		  	break;
		case 'a':
			renderer.kamera.yawSpace(-3.0f);	// okolo y-osi
			glutPostRedisplay();
		  	break;
		case 'd':
			renderer.kamera.yawSpace(3.0f);
			glutPostRedisplay();
			break;
		case 'w':
			renderer.kamera.pitch(-3.0f);	// okolo x-osi
			glutPostRedisplay();
			break;
		case 's':
			renderer.kamera.pitch(3.0f);
			glutPostRedisplay();
			break;
		case '+':
			renderer.kriv.presnost++;
			if (renderer.kriv.presnost>renderer.kriv.presnost_max) renderer.kriv.presnost=renderer.kriv.presnost_max;
			cout << "presnost krivky: " << (int)renderer.kriv.presnost << endl;
			// nanovo vytvorime lookup table
			renderer.vytvorEdtLut();
			glutPostRedisplay();
			break;
		case '-':
			if (renderer.kriv.presnost>0) renderer.kriv.presnost--;
			if (renderer.kriv.presnost<renderer.kriv.presnost_min) renderer.kriv.presnost=renderer.kriv.presnost_min;
			cout << "presnost krivky: " << (int)renderer.kriv.presnost << endl;
			// nanovo vytvorime lookup table
			renderer.vytvorEdtLut();
			glutPostRedisplay();
			break;
		case 'l':
			cout << renderer.svetlo.pos.x << "," << renderer.svetlo.pos.y << "," << renderer.svetlo.pos.z << endl;
			cout << renderer.svetlo.dir.x << "," << renderer.svetlo.dir.y << "," << renderer.svetlo.dir.z << endl;
			break;
		case 'k':
			cout << renderer.kamera.from.x << "," << renderer.kamera.from.y << "," << renderer.kamera.from.z << endl;
			cout << renderer.kamera.to.x << "," << renderer.kamera.to.y << "," << renderer.kamera.to.z << endl;
			break;
		case '.':
			renderer.kamera.zoom+=1.0f;
			cout << "zoom: " << renderer.kamera.zoom << endl;
			glutPostRedisplay();
			break;
		case ',':
			renderer.kamera.zoom-=1.0f;
			cout << "zoom: " << renderer.kamera.zoom << endl;
			glutPostRedisplay();
			break;
		case 'h':
			renderer.data.posunx+=50;
			cout << "posunx: " << renderer.data.posunx << endl;
			glutPostRedisplay();
			break;
		case 'f':
			renderer.data.posunx-=50;
			cout << "posunx: " << renderer.data.posunx << endl;
			glutPostRedisplay();
			break;
		case 't':
			renderer.data.posuny+=50;
			cout << "posuny: " << renderer.data.posuny << endl;
			glutPostRedisplay();
			break;
		case 'g':
			renderer.data.posuny-=50;
			cout << "posuny: " << renderer.data.posuny << endl;
			glutPostRedisplay();
			break;
	}
} 







void onResize(int w, int h) // w a h reprezentuje novu velkost okna
{
	if (h==0) h = 1;	
	glViewport(0, 0, w , h);
	glMatrixMode(GL_PROJECTION); // budeme menit projekcnu maticu (transformaciu)
	glLoadIdentity(); // vynulovanie
	gluOrtho2D(0,w,h,0); // mapovanie abstraktnych suradnic na souradnice okna, ak chceme skalovanie tak zamenit w,h za sirka,vyska
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
} 


/* povodne linuxove
void onResize(int w, int h)				// w a h reprezentuje novu velkost okna
{
	if (h==0) h = 1;	
	glViewport(0, 0, w , h);
	glMatrixMode(GL_PROJECTION);                    // budeme menit projekcnu maticu (transformaciu)
	glLoadIdentity();                               // vynulovanie
	glOrtho(0, w, h, 0, -1, 1);                 	// mapovanie abstraktnych suradnic na souradnice okna, ak chceme skalovanie tak zamenit w,h za sirka,vyska
}
*/

// opacity editor
void opacEdt()
{
	glDisable(GL_TEXTURE_2D);

	glColor3f(0.73f,0.47f,0.31f);
	glBegin(GL_LINE_STRIP);
		// ohranicujuci obdlznik
		glVertex2i(edt_left+0, edt_top+edt_vys-1);
		glVertex2i(edt_left+edt_sir-1, edt_top+edt_vys-1);
		glVertex2i(edt_left+edt_sir-1, edt_top+0);
		glVertex2i(edt_left+0, edt_top+0);
		glVertex2i(edt_left+0, edt_top+edt_vys-1);
		// vyrovnavacia lista
		glVertex2i(edt_left-9, edt_top+edt_vys-1);
		glVertex2i(edt_left-9, edt_top+0);
		glVertex2i(edt_left+0, edt_top+0);
	glEnd();
	// krivka & stuff
	kresliSpojnice(renderer.kriv);
	kresliKrivku(renderer.kriv);
	kresliBody(5.0f,renderer.kriv);
}




void onDisplay(void) // renderovanie obrazovky
{
	static double t0,t1; // pre vypocet FPS
	t0=timer();
	
	glClearColor(0.0, 0.0, 0.0, 0.0); // nastavenie mazacej farby na ciernu
	glClear(GL_COLOR_BUFFER_BIT); // vymazanie bitovych rovin farebneho buffera

	//glLoadIdentity();
	
	// RENDERING
	// este povolit aj kreslenie textury
	tienuj ? renderer.renderujPhong() : renderer.renderuj();
	
	glColor3f(1.0f,1.0f,1.0f); // toto tu musi byt aby bola textura uplne jasna
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	
	// parametre: 2D textura, kvalita, vystupny format, sirka, vyska, border, vstupny format, typ dat, data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, renderer.textura.sirka, renderer.textura.vyska, 0, GL_RGB, GL_UNSIGNED_BYTE, renderer.textura.data);
	
	glBegin(GL_QUADS);
	glTexCoord2i(0,0); glVertex2i(0,0);
		glTexCoord2i(1,0); glVertex2i(obr_sirka,0);
		glTexCoord2i(1,1); glVertex2i(obr_sirka,obr_vyska);
		glTexCoord2i(0,1); glVertex2i(0,obr_vyska);
	glEnd();
		
	opacEdt(); // opacity editor (krivka)
	
	glutSwapBuffers();
	
	static float fps;
	//static char s[30];
	t1=timer();
	fps=1000.0f/(float)(t1-t0);
	
	cout << "FPS: " << fps << endl;
	//sprintf(s, "FPS: %f",fps);
	//glutSetWindowTitle(s);
} 


void initGL()
{
	InitTextury();										// vytvorenie textury a docasneho datovej struktury pomocou ktorej budeme texturu zaplnat
	glEnable(GL_TEXTURE_2D);					// povolenie textur v OpenGL
} 




void mouse_down(int button, int state, int x, int y )	// klik na mysi
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state==GLUT_DOWN) // lave mysidlo down
		{
				// obsluha krivky
				if (lb==false)
				{
					renderer.kriv.sindex=vratBod(x,y,renderer.kriv);
					// ak sme klikli na bod
					if (renderer.kriv.sindex!=-1) 
					{
						renderer.kriv.dx=float(x)-renderer.kriv.suradnice[renderer.kriv.sindex].x; //pociatocny posun oproti suradniciam bodu, ked posuvam bod tak ho nenastavujem priamo pod kurzor
						renderer.kriv.dy=float(y)-renderer.kriv.suradnice[renderer.kriv.sindex].y;
					}
					// vytvorime novy bod na krivke
					else 
					{
						// ak este mozeme pridat bod
						float posl_bod_x;
						// nemozeme pridat "pred" posledne pridany bod
						if (renderer.kriv.pocet>0) posl_bod_x=renderer.kriv.suradnice[renderer.kriv.pocet-1].x; else  posl_bod_x=-1.0f;
						// test
						if ((renderer.kriv.pocet<renderer.kriv.c-1)
							  && (x>edt_left) && (x<edt_left+edt_sir-1)
								&& (y>edt_top) && (y<edt_top+edt_vys-1)
								&& (float(x)>posl_bod_x))
						{
							renderer.kriv.sindex=-1; // pri mouse_move, ci drzime nejaky bod
							renderer.kriv.suradnice[renderer.kriv.pocet].x=x; 
							renderer.kriv.suradnice[renderer.kriv.pocet].y=y; 
							renderer.kriv.pocet++;
							// nanovo vytvorime lookup table
							renderer.vytvorEdtLut();
							glutPostRedisplay();
						}
						// vyrovnavacia lista
						if ((renderer.kriv.pocet>0)
							  && (x>edt_left-9) && (x<edt_left)
								&& (y>edt_top) && (y<edt_top+edt_vys-1))
						{
							for (int i=0; i<renderer.kriv.pocet; i++) renderer.kriv.suradnice[i].y=(float)y;
							// nanovo vytvorime lookup table
							renderer.vytvorEdtLut();
							glutPostRedisplay();
						}
					}
				}
				// koniec obsluhy krivky
			lb=true;
			left_mouse_x=x;
			left_mouse_y=y;
		} else											// lave mysidlo up
		{
			renderer.cam_old_dx=renderer.cam_dx;
      renderer.cam_old_dy=renderer.cam_dy;
			lb=false;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state==GLUT_DOWN)				// prave mysidlo down
		{
				// krivka
				if ((!rb) && (renderer.kriv.pocet>0))
				{
					static short rbod;
					rbod=vratBod(x,y,renderer.kriv);
					// ak sa nasiel nejaky bod tak ho zmazeme
					if (rbod!=-1)
					{
						renderer.kriv.pocet--;
						// vsetky body dalej od zmazaneho bodu << 1
						for (int i=rbod;i<renderer.kriv.pocet;i++)
						{
							renderer.kriv.suradnice[i]=renderer.kriv.suradnice[i+1];
						}
						// nanovo vytvorime lookup table
						renderer.vytvorEdtLut();
						glutPostRedisplay();
					}
				}
				// koniec krivky
			rb=true;
			right_mouse_y=y;
		} else											// prave mysidlo up
		{
			rb=false;
			renderer.cam_old_vz=renderer.cam_vz;
		}
		break;
	}
}





// mys sa hybe
void mouse_move(int x, int y)
{
	if (lb==true)
	{
		// ak drzime nejaky bod krivky tak presuvame
		float prev_bod_x, next_bod_x;
		// nie vsade ho mozeme presunut, x_i-1 < x_i < x_i+1
		if (renderer.kriv.pocet>0)
		{
			if (renderer.kriv.sindex>0) prev_bod_x=renderer.kriv.suradnice[renderer.kriv.sindex-1].x; else prev_bod_x=-1.0f;
			if (renderer.kriv.sindex<renderer.kriv.pocet-1) next_bod_x=renderer.kriv.suradnice[renderer.kriv.sindex+1].x; else next_bod_x=float(obr_sirka+1);
		} else { prev_bod_x=-1; next_bod_x=obr_sirka+1; }
		// test
		if ((renderer.kriv.sindex!=-1)
			  && (float(x)-renderer.kriv.dx>float(edt_left)) && (float(x)-renderer.kriv.dx<float(edt_left+edt_sir-1))
				&& (float(y)-renderer.kriv.dy>float(edt_top)) && (float(y)-renderer.kriv.dy<float(edt_top+edt_vys-1))
				&& (float(x)-renderer.kriv.dx>prev_bod_x) && (float(x)-renderer.kriv.dx<next_bod_x))
		{
			renderer.kriv.suradnice[renderer.kriv.sindex].x=(float)x-renderer.kriv.dx;
			renderer.kriv.suradnice[renderer.kriv.sindex].y=(float)y-renderer.kriv.dy;
			// nanovo vytvorime lookup table
			renderer.vytvorEdtLut();
		} else {
			// otacame kameru, nefunguje ak sme v prostredi opacity editora
			if ((x<edt_left) || (x>edt_left+edt_sir) || (y<edt_top) || (y>edt_top+edt_vys))
			{
				renderer.cam_dx=renderer.cam_old_dx+((float)(x-left_mouse_x))/200.0f;		
				renderer.cam_dy=renderer.cam_old_dy+((float)(y-left_mouse_y))/200.0f;	
				renderer.setCam(); // nastavi kameru na novu poziciu
			}
		}
		glutPostRedisplay();
	}
	if (rb==true)
	{
		renderer.cam_vz=renderer.cam_old_vz+((float)(y-right_mouse_y))/10.0f;
		cout << renderer.cam_vz << endl;
		renderer.setCam(); // nastavi kameru na novu poziciu
		glutPostRedisplay();
	}
} 




void special_keys(int a_keys, int x, int y)				// Procedura obsluhujuca stlacenie specialnej klavesy
{
	switch (a_keys) 
    {
		case GLUT_KEY_F1:
			renderer.initCamRot();
			renderer.setCam();
			glutPostRedisplay();
		  break;
		case GLUT_KEY_F2:
			tienuj=false;
			glutPostRedisplay();
			break;
		case GLUT_KEY_F3:
			tienuj=true;
			glutPostRedisplay();
			break;
	}
}




// MAIN PROGRAM //////////////////////////////////////////////





int main(int argc, char* argv[])
{
	cout << endl << "Raycasting Initialization" << endl;
	cout << "Raycaster storage class..."; 
	cout.flush();

	// vytvorenie celej sceny a rendera
	renderer = CRaycaster(cam_plane_x,cam_plane_y);
	// alokovanie pola pre suradnice krivky
	renderer.initBKriv();
	// nastavenie rozmerov editora
	renderer.initOpacEdt(edt_sir,edt_vys,edt_left, edt_top);
	// vytvorenie defalut krivky
	setOpacEdt();
	// vytvorenie lookup table
	renderer.vytvorEdtLut();
	// alokovanie pola pre texturu
	renderer.initTexture();

	cout << "OK" << endl;
	cout << "Generating or loading the data...";
	cout.flush();


	// vytvorenie dat
	renderer.data.init(); //renderer.data.initLSM();
	renderer.initCamRot(); // prisposobime vzdialenost kamery datam

	cout << "OK" << endl;
	cout << "Generating distance volume...";
	cout.flush();

	// distance volume
	renderer.data.initDVolume();
		
	cout << "OK" << endl;
	cout << "Generating 2nd data (other addressing method)...";
	cout.flush();

	// vytvorenie inak adresovaych dat
	renderer.data.initBrick();

	cout << "OK" << endl;
	cout << "Generating the homog. lookup data...";
	cout.flush();

	// vytvorenie homog lookup dat
	renderer.data.initHomog();

	cout << "OK" << endl;
	cout << "Checking settings before rendering...";
	cout.flush();
    
	// je spravne nastavena kamera, data?
	if (!renderer.overNastavenia())
	{
		// ak nie je, koncime
		cout << "failed!" << endl;
		exit(1);
	} else
	{
		cout << "OK" << endl;
	}
	



  glutInit(&argc, argv);                      	// inicializacia kniznice GLUT
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);	// Display Mod (Rgb), Double Buffering
  glutInitWindowSize(obr_sirka,obr_vyska);			// Nastavime pociatocnu velkost okna
  glutCreateWindow("LCM volume data renderer");			// vytvorenie okna pre kreslenie
  glutDisplayFunc(onDisplay);               		// registracia funkcie volanej pri prekreslovani okna
  glutReshapeFunc(onResize);               		 	// registracia funkcie volanej pri zmene velkosti okna
  glutKeyboardFunc(onKeyboard);              		// registracia funkcie volanej pri stlaceni klavesy
  glutMouseFunc(mouse_down);										// stlacene tlacidlo na mysi
  glutMotionFunc(mouse_move);										// mys sa hybe
  glutSpecialFunc(special_keys);								// specialne klavesy
  glutPositionWindow(400, 50);									// pozicia laveho horneho rohu okna
  initGL();																			// niektore nastavenia grafickeho systemu
  glutMainLoop();                             	// nekonecny cyklus, kde se volaju zaregistrovane funkcie
	
  return EXIT_SUCCESS; 
}



