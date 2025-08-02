#include "errno.h"
#include "globals.h"

#include "test/usertest.h"
#include "test/proctest.h"

#include "util/debug.h"
#include "util/printf.h"
#include "util/string.h"

#include "mm/mm.h"
#include "mm/page.h"
#include "mm/slab.h"
#include "mm/kmalloc.h"
#include "vm/vmmap.h"

long test_vmmap() {
    vmmap_t *map = curproc->p_vmmap;

    // Make sure we start out cleanly
    KASSERT(vmmap_is_range_empty(map, ADDR_TO_PN(USER_MEM_LOW), ADDR_TO_PN(USER_MEM_HIGH - USER_MEM_LOW)));

    // Go through the address space, make sure we find nothing
    for (size_t i = USER_MEM_LOW; i < ADDR_TO_PN(USER_MEM_HIGH); i += PAGE_SIZE) {
        KASSERT(!vmmap_lookup(map, i));
    }
    
    // You can probably change this.
    size_t num_vmareas = 5;
    // Probably shouldn't change this to anything that's not a power of two.
    size_t num_pages_per_vmarea = 16;

    size_t prev_start = ADDR_TO_PN(USER_MEM_HIGH);
    for (size_t i = 0; i < num_vmareas; i++) {
        ssize_t start = vmmap_find_range(map, num_pages_per_vmarea, VMMAP_DIR_HILO);
        test_assert(start + num_pages_per_vmarea == prev_start, "Incorrect return value from vmmap_find_range");
        
        vmarea_t *vma = kmalloc(sizeof(vmarea_t));
        KASSERT(vma && "Unable to alloc the vmarea");
        memset(vma, 0, sizeof(vmarea_t));

        vma->vma_start = start;
        vma->vma_end = start + num_pages_per_vmarea;
        vmmap_insert(map, vma);

        prev_start = start;
    }

    // Now, our address space should look like:
    // EMPTY EMPTY EMPTY [  ][  ][  ][  ][  ]
    // ^LP
    //                                      ^HP
    //                   ^section_start 
    // HP --> the highest possible userland page number
    // LP --> the lowest possible userland page number 
    // section start --> HP - (num_vmareas * num_pages_per_vmarea) 
    
    list_iterate(&map->vmm_list, vma, vmarea_t, vma_plink) {
        list_remove(&vma->vma_plink); 
        kfree(vma);
    }
    
    return 0; 
}

long vmtest_main(long arg1, void* arg2) {
    test_init(); 
    test_vmmap(); 

    // Write your own tests here!

    test_fini(); 
    return 0; 
}
