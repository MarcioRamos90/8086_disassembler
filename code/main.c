#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "include/file_utils.c"
#include "include/disassembler8086.c"
#include "include/path_handler.c"

char *path_assembled;
char *bkp_name_disassembled;
char *name_disassembled;

int main(int argc, char * argv[])
{
	if (argc <= 1)
	{
		perror("please send the path for the assembled file");
		return EXIT_FAILURE;
	}
	name_disassembled = (char *) calloc(strlen(argv[1]), sizeof(char));
	bkp_name_disassembled = (char *) calloc(strlen(argv[1]), sizeof(char));
	path_assembled = (char *)calloc(strlen(argv[1]), sizeof(char));
	
	strcat(path_assembled, argv[1]);

	if (extract_file_name_from_path(path_assembled, strlen(path_assembled), name_disassembled) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	strcat(name_disassembled, "_out");
	strcat(bkp_name_disassembled, name_disassembled);
	strcat(name_disassembled, ".asm");
	
	FILE *file;

	if ((file = fopen(path_assembled, "rb")) == NULL)
	{
		perror("was unable to open this File\n");
		return EXIT_FAILURE;
	} else printf("File opened successfully\n");

	int BUFFER_SIZE = (int) file_lenght(file);
	char *buffer_out = (char *) calloc(255, sizeof(char));

	FileObject fileObject = {file};
	unsigned char *data = (unsigned char *) calloc(BUFFER_SIZE, sizeof(unsigned char));

	size_t bytes_read = fread(data, sizeof(unsigned char), BUFFER_SIZE, fileObject.file);

	DataLookup dataLookup = {data, 0};

	while (dataLookup.pos <= BUFFER_SIZE) {
		/*
			DISASSEMBLYNG THE BYTE
		*/
		if (disassembly8086(&dataLookup, buffer_out) == -1)
		{
			break;
		}
	}

	if (fclose(file))
	{
		printf("File was not closed\n");
		return EXIT_FAILURE;
	} else printf("File closed\n");

	if (buffer_out != NULL) 
	{
		FILE * file_output = fopen(name_disassembled, "w+");
		if (!file_output)
		{
			perror("you can't open a file right now");
			return EXIT_FAILURE;
		}
		fprintf(file_output, buffer_out);
		if (fclose(file_output))
		{
			printf("output file was not closed");
			return EXIT_FAILURE;
		}
	}

	printf("\n\n------ Commands to verify correcteness: ------\n");
	printf("nasm %s\n", name_disassembled);
	printf("fc %s %s\n", path_assembled, bkp_name_disassembled);

	free(buffer_out);
	free(name_disassembled);
	free(bkp_name_disassembled);
	free(path_assembled);
	
	printf("------ Finished ------\n");
	return EXIT_SUCCESS;
}

