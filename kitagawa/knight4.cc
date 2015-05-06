// 長尾さんの nagao.cc を小改変したもの
#include <iostream>
#include <utility>
#include <list>

using namespace std;

#ifndef SIZE
#define SIZE 7
#endif
typedef pair<int,int> position;
typedef list<int> positions;
positions possible[SIZE*SIZE];

#define PAIR(x,y) ((x)*SIZE + (y))

inline void remove(positions &l, const int p)
{
    for (positions::iterator curr = l.begin(); curr != l.end(); ++curr)
        if (*curr == p) {
            l.erase(curr);
            break;
        }
}

long long int nfounds = 0LL;

void search(const int p1, const int n1,
            const int p2, const int n2)
{
    const positions &current = possible[p1];
    int uniques = 0;
    int np; // next position
    for (const int &n: current) {
        positions &l = possible[n];
        remove(l, p1);
        if (n!=p2 && l.size() == 1) {
            uniques++;
            np=n;
        }
    }
    switch (uniques) {
    case 1:
        if (n1 + n2 == SIZE*SIZE-1)
            nfounds++;
        else
            search(p2, n2, np, n1+1);
        break;
    case 0:
        for (const int &n: current) {
            if ( n!=p2 )
                search(p2, n2, n, n1+1);
        }
    }
    for (const int &n: current)
        possible[n].push_front(p1);
}

int main()
{
    const position next_positions[8] = {
        make_pair(+2, +1), make_pair(+2, -1),
        make_pair(+1, +2), make_pair(+1, -2),
        make_pair(-1, +2), make_pair(-1, -2),
        make_pair(-2, +1), make_pair(-2, -1)
    };
    for (int x = 0; x < SIZE; ++x)
        for (int y = 0; y < SIZE; ++y) {
            for (int i = 0; i < 8; ++i) {
                const position d = next_positions[i];
                const int dx = d.first, dy = d.second;
                if ( (unsigned)(x+dx)<SIZE && (unsigned)(y+dy)<SIZE)
                    possible[PAIR(x,y)].push_front( PAIR(x+dx, y+dy) );
            }
        }
    for (const int &n: possible[0]) remove(possible[n], 0);
    search(PAIR(SIZE-1, SIZE-1), 1, PAIR(1, 2), 2);
    cout << nfounds*2 << endl;
    return 0;
}
