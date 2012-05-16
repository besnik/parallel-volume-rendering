// binswap.h
// by skip 2006
//


#include "kdtree.h"
#include "vektor3.h"


typedef struct // binary swap kompozicia procesory
{
	int pid;	// s ktorym proceosor budem komunikovat
	int over;	// kompozicia nad/pod ? 1/0
} Plan;


bool inInterv(int co, int a, int b); // je cislo v intervale <a,b> ?
void vytvorPlan(int ID, int hlbka, Node* n, const CVektor3& v, Plan*& out);	// pre kazdy procesor urci vymenny plan, ID procesora, hlbka stromu-1, koren, vystupny plan
void traverzuj(Node* n); // vypise ID jednotlivych dat, traverzovanie front-to-back
