#include <iostream>
#include <ctime>
#include <cstdlib>

struct Point {
    int X;
    int Y;
};

typedef const int cnstInt;
using namespace std;

cnstInt rows = 15, cols = 45;
cnstInt lengthWais = 4;
const Point Start{ 0,0 }, End{ rows - 1,cols - 1 };
int Wall[rows][cols];

void Print(const Point* points, cnstInt length);
const Point* get_next_nodes(int& lengthExplored);
const Point* pathElements(const Point* explored, int& length);
bool check_next_node(cnstInt x, cnstInt y);
bool check_subsequent_blocks(const Point Explored);
bool TempSimilarityELemCheck(const Point* explored, cnstInt* wais, cnstInt count);
Point* MemoryFree(const Point* explored, cnstInt SizeExplored);


int main() {
    srand(time(NULL));

    // Initialize Walls randomly

    cnstInt percent_Wall_To_field = 4;//Max 15

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int n = rand() % 15;
            Wall[i][j] = (n <= percent_Wall_To_field) ? 1 : 0;
        }
    }
    Wall[Start.X][Start.Y] = 0;
    Wall[End.X][End.Y] = 0;

    int lengthExplored = 1;
    const Point* Explored = get_next_nodes(lengthExplored);
    const Point* path = pathElements(Explored, lengthExplored);

    // Mark the path in the Wall array
    for (int f = 0; f < lengthExplored; f++) {
        Wall[path[f].X][path[f].Y] = 2;
    }

    // Print the Wall array
    for (int i = 0; i < rows; i++) {
        for (int f = 0; f < cols; f++) {
            if (Wall[i][f] == 0)
            {
                cout << "  ";
            }
            else if (Wall[i][f] == 1) {
                cout << "# ";
            }
            else {
                cout << "O ";
            }
        }
        cout << endl;
    }
    cout << endl;

    cout << "Path< ";
    Print(path, lengthExplored);
    cout << endl;

    // Free allocated memory
    delete[] Explored;
    delete[] path;
}

void Print(const Point* points, cnstInt length) {
    for (int i = 0; i < length; i++) {
        cout << points[i].X << "." << points[i].Y << " || ";
    }
    cout << endl;
}

bool check_next_node(cnstInt x, cnstInt y) {
    return (x < 0 || y < 0 || x > rows - 1 || y > cols - 1 || Wall[x][y] == 1);
}

bool check_subsequent_blocks(const Point Explored) {
    return (Explored.X == Start.X && Explored.Y == Start.Y || Explored.X < 0);
}

bool TempSimilarityELemCheck(const Point* explored, cnstInt* wais, cnstInt count) {
    for (int x = count; explored[x].X >= 0; x++) {
        if (wais[0] == explored[x].X && wais[1] == explored[x].Y) {
            return false;
        }
    }
    return true;
}

Point* MemoryFree(const Point* explored, cnstInt SizeExplored) {
    Point* points = new Point[SizeExplored];
    for (int i = 0; explored[i].X >= 0; i++) {
        points[i].X = explored[i].X;
        points[i].Y = explored[i].Y;
    }
    delete[] explored;
    return points;
}

const Point* pathElements(const Point* Explored, int& length) {
    int pathIndex = 0, x = Start.X, y = Start.Y;
    Point* pathElements = new Point[rows * cols];

    for (int i = length - 1; i > -1; i--) {
        int wais[lengthWais][2]{ {x - 1,y},{x + 1,y},{x,y - 1},{x,y + 1} };
        for (int elem = 0; elem < lengthWais; elem++) {
            if (Explored[i].X == wais[elem][0] && Explored[i].Y == wais[elem][1]) {
                x = pathElements[pathIndex].X = Explored[i].X;
                y = pathElements[pathIndex].Y = Explored[i].Y;
                pathIndex++;
                break;
            }
        }
    }

    length = pathIndex;
    return MemoryFree(pathElements, pathIndex);
}


void explore_neighbors(const Point& currentPoint, Point* Explored, int& SizeExplored, int& count);

const Point* get_next_nodes(int& SizeExplored) {
    Point* Explored = new Point[rows * cols];
    int count = 0, Index = 0;

    Explored[0].X = End.X;
    Explored[0].Y = End.Y;

    while (true) {
        explore_neighbors(Explored[Index], Explored, SizeExplored, count);

        Index++;

        if ((Index > rows + 10) & (Index > cols + 10)) {//should explore neighbors
            count++;
        }

        if (check_subsequent_blocks(Explored[Index])) {
            return MemoryFree(Explored, SizeExplored);
        }
    }
}

void explore_neighbors(const Point& currentPoint, Point* Explored, int& SizeExplored, int& count) {
    int wais[lengthWais][2]{ {currentPoint.X - 1, currentPoint.Y}, {currentPoint.X + 1, currentPoint.Y},
                             {currentPoint.X, currentPoint.Y - 1}, {currentPoint.X, currentPoint.Y + 1} };

    for (int elem = 0; elem < lengthWais; elem++) {
        if (check_next_node(wais[elem][0], wais[elem][1]) || !TempSimilarityELemCheck(Explored, wais[elem], count)) {
            continue;
        }
        Explored[SizeExplored].X = wais[elem][0];
        Explored[SizeExplored].Y = wais[elem][1];
        SizeExplored++;
    }
}


