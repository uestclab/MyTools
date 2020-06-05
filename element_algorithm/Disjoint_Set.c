#include <stdio.h>
#include <stdlib.h>

#define MAXN 5005

int fa[MAXN]; // father 
int rank[MAXN];

void init(int n)
{
    for (int i = 1; i <= n; ++i)
    {
        fa[i] = i;
        rank[i] = 1;
    }
}

int find(int x) // 路径压缩
{
    // if(x == fa[x])
    //     return x;
    // else{
    //     fa[x] = find(fa[x]);  //父节点设为根节点
    //     return fa[x];         //返回父节点
    // }
    return x == fa[x] ? x : (fa[x] = find(fa[x]));
}

// inline void merge(int i, int j)
// {
//     fa[find(i)] = find(j);
// }

void merge(int i, int j)
{
    int x = find(i), y = find(j);    //先找到两个根节点
    if (rank[x] <= rank[y])
        fa[x] = y;
    else
        fa[y] = x;
    if (rank[x] == rank[y] && x!=y)
        rank[y]++;                   //如果深度相同且根节点不同，则新的根节点的深度+1
}

int main(void){
    int n, m, p, x, y;
    scanf("%d%d%d", &n, &m, &p);
    init(n);
    for (int i = 0; i < m; ++i)
    {
        scanf("%d%d", &x, &y);
        merge(x, y);
    }
    for (int i = 0; i < p; ++i)
    {
        scanf("%d%d", &x, &y);
        printf("%s\n", find(x) == find(y) ? "Yes" : "No");
    }
    return 0;
}