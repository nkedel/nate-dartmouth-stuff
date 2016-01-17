#include "random.h"

uint rnd0(uint x){ 
    // 0<= RND <=x
    return(random()%x);
}

uint rnd1(uint x){
    // 1<= RND <=x
    return(rnd0(x-1)+1);
}
