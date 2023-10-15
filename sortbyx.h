// 这个头文件不要引用，只是一个工具的使用示例
#include <cstdio>
#include <cstdlib>

// 注意这里有函数，这个头文件不可以多次引用
struct tower
{
    int x, y;
};

int comparePoints(const void *a, const void *b)
{
    return ((tower *)a)->x - ((tower *)b)->x;
}

int main()
{
    int n;
    printf("%d", &n);
    tower *towers = new tower[n];
    for (int i = 0; i < n; i++)
    {
        scanf("%d %d", &towers[i].x, &towers[i].y);
    }
    qsort(towers, n, sizeof(tower), comparePoints);
    return 0;
}