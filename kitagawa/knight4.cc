// 長尾さんの nagao.cc を小改変したもの
// ./knight4_7  9090.30s user 0.53s system 301% cpu 50:11.98 total
#include <pthread.h>
#include <stdio.h>
#include <set>

using namespace std;

#ifndef SIZE
#define SIZE 7
#endif

typedef unsigned long long int ULL;
typedef set<int> positions;
positions possible_pool[5][SIZE*SIZE];

#define PAIR(x,y) ((x)*SIZE + (y))

void search(const int p1, const int n1,
            const int p2, const int n2, ULL* sum, positions possible[])
{
    const positions &current = possible[p1];
    int uniques = 0;
    int np; // next position
    for (const int &n: current) {
        positions &l = possible[n];
        l.erase(p1);
        if (n!=p2 && l.size() == 1) {
            uniques++;
            np=n;
        }
    }
    switch (uniques) {
    case 1:
        if (n1 + n2 == SIZE*SIZE-1) 
            {(*sum)++; if (!((*sum)&0xffff)) printf("%lld\n", *sum); }
        else
            search(p2, n2, np, n1+1, sum, possible);
        break;
    case 0:
        for (const int &n: current) {
            if ( n!=p2 )
                search(p2, n2, n, n1+1, sum, possible);
        }
    }
    for (const int &n: current) possible[n].insert(p1);
}


ULL sum[5];
void* loop_1(void *) {
  search(PAIR(SIZE-1, SIZE-1), 1, PAIR(0, 4), 3, sum, possible_pool[0]);
}
void* loop_2(void *) {
  search(PAIR(SIZE-1, SIZE-1), 1, PAIR(2, 4), 3, sum + 1, possible_pool[1]);
}
void* loop_3(void *) {
  search(PAIR(SIZE-1, SIZE-1), 1, PAIR(3, 3), 3, sum + 2, possible_pool[2]);
}
void* loop_4(void *) {
  search(PAIR(SIZE-1, SIZE-1), 1, PAIR(3, 1), 3, sum + 3, possible_pool[3]);
}
void* loop_5(void *) {
  search(PAIR(SIZE-1, SIZE-1), 1, PAIR(2, 0), 3, sum + 4, possible_pool[4]);
}


typedef pair<int,int> position;
int main()
{
  ULL gt = 0;
  pthread_t tid1, tid2, tid3, tid4, tid5;

    const position next_positions[8] = {
        make_pair(+2, +1), make_pair(+2, -1),
        make_pair(+1, +2), make_pair(+1, -2),
        make_pair(-1, +2), make_pair(-1, -2),
        make_pair(-2, +1), make_pair(-2, -1)
    };
    for (int x = 0; x < SIZE; ++x)
        for (int y = 0; y < SIZE; ++y)
	  for (const position &d: next_positions) {
	    const int dx = d.first, dy = d.second;
	    if ( (unsigned)(x+dx)<SIZE && (unsigned)(y+dy)<SIZE )
	      for(int k=0;k<5;k++) possible_pool[k][PAIR(x,y)].insert( PAIR(x+dx, y+dy) );
	  };

    for(int k=0;k<5;k++) {
      for (const int &n: possible_pool[k][0]) possible_pool[k][n].erase(0); 
      for (const int &n: possible_pool[k][PAIR(1,2)]) possible_pool[k][n].erase(PAIR(1,2)); 
    }

  pthread_create(&tid1, NULL, loop_1, NULL);
  pthread_create(&tid2, NULL, loop_2, NULL);
  pthread_create(&tid3, NULL, loop_3, NULL);
  pthread_create(&tid4, NULL, loop_4, NULL);
  pthread_create(&tid5, NULL, loop_5, NULL);

  pthread_join(tid1,NULL);
  pthread_join(tid2,NULL);
  pthread_join(tid3,NULL);
  pthread_join(tid4,NULL);
  pthread_join(tid5,NULL);

 for (int i=0;i<5;i++) {
    gt += sum[i]<<1;
    printf("%c: sum %lld\n", 'A'+i, sum[i]<<1);
  }
  printf("total %lld\n", gt);
  return 0;
}
