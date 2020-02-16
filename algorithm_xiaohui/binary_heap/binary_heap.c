#include <stdio.h>
#include <stdlib.h>

typedef struct vector{
    int *data;
    int len;
    int start;
}vector;


void print_array(vector* v_data){
    for(int i=v_data->start;i<v_data->len;i++){
        printf("-%d",v_data->data[i]);
    }
    printf("\n");
}

/**
 * insert new data
 * @param v_data  heap in array form
 */
 
void upAdjust(vector* v_data){
    int childIndex = v_data->len - 1;
    int parentIndex = (childIndex-1)/2;

    int temp = v_data->data[childIndex];

    while(childIndex > 0 && temp < v_data->data[parentIndex]){
        v_data->data[childIndex] = v_data->data[parentIndex];
        childIndex = parentIndex;
        parentIndex = (childIndex-1)/2;
    }

    v_data->data[childIndex] = temp;
}
/**
 * downAdjust
 * @param v_data    heap which need adjust
 * @param parentIndex   parent node which need down adjust
 */
void downAdjust(vector* v_data, int parentIndex){
    int temp = v_data->data[parentIndex];

    int childIndex = 2*parentIndex+1;

    // printf("parentIndex = %d , childIndex = %d , temp = %d \n", parentIndex, childIndex, temp);

    while(childIndex < v_data->len){
        if(childIndex+1<v_data->len && v_data->data[childIndex+1]<v_data->data[childIndex]){
            childIndex = childIndex+1;
        }

        if(temp<=v_data->data[childIndex]){
            break;
        }

        v_data->data[parentIndex] = v_data->data[childIndex];
        parentIndex = childIndex;
        childIndex = 2*parentIndex +1;
    }

    v_data->data[parentIndex] =temp;

    // print_array(v_data);

    // printf("end downAdjust \n");
}

/**
 * 1. delete first element ; 2. last element fill in first position; 3. down adjust first element
 * @param v_data    heap which need delete and down adjust
 */
int deleteHeapNode(vector* v_data){
    if(v_data->len <= 0){
        return -1;
    }

    int temp = v_data->data[v_data->start];
    v_data->data[v_data->start] = v_data->data[v_data->len-1];
    v_data->len = v_data->len - 1;

    downAdjust(v_data,v_data->start);

    return temp;
}

/**
 * build heap
 * @param v_data  heap which need adjust
*/
void buildHeap(vector* v_data){
    int last_leaf = v_data->len - 1;
    int last_non_leaf = (last_leaf - 1)/2;

    for(int i = last_non_leaf;i>=0;i--){
        downAdjust(v_data,i);
    }
}

void main(void){
    int array[10] = {1,3,2,6,5,7,8,9,10,0};

    vector* v_data = (vector*)malloc(sizeof(vector));
    v_data->data = array;
    v_data->len  = 10;
    v_data->start = 0;
    
    printf("original heap \n");
    print_array(v_data);

    upAdjust(v_data);
    printf("upAdjust heap \n");
    print_array(v_data);


    int new_array[9] = {7,1,3,10,5,2,8,9,6};

    v_data->data = new_array;
    v_data->len  = 9;
    v_data->start = 0;

    printf("original heap \n");
    print_array(v_data);

    buildHeap(v_data);

    printf("build heap \n");
    print_array(v_data);

    printf("start delete heap \n");
    for(int i = 0;i<9;i++){
        printf("pop data : %d \n", deleteHeapNode(v_data));
    }
}

