# include  <stdio.h>
#include<cstring>
# include  <stdlib.h>
# include  <pthread.h>
# include  <unistd.h>
# include  <semaphore.h>
#include <iostream>
#include <time.h>
#include "Taxi.h"
#include <list>
#include <cstdlib>
#include <iterator>
using namespace std ;
#define MAX_CUSTOMERS 7
void *customer(void *num);
void *taxi_driver(void *);
void randwait(int secs);
//Code By Abhishek Pradeep

Taxi:: Taxi():custId(0),custName("Customer"),phone("98439393"),d(0) {}
int Taxi:: getid()
{
    cout<<"Enter Customer ID :";
    int x;
    cin>>x;
    return x;
}
string Taxi:: getnam()
{
    cout<<"Enter Customer Name :";
    string y;
    cin>>y;
    return y;
}
string Taxi:: getphon()
{
    cout<<"Enter Customer Phone Number :";
    string p1;
    cin>>p1;
    return p1;
}
int Taxi:: getD()
{
    cout<<"Enter Distance Till Location  :";
    int z;
    cin>>z;
    return z;
}
Taxi t;




//Define the semaphores.
// waitingRoom Limits the # of customers allowed  to enter the waiting in the cab at one time.
sem_t waitingRoom;
// taxiChair ensures mutually exclusive access to the taxi seat.
sem_t taxiChair;
// taxiPillow is used to allow the driver to be lazy and sleepy until a customer arrives.
sem_t taxiPillow;
// seatBelt is used to make the customer to wait until the taxi driver is done cutting his/her hair.
sem_t seatBelt;
// Flag to stop the driver thread when all customers have been serviced.
int allDone = 0;

int main(int argc, char *argv[])
{
    pthread_t btid;
    pthread_t tid[MAX_CUSTOMERS];
    cout<<"\t******************************\n";
    cout<<"\t*            Lazy           *\n";
    cout<<"\t*           Driver              *\n";
    cout<<"\t*    Cab Booking Problem      *\n";
    cout<<"\t******************************\n";
    int i, x,y, numCustomers, numChairs;
    int Number[MAX_CUSTOMERS];
    cout<<"Maximum number of customers in the cab can only be 7. Enter number of customers and seats in car.\n";
    cin>>x;
    numCustomers = x;
    cin>>y;
    numChairs = y;
    cout<<"\n"<<"Price per km is 10 plus variable cost after 5km";
//List to get Name, ID and phone number from the customer
    list <int> id1;
    list <string> nam1;
    list<string> ph;
    list<int> d1;
    for (int i = 0; i < x; ++i)
    {
        cout<<"For Customer: "<<i<<"\n";
        id1.push_back(t.getid());
        nam1.push_back(t.getnam());
        ph.push_back(t.getphon());
        d1.push_back(t.getD());

    }

    int sum=0;
     for (std::list<int>::iterator it=d1.begin(); it != d1.end(); ++it)
    {

        if((*it<5)||(numCustomers<2)){
        sum+=*it*10;
        bonus=0;
        }
        else if(((*it>5)&&(*it<10))||((numCustomers>=2)&&(numCustomers<=5))){
            sum+=(5*10)+((*it-5)*2);
        }
        else{
            sum+=(5*10)+((*it-5)*4);
            bonus=20;
        }
    }

    int cost ;
    cost =sum+bonus;
    cout<<"\nThe driver has made "<<cost<<" today\n";
    if (numCustomers > MAX_CUSTOMERS)
    {
        cout<<"The maximum number of Customers is "<<MAX_CUSTOMERS<<"\n";
        system("PAUSE");
        return 0;
    }
    cout<<"A solution to the sleeping taxi driver problem using semaphores."<<"\n";
    for (i = 0; i < MAX_CUSTOMERS; i++)
    {
        Number[i] = i;
    }
    cout<<"Assigning ID to Customers\n";
    // Initialize the semaphores with initial values...
    sem_init(&waitingRoom, 0, numChairs);
    sem_init(&taxiChair, 0, 1);
    sem_init(&taxiPillow, 0, 0);
    sem_init(&seatBelt, 0, 0);

    // Create the driver.
    pthread_create(&btid, NULL, taxi_driver, NULL);

    // Create the customers.
    for (i = 0; i < numCustomers; i++)
    {
        pthread_create(&tid[i], NULL, customer, (void *)&Number[i]);
    }
    // Join each of the threads to wait for them to finish.
    for (i = 0; i < numCustomers; i++)
    {
        pthread_join(tid[i],NULL);
    }
    // When all of the customers are finished, kill the driver thread.
    allDone = 1;
    sem_post(&taxiPillow); // Wake the Taxi Driver so he will exit.
    pthread_join(btid,NULL);
    system("PAUSE");
    return 0;
}

void *customer(void *number)
{
    int num = *(int *)number; // Leave for the cab and take some random amount of  time to arrive.
    cout<<"Customer "<<num<<" Booked a share cab ."<<"\n";
    randwait(5);
    cout<<"Customer "<<num<<" arrived at share cab"<<"\n"; // Wait for space to open up in the waitinng...
    sem_wait(&waitingRoom);
    cout<<"Customer "<<num<<" entering inside share cab.\n"; // Wait for the taxi seat to become free.
    sem_wait(&taxiChair); // The seat is free so give up your spot in the  waiting room.
    sem_post(&waitingRoom); // Wake up the driver...
    cout<<"Customer "<<num<<" waking up the cab driver."<<"\n";
    sem_post(&taxiPillow); // Wait for the driver to finish driving to your location.
    sem_wait(&seatBelt); // Give up the seat.
    sem_post(&taxiChair);
    cout<<"Customer "<<num<<" arrived at location."<<"\n";
}

void *taxi_driver(void *junk)
{
    while (!allDone)
    {
        cout<<"The cab driver is lazily driving\n";
        sem_wait(&taxiPillow); // Skip this stuff at the end...
        if (!allDone)
        {
            // Take a random amount of time to drive .
            cout<<"The cab driver is driving\n";
            randwait(3);
            cout<<"The cab driver has finished driving and arrived at location.\n"; // Release the customer when reaching location...
            sem_post(&seatBelt);
        }
        else
        {
            cout<<"The cab driver is going home for the day.\n";
        }
    }
}

void randwait(int secs)
{
    int len = 1; // Generate an arbit number...
    sleep(len);
}
