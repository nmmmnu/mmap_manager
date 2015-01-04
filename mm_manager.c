#include "mm_manager.h"


#include <fcntl.h>	// open
#include <unistd.h>	// close
#include <sys/stat.h>	// fstat
#include <sys/mman.h>	// mmap


static int _mm_getfilesize(mm_t *mm);



//#define MIN(a, b) (a) > (b) ? (a) : (b)
//#define MAX(a, b) (a) < (b) ? (a) : (b)



mm_t *mm_create(mm_t *mm, const char *filename, size_t mmap_size){
	//memset(mm, 0, sizeof(mm_t));

	mm->filename = filename;

	mm->fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

	mm->mmap_size = mmap_size;

	if (_mm_getfilesize(mm))
		return NULL;

	mm->mem = mmap(NULL, mm->mmap_size, PROT_WRITE, MAP_SHARED, mm->fd, /* offset */ 0);

	if (mm->mem == MAP_FAILED){
		close(mm->fd);
		return NULL;
	}

	return mm;
}

void mm_destroy(mm_t *mm){
	munmap(mm->mem, mm->mmap_size);
}

int mm_resize(mm_t *mm, size_t file_size){
	if (file_size == mm->file_size){
		// already resized, thank you...
		return 0;
	}

	if (file_size > mm->mmap_size){
		// need to munmap(), then to mmap() again...
		// or probably use Linux's mremap()
		return -1;
	}

	// change file size
	if (ftruncate(mm->fd, file_size))
		return -2;

	mm->file_size = file_size;

	return 0;
}

// ==================================================

static int _mm_getfilesize(mm_t *mm){
	struct stat buffer;

	if (fstat(mm->fd, & buffer)){
		close(mm->fd);
		return -1;
	}

	mm->file_size = buffer.st_size;

	return 0;
}


