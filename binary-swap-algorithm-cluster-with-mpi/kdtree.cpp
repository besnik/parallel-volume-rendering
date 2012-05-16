// kdtree.cpp : Defines the entry point for the console application.
// by skip


#include "kdtree.h"
#include "vektor3.h"
#include "definicie.h"

#include <stdio.h>
#include <math.h>


// konstruktory

KDTree::KDTree()
{
	root = NULL;
	hlbka=0;
}



KDTree::KDTree(const TVektor<int>& a, const TVektor<int>& b, int hlbka, int delenie)
{
	root = NULL;
	this->hlbka=hlbka; // 0..hlbka, teda pocet urovni = hlbka+1
	createNode(root, a,b, hlbka, delenie, 0, (int)pow(2,hlbka)-1 );
}



// destruktor

KDTree::~KDTree()
{
	freeNode(root);
}



void KDTree::createNode(Node*& n, const TVektor<int>& a, const TVektor<int>& b, int hlbka, int delenie, int i1, int i2) // vytvori uzol a podstrom do hlbky
{
	n=new Node;
	n->a=a; n->b=b;						// body ohranicujuce obsah
	n->delenie=delenie;
	n->front=NULL; n->back=NULL;
	n->i1=i1; n->i2=i2;				// ktore procesory drzi uzol
	
	if (hlbka!=0)							// ak nie som list tak vytvorim synov
	{
		TVektor<int> a1,b1;						// hranicne body synov
		TVektor<int> a2,b2;

		switch(delenie)					// rozdelim data podla prislusnej roviny
		{
		case 0:									// rovina rovnobezna s osou X
			// front (up)
			a1.x=a.x; a1.y=a.y+(ABS(a.y-b.y)+1)/2; a1.z=a.z;
			b1=b;
			// back (down)
			a2=a;
			b2.x=b.x; b2.y=a1.y-1; b2.z=b.z;
			break;
		case 1:									// rovina rovnobezna s osou Y
			// front (right)
			a1.x=a.x+(ABS(a.x-b.x)+1)/2; a1.y=a.y; a1.z=a.z;
			b1=b;
			// back (left)
			a2=a;
			b2.x=a1.x-1; b2.y=b.y; b2.z=b.z;
			break;
		case 2:									// rovina rovnobezna s osou Z
			a1.x=a.x; a1.y=a.y; a1.z=a.z+(ABS(a.z-b.z)+1)/2;;
			b1=b;			

			a2=a;
			b2.x=b.x; b2.y=b.y; b2.z=a1.z-1;
			break;
		}

		int fi1,fi2;
		int bi1,bi2;

		fi1=i1 + (ABS(i1-i2)+1)/2;
		fi2=i2;

		bi1=i1;
		bi2=fi1-1;

		createNode(n->front, a1,b1, hlbka-1, (delenie+1)%3, fi1, fi2);
		createNode(n->back,  a2,b2, hlbka-1, (delenie+1)%3, bi1, bi2);
	}
}



void KDTree::freeNode(Node*& n)					// uvolni uzol a jeho podstrom (ak existuje) z pamate
{
	if (!n) return;												// ak nic nedrzi tak hned vyskoc
	if (n->front) freeNode(n->front);			// ak existuje predny syn, tak ho rekurzivne zmaz
	if (n->back) freeNode(n->back);				// detto pre laveho 

	if (n!=NULL) delete n; n=NULL;				// ak nemas synov tak ta zmazem
}
