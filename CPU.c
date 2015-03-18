#include <stdio.h>
#include "CPUs.h"
#include "processQueue.h"


void* FCFScpu (void* param){

 sharedVars* shared = ((cpuParams*)param)->svars;
 int threadnum = ((cpuParams*)param)->threadNumber;
 process* p = NULL;

 while(1){

  sem_wait(&(shared->cpuSems[threadnum])); 
  printf("made it 1 \n"); 
  if(p == NULL){

   pthread_mutex_lock(&(shared->readyQLock));
   printf("made it 2 \n");
   if((shared->readyQ).head != NULL){
    int lowArrivalTime = 100;
    int index = 0;
    int lowDex = 0; 
    node* search = (shared->readyQ).head;  
    p = (shared->readyQ).head->data;

    while(search != NULL){
     
     printf("made it 3 \n");    
     if((search->data->arrivalTime) < (lowArrivalTime)){    
      printf("made it 4 \n");
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
     
     printf("made it 5 + index %d\n", index);
   }

      printf("made it 6 + lowdex %d\n", lowDex);
      p = Qremove((&(shared->readyQ)),lowDex); 
      printf("made it 7\n");  
   
  }  
   pthread_mutex_unlock(&(shared->readyQLock)); 
    printf("made it 8 \n");
   
    
  
  }
  
   printf("made it 9 \n");

  

  if(p != NULL){
   
   printf("burstRemaining %d\n", p->burstRemaining);
   p->burstRemaining = p->burstRemaining - 1;
 

  printf("made it 10 \n");
   
  if(p->burstRemaining==0){
   printf("Process %d has been scheduled\n", (p->PID));
   pthread_mutex_lock(&(shared->finishedQLock));
    printf("made it 11 \n");
    Qinsert(&(shared->finishedQ),p);    
   pthread_mutex_unlock(&(shared->finishedQLock));

   p = NULL;
  			
   }
  }
  sem_post(&(shared->mainSem));
  

  }
 
}

void* SJFcpu (void* param){
  sharedVars* shared = ((cpuParams*)param)->svars;
 int threadnum = ((cpuParams*)param)->threadNumber;
 process* p = NULL;

 while(1){

  sem_wait(&(shared->cpuSems[threadnum])); 
  printf("made it 1 \n"); 
  if(p == NULL){

   pthread_mutex_lock(&(shared->readyQLock));
   printf("made it 2 \n");
   if((shared->readyQ).head != NULL){
    int lowBurst = 1000;
    int index = 0;
    int lowDex = 0; 
    node* search = (shared->readyQ).head;  
    p = (shared->readyQ).head->data;

    while(search != NULL){
     
     printf("made it 3 \n");    
     if((search->data->burstTotal) < (lowBurst)){    
      printf("made it 4 \n");
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
     
     printf("made it 5 + index %d\n", index);
   }

      printf("made it 6 + lowdex %d\n", lowDex);
      p = Qremove((&(shared->readyQ)),lowDex); 
      printf("made it 7\n");  
   
  }  
   pthread_mutex_unlock(&(shared->readyQLock)); 
    printf("made it 8 \n");
   
    
  
  }
  
   printf("made it 9 \n");

  

  if(p != NULL){
   
   printf("burstRemaining %d\n", p->burstRemaining);
   p->burstRemaining = p->burstRemaining - 1;
 

  printf("made it 10 \n");
   
  if(p->burstRemaining==0){
   printf("Process %d has been scheduled\n", (p->PID));
   pthread_mutex_lock(&(shared->finishedQLock));
    printf("made it 11 \n");
    Qinsert(&(shared->finishedQ),p);    
   pthread_mutex_unlock(&(shared->finishedQLock));

   p = NULL;
  			
   }
  }
  sem_post(&(shared->mainSem));
  

  }
}

void* NPPcpu (void* param){

 sharedVars* shared = ((cpuParams*)param)->svars;
 int threadnum = ((cpuParams*)param)->threadNumber;
 process* p = NULL;

 while(1){

  sem_wait(&(shared->cpuSems[threadnum])); 
  printf("made it 1 \n"); 
  if(p == NULL){

   pthread_mutex_lock(&(shared->readyQLock));
   printf("made it 2 \n");
   if((shared->readyQ).head != NULL){
    int lowPriority = 100;
    int index = 0;
    int lowDex = 0; 
    node* search = (shared->readyQ).head;  
    p = (shared->readyQ).head->data;

    while(search != NULL){
     
     printf("made it 3 \n");    
     if((search->data->priority) < (lowPriority)){    
      printf("made it 4 \n");
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
     
     printf("made it 5 + index %d\n", index);
   }

      printf("made it 6 + lowdex %d\n", lowDex);
      p = Qremove((&(shared->readyQ)),lowDex); 
      printf("made it 7\n");  
   
  }  
   pthread_mutex_unlock(&(shared->readyQLock)); 
    printf("made it 8 \n");
   
    
  
  }
  
   printf("made it 9 \n");

  

  if(p != NULL){
   
   printf("burstRemaining %d\n", p->burstRemaining);
   p->burstRemaining = p->burstRemaining - 1;
 

  printf("made it 10 \n");
   
  if(p->burstRemaining==0){
   printf("Process %d has been scheduled\n", (p->PID));
   pthread_mutex_lock(&(shared->finishedQLock));
    printf("made it 11 \n");
    Qinsert(&(shared->finishedQ),p);    
   pthread_mutex_unlock(&(shared->finishedQLock));

   p = NULL;
  			
   }
  }
  sem_post(&(shared->mainSem));
  

  }
}

void* RRcpu (void* param){
sharedVars* shared = ((cpuParams*)param)->svars;
int threadnum = ((cpuParams*)param)->threadNumber;
int quant = ((cpuParams*)param)->svars->quantum;
int tracker = 0;
 
 process* p = NULL;

 while(1){

  sem_wait(&(shared->cpuSems[threadnum])); 
  printf("made it 1 + quantum %d\n", quant); 
  if(p == NULL){

   pthread_mutex_lock(&(shared->readyQLock));
   printf("made it 2 \n");
   if((shared->readyQ).head != NULL){
    int lowArrivalTime = 100;
    int index = 0;
    int lowDex = 0; 
    node* search = (shared->readyQ).head;  
    p = (shared->readyQ).head->data;

    while(search != NULL){
     
     printf("made it 3 \n");    
     if((search->data->arrivalTime) < (lowArrivalTime)){    
      printf("made it 4 \n");
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
     
     printf("made it 5 + index %d\n", index);
   }

      printf("made it 6 + lowdex %d\n", lowDex);
      p = Qremove((&(shared->readyQ)),lowDex); 
      printf("made it 7\n");  
   
  }  
   pthread_mutex_unlock(&(shared->readyQLock)); 
    printf("made it 8 \n");
   
    
  
  }
  
   printf("made it 9 \n");

  

  if(p != NULL){
   
   tracker++;
   printf("burstRemaining %d\n", p->burstRemaining);
   p->burstRemaining = p->burstRemaining - 1;


  printf("made it 10 \n");
   
  if(p->burstRemaining==0){
   printf("Process %d has been scheduled\n", (p->PID));
   pthread_mutex_lock(&(shared->finishedQLock));
    printf("made it 11 \n");
    Qinsert(&(shared->finishedQ),p);    
   pthread_mutex_unlock(&(shared->finishedQLock));

   p = NULL;
  			
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

void* SRTFcpu (void* param){

sharedVars* shared = ((cpuParams*)param)->svars;
 int threadnum = ((cpuParams*)param)->threadNumber;
 process* p = NULL;

 while(1){

  sem_wait(&(shared->cpuSems[threadnum])); 
  printf("made it 1 \n"); 
  if(p == NULL){

   pthread_mutex_lock(&(shared->readyQLock));
   printf("made it 2 \n");
   if((shared->readyQ).head != NULL){
    int lowBurstRemain = 100;
    int index = 0;
    int lowDex = 0; 
    node* search = (shared->readyQ).head;  
    p = (shared->readyQ).head->data;

    while(search != NULL){
     
     printf("made it 3 \n");    
     if((search->data->burstRemaining) < (lowBurstRemain)){    
      printf("made it 4 \n");
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
     
     printf("made it 5 + index %d\n", index);
   }

      printf("made it 6 + lowdex %d\n", lowDex);
      p = Qremove((&(shared->readyQ)),lowDex); 
      printf("made it 7\n");  
   
  }  
   pthread_mutex_unlock(&(shared->readyQLock)); 
    printf("made it 8 \n");
   
    
  
  }
  
   printf("made it 9 \n");

  

  if(p != NULL){
   
   printf("burstRemaining %d\n", p->burstRemaining);
   p->burstRemaining = p->burstRemaining - 1;
 

  printf("made it 10 \n");
   
  if(p->burstRemaining==0){
   printf("Process %d has been scheduled\n", (p->PID));
   pthread_mutex_lock(&(shared->finishedQLock));
    printf("made it 11 \n");
    Qinsert(&(shared->finishedQ),p);    
   pthread_mutex_unlock(&(shared->finishedQLock));

   p = NULL;
  			
   }
  }

  pthread_mutex_lock(&(shared->readyQLock));
  printf("made it 12 \n");

  if((shared->readyQ).head != NULL && p != NULL){

   if(((shared->readyQ).head->data->burstRemaining)  <   (p->burstRemaining)){
     printf("made it 13 \n");
     
     p->requeued = true;
     Qinsert(&(shared->readyQ),p);
    
    printf("made it 14 \n");
    p = NULL;   
    printf("made it 15 \n");
  }
}
  pthread_mutex_unlock(&(shared->readyQLock));

  sem_post(&(shared->mainSem));
  

  }
}

void* PPcpu (void* param){

sharedVars* shared = ((cpuParams*)param)->svars;
 int threadnum = ((cpuParams*)param)->threadNumber;
 process* p = NULL;

 while(1){

  sem_wait(&(shared->cpuSems[threadnum])); 
  printf("made it 1 \n"); 
  if(p == NULL){

   pthread_mutex_lock(&(shared->readyQLock));
   printf("made it 2 \n");
   if((shared->readyQ).head != NULL){
    int lowPriority = 100;
    int index = 0;
    int lowDex = 0; 
    node* search = (shared->readyQ).head;  
    p = (shared->readyQ).head->data;

    while(search != NULL){
     
     printf("made it 3 \n");    
     if((search->data->priority) < (lowPriority)){    
      printf("made it 4 \n");
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
     
     printf("made it 5 + index %d\n", index);
   }

      printf("made it 6 + lowdex %d\n", lowDex);
      p = Qremove((&(shared->readyQ)),lowDex); 
      printf("made it 7\n");  
   
  }  
   pthread_mutex_unlock(&(shared->readyQLock)); 
    printf("made it 8 \n");
   
    
  
  }
  
   printf("made it 9 \n");

  

  if(p != NULL){
   
   printf("burstRemaining %d\n", p->burstRemaining);
   p->burstRemaining = p->burstRemaining - 1;
 

  printf("made it 10 \n");
   
  if(p->burstRemaining==0){
   printf("Process %d has been scheduled\n", (p->PID));
   pthread_mutex_lock(&(shared->finishedQLock));
    printf("made it 11 \n");
    Qinsert(&(shared->finishedQ),p);    
   pthread_mutex_unlock(&(shared->finishedQLock));

   p = NULL;
  			
   }
  }

  pthread_mutex_lock(&(shared->readyQLock));
  printf("made it 12 \n");

  if((shared->readyQ).head != NULL && p != NULL){

   if(((shared->readyQ).head->data->priority)  <   (p->priority)){
     printf("made it 13 \n");
     
     p->requeued = true;
     Qinsert(&(shared->readyQ),p);
    
    printf("made it 14 \n");
    p = NULL;   
    printf("made it 15 \n");
  }
}
  pthread_mutex_unlock(&(shared->readyQLock));

  sem_post(&(shared->mainSem));
  

  }
}