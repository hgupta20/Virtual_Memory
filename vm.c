#include "vm.h"
#include <stdio.h>


int convertArrtoNum(int array[32], int len){
    int decimal = 0, power = 1;
    int index = len -1;
    for(int i = 0; i < len; i++)
    {
        decimal += array[index-i]*power;
        power *= 2;
        
    }
    return decimal;
    
}

int insert_pte(uint32 p, uint32 f, uint32 read, uint32 write){
    int bin_array[32];
    int l1_array[10];
    int l2_array[10];
    int offset_array[12];
    int temp = (int)(p/1024);
    int p1 = temp+1;
    int p2 = (int)(p%1024);
    int n, c, k;
    uint32 readWrite = 1;
    n = p;
    for (c = 31; c >= 0; c--){
        k = n >> c;
 
        if (k & 1){
        //printf("1");
        bin_array[c] = 1;
        }
      
        else{
        //printf("0");
        bin_array[c] = 0;
        }

    }
    if ((pageTable[p1][p2] & 0x1) == 1){
        return -4; // 2nd level page table entry already defined (when doing an insert)

    }
    
    //printf("\n");
    for(int j=31; j>=0; j--){
        //printf("%d", bin_array[j]);
    }
    //printf("\n");
    for(int l=11; l>=0; l--){
        //printf("%d", bin_array[l]);
        offset_array[11-l] = bin_array[l];
    }
    //printf("\n");
    for(int m=21; m>=12; m--){
        //printf("%d", bin_array[m]);
        l2_array[21-m] = bin_array[m];
    }
    //printf("\n");
    for(int k=31; k>=22; k--){
        //printf("%d", bin_array[k]);
        l1_array[31-k] = bin_array[k];
    }
    //printf("\nTesting the function\n");
    //int ans = convertArrtoNum(offset_array, 12);
    //printf("%d\n", ans);
    pageTable[0][p1-1] = p1;
    pageTable[0][p1-1] = pageTable[0][p1-1]<<12;
    pageTable[0][p1-1] = pageTable[0][p1-1] + 7;

    //set the frame
    pageTable[p1][p2] = f;
    pageTable[p1][p2] = pageTable[p1][p2]<<12;

    if((read == 0) && (write ==1)){
        readWrite = 3; // set the permission for 011
    }
    else if((read == 1) && (write ==0)){
        readWrite = 5; // // set the permission for 101
    }
    else if((read == 1) && (write ==1)){
        readWrite = 7; // // set the permission for 111
    }
    else
    {
        readWrite = 1; // otherwise the permission are set 1 by default
    }
    pageTable[p1][p2] = pageTable[p1][p2] + readWrite;

    return 1; 
    
}
int remove_pte(uint32 p){
    int temp = (int)(p/1024);
    int p1 = temp+1;
    int p2 = (int)(p%1024);
    uint32 val = pageTable[p1][p2]<<31; // bitshifting to get the last bit
    if((val>>31) ==0){ // to check if null
        return -3; // return error not present
    }
    pageTable[p1][p2]=(pageTable[p1][p2]<<31);
    pageTable[p1][p2]=(pageTable[p1][p2]>>31);
    pageTable[p1][p2]=(pageTable[p1][p2]>>1);
    return 1;

}
int read_vaddr(uint32 vaddr, char *c){
    uint32 readval = vaddr>> 12;
    int offsetval = (int)(vaddr-readval*4096);
    int temp = (int)(readval/1024); 
    int p1 = temp+1;
    int p2 = (int)(readval%1024); 
    uint32 val = pageTable[p1][p2]<<31; // bitshifting to get the last bit
    uint32 val1 = pageTable[p1][p2]<<29; // get the last 3 bits
    if((val>>31) ==0){ // check if null
        return -3; // not present
    }
    if((val1>>31) == 0){ // check if null or missing read permission
        return -1; // missing read permission
    }
    readval = (int)(pageTable[p1][p2] & 0x000fff000);
    *c = userPages[readval+offsetval];
    return 1;

}
int write_vaddr(uint32 vaddr, char c){
    uint32 writeval = vaddr>> 12;
    int offsetval = (int)(vaddr-writeval*4096);
    int temp = (int)(writeval/1024);
    int p1 = temp+1;
    int p2 = (int)(writeval%1024);
    uint32 val = (int)pageTable[p1][p2] & 0x1; // masking to get the bits
    uint32 val1 = pageTable[p1][p2]<<30;
    if(val== 0){ // to check if null
        return -3; // return error not present
    }
    if((val1>>30) !=3){ // to check the permission bits and if it is not write return error
        return -2; // missing write permission
    }
    writeval = (int)(pageTable[p1][p2] & 0x000FFF000); 
    userPages[writeval+offsetval] = c; // write the val if everything is correct
    return 1;
}
