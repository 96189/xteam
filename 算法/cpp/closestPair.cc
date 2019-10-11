
#include <cmath>
#include <cfloat>
#include <algorithm>

// closest pair
// 分支定界
struct Point2d
{
    int x_;
    int y_;
};

int compareX(const void *a, const void *b)
{
    Point2d *p1 = (Point2d *)a;
    Point2d *p2 = (Point2d *)b;
    return p1->x_ - p2->x_;
}

int compareY(const void *a, const void *b)
{
    Point2d *p1 = (Point2d *)a;
    Point2d *p2 = (Point2d *)b;
    return p1->y_ - p2->y_;
}
float dist(const Point2d &p1, const Point2d &p2)
{
    return sqrt((p1.x_-p2.x_)*(p1.x_-p2.x_) + (p1.y_-p2.y_)*(p1.y_-p2.y_));
}

float bruteForce(Point2d P[], int n)
{
    float min = FLT_MAX;
    for (int i = 0; i < n; ++i)
    {
        for (int j = i+1; j < n; ++j)
        {
            float currentDist = dist(P[i], P[j]);
            if (currentDist < min)
            {
                min = currentDist;
            }
        }
    }
    return min;
}

// 方法1: n * log(n) * log(n)
float stripClosest(Point2d strip[], int size, float d)
{
    float min = d;

    qsort(strip, size, sizeof(Point2d), compareY);

    for (int i = 0; i < size; ++i)
    {
        for (int j = i+1; j < size && (strip[j].y_-strip[i].y_) < min; ++j)
        {
            int newDist = dist(strip[i], strip[j]);
            if (newDist < min)
                min = newDist;
        }
    }
    return min;
}

float closestUtil(Point2d P[], int n)
{
    if (n <= 3)
        return bruteForce(P, n);
    
    int mid = n/2;
    Point2d midPoint = P[mid];

    float dl = closestUtil(P, mid);
    float dr = closestUtil(P+mid, n-mid);

    float d = std::min(dl, dr);

    Point2d strip[n];
    int j = 0;
    for (int i = 0; i < n; ++i)
    {
        if (abs(P[i].x_ - midPoint.x_) < d)
        {
            strip[j] = P[i];
            ++j;
        }
    }
    return std::min(d, stripClosest(strip, j, d));
}

float closest(Point2d P[], int n)
{
    qsort(P, n, sizeof(Point2d), compareX);
    return closestUtil(P, n);
}

// 方法2:n * log(n)
float stripClosest(Point2d strip[], int size, float d)
{
    float min = d;
    for (int i = 0; i < size; ++i)
    {
        for (int j = i+1; j < size && strip[j].y_ - strip[i].y_ < min; ++j)
        {
            int currentDist = dist(strip[i], strip[j]);
            if (currentDist < min)
                min = dist(strip[i], strip[j]);
        }
    }
    return min;
}

float closestUtil(Point2d Px[], Point2d Py[], int n)
{
    if (n <= 3)
        return bruteForce(Px, n);

    int mid = n/2;
    Point2d midPoint = Px[mid];

    Point2d Pyl[mid+1];
    Point2d Pyr[n-mid-1];
    int li = 0;
    int ri = 0;
    for (int i = 0; i < n; ++i)
    {
        if (Py[i].x_ <= midPoint.x_)
            Pyl[li++] = Py[i];
        else 
            Pyr[ri++] = Py[i];
    }

    float dl = closestUtil(Px, Pyl, mid);
    float dr = closestUtil(Px+mid, Pyr, n-mid);

    float d = std::min(dl, dr);

    Point2d strip[n];
    int j = 0;
    for (int i = 0; i < n; ++i)
    {
        if (abs(Py[i].x_ - midPoint.x_) < d)
        {
            strip[j] = Py[i];
            ++j;
        }
    }

    return std::min(d, stripClosest(strip, j, d));
}

float closest(Point2d P[], int n)
{
    Point2d Px[n];
    Point2d Py[n];
    for (int i = 0; i < n; ++i)
    {
        Px[i] = P[i];
        Py[i] = P[i];
    }

    qsort(Px, n, sizeof(Point2d), compareX);
    qsort(Py, n, sizeof(Point2d), compareY);

    return closestUtil(Px, Py, n);
}

