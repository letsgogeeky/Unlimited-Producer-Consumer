/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: ramy
 * Title: Unlimited Space Producer/Consumer solution
 * Created on December 31, 2017, 9:31 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

const unsigned int ARRAY_SIZE = 1000000;
int data_arr [1000000];
int CurrentIndex = 0;
int ReadIndex = 0;
int lastGenerated = 0;

sem_t semaphore, delay;
int n = 0;


int Produce(){
    int val = 0;
    srand(time(NULL));
    val = rand() % 1000;
    while(val == lastGenerated)
        val = rand() % 1000;
    lastGenerated = val;
  
    return val;
     
}

void* Store(int val){
      
    FILE *f = fopen("/home/ramy/os/producer-consumer/produce.txt", "a");
    if(f == NULL) printf("failed to open the file");
    else
        fprintf(f, "%d\n", val);
    fclose(f);
    
    data_arr[CurrentIndex] = val;
    CurrentIndex++;
}

int Read(){
 
    FILE * prod = fopen("/home/ramy/os/producer-consumer/produce.txt", "r");
    int count = 0;
    if ( prod != NULL )
    {
        char line[5];  
        while (fgets(line, sizeof line, prod) != NULL)  
        {
            if (count == ReadIndex)
            {
                fclose(prod);
               
                //printf("%d\t",data_arr[ReadIndex]);
                //ReadIndex = ReadIndex + 1;
                
            }
            else
            {
                count++;
            }
        }
    }
    
     
    int val = data_arr[ReadIndex];
     ReadIndex = ReadIndex + 1;
     return val;
}
 
void* Consume(int val){
  
     FILE *f = fopen("/home/ramy/os/producer-consumer/result.txt", "a");
                if(f == NULL) printf("failed to open the file");
                else
                fprintf(f, "%d\t", val);
                fclose(f);
    printf("%d\t",val);
   
    
}
void * ProducerThread(){
    
    printf("Producer Thread Started!");
    int producedItems = 0;
    while(CurrentIndex < 10000){
        //if(CurrentIndex >= 10000) break;
        int val = Produce();
          sem_wait(&semaphore);
          //critical section
          Store(val);
          //End of critical
          
          n++;
          if(n == 1) sem_post(&delay);
          sem_post(&semaphore);
          
          if(CurrentIndex % 1000 == 0){
              printf("Hello, %d Iterations passed!", CurrentIndex);
          }
          producedItems++;
    }
    
    printf("Produced Items equals %d", producedItems);
    return NULL;
}

void * ConsumerThread(){
    printf("Consumer Thread Started!");
    int itemsConsumed = 0;
    while(ReadIndex < 10000){
        //if(ReadIndex >= 10000) break;
        if(n == 0) sem_wait(&delay);
       
        sem_wait(&semaphore);
        //Critical sections
        int val = Read();
        itemsConsumed++;
        n--;
        sem_post(&semaphore);
        Consume(val);
    }
    printf("Consumed Items is %d", itemsConsumed);
    return NULL;
}


void* MonitorThread(){
    while(1){
        //printf("CurrentIndex is %d", CurrentIndex);
        if(CurrentIndex - ReadIndex > 1000){
           // printf("The difference is: %d at %d Write and %d Read\n",
             //       CurrentIndex - ReadIndex, CurrentIndex, ReadIndex);
        }
        if(CurrentIndex == 10000){
            break;
        }
    }
}

int main(int argc, char** argv) {
  
     
    sem_init(&semaphore, 0, 0);
    sem_init(&delay, 0, 0);
    
     pthread_t t1, t2, monitor;
     sem_post(&semaphore);
     pthread_create(&t1,NULL, ProducerThread,NULL);
     pthread_create(&t2,NULL, ConsumerThread, NULL);
    // pthread_create(&monitor, NULL, MonitorThread, NULL);
     
    
     pthread_join(t1, NULL);
     pthread_join(t2, NULL);
     //pthread_join(monitor, NULL);
   
     
    return (EXIT_SUCCESS);
}
