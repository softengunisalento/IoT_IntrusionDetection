#include <stdio.h>

static int result=0;
static int input=0;
#ifndef RESULT_C
#define RESULT_C

void setStatus(int n){
    result=n;
}

void setInput(int c){
    input = c;
}

int getStatus(){
    return result;
}

int getInput(){
    return input;
}
#endif
