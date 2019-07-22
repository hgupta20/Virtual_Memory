#define PAGE_SIZE_IN_BYTES 4096
#define PAGE_SIZE_IN_UINTS 1024
#define PAGE_TABLE_PAGES 1025
#define USER_PAGE_PAGES 1000

#ifndef HW5
#define HW5

typedef unsigned int uint32;
typedef uint32 PtePage[1024];

PtePage pageTable[PAGE_TABLE_PAGES];
char userPages[PAGE_SIZE_IN_BYTES*USER_PAGE_PAGES];

int insert_pte(uint32 p, uint32 f, uint32 read, uint32 write);
int remove_pte(uint32 p);
int read_vaddr(uint32 vaddr, char *c);
int write_vaddr(uint32 vaddr, char c);

#endif