// k-D Strom (3D)
// pouziva ho binswap.cpp na rovnomerne rozdelenie dat
// by skip
// version 1.01
// 21.3.2006


#ifndef __KDTREE_H_
#define __KDTREE_H_
 

template<class T>
class TVektor
{
public:
	T x,y,z;
};

// uzol

typedef struct _Node 
{
	int delenie;							// rovina cez ktoru delime data

	TVektor<int> a,b;								// body ohranicujuce data. dolny, horny
	int i1,i2;								// aky interval procesorov drzi uzol

	struct _Node* front;			// binarne delenie dat, predny syn
	struct _Node* back;				// zadny syn
} Node;



// k-D strom

class KDTree
{
public:
  Node* root;								// koren
	int hlbka;								// hlbka stromu

	KDTree();									// konstruktor
	KDTree(const TVektor<int>& a, const TVektor<int>& b, int hlbka, int delenie); // ohranicenie objemu 2 bodmi(3D), do akej hlbky delit, ktorou rovinou zacat delit[0..2 |,-,\]
	~KDTree();								// destruktor
	void freeNode(Node*& n);		// uvolni uzol (podstrom na nom zaveseny -- rekurzivne)
	void createNode(Node*& n, const TVektor<int>& a, const TVektor<int>& b, int hlbka, int delenie, int i1, int i2); // parameter ako u konstruktora, rekurzivne vytvara strom dokial nie je hlbka nulova, interval procesorov, ktore uzol drzi
};
			



#endif 
