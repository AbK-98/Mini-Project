#ifndef _TAXI_H_
#define _TAXI_H_
# include  <stdio.h>
#include<cstring>
# include  <stdlib.h>
# include  <pthread.h>
# include  <unistd.h>
# include  <semaphore.h>
#include <iostream>
#include <time.h>
using namespace std ;
class Taxi {
    int custId;
    string custName;
    string phone;
    int d;
public:
   Taxi();
   int getid();
   string getnam();
   string getphon();
   int getD();
};

#endif
