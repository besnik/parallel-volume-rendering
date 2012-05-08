// binary swap algorithm. handles communication between machines in the cluster and exchange of the data

#include "kdtree.h"
#include "vektor3.h"

typedef struct // binary swap plan
{
	int pid;	// processor id (machine ID). with which PC in the cluster i'm going to commnunicate?
	int over;	// composition over/under? 1/0
} Plan;


bool inInterv(int co, int a, int b); // is number in interval <a,b> ?
void vytvorPlan(int ID, int hlbka, Node* n, const CVektor3& v, Plan* out);	// creates exchange plan for all processors, ID of the processor, depth of the tree - 1, root, output plan.
void traverzuj(Node* n); // displays IDs of the data. from-to-back traversing.
