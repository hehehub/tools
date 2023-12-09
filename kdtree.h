#pragma once
#include "Vector.h"

struct Point
{
    int x = 0, y = 0;
    int temp = 0;
    int count = 0;
    int sum = 0;
    int _left = 0, _right = 0, _up = 0, _down = 0;
};

int compareX(const void *a, const void *b)
{
    Point *pointA = (Point *)a;
    Point *pointB = (Point *)b;
    return (pointA->x > pointB->x) - (pointA->x < pointB->x);
}

int compareY(const void *a, const void *b)
{
    Point *pointA = (Point *)a;
    Point *pointB = (Point *)b;
    return (pointA->y > pointB->y) - (pointA->y < pointB->y);
}

struct Node
{
    Point point;
    Node *left, *right;
    Node(Point p) : point(p), left(nullptr), right(nullptr) {}
};

class KDTree
{
    int temperature_in_range = 0;
    int count_in_range = 0;

    Node *buildTree(Vector<Point> &points, int depth = 0)
    {
        if (points.empty())
            return nullptr;

        int axis = depth % 2;
        Vector<Point> temp = points;
        if (axis == 0)
        {
            qsort(&points[0], points.size(), sizeof(Point), compareX);
            qsort(&temp[0], temp.size(), sizeof(Point), compareY);
        }
        else
        {
            qsort(&points[0], points.size(), sizeof(Point), compareY);
            qsort(&temp[0], temp.size(), sizeof(Point), compareX);
        }

        int median = points.size() / 2;
        Node *node = new Node(points[median]);
        node->point.count = points.size();
        if (axis == 0)
        {
            node->point._left = points[0].x;
            node->point._right = points.back().x;
            node->point._down = temp[0].y;
            node->point._up = temp.back().y;
        }
        else
        {
            node->point._down = points[0].y;
            node->point._up = points.back().y;
            node->point._left = temp[0].x;
            node->point._right = temp.back().x;
        }
        for (Point &it : points)
        {
            node->point.sum += it.temp;
        }

        Vector<Point> leftPoints(points.begin(), points.begin() + median);
        Vector<Point> rightPoints(points.begin() + median + 1, points.end());

        node->left = buildTree(leftPoints, depth + 1);
        node->right = buildTree(rightPoints, depth + 1);

        return node;
    }

public:
    Node *root;

    KDTree(Vector<Point> &points)
    {
        root = buildTree(points);
    }

    void rangeQuery(Node *node, Point lower, Point upper, int depth = 0)
    {
        if (!node)
            return;

        int axis = depth % 2;
        if (lower.x <= node->point._left && node->point._right <= upper.x &&
            lower.y <= node->point._down && node->point._up <= upper.y)
        {
            temperature_in_range += node->point.sum;
            count_in_range += node->point.count;
        }
        else
        {
            if (lower.x <= node->point.x && node->point.x <= upper.x &&
                lower.y <= node->point.y && node->point.y <= upper.y)
            {
                temperature_in_range += node->point.temp;
                count_in_range++;
            }

            if (axis == 0)
            {
                if (lower.x <= node->point.x)
                    rangeQuery(node->left, lower, upper, depth + 1);
                if (upper.x >= node->point.x)
                    rangeQuery(node->right, lower, upper, depth + 1);
            }
            else
            {
                if (lower.y <= node->point.y)
                    rangeQuery(node->left, lower, upper, depth + 1);
                if (upper.y >= node->point.y)
                    rangeQuery(node->right, lower, upper, depth + 1);
            }
        }
    }

    int gettemperature()
    {
        return temperature_in_range;
    }

    int getcount()
    {
        return count_in_range;
    }

    void reset()
    {
        temperature_in_range = 0;
        count_in_range = 0;
    }
};