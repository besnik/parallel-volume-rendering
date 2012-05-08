// initialization of cluster
// communication between nodes in the farm (send/receive)
// initialization of the master node

#ifndef __INIT_H_
#define __INIT_H_

#include <iostream>
#include <string>

#include "kdtree.h"
#include "loadlsm.h"
#include "mpi.h"

using namespace std;

bool vrat_velkost(const string& s, TVektor<int>& a, TVektor<int>& b, CLSMFile*& lsmfile); // pociatocna inicializacia master procesora
void init_master(const string& s, TVektor<int>& a, TVektor<int>& b, CLSMFile*& lsmfile); // inicializacia renderovania
void init_node(TVektor<int>& a, TVektor<int>& b); // inicializacia renderovania
void posli_data(UINT16* buf, int pocet, int kam, int tag); // posle data na dany procesor pomocou MPI_Send
void prijmi_data(UINT16* buf, int pocet, int odkial, int tag, int& prijate); // prijem velkeho bloku dat

#endif

