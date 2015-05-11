#include <stdio.h>
#include <thread>
using namespace std;
#include <signal.h>
#include <unistd.h>

#ifndef SIZE
#define SIZE 7
#endif

#define MTHREAD

#define BIT(a) (1ULL<<(a))
typedef unsigned long long int ULL;

#define RIGHTMOST (SIZE-1)
#define FULL      (BIT(SIZE*SIZE)-1ULL)
#define end_flag  BIT((SIZE+1)*(SIZE-1))
const ULL nwcorner_flag = BIT(SIZE*(SIZE-1));
const ULL secorner_flag = BIT(SIZE-1);
#define nw_1_flag BIT((SIZE-2)*SIZE+2)
#define nw_2_flag BIT((SIZE-3)*SIZE+1)
#define ne_1_flag BIT((SIZE-2)*SIZE+(SIZE-3))
#define ne_2_flag BIT((SIZE-3)*SIZE+(SIZE-2))
#define se_1_flag BIT(1*SIZE+(SIZE-3))
#define se_2_flag BIT(2*SIZE+(SIZE-2))

ULL mask[SIZE*SIZE];

#ifdef __GNUC__ // GCC
ULL inline bit_to_mask(const ULL a) {
  return mask [__builtin_ctzll(a)];
}
#else
#ifdef  _MSC_VER  // Visual Studio（未確認）
#include <intrin.h>
#pragma intrinsic(_BitScanReverse)
ULL inline bit_to_mask(const ULL a){
  int x;
  _BitScanReverse64(&x,a);
  return mask[x];
}
#endif
#endif


void main_loop(const ULL p, const ULL state, ULL * const sum);

#define inn_loop() \
  { const ULL pos_msb = pos&(-pos); \
    main_loop(pos_msb, state|pos_msb, sum) ; \
    pos-=pos_msb; } \
  if(!pos) return;

void inline loop_aux(const ULL p, const ULL state, ULL * const sum) {
  ULL pos = (~state)&bit_to_mask(p); // p から行ける場所
  if (pos) {
    // pos の最上位ビットから順番に調べていく．
    // 高々 8 箇所しかないので手動でループ展開
    inn_loop(); inn_loop(); inn_loop(); inn_loop();
    inn_loop(); inn_loop(); inn_loop(); 
    main_loop(pos, state|pos, sum);
  }
}

void inline corner_aux(const ULL p, const ULL q, const ULL c, const ULL state, ULL * const sum) {
  if ( !(state&c) ) {// 角 (c) 未到達
    if ( !(state&q) ) main_loop(q, state|(q|c), sum);
  }
  else loop_aux(p, state, sum);
}

void main_loop(const ULL p,  const ULL state, ULL * const sum) {
  switch (p) {
#if (SIZE >= 6)
  case ne_1_flag:
    if ( state&ne_2_flag ) {
      if (state==FULL-end_flag) ++(*sum);
    } else loop_aux(ne_1_flag, state, sum);
    break;
  case ne_2_flag:
    if ( state&ne_1_flag ) {
      if (state==FULL-end_flag) ++(*sum);
    } else loop_aux(ne_2_flag, state, sum);
    break;
#else
  case end_flag: 
    if (state==FULL) ++(*sum); break;
#endif
  case nw_1_flag:
    corner_aux(nw_1_flag, nw_2_flag, nwcorner_flag, state, sum); break;
  case nw_2_flag:
    corner_aux(nw_2_flag, nw_1_flag, nwcorner_flag, state, sum); break;
  case se_1_flag:
    corner_aux(se_1_flag, se_2_flag, secorner_flag, state, sum); break;
  case se_2_flag:
    corner_aux(se_2_flag, se_1_flag, secorner_flag, state, sum); break;
  default:
    loop_aux(p, state, sum);
  }
}

ULL sum[5];


#define main_loop_call(a,b,c,d)  main_loop(BIT(a+b), c+BIT(a+b), sum+d)

#define setmask(dx, dy) \
  if( ((unsigned)(i+dx)<SIZE)&&((unsigned)(j+dy)<SIZE) ) \
    mask[i+j*SIZE] |= BIT( (i+dx)+SIZE*(j+dy) );

static void sigint_handler(int sig) {
  ULL gt = 0;
  for (int i=0;i<5;i++)  gt += sum[i]<<1;
  printf("! interrupt (%s). %lld\n", 
	 (sig==SIGINT)?"SIGINT":((sig==SIGTERM)?"SIGTERM":"???"),
	 gt);
  exit(sig);
}

int main(void) {
  signal(SIGINT, sigint_handler);
  signal(SIGTERM, sigint_handler);
  printf("size %d\n", SIZE);

 { // mask 初期化
   for (int i=0;i<SIZE*SIZE;i++) mask[i]=0;
   for (int i=0;i<SIZE;i++) 
     for (int j=0;j<SIZE;j++) {
       setmask(+2,+1); setmask(+2,-1); setmask(-2,+1); setmask(-2,-1);
       setmask(+1,+2); setmask(+1,-2); setmask(-1,+2); setmask(-1,-2);
     }
   for (int i=0;i<SIZE*SIZE;i++) mask[i]&= ~(1ULL+BIT(SIZE+2));
 }


#ifdef MTHREAD
  auto th1 = thread([]{ main_loop_call ( 4,      0, 1ULL+BIT(SIZE+2), 0 ); });
  auto th2 = thread([]{ main_loop_call ( 4, 2*SIZE, 1ULL+BIT(SIZE+2), 1 );});
  auto th3 = thread([]{ main_loop_call ( 3, 3*SIZE, 1ULL+BIT(SIZE+2), 2 );});
  auto th4 = thread([]{ main_loop_call ( 1, 3*SIZE, 1ULL+BIT(SIZE+2), 3 );});
  auto th5 = thread([]{ main_loop_call ( 0, 2*SIZE, 1ULL+BIT(SIZE+2), 4 );});

  th1.join(); th2.join(); th3.join(); th4.join(); th5.join();

#else
  main_loop_call ( 2, SIZE, 1ULL, 0 );
#endif

  { // 合計表示
    ULL gt = 0;
    for (int i=0;i<5;i++) {
      gt += sum[i]<<1;
      printf("%c: sum %lld\n", 'A'+i, sum[i]<<1);
    }
    printf("total %lld\n", gt);
  }
}

