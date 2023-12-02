#pragma once
#include "Vector.h"
#include <cstdlib>

struct Point
{
    double x, y;
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
private:
    Node *buildTree(Vector<Point> &points, int depth = 0)
    {
        if (points.empty())
            return nullptr;

        int axis = depth % 2;
        if (axis == 0)
        {
            qsort(&points[0], points.size(), sizeof(Point), compareX);
        }
        else
        {
            qsort(&points[0], points.size(), sizeof(Point), compareY);
        }

        int median = points.size() / 2;
        Node *node = new Node(points[median]);
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

    void rangeQuery(Node *node, Point lower, Point upper, Vector<Point> &result, int depth = 0)
    {
        if (!node)
            return;

        int axis = depth % 2;
        if (lower.x <= node->point.x && node->point.x <= upper.x &&
            lower.y <= node->point.y && node->point.y <= upper.y)
        {
            result.push_back(node->point);
        }

        if (axis == 0)
        {
            if (lower.x <= node->point.x)
                rangeQuery(node->left, lower, upper, result, depth + 1);
            if (upper.x >= node->point.x)
                rangeQuery(node->right, lower, upper, result, depth + 1);
        }
        else
        {
            if (lower.y <= node->point.y)
                rangeQuery(node->left, lower, upper, result, depth + 1);
            if (upper.y >= node->point.y)
                rangeQuery(node->right, lower, upper, result, depth + 1);
        }
    }
};
