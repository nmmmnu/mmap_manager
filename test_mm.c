#include <stdint.h>

#include <stdlib.h>	// exit
#include <string.h>	// memcpy

#include <stdio.h>


#include "mm_manager.h"


int main(const int argc, const char **argv){
	// must be significantly lower for 32 bit OS
	const size_t sizetomap = (size_t) 1024 * 1024 * 1024 * 8;
	const char *filename = "file.bin";

	mm_t mm_real;

	mm_t *mm = mm_create(&mm_real, filename, sizetomap);

	if (mm == NULL){
		printf("Can not create mmap manager...\n");
		exit(1);
	}


	char *mem = mm_getmem(mm);

	const char **str;
	unsigned int len;
	unsigned int i;

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

	size_t datasize = 0;

	for(i = 0; i < len; i++){
		datasize = datasize + strlen(str[i]) + 1;
		printf("%3i | %3zu\n", i, datasize);
	}

	uint32_t *table = (void *) & mem[0];

	uint32_t pos = sizeof(uint32_t) * (len + 1);

	mm_resize(mm, pos + datasize + 4);

	for(i = 0; i < len; i++){
		table[i] = pos;

		const char *item = str[i];
		const unsigned int item_size = strlen(item) + 1;

		printf("%3u : %4x : %2u : %s\n", i, pos, item_size, item);

		memcpy(&mem[pos], item, item_size);

		pos = pos + item_size;
	}

	table[len] = 0;

	const char deedbeef[] = { 0xDE, 0xED, 0xBE, 0xEF };
	memcpy(& mem[pos], deedbeef, 4);



	mm_destroy(mm);

	return 0;
}
