#include <sys/types.h>
#include <stdio.h>
#include <utility/trace.h>
#include <errno.h>

// Register name faking - works in collusion with the linker.
register char * stack_ptr asm ("sp");

void *_sbrk(ptrdiff_t increment) {

	extern char _sheap_, _eheap_;
	static void *cur_heap_pos = 0;

	if(cur_heap_pos == 0) {
		cur_heap_pos = &_sheap_;
	}

	//printf("_sheap_ 0x%x, _eheap_ 0x%x \n\r", (unsigned int)&_sheap_, (unsigned int)&_eheap_);
	//printf("cur_heap_pos 0x%x\n\r", (unsigned int)cur_heap_pos);

	if((cur_heap_pos + increment) > (void *) &_eheap_) {
		printf("\n\r _sbrk: Out of heap-space! \n\r");
		errno = ENOMEM;
		return (void *) -1;
	}

	// No collision checking as FreeRTOS places task stack inside the heap.
//	if((char*)(_eheap_ + increment) > stack_ptr) {
//		// Some of the libstdc++-v3 tests rely upon detecting
//		//out of memory errors, so do not abort here.
//#if 0
//		printf("\n\r _sbrk: Heap and stack collision! RESTARTING! \n\r");
//		restart();
//#else
//		printf("\n\r _sbrk: Heap and stack collision! \n\r");
//		errno = ENOMEM;
//		return (void *) -1;
//#endif
//	}

	void * old_heap_pos = cur_heap_pos;
	cur_heap_pos += increment;
	return old_heap_pos;
}
