#include <stdio.h>
#include <stdlib.h>

typedef struct vector{
    int *data;
    int len;
    int start;
}vector;

int vector_pop(vector* v_data){
    if(v_data->len == 0 || v_data->data == NULL){
        return -1;
    }

    int data = v_data->data[v_data->start];
    v_data->len = v_data->len-1;
    v_data->start = v_data->start + 1;

    return data;
}

typedef struct TreeNode{
    int data;
    struct TreeNode* leftChild;
    struct TreeNode* rightChild;
}TreeNode;


TreeNode* createBinaryTree(vector *v_data){
    TreeNode* node = NULL;
    if(v_data->data == NULL || v_data->len == 0){
        return NULL;
    }

    int node_data = vector_pop(v_data);
    if(node_data != -1){
        node = (TreeNode*)malloc(sizeof(TreeNode));
        node->data = node_data;
        node->leftChild = createBinaryTree(v_data);
        node->rightChild = createBinaryTree(v_data);
    }
    return node;
}

/**
 * recursive preOrder
 * @param node  root of binary tree
 */
void preOrderTraveral(TreeNode* node){
    if(node == NULL){
        return;
    }
    printf("->%d\n",node->data);
    preOrderTraveral(node->leftChild);
    preOrderTraveral(node->rightChild);
}

/**
 * recursive inOrder
 * @param node  root of binary tree
 */
void inOrderTraveral(TreeNode* node){
    if(node == NULL){
        return;
    }
    inOrderTraveral(node->leftChild);
    printf("->%d\n",node->data);
    inOrderTraveral(node->rightChild);
}

/**
 * recursive postOrder
 * @param node  root of binary tree
 */
void postOrderTraveral(TreeNode* node){
    if(node == NULL){
        return;
    }
    postOrderTraveral(node->leftChild);
    postOrderTraveral(node->rightChild);
    printf("->%d\n",node->data);
}

/**
 * non-recursive preOrder : dfs
 * @param root  root of binary tree
 */
void preOrderTraveralWithStack(TreeNode* root){
    TreeNode* stack[1024];
    int idx = -1;
    TreeNode* node = root;
    while(node != NULL || idx != -1){
        while(node != NULL){
            printf("->%d\n",node->data);
            idx = idx + 1;
            stack[idx] = node;
            node = node->leftChild;
        }

        if(idx != -1){
            node = stack[idx];
            idx = idx - 1;
            node = node->rightChild;
        }
    }
}

/**
 * non-recursive preOrder_1 : stack FILO
 * @param root  root of binary tree
 */
void preOrderTraveralWithStack_1(TreeNode* root){
    if(root == NULL){
        return;
    }
    TreeNode* stack[1024];
    int idx = -1;
    TreeNode* node = root;
    idx = idx + 1;
    stack[idx] = root;
    while(idx != -1){
        node = stack[idx];
        idx = idx - 1;
        printf("->%d\n",node->data);
        if(node->rightChild != NULL){
            idx++;
            stack[idx] = node->rightChild;
        }
        if(node->leftChild != NULL){
            idx++;
            stack[idx] = node->leftChild;
        }
    }
}

/**
 * non-recursive inOrder : dfs
 * @param root  root of binary tree
 */
void inOrderTraveralWithStack(TreeNode* root){
    TreeNode* stack[1024];
    int idx = -1;
    TreeNode* node = root;
    while(node != NULL || idx != -1){
        while(node != NULL){
            //printf("->%d\n",node->data);
            idx = idx + 1;
            stack[idx] = node;
            node = node->leftChild;
        }

        if(idx != -1){
            node = stack[idx];
            idx = idx - 1;
            printf("->%d\n",node->data);
            node = node->rightChild;
        }
    }
}

/**
 * non-recursive postOrder : two stack
 * @param root  root of binary tree
 */
void postOrderTraveralWithStack(TreeNode* root){
    if(root == NULL){
        return;
    }

    TreeNode* stack_1[1024];
    TreeNode* stack_2[1024];
    int idx_1 = -1;
    int idx_2 = -1;
    TreeNode* node = root;

    idx_1++;
    stack_1[idx_1] = node;

    while(idx_1 != -1){
        node = stack_1[idx_1];
        idx_1--;
        idx_2++;
        stack_2[idx_2] = node;

        if(node->leftChild != NULL){
            idx_1++;
            stack_1[idx_1] = node->leftChild;
        }

        if(node->rightChild != NULL){
            idx_1++;
            stack_1[idx_1] = node->rightChild;
        }
    }

    while(idx_2 != -1){
        node = stack_2[idx_2];
        idx_2--;
        printf("->%d\n",node->data);
    }
}

/**
 * levelOrder : queue
 * @param root  root of binary tree
 */
void levelOrderTraversal(TreeNode* root){
    TreeNode* queue[1024];
    int head = 0;
    int tail = -1;
    int len = 0;
    tail++;
    queue[tail] = root;
    TreeNode* node = root;
    len++;
    while(len > 0){
        node = queue[head];
        head++;
        len--;
        printf("->%d\n",node->data);
        if(node->leftChild != NULL){
            tail++;
            queue[tail] = node->leftChild;
            len++;
        }
        if(node->rightChild != NULL){
            tail++;
            queue[tail] = node->rightChild;
            len++;
        }
    }

}

void main(void){
    printf("hello, binary tree \n"); 

    int init_data[11] = {3,2,9,-1,-1,10,-1,-1,8,-1,4};
    vector* v_data = (vector*)malloc(sizeof(vector));
    v_data->data = init_data;
    v_data->len  = 11;
    v_data->start = 0;
    TreeNode *treeNode = createBinaryTree(v_data);

    printf("preOrderTraveral : \n");
    preOrderTraveral(treeNode);

    printf("inOrderTraveral : \n");
    inOrderTraveral(treeNode);

    printf("postOrderTraveral : \n");
    postOrderTraveral(treeNode);


    printf("preOrderTraveralWithStack : \n");
    preOrderTraveralWithStack(treeNode);

    printf("preOrderTraveralWithStack_1 : \n");
    preOrderTraveralWithStack_1(treeNode);

    printf("inOrderTraveralWithStack : \n");
    inOrderTraveralWithStack(treeNode);

    printf("postOrderTraveralWithStack : \n");
    postOrderTraveralWithStack(treeNode);

    printf("levelOrderTraversal : \n");
    levelOrderTraversal(treeNode);
}