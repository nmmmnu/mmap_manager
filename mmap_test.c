#include <stdint.h>

#include <sys/mman.h>	// mmap
#include <fcntl.h>	// open
#include <stdlib.h>	// exit
#include <unistd.h>	// ftruncate
#include <string.h>	// memcpy

#include <stdio.h>

void *prepareMMAP(const char *filename, size_t sizetomap, int *fd){
	*fd = open(filename, O_RDWR);

	if (*fd < 0){
		printf("Can not open()...\n");
		exit(1);
	}

	void *mem = mmap(NULL, sizetomap, PROT_WRITE, MAP_SHARED, *fd, /* offset */ 0);

	if (mem == MAP_FAILED){
		printf("Can not mmap()...\n");
		exit(1);
	}

	// OK this is wrong...
	if (ftruncate(*fd, 1024 * 4)){
		printf("Can not ftruncate()...\n");
		exit(1);
	}

	return mem;
}

int main(const int argc, const char **argv){
	// must be significantly lower for 32 bit OS
	const size_t sizetomap = (size_t) 1024 * 1024 * 1024 * 8;
	int fd;
	char *mem = prepareMMAP("file.bin", sizetomap, & fd);

	const char **str;
	unsigned int len;

	if (argc <= 1){
		const char *str2[] = {
			"Hello", "my", "name", "is", "Malcom", "X",
			NULL
		};

		str = str2;

		for(len = 0; str[len]; len++);
	}else{
		str = & argv [1];
		len = argc - 1;
	}

	uint32_t *ptable = (void *) & mem[0];

	uint32_t ppos = sizeof(uint32_t) * (len + 1);

	unsigned int i;
	for(i = 0; i < len; i++){
		ptable[i] = ppos;

		const char *item = str[i];
		const unsigned int item_size = strlen(item) + 1;

		printf("%3u : %4x : %2u : %s\n", i, ppos, item_size, item);

		memcpy(&mem[ppos], item, item_size);

		ppos = ppos + item_size;
	}

	ptable[len] = 0;

	mem[ppos + 0] = 0xDE;
	mem[ppos + 1] = 0xED;
	mem[ppos + 2] = 0xBE;
	mem[ppos + 3] = 0xEF;

	ftruncate(fd, ppos + 4);

	munmap(mem, sizetomap);

	return 0;
}
