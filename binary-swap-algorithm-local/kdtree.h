// k-D Strom (3D)
// used by Binary Swap algorithm (binswap.cpp) to equally distribute the data
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

// node of the tree
typedef struct _Node 
{
	int delenie;		// plane used to divide the datas

	TVektor<int> a,b;	// points allocating the data (upper, lower)
	int i1,i2;			// which processor interval stores the node?

	struct _Node* front;// front child
	struct _Node* back;	// back child
} Node;



// k-D tree

class KDTree
{
public:
	Node* root;
	int hlbka; // depth of the tree

	KDTree(); // constructor
	KDTree(const TVektor<int>& a, const TVektor<int>& b, int hlbka, int delenie); // limitation of the volume using two points(3D), to what depth divide, which plane to start divide[0..2 |,-,\]
	~KDTree(); // destructor
	void freeNode(Node*& n);. // recursively free node (sub-tree of the node)
	void createNode(Node*& n, const TVektor<int>& a, const TVektor<int>& b, int hlbka, int delenie, int i1, int i2); // parameters as in constructor, recursively creates tree until the depth is not zero, interval of processors that holds the node
};

#endif 
