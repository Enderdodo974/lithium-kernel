// arch.h

#define PAGE_SIZE              128 // words per page (each word is 2 bytes)
#define PAGE_SHIFT               7 // bits of offset within a page

#define PAGE_ROUND_UP(size)   (((size)+PAGE_SIZE-1) & ~(PAGE_SIZE-1));
#define PAGE_ROUND_DOWN(p) ((p) & ~(PAGE_SIZE-1))

#define PTE_V (1 << 0) // valid bit
#define PTE_R (1 << 1) // is readable
#define PTE_W (1 << 2) // is writable
#define PTE_X (1 << 3) // is executable
#define PTE_U (1 << 4) // user can access it

// shift a physical address to the right place for a PTE
#define PA_TO_PTE(pa) ((((uint64)pa) >> 12) << 10) // TODO
#define PTE_TO_PA(pte) (((pte) >> 10) << 12)       // TODO


#define MAX_VA (1 << (6 + 6 + 7))
