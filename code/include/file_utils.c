#include<stdlib.h>


const long get_file_size(FILE *f)
{
	long size;
	long current_pos = ftell(f);
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, current_pos, SEEK_SET);
	return size + 1;
}