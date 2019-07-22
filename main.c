#include "hw4.h"
#include <stdio.h>

void main(){
    insert_pte(4097,999,1,1);
    insert_pte(4097, 29, 0, 1)
    write_vaddr(1023*4096+131, ’x’);
    read_vaddr(1023*4096+131, c);
    printf("\n");
}