#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tree.h"

#define ASCII 256

void clear(){
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}



