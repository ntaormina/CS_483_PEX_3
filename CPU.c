#include <stdio.h>
#include "CPUs.h"
#include "processQueue.h"

//C1C Nik Taormina
//Documentation: The only help I recieved was the suggestion to use a helper node to traverse the queue by C1C Miller
// I was advised that changing the head node to next was not traversing but actually changing the head
// I also recieved conceptual help from major Hancock on preemption and the idea to search through the queue

//First Come First Served: This was the first part I implemented and all of the others are based on this one so I'll explain everything
//in this one
void* FCFScpu (void* param){

 sharedVars* shared = ((cpuParams*)param)->svars;
 int threadnum = ((cpuParams*)param)->threadNumber;
 process* p = NULL;//helper process to only access queues in mutex locks

 while(1){

  sem_wait(&(shared->cpuSems[threadnum])); //wait until thread is signaled
 
  if(p == NULL){

   pthread_mutex_lock(&(shared->readyQLock));//enter critical section
  
   if((shared->readyQ).head != NULL){

    int lowArrivalTime = 1000;
    int index = 0;
    int lowDex = 0; 
  
    node* search = (shared->readyQ).head;  //helper node to traverse or "search" the ready queue
    p = (shared->readyQ).head->data;

    while(search != NULL){     
        
     if((search->data->arrivalTime) < (lowArrivalTime)){   //if arrival time is less than the lowest one 
      
      lowArrivalTime = ((search->data->arrivalTime)); //new low arrival
      p = search->data; //p set to soonest arrival process 
      lowDex = index;

     }

     if(search->next != NULL){
      search = search->next;
     }else{
      search = NULL;
     }
     index++;   
     
    }

   p = Qremove((&(shared->readyQ)),lowDex);      //remove process from ready queue  
   printf("Process %d has been scheduled\n", (p->PID));
  } 
 
  pthread_mutex_unlock(&(shared->readyQLock)); //exit critical section
   
 }
  
 if(p != NULL){
   
 
  p->burstRemaining = p->burstRemaining - 1;// simulates running process
 

  if(p->burstRemaining==0){
   
   pthread_mutex_lock(&(shared->finishedQLock));
    Qinsert(&(shared->finishedQ),p);    //add finished process to finished queue
   pthread_mutex_unlock(&(shared->finishedQLock));

   p = NULL;
  			
   }
  }
  sem_post(&(shared->mainSem));//signal main thread
  

  }
 
}
//Shortest Job First: I implemented this the same as FCFS but based on burst Total
void* SJFcpu (void* param){
 sharedVars* shared = ((cpuParams*)param)->svars;
 int threadnum = ((cpuParams*)param)->threadNumber;
 process* p = NULL;

 while(1){

  sem_wait(&(shared->cpuSems[threadnum])); 
   
  if(p == NULL){

   pthread_mutex_lock(&(shared->readyQLock));
  
   if((shared->readyQ).head != NULL){
    int lowBurst = 1000;
    int index = 0;
    int lowDex = 0; 
    node* search = (shared->readyQ).head;  
    p = (shared->readyQ).head->data;

    while(search != NULL){
     
        
     if((search->data->burstTotal) < (lowBurst)){    
     
      lowBurst = ((search->data->burstTotal));
      p = search->data;
      lowDex = index;
     }
     if(search->next != NULL){
      search = search->next;
     }else{
      search = NULL;
     }
         index++;   
     
     
   }     
      p = Qremove((&(shared->readyQ)),lowDex); 
      printf("Process %d has been scheduled\n", (p->PID));  
  }  
   pthread_mutex_unlock(&(shared->readyQLock)); 
     
  }
  
   
  if(p != NULL){
   
   
   p->burstRemaining = p->burstRemaining - 1;
 

   if(p->burstRemaining==0){
   
    pthread_mutex_lock(&(shared->finishedQLock));
     Qinsert(&(shared->finishedQ),p);    
    pthread_mutex_unlock(&(shared->finishedQLock));

   p = NULL;
  			
   }
  }
  sem_post(&(shared->mainSem));
  

  }
}
//Non Preemptive Priority: I implemented this the same as FCFS but based on priority
void* NPPcpu (void* param){

 sharedVars* shared = ((cpuParams*)param)->svars;
 int threadnum = ((cpuParams*)param)->threadNumber;
 process* p = NULL;

 while(1){

  sem_wait(&(shared->cpuSems[threadnum])); 
  
  if(p == NULL){

   pthread_mutex_lock(&(shared->readyQLock));
   
   if((shared->readyQ).head != NULL){
    int lowPriority = 100;
    int index = 0;
    int lowDex = 0; 
    node* search = (shared->readyQ).head;  
    p = (shared->readyQ).head->data;

    while(search != NULL){
     
        
     if((search->data->priority) < (lowPriority)){    
      
      lowPriority = ((search->data->priority));
      p = search->data;
      lowDex = index;
     }
     if(search->next != NULL){
      search = search->next;
     }else{
      search = NULL;
     }
         index++;   
     
     
   }

      
      p = Qremove((&(shared->readyQ)),lowDex); 
      printf("Process %d has been scheduled\n", (p->PID)); 
   
  }  
   pthread_mutex_unlock(&(shared->readyQLock)); 
     
  }  

  if(p != NULL){   
   
   p->burstRemaining = p->burstRemaining - 1; 

   if(p->burstRemaining==0){
   
    pthread_mutex_lock(&(shared->finishedQLock));
     Qinsert(&(shared->finishedQ),p);    
    pthread_mutex_unlock(&(shared->finishedQLock));

    p = NULL;
  			
   }
  }
  sem_post(&(shared->mainSem));
  

  }
}


