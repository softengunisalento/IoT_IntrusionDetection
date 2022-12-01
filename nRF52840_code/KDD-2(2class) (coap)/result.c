#include <stdio.h>

static int result=0;

#ifndef RESULT_C
#define RESULT_C

void setStatus(int n){
    result=n;
}

int getStatus(){
    return result;
}

#endif
