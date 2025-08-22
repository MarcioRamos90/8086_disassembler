#pragma once

#include<stdlib.h>
#include<stdint.h>

typedef struct FileObject_s
{
    FILE *file;
} FileObject;

int file_lenght(FILE *f)
{
	int size;
	int current_pos = ftell(f);
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, current_pos, SEEK_SET);
	return size+4;
}