//Round Robin: I implemented this as the same as FCFS but with a quantum
void* RRcpu (void* param){

sharedVars* shared = ((cpuParams*)param)->svars;
int threadnum = ((cpuParams*)param)->threadNumber;
int quant = ((cpuParams*)param)->svars->quantum;
int tracker = 0;
 
 process* p = NULL;

 while(1){

  sem_wait(&(shared->cpuSems[threadnum])); 
   
  if(p == NULL){

   pthread_mutex_lock(&(shared->readyQLock));
   
   if((shared->readyQ).head != NULL){
    int lowArrivalTime = 100;
    int index = 0;
    int lowDex = 0; 
    node* search = (shared->readyQ).head;  
    p = (shared->readyQ).head->data;

    while(search != NULL){
        
     if((search->data->arrivalTime) < (lowArrivalTime)){    
      
      lowArrivalTime = ((search->data->arrivalTime));
      p = search->data;
      lowDex = index;

     }
     if(search->next != NULL){
      search = search->next;
     }else{
      search = NULL;
     }
     index++;   
    
   }

      p = Qremove((&(shared->readyQ)),lowDex); 
      printf("Process %d has been scheduled\n", (p->PID));   
   
  }  
   pthread_mutex_unlock(&(shared->readyQLock));    
  
  }
  
  if(p != NULL){
   
   tracker++;
   p->burstRemaining = p->burstRemaining - 1;

   
  if(p->burstRemaining==0){

   pthread_mutex_lock(&(shared->finishedQLock));
    Qinsert(&(shared->finishedQ),p);    
   pthread_mutex_unlock(&(shared->finishedQLock));

   p = NULL;
   tracker = 0;			
   }
  }
   
   if(tracker == quant && p != NULL){
    p->requeued = true;
 
    pthread_mutex_lock(&(shared->readyQLock)); 
     Qinsert(&(shared->readyQ),p);
    pthread_mutex_unlock(&(shared->readyQLock));
    
    p = NULL;
    tracker = 0;

  }

  sem_post(&(shared->mainSem));
  

  }
}
//Shortest Remaining Time First: preemptive version of shortest job first 
void* SRTFcpu (void* param){

sharedVars* shared = ((cpuParams*)param)->svars;
 int threadnum = ((cpuParams*)param)->threadNumber;
 process* p = NULL;

 while(1){

  sem_wait(&(shared->cpuSems[threadnum])); 
   
  if(p == NULL){

   pthread_mutex_lock(&(shared->readyQLock));
  
   if((shared->readyQ).head != NULL){
    int lowBurstRemain = 100;
    int index = 0;
    int lowDex = 0; 
    node* search = (shared->readyQ).head;  
    p = (shared->readyQ).head->data;

    while(search != NULL){
        
     if((search->data->burstRemaining) < (lowBurstRemain)){    
     
      lowBurstRemain = ((search->data->burstRemaining));
      p = search->data;
      lowDex = index;
     }
     if(search->next != NULL){
      search = search->next;
     }else{
      search = NULL;
     }
     index++;   
     
   }

      p = Qremove((&(shared->readyQ)),lowDex); 
      printf("Process %d has been scheduled\n", (p->PID)); 
   
  }  
   pthread_mutex_unlock(&(shared->readyQLock)); 
    
  }

  if(p != NULL){
   
 
   p->burstRemaining = p->burstRemaining - 1;
 
   
  if(p->burstRemaining==0){
 
   pthread_mutex_lock(&(shared->finishedQLock));
    Qinsert(&(shared->finishedQ),p);    
   pthread_mutex_unlock(&(shared->finishedQLock));

   p = NULL;
  			
   }
  }

  pthread_mutex_lock(&(shared->readyQLock));
  
  if((shared->readyQ).head != NULL && p != NULL){

   if(((shared->readyQ).head->data->burstRemaining)  <   (p->burstRemaining)){//searches every step for a shorter process than the current
     
     p->requeued = true;
     Qinsert(&(shared->readyQ),p);
    
     p = NULL;   
  }
}
  pthread_mutex_unlock(&(shared->readyQLock));

  sem_post(&(shared->mainSem));
  

  }
}
//Preemptive Priority: same as SRTF but based on priority
void* PPcpu (void* param){

 sharedVars* shared = ((cpuParams*)param)->svars;
 int threadnum = ((cpuParams*)param)->threadNumber;
 process* p = NULL;

 while(1){

  sem_wait(&(shared->cpuSems[threadnum])); 
   
  if(p == NULL){

   pthread_mutex_lock(&(shared->readyQLock));
  
   if((shared->readyQ).head != NULL){
    int lowPriority = 100;
    int index = 0;
    int lowDex = 0; 
    node* search = (shared->readyQ).head;  
    p = (shared->readyQ).head->data;

    while(search != NULL){
         
     if((search->data->priority) < (lowPriority)){    
      
      lowPriority = ((search->data->priority));
      p = search->data;
      lowDex = index;
     }
     if(search->next != NULL){
      search = search->next;
     }else{
      search = NULL;
     }
         index++;   
     
     
   }

      p = Qremove((&(shared->readyQ)),lowDex); 
      printf("Process %d has been scheduled\n", (p->PID));
   
  }  
   pthread_mutex_unlock(&(shared->readyQLock));  
  
  }

  if(p != NULL){
  
   p->burstRemaining = p->burstRemaining - 1;
   
  if(p->burstRemaining==0){
   
   pthread_mutex_lock(&(shared->finishedQLock));
   
    Qinsert(&(shared->finishedQ),p);    
   pthread_mutex_unlock(&(shared->finishedQLock));

   p = NULL;
  			
   }
  }

  pthread_mutex_lock(&(shared->readyQLock));

  if((shared->readyQ).head != NULL && p != NULL){

   if(((shared->readyQ).head->data->priority)  <   (p->priority)){
 
     p->requeued = true;
     Qinsert(&(shared->readyQ),p);
 
    p = NULL;   
   
  }
}
  pthread_mutex_unlock(&(shared->readyQLock));

  sem_post(&(shared->mainSem));  

  }
}