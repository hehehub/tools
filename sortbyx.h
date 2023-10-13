#include <cstdlib>

// 注意这里有函数，这个头文件不可以多次引用
struct tower
{
    long long x, y;
};

int comparePoints(const void *a, const void *b)
{
    struct tower *pointA = (struct tower *)a;
    struct tower *pointB = (struct tower *)b;

    if (pointA->x < pointB->x)
        return -1;
    if (pointA->x > pointB->x)
        return 1;
    return 0;
}

// main函数里面这么操作
// struct tower* towers = (struct tower*)malloc(n * sizeof(struct tower));// 存储所有灯塔坐标，这是创建数组，元素是二维数对
// …………
// qsort(towers, n, sizeof(struct tower), comparePoints);// 所有灯塔x坐标从小到大，这是qsort的用法