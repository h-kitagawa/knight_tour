// コンパイル方法
// cl /Ox /EHsc knights.cc (Visual Studio 2015用)
// c++ -std=c++11 -O2 -march=native knights.cc (gcc用)
#include <iostream>
#include <utility>
#include <list>

using namespace std;

const int N = 7; // チェス盤のサイズ
typedef pair<int,int> position;
typedef list<position> positions;
positions possible[N][N];

inline void remove(positions &l, const int x, const int y)
{
    for (positions::iterator curr = l.begin(); curr != l.end(); ++curr)
        if (curr->first == x && curr->second == y) {
            l.erase(curr);
            break;
        }
}

long long int nfounds = 0LL;

void search(const int x1, const int y1, const int n1,
            const int x2, const int y2, const int n2)
{
    // x1, y1は次に動かすナイトの位置，x2, y2は逆側のナイトの位置
    // n1はこちら側がどれだけ進んだか，n2は逆側からどれだけ進んだかを表す．
    // n1 + n2 < N*Nが常に成立する．
    // remove()したところだけsize()を見る．
    // size() == 1なら必ずそちらに進む．
    // その候補(uniques)が2つ以上あったら終わり．
    const positions &current = possible[x1][y1];
    int uniques = 0;
    int nx, ny; // next position
    for (const position &n: current) {
        positions &l = possible[n.first][n.second];
        remove(l, x1, y1);
        if (!(n.first == x2 && n.second == y2) && l.size() == 1) {
            uniques++;
            nx = n.first; ny = n.second;
        }
    }
    switch (uniques) {
    case 1:
        if (n1 + n2 == N*N-1)
            nfounds++;
        else
            search(x2, y2, n2, nx, ny, n1+1); // nx, nyは代入されている．
        break;
    case 0:
        for (const position &n: current) {
            nx = n.first; ny = n.second;
            if (!(nx == x2 && ny == y2))
                search(x2, y2, n2, nx, ny, n1+1);
        }
    }
    const position &pair = make_pair(x1, y1);
    for (const position &n: current)
        possible[n.first][n.second].push_front(pair);
}

int main()
{
    const position next_positions[8] = {
        make_pair(+2, +1), make_pair(+2, -1),
        make_pair(+1, +2), make_pair(+1, -2),
        make_pair(-1, +2), make_pair(-1, -2),
        make_pair(-2, +1), make_pair(-2, -1)
    };
    for (int x = 0; x < N; ++x)
        for (int y = 0; y < N; ++y) {
            for (int i = 0; i < 8; ++i) {
                const position d = next_positions[i];
                const int dx = d.first, dy = d.second;
                if (0 <= x+dx && x+dx < N && 0 <= y+dy && y+dy < N)
                    possible[x][y].push_front(make_pair(x+dx, y+dy));
            }
        }
    for (const position &n: possible[0][0])
        remove(possible[n.first][n.second], 0, 0);
    search(N-1, N-1, 1, 1, 2, 2);
    cout << nfounds*2 << endl;
    return 0;
}
