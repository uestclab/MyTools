#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXN 100
#define lowbit(x) ((x) & (-x))
int tree[MAXN];
void update(int i, int x)
{
    for (int pos = i; pos < MAXN; pos += lowbit(pos))
        tree[pos] += x;
}

int query_sum(int n)
{
    int ans = 0;
    for (int pos = n; pos; pos -= lowbit(pos))
        ans += tree[pos];
    return ans;
}

int query(int a, int b)
{
    return query_sum(b) - query_sum(a - 1);
}

void println(){
    printf("begin display ---------- \n");
    for(int i = 0;i<MAXN;i++){
        printf("%d,",tree[i]);
    }

    printf("end display ------------ \n");
}

void show(int i){
    printf("begin show : start pos = %d \n", i);
    for (int pos = i; pos < MAXN; pos += lowbit(pos)){
        printf("pos = %d \n", pos);
    }

}

int main(void){
    memset(tree, 0, sizeof(tree));
    int idx = 0;
    int x = 10;
    int tmp = lowbit(idx);
    show(1);
    update(idx,x);

    println();

    return 0;
}