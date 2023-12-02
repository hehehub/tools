#pragma once
#include "Vector.h"
#include <algorithm>

struct Point
{
    double x, y;
};

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
            std::sort(points.begin(), points.end(), [](Point a, Point b)
                      { return a.x < b.x; });
        }
        else
        {
            std::sort(points.begin(), points.end(), [](Point a, Point b)
                      { return a.y < b.y; });
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
