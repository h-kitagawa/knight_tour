// 長尾さんの nagao.cc を小改変したもの
// ./knight4_7  9090.30s user 0.53s system 301% cpu 50:11.98 total
#include <stdio.h>
#include <set>

using namespace std;

#ifndef SIZE
#define SIZE 7
#endif

#define MULTITHREAD

typedef unsigned long long int ULL;
typedef set<int> positions;
positions possible_pool[5][SIZE*SIZE];


#define PAIR(x,y) ((x)*SIZE + (y))
#define GOAL PAIR(SIZE-1,SIZE-1)
#define NW_CC PAIR(0,SIZE-1)
#define NW_C1 PAIR(2,SIZE-2)
#define NW_C2 PAIR(1,SIZE-3)
#define SE_CC PAIR(SIZE-1,0)
#define SE_C1 PAIR(SIZE-2,2)
#define SE_C2 PAIR(SIZE-3,1)
#define BIT(a) (1ULL<<(a))

#define OUT_SET(p) for(positions::iterator itr=possible[p].begin(); itr!=possible[p].end(); itr++) printf("%d ", *itr); printf("\n");
void search(const int p1, const ULL state, ULL* sum, positions possible[])
{

  const positions &current = possible[p1];

  if (p1==NW_C1 && !(state&BIT(NW_CC))) {
    if (!(state&BIT(NW_C2))) {
      possible[NW_CC].erase(NW_C2);
      search(NW_C2, state|BIT(NW_CC)|BIT(NW_C2), sum, possible);
      possible[NW_CC].insert(NW_C2);
    }
  } else  if (p1==NW_C2 && !(state&BIT(NW_CC))) {
    if (!(state&BIT(NW_C1))) {
      possible[NW_CC].erase(NW_C1);
      search(NW_C1, state|BIT(NW_CC)|BIT(NW_C1), sum, possible);
      possible[NW_CC].insert(NW_C1);
    }
  } else if (p1==SE_C1 && !(state&BIT(SE_CC))) {
    if (!(state&BIT(SE_C2))) {
      possible[SE_CC].erase(SE_C2);
      search(SE_C2, state|BIT(SE_CC)|BIT(SE_C2), sum, possible);
      possible[SE_CC].insert(SE_C2);
    }
  } else  if (p1==SE_C2 && !(state&BIT(SE_CC))) {
    if (!(state&BIT(SE_C1))) {
      possible[SE_CC].erase(SE_C1);
      search(SE_C1, state|BIT(SE_CC)|BIT(SE_C1), sum, possible);
      possible[SE_CC].insert(SE_C1);
    }
  } else {
    int uniques = 0;
    int np; // next position
    for (const int &n: current) {
      positions &l = possible[n];
      l.erase(p1);
      if (n!=GOAL && l.size() == 1) {
	uniques++; np=n;
      }
    }
    switch (uniques) {
    case 1:
      if ( (state|BIT(np)) == BIT(SIZE*SIZE-1)-1ULL ) {
	(*sum)++; if (!((*sum)&0xffff)) printf("%lld\n", *sum); 
      } else
	search(np, state|BIT(np), sum, possible);
      break;
    case 0:
      for (const int &n: current) {
	if ( n!=GOAL )
	  search(n, state|BIT(n), sum, possible);
      }
    }
    for (const int &n: current) possible[n].insert(p1);
  }
}


ULL sum[5];
void* loop_1(void *) {
  search(PAIR(0, 4), BIT(PAIR(0,0))+BIT(PAIR(1,2))+BIT(PAIR(0,4)), sum, possible_pool[0]);
}
void* loop_2(void *) {
  search(PAIR(2, 4), BIT(PAIR(0,0))+BIT(PAIR(1,2))+BIT(PAIR(2,4)), sum + 1, possible_pool[1]);
}
void* loop_3(void *) {
  search(PAIR(3, 3), BIT(PAIR(0,0))+BIT(PAIR(1,2))+BIT(PAIR(3,3)), sum + 2, possible_pool[2]);
}
void* loop_4(void *) {
  search(PAIR(3, 1), BIT(PAIR(0,0))+BIT(PAIR(1,2))+BIT(PAIR(3,1)), sum + 3, possible_pool[3]);
}
void* loop_5(void *) {
  search(PAIR(2, 0), BIT(PAIR(0,0))+BIT(PAIR(1,2))+BIT(PAIR(2,0)), sum + 4, possible_pool[4]);
}

#ifdef MULTITHREAD
#include <pthread.h>
#endif

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

#ifdef MULTITHREAD
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
#else
  search(PAIR(1,2), BIT(PAIR(0,0))+BIT(PAIR(1,2)), sum, possible_pool[0]);
#endif

 for (int i=0;i<5;i++) {
    gt += sum[i]<<1;
    printf("%c: sum %lld\n", 'A'+i, sum[i]<<1);
  }
  printf("total %lld\n", gt);
  return 0;
}
