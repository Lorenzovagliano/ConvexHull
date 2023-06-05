#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cmath>

using namespace std;

struct Point {
    int x;
    int y;
};

class ConvexHull {
public:
    ConvexHull(const vector<Point>& points) : points(points) {}

    void mergeSort(vector<Point>& points, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSort(points, left, mid);
            mergeSort(points, mid + 1, right);
            merge(points, left, mid, right);
        }
    }

    void insertionSort(vector<Point>& points) {
        int n = points.size();
        for (int i = 1; i < n; i++) {
            Point key = points[i];
            int j = i - 1;
            while (j >= 0 && comparePoints(points[j], key) > 0) {
                points[j + 1] = points[j];
                j--;
            }
            points[j + 1] = key;
        }
    }

    void radixSort(vector<Point>& points) {
        int n = points.size();
        int maxVal = getMaxValue(points);

        for (int exp = 1; maxVal / exp > 0; exp *= 10)
            countSort(points, exp);
    }

    void getConvexHull() {
        convexHull.clear();

        int n = points.size();
        if (n < 3)
            return;

        int leftmost = 0;
        for (int i = 1; i < n; i++) {
            if (points[i].x < points[leftmost].x)
                leftmost = i;
        }

        int p = leftmost, q;
        do {
            convexHull.push_back(points[p]);
            q = (p + 1) % n;
            for (int i = 0; i < n; i++) {
                if (orientation(points[p], points[i], points[q]) == 2)
                    q = i;
            }
            p = q;
        } while (p != leftmost);

        cout << "FECHO CONVEXO:\n";
        for (const auto& point : convexHull)
            cout << point.x << " " << point.y << endl;
        cout << endl;
    }

private:
    vector<Point> points;
    vector<Point> convexHull;

    void merge(vector<Point>& points, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        vector<Point> leftPoints(n1);
        vector<Point> rightPoints(n2);

        for (int i = 0; i < n1; i++)
            leftPoints[i] = points[left + i];
        for (int j = 0; j < n2; j++)
            rightPoints[j] = points[mid + 1 + j];

        int i = 0, j = 0, k = left;

        while (i < n1 && j < n2) {
            if (comparePoints(leftPoints[i], rightPoints[j]) <= 0) {
                points[k] = leftPoints[i];
                i++;
            }
            else {
                points[k] = rightPoints[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            points[k] = leftPoints[i];
            i++;
            k++;
        }

        while (j < n2) {
            points[k] = rightPoints[j];
            j++;
            k++;
        }
    }

    int comparePoints(const Point& p1, const Point& p2) {
        if (p1.x < p2.x)
            return -1;
        if (p1.x > p2.x)
            return 1;
        if (p1.y < p2.y)
            return -1;
        if (p1.y > p2.y)
            return 1;
        return 0;
    }

    int orientation(const Point& p, const Point& q, const Point& r) {
        int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

        if (val == 0)
            return 0;
        return (val > 0) ? 1 : 2;
    }

    int getMaxValue(const vector<Point>& points) {
        int maxVal = 0;
        for (const auto& point : points) {
            maxVal = max(maxVal, max(point.x, point.y));
        }
        return maxVal;
    }

    void countSort(vector<Point>& points, int exp) {
        int n = points.size();
        vector<Point> output(n);
        int count[10] = { 0 };

        for (int i = 0; i < n; i++)
            count[(points[i].x / exp) % 10]++;

        for (int i = 1; i < 10; i++)
            count[i] += count[i - 1];

        for (int i = n - 1; i >= 0; i--) {
            output[count[(points[i].x / exp) % 10] - 1] = points[i];
            count[(points[i].x / exp) % 10]--;
        }

        for (int i = 0; i < n; i++)
            points[i] = output[i];
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: ./convexhull input.txt\n";
        return 1;
    }

    string filename = argv[1];
    ifstream inputFile(filename);

    if (!inputFile) {
        cout << "Failed to open the input file.\n";
        return 1;
    }

    vector<Point> points;
    int x, y;

    while (inputFile >> x >> y) {
        points.push_back({ x, y });
    }

    inputFile.close();

    // Run Graham + MergeSort
    ConvexHull convexHull(points);

    auto start = chrono::high_resolution_clock::now();
    convexHull.mergeSort(points, 0, points.size() - 1);
    convexHull.getConvexHull();
    auto end = chrono::high_resolution_clock::now();
    double duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "GRAHAM+MERGESORT: " << duration / 1000.0 << "s\n";

    // Run Graham + InsertionSort
    start = chrono::high_resolution_clock::now();
    convexHull.insertionSort(points);
    convexHull.getConvexHull();
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "GRAHAM+INSERTIONSORT: " << duration / 1000.0 << "s\n";

    // Run Graham + RadixSort
    start = chrono::high_resolution_clock::now();
    convexHull.radixSort(points);
    convexHull.getConvexHull();
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "GRAHAM+RADIXSORT: " << duration / 1000.0 << "s\n";

    // Run Jarvis
    start = chrono::high_resolution_clock::now();
    convexHull.getConvexHull();
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "JARVIS: " << duration / 1000.0 << "s\n";

    return 0;
}
