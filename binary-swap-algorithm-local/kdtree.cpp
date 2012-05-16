// kdtree.cpp : Defines the entry point for the console application.
// by skip

#include "kdtree.h"
#include "vektor3.h"
#include "definicie.h"

#include <stdio.h>
#include <math.h>

// consructors

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

// creates node and sub-tree
void KDTree::createNode(Node*& n, const TVektor<int>& a, const TVektor<int>& b, int hlbka, int delenie, int i1, int i2)
{
	n=new Node;
	n->a=a; n->b=b;	// points that specify the volume
	n->delenie=delenie;
	n->front=NULL; n->back=NULL;
	n->i1=i1; n->i2=i2;	// which processors are used to store node
	
	if (hlbka!=0) // not a leaf, create children
	{
		TVektor<int> a1,b1;	// boundaries of the children
		TVektor<int> a2,b2;

		switch(delenie)	// divide data by plane
		{
		case 0:	// plane aligned with axis X
			// front (up)
			a1.x=a.x; a1.y=a.y+(ABS(a.y-b.y)+1)/2; a1.z=a.z;
			b1=b;
			// back (down)
			a2=a;
			b2.x=b.x; b2.y=a1.y-1; b2.z=b.z;
			break;
		case 1:	// plane aligned with axis Y
			// front (right)
			a1.x=a.x+(ABS(a.x-b.x)+1)/2; a1.y=a.y; a1.z=a.z;
			b1=b;
			// back (left)
			a2=a;
			b2.x=a1.x-1; b2.y=b.y; b2.z=b.z;
			break;
		case 2:	// plane aligned with axis Z
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

// free node and it's subtree from the memory
void KDTree::freeNode(Node*& n)			
{
	if (!n) return;
	if (n->front) freeNode(n->front);
	if (n->back) freeNode(n->back);

	if (n!=NULL) delete n; n=NULL;
}
