//gcc ${=CFLAGS} -Ofast -o knight2 knight2.c;
#include<stdio.h>
#define SIZE 6
#define BIT(a) (1ULL<<(a))
typedef unsigned long long int ULL;

ULL total = 0;
typedef struct {
  unsigned eh: 8;
  signed   v:  4;
} hv_store;

const unsigned int rightmost = 1<<(SIZE-1);
const int topmost = SIZE*(SIZE-1);
const ULL end_flag =  BIT((SIZE+1)*(SIZE-1));
const ULL full = BIT(SIZE*SIZE)-1ULL;

const ULL se_stuck = ( BIT(2*SIZE-3) + BIT(3*SIZE-2) );
const ULL nw_stuck = ( BIT(SIZE*(SIZE-2)+2) + BIT(SIZE*(SIZE-3)+1) );

#define check_and_rec(nh, dv) \
    {if ( v+dv<SIZE*SIZE ) { const ULL s = ((ULL)nh)<<(v+dv);\
      if ( ! (state&s) ) main_loop(nh, v+dv, state|s);}} \
    {if ( v-dv>=0 ) {        const ULL s = ((ULL)nh)<<(v-dv);\
      if ( ! (state&s) ) main_loop(nh, v-dv, state|s);}}

void main_loop(const unsigned int eh, const int v, const ULL state) {
  if ( state & end_flag ) {
      if ( state==full ) { total+=2; printf("%lld\n", total); }
  } else {
    const ULL sa = ((ULL)eh)<<v;
    if (eh <rightmost/2 ) {
      check_and_rec(eh<<1, 2*SIZE); 
      check_and_rec(eh<<2, SIZE);
    } else if (eh < rightmost) { 
      check_and_rec(eh<<1, 2*SIZE); 
    } 
    if (eh>>2) {
      check_and_rec(eh>>1, 2*SIZE);
      check_and_rec(eh>>2, SIZE);
    } else if (eh>>1) {
      check_and_rec(eh>>1, 2*SIZE);
    }
  }
}

int main(void) {
  printf("size %d\n", SIZE);
  main_loop(4,SIZE,1ULL+BIT(SIZE+2));
  printf("total %lld\n", total);
}

