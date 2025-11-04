#include "m61.hh"
#include <cstdlib>
#include <cstddef>
#include <cstring>
#include <cstdio>
#include <cinttypes>
#include <cassert>
#include <sys/mman.h>
#include <map>
#include <iostream>

static std::map<void*, size_t> active_sizes;
static std::map<void*, size_t> freed_sizes;

static m61_statistics gstats = {
    .nactive = 0,
    .active_size = 0,
    .ntotal = 0,
    .total_size = 0,
    .nfail = 0,
    .fail_size = 0,
    .heap_min = 0,
    .heap_max = 0
    };
struct m61_memory_buffer {
    char* buffer;
    size_t pos = 0;
    size_t size = 8 << 20; /* 8 MiB */

    m61_memory_buffer();
    ~m61_memory_buffer();
};

static m61_memory_buffer default_buffer;


m61_memory_buffer::m61_memory_buffer() {
    void* buf = mmap(nullptr,    // Place the buffer at a random address
        this->size,              // Buffer should be 8 MiB big
        PROT_WRITE,              // We want to read and write the buffer
        MAP_ANON | MAP_PRIVATE, -1, 0);
                                 // We want memory freshly allocated by the OS
    assert(buf != MAP_FAILED);
    this->buffer = (char*) buf;
}

m61_memory_buffer::~m61_memory_buffer() {
    munmap(this->buffer, this->size);
}




/// m61_malloc(sz, file, line)
///    Returns a pointer to `sz` bytes of freshly-allocated dynamic memory.
///    The memory is not initialized. If `sz == 0`, then m61_malloc may
///    return either `nullptr` or a pointer to a unique allocation.
///    The allocation request was made at source code location `file`:`line`.

static void* m61_find_free_space(size_t sz) {
    if (default_buffer.pos + sz > default_buffer.size) {
        // Not enough space left in default buffer for allocation
        //fprintf(stdout, "Not enough space left!\n");
        ++gstats.nfail;
        gstats.fail_size += sz;
        return nullptr;
    }
    if(sz == 0){
        return nullptr;
    }
    if(sz > default_buffer.size){
        //fprintf(stdout, "Size too big for buffer!\n");
        ++gstats.nfail;
        gstats.fail_size += sz;
        return nullptr;
    }
    for(auto const & [k,v]: freed_sizes){
        fprintf(stdout,"Freed sizes at start of loop: %ld\n",freed_sizes.size());
        if(v >= sz){
            freed_sizes.erase(k);
            //fprintf(stdout,"Reused memory!\n");
            return k;
        }
        fprintf(stdout, "Memory block not big enough!\n");
        std::cout << "Pointer: " << k << " Size of allocation: \n" << v << std::endl;
    }
    fprintf(stdout,"No reusable memory found!\n");
    return nullptr;
}

void* m61_malloc(size_t sz, const char* file, int line) {
    (void) file, (void) line;   // avoid uninitialized variable warnings
    // Your code here.
    if(default_buffer.pos % sizeof(max_align_t) != 0){
        default_buffer.pos = (default_buffer.pos + sizeof(max_align_t) - 1) & ~(sizeof(max_align_t) - 1);
    }
    void *ptr = m61_find_free_space(sz);
    // Otherwise there is enough space; claim the next `sz` bytes
    ++gstats.ntotal;
    gstats.total_size += sz;
    ++gstats.nactive;
    ptr = &default_buffer.buffer[default_buffer.pos];
    if(gstats.heap_min == 0 || (uintptr_t) ptr <= gstats.heap_min){
        gstats.heap_min = (uintptr_t) ptr;
    }
    if(gstats.heap_max == 0 || (uintptr_t) ptr + sz >= gstats.heap_max){
        gstats.heap_max = ((uintptr_t) ptr) + sz;
    }
    default_buffer.pos += sz;
    active_sizes[ptr] = sz;
    gstats.active_size = gstats.active_size + sz;
    return ptr;
}


/// m61_free(ptr, file, line)
///    Frees the memory allocation pointed to by `ptr`. If `ptr == nullptr`,
///    does nothing. Otherwise, `ptr` must point to a currently active
///    allocation returned by `m61_malloc`. The free was called at location
///    `file`:`line`.

void m61_free(void* ptr, const char* file, int line) {
    // avoid uninitialized variable warnings
    (void) ptr, (void) file, (void) line;
    // Your code here. The handout code does nothing!
    fprintf(stdout, "Number of active allocations: %lld\n", gstats.nactive);  
    if(ptr != nullptr && gstats.nactive > 0){
        gstats.nactive = gstats.nactive - 1;
    }
    size_t temp = active_sizes[ptr];
    freed_sizes[ptr] = temp;
    gstats.active_size = gstats.active_size - active_sizes[ptr];
    active_sizes.erase(ptr);
}


/// m61_calloc(count, sz, file, line)
///    Returns a pointer a fresh dynamic memory allocation big enough to
///    hold an array of `count` elements of `sz` bytes each. Returned
///    memory is initialized to zero. The allocation request was at
///    location `file`:`line`. Returns `nullptr` if out of memory; may
///    also return `nullptr` if `count == 0` or `size == 0`.

void* m61_calloc(size_t count, size_t sz, const char* file, int line) {
    // Your code here (not needed for first tests)
    if(count > default_buffer.size){
        ++gstats.nfail;
        return nullptr;
    }
    if(sz > default_buffer.size){
        ++gstats.nfail;
        return nullptr;
    }
    void* ptr = m61_malloc(count * sz, file, line);
    if (ptr) {
        memset(ptr, 0, count * sz);
    }
    return ptr;
}


/// m61_get_statistics()
///    Return the current memory statistics.

m61_statistics m61_get_statistics() {
    // Your code here.
    // The handout code sets all statistics to enormous numbers.
    return gstats;
}


/// m61_print_statistics()
///    Prints the current memory statistics.

void m61_print_statistics() {
    m61_statistics stats = m61_get_statistics();
    printf("alloc count: active %10llu   total %10llu   fail %10llu\n",
           stats.nactive, stats.ntotal, stats.nfail);
    printf("alloc size:  active %10llu   total %10llu   fail %10llu\n",
           stats.active_size, stats.total_size, stats.fail_size);
}


/// m61_print_leak_report()
///    Prints a report of all currently-active allocated blocks of dynamic
///    memory.

void m61_print_leak_report() {
    // Your code here.
}
