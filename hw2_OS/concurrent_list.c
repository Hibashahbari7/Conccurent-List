#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "concurrent_list.h"

typedef struct node {
  int value;
  struct node* next;
  // add more fields
  pthread_mutex_t lock; // mutex for node
}nodee;

struct list {
    nodee* head;
  // add fields
  pthread_mutex_t lock; //mutex for the list
};

void print_node(nodee* node)
{
  // DO NOT DELETE
  if(node)
  {
    printf("%d ", node->value);
  }
}

nodee* create_node(int value){
    nodee* newn =  (nodee*) malloc(sizeof(nodee));
    if(newn == NULL){
        return NULL;
    }
    newn->value = value;
    newn->next = NULL;
    pthread_mutex_init(&newn->lock, NULL);
    return newn;
}

list* create_list()
{
  // add code here
    list* newl = (list*) malloc(sizeof(list));
    if(newl == NULL){
        return NULL;
    }
    newl->head = NULL;
    pthread_mutex_init(&newl->lock, NULL);
    return newl;
  //return NULL; // REPLACE WITH YOUR OWN CODE
}

void delete_list(list* list)
{
  // add code here
  if(list == NULL){
      return;
  }
  pthread_mutex_lock(&list->lock);
  nodee* toDelete = list->head;
  while(toDelete != NULL){
      nodee* next = toDelete->next;
      pthread_mutex_destroy(&toDelete->lock);
      free(toDelete);
      toDelete = next;
  }
  pthread_mutex_unlock(&list->lock);
  pthread_mutex_destroy(&list->lock);
  free(list);
}

void insert_value(list* list, int value)
{
  // add code here
  if(list == NULL){
      return;
  }
  nodee* newn = create_node(value);
  if(newn == NULL){
      return;
  }
  pthread_mutex_lock(&list->lock);
  if((list->head == NULL) || (list->head->value > value)){
      newn->next = list->head;
      list->head = newn;
      pthread_mutex_unlock(&list->lock);
      return;
  }
  nodee* prev = list->head;
  pthread_mutex_lock(&prev->lock);
  pthread_mutex_unlock(&list->lock);
  nodee* curr = prev->next;
  while((curr != NULL) && (curr->value < value)){
      pthread_mutex_lock(&curr->lock);
      pthread_mutex_unlock(&prev->lock);
      prev = curr;
      curr = curr->next;
  }
  newn->next = curr;
  prev->next = newn;
  pthread_mutex_unlock(&prev->lock);
}

void remove_value(list* list, int value)
{
  // add code here
    if(list == NULL){
        return;
    }

    pthread_mutex_lock(&list->lock);
    if(list->head == NULL){
        pthread_mutex_unlock(&list->lock);
        return;
    }

    nodee* curr = list->head;
    pthread_mutex_lock(&curr->lock);
    pthread_mutex_unlock(&list->lock);
    if(list->head->value == value){
        list->head = curr->next;
        pthread_mutex_unlock(&curr->lock);
        pthread_mutex_destroy(&curr->lock);
        free(curr);
        return;
    }

    nodee* prev = curr;
    curr = prev->next;
    while(curr != NULL) {
        pthread_mutex_lock(&curr->lock);
        if (curr->value == value) {
            prev->next = curr->next;
            pthread_mutex_unlock(&prev->lock);
            pthread_mutex_unlock(&curr->lock);
            pthread_mutex_destroy(&curr->lock);
            free(curr);  //free the node
            return;
        }
        pthread_mutex_unlock(&prev->lock);
        prev = curr;
        curr = curr->next;
    }
    pthread_mutex_unlock(&prev->lock);
}

void print_list(list* list)
{
  // add code here
  if(list == NULL){
      printf("\n"); // DO NOT DELETE
      return;
  }

  pthread_mutex_lock(&list->lock);
  nodee* temp = list->head;
  if(temp != NULL){
      pthread_mutex_lock(&temp->lock);
  }
  pthread_mutex_unlock(&list->lock);

  while(temp != NULL){
      printf("%d ",temp->value);
      nodee* next = temp->next;
      if(next != NULL){
          pthread_mutex_lock(&next->lock);
      }
      pthread_mutex_unlock(&temp->lock);
      temp = next;
  }
  printf("\n"); // DO NOT DELETE
}

void count_list(list* list, int (*predicate)(int))
{
  int count = 0; // DO NOT DELETE

  // add code here

  if((list == NULL) || (predicate == NULL)) {
      printf("%d items were counted\n", count); // DO NOT DELETE
      return;
  }
      pthread_mutex_lock(&list->lock);
      nodee *temp = list->head;
      if (temp != NULL) {
          pthread_mutex_lock(&temp->lock);
      }
      pthread_mutex_unlock(&list->lock);
      while (temp != NULL) {
          if (predicate(temp->value) == 1) {
              count++;
          }
          nodee *next = temp->next;
          if (next != NULL) {
              pthread_mutex_lock(&next->lock);
          }
          pthread_mutex_unlock(&temp->lock);
          temp = next;
      }
  printf("%d items were counted\n", count); // DO NOT DELETE
}
