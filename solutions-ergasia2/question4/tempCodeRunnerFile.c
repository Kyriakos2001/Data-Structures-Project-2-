#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Point
{
    float x, y;
    char *name; // City name
} Point;

typedef struct QuadTreeNode
{
    Point *point;
    float x, y;
    float width, height;
    struct QuadTreeNode *ne, *nw, *se, *sw;
} QuadTreeNode;

typedef struct QuadTree
{
    QuadTreeNode *root;
} QuadTree;

QuadTreeNode *initNode(float x, float y, float width, float height)
{
    QuadTreeNode *node = (QuadTreeNode *)malloc(sizeof(QuadTreeNode));
    node->x = x;
    node->y = y;
    node->width = width;
    node->height = height;
    node->ne = node->nw = node->se = node->sw = NULL;
    node->point = NULL;
    return node;
}

QuadTree *initQuadTree(float width, float height)
{
    QuadTree *tree = (QuadTree *)malloc(sizeof(QuadTree));
    tree->root = initNode(0, 0, width, height);
    return tree;
}

bool QuadTreeInsert(QuadTreeNode *node, Point *point)
{
    // Base case: if the point is out of the bounds of the node
    if (point->x < node->x || point->y < node->y || point->x >= node->x + node->width || point->y >= node->y + node->height)
        return false;

    // If there is space in this quadtree node and it does not contain a point, insert here
    if (node->point == NULL && node->ne == NULL && node->nw == NULL && node->se == NULL && node->sw == NULL)
    {
        node->point = point;
        return true;
    }

    // Otherwise, subdivide and insert the point into the appropriate quadrant
    if (node->ne == NULL)
    { // First subdivision
        float halfWidth = node->width / 2.0;
        float halfHeight = node->height / 2.0;
        node->ne = initNode(node->x + halfWidth, node->y, halfWidth, halfHeight);
        node->nw = initNode(node->x, node->y, halfWidth, halfHeight);
        node->se = initNode(node->x + halfWidth, node->y + halfHeight, halfWidth, halfHeight);
        node->sw = initNode(node->x, node->y + halfHeight, halfWidth, halfHeight);
    }

    // Determine correct quadrant and insert recursively
    if (point->x < node->x + node->width / 2)
    {
        if (point->y < node->y + node->height / 2)
            return QuadTreeInsert(node->nw, point);
        else
            return QuadTreeInsert(node->sw, point);
    }
    else
    {
        if (point->y < node->y + node->height / 2)
            return QuadTreeInsert(node->ne, point);
        else
            return QuadTreeInsert(node->se, point);
    }
}

Point *QuadTreeSearchPoint(QuadTreeNode *node, float x, float y)
{
    // Check bounds first
    if (x < node->x || y < node->y || x >= node->x + node->width || y >= node->y + node->height)
        return NULL;

    // Check if the current node contains the point
    if (node->point != NULL && node->point->x == x && node->point->y == y)
        return node->point;

    // Otherwise, search in the appropriate quadrant
    if (node->ne == NULL) // No children
        return NULL;

    if (x < node->x + node->width / 2)
    {
        if (y < node->y + node->height / 2)
            return QuadTreeSearchPoint(node->nw, x, y);
        else
            return QuadTreeSearchPoint(node->sw, x, y);
    }
    else
    {
        if (y < node->y + node->height / 2)
            return QuadTreeSearchPoint(node->ne, x, y);
        else
            return QuadTreeSearchPoint(node->se, x, y);
    }
}

void QuadTreeSearchWithinRadius(QuadTreeNode *node, float centerX, float centerY, float radius, Point *points[], int *count, int maxPoints)
{
    if (node == NULL || *count >= maxPoints)
        return;

    // Check if the current node's point is within the radius
    if (node->point != NULL)
    {
        float dx = centerX - node->point->x;
        float dy = centerY - node->point->y;
        if (dx * dx + dy * dy <= radius * radius)
        {
            points[*count] = node->point;
            (*count)++;
        }
    }

    // Check bounds for quadrant relevance
    if (node->ne == NULL) // No children
        return;

    // Recursively search in each quadrant
    QuadTreeSearchWithinRadius(node->nw, centerX, centerY, radius, points, count, maxPoints);
    QuadTreeSearchWithinRadius(node->ne, centerX, centerY, radius, points, count, maxPoints);
    QuadTreeSearchWithinRadius(node->sw, centerX, centerY, radius, points, count, maxPoints);
    QuadTreeSearchWithinRadius(node->se, centerX, centerY, radius, points, count, maxPoints);
}

void freeQuadTreeNode(QuadTreeNode *node)
{
    if (node == NULL)
        return;
    freeQuadTreeNode(node->ne);
    freeQuadTreeNode(node->nw);
    freeQuadTreeNode(node->se);
    freeQuadTreeNode(node->sw);
    free(node->point); 
    free(node);
}

void freeQuadTree(QuadTree *tree)
{
    if (tree == NULL)
        return;
    freeQuadTreeNode(tree->root);
    free(tree);
}

int main()
{
    // Initialize QuadTree with given dimensions
    QuadTree *qt = initQuadTree(100.0, 100.0);

    // Create some points
    Point *p1 = (Point *)malloc(sizeof(Point));
    p1->x = 25.0;
    p1->y = 30.0;
    p1->name = "City1";
    Point *p2 = (Point *)malloc(sizeof(Point));
    p2->x = 70.0;
    p2->y = 80.0;
    p2->name = "City2";
    Point *p3 = (Point *)malloc(sizeof(Point));
    p3->x = 50.0;
    p3->y = 50.0;
    p3->name = "City3";

    // Insert points into the QuadTree
    QuadTreeInsert(qt->root, p1);
    QuadTreeInsert(qt->root, p2);
    QuadTreeInsert(qt->root, p3);

    // Perform a search
    Point *searchResult = QuadTreeSearchPoint(qt->root, 70.0, 80.0);
    if (searchResult != NULL)
    {
        printf("Found: %s at (%.1f, %.1f)\n", searchResult->name, searchResult->x, searchResult->y);
    }
    else
    {
        printf("Not Found\n");
    }

    // Clean up
    freeQuadTree(qt);

    return 0;
}
