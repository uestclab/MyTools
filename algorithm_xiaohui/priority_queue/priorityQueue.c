#include <stdio.h>
#include <stdlib.h>

#define QUEUE_SIZE 5

typedef struct priorityQueue{
    int *data;
    int len;
    int size;
    int start;
}priorityQueue;

/**
 * resize queue
 * @param   queue   queue handler
 */
void resize(priorityQueue* queue){
    int new_size = queue->size * 2;
    int* new_data = (int*)malloc(sizeof(int)*new_size);

    for(int i=0;i<queue->len;i++){
        new_data[i] = queue->data[i];
    }

    free(queue->data);
    queue->data = new_data;
    queue->start = 0;
    queue->size = new_size;

    printf("resize() \n");
}


void print_array(priorityQueue* queue){
    if(queue->len == 0){
        printf("empty queue \n");
        return;
    }

    for(int i=queue->start;i<queue->len;i++){
        printf("-%d",queue->data[i]);
    }
    printf("\n");
}

/**
 * insert new data into queue
 * @param queue  priority queue handler
 */
 
void upAdjust(priorityQueue* queue){
    int childIndex = queue->len - 1;
    int parentIndex = (childIndex-1)/2;

    int temp = queue->data[childIndex];

    while(childIndex > 0 && temp < queue->data[parentIndex]){
        queue->data[childIndex] = queue->data[parentIndex];
        childIndex = parentIndex;
        parentIndex = (childIndex-1)/2;
    }

    queue->data[childIndex] = temp;
}
/**
 * downAdjust
 * @param queue    priority queue handler
 * @param parentIndex   parent node which need down adjust
 */
void downAdjust(priorityQueue* queue, int parentIndex){
    if(queue->len == 0 || queue->len == 1){
        return;
    }

    int temp = queue->data[parentIndex];

    int childIndex = 2*parentIndex+1;

    // printf("parentIndex = %d , childIndex = %d , temp = %d \n", parentIndex, childIndex, temp);

    while(childIndex < queue->len){
        if(childIndex+1<queue->len && queue->data[childIndex+1]<queue->data[childIndex]){
            childIndex = childIndex+1;
        }

        if(temp<=queue->data[childIndex]){
            break;
        }

        queue->data[parentIndex] = queue->data[childIndex];
        parentIndex = childIndex;
        childIndex = 2*parentIndex +1;
    }

    queue->data[parentIndex] =temp;

    // print_array(queue);

    // printf("end downAdjust \n");
}

/**
 * enqueue
 * @param   queue   queue handler
 */
void enPriorityQueue(int key, priorityQueue* queue){
    if(queue->size <= queue->len){
        resize(queue);
    }

    queue->data[queue->len] = key;
    queue->len++;

    upAdjust(queue);

    print_array(queue);
}

/**
 * dequeue
 * @param   queue   queue handler
 * @return  dequeue element : min element in queue
 */
int dePriorityQueue(priorityQueue* queue){
    if(queue->len == 0){
        print_array(queue);
        return -1;
    }

    int head = queue->data[0];
    queue->data[0] = queue->data[queue->len-1];
    queue->len = queue->len - 1;
    
    downAdjust(queue,0);

    print_array(queue);

    printf("min = %d \n", head);

    return  head; 
}

void main(void){
    priorityQueue* queue = (priorityQueue*)malloc(sizeof(priorityQueue));
    queue->data = (int*)malloc(sizeof(int)*QUEUE_SIZE);
    queue->len  = 0;
    queue->size = QUEUE_SIZE;
    queue->start = 0;
    
    printf("original queue \n");
    print_array(queue);

    enPriorityQueue(3,queue);
    enPriorityQueue(5,queue);
    enPriorityQueue(10,queue);
    enPriorityQueue(2,queue);
    enPriorityQueue(7,queue);
    enPriorityQueue(8,queue);
    
    dePriorityQueue(queue);
    dePriorityQueue(queue);
    dePriorityQueue(queue);
    dePriorityQueue(queue);

    enPriorityQueue(9,queue);
    enPriorityQueue(1,queue);
    enPriorityQueue(4,queue);
    enPriorityQueue(6,queue);

    dePriorityQueue(queue);
    dePriorityQueue(queue);
    dePriorityQueue(queue);
    dePriorityQueue(queue);
    dePriorityQueue(queue);
    dePriorityQueue(queue);
    dePriorityQueue(queue);

}

