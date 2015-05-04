//gcc ${=CFLAGS} -Ofast -o knight2 knight2.c;
#include<stdio.h>
#define SIZE 6
#define BIT(a) (1ULL<<(a))
typedef unsigned long long int ULL;
#define PACK(a,b) (((a)<<16)+b)

ULL total = 0;

#define RIGHTMOST (1<<(SIZE-1))
#define FULL      (BIT(SIZE*SIZE)-1ULL)
const ULL end_flag = BIT((SIZE+1)*(SIZE-1));

#define check_and_rec(nh, dv) \
    {if ( v<SIZE*SIZE-dv ) \
      if ( ! (state&(((ULL)nh)<<(v+dv))) ) \
       main_loop( PACK(nh, v+dv), state|(((ULL)nh)<<(v+dv)));}; \
    {if ( v>=dv ) \
      if ( ! (state&(((ULL)nh)<<(v-dv))) ) \
       main_loop( PACK(nh, v-dv), state|(((ULL)nh)<<(v-dv)));};

void main_loop(const unsigned int p, const ULL state) {
  if ( state & end_flag ) { // reached the goal
      if ( state==FULL ) printf("%lld\n", ++total); 
  } else {
    const unsigned int eh = p>>16;
    const unsigned int v =  p&0xffff;
    switch(eh) {
    case 1:
      check_and_rec(2, 2*SIZE); 
      check_and_rec(4, SIZE);
      break;
    case 2:
      check_and_rec(1, 2*SIZE);
      check_and_rec(4, 2*SIZE); 
      check_and_rec(8, SIZE);
      break;
    case RIGHTMOST:
      check_and_rec(RIGHTMOST>>1, 2*SIZE); 
      check_and_rec(RIGHTMOST>>2, SIZE);
      break;
    case RIGHTMOST/2:
      check_and_rec(RIGHTMOST, 2*SIZE); 
      check_and_rec(RIGHTMOST>>2, 2*SIZE); 
      check_and_rec(RIGHTMOST>>3, SIZE);
      break;
    default:
      check_and_rec(eh>>1, 2*SIZE);
      check_and_rec(eh<<1, 2*SIZE); 
      check_and_rec(eh<<2, SIZE);
      check_and_rec(eh>>2, SIZE);
     }
  }
}

int main(void) {
  printf("size %d\n", SIZE);
  main_loop( PACK (4,SIZE) ,1ULL+BIT(SIZE+2));
  printf("total %lld\n", total<<1);
}

