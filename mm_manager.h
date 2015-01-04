#ifndef _MM_MANAGER_H
#define _MM_MANAGER_H

#include <stdlib.h>



typedef struct _mm_t{
	char	*mem;
	size_t	mmap_size;
	size_t	file_size;
	int	fd;
	const char *filename;
} mm_t;



mm_t *mm_create(mm_t *mm, const char *filename, size_t mmap_size);
void mm_destroy(mm_t *mm);

int mm_resize(mm_t *mm, size_t file_size);

inline static char *mm_getmem(mm_t *mm){
	return mm->mem;
}

inline static size_t mm_getsize(mm_t *mm){
	return mm->mmap_size < mm->file_size ? mm->mmap_size : mm->file_size;
}



#endif

