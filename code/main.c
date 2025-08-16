#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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

	if ((file = fopen(path_assembled, "r")) == NULL)
	{
		perror("was unable to open this File\n");
		return EXIT_FAILURE;
	} else printf("File opened successfully\n");

	const int buffer_in_size =256;

	char *buffer_in = (char *) calloc(buffer_in_size, sizeof(char));
	char *buffer_out = (char *) calloc(buffer_in_size, sizeof(char));
	int n = 3;
	printf("\n\n---proccess start \n");

	while (fgets(buffer_in, n, file))
	{
		disassembly8086(buffer_in, n, buffer_out);
	}
	printf("---proccess end \n");

	printf("\n\n---result start \n");
	printf("res \n%s", buffer_out);
	printf("---result end \n");

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

	free(buffer_in);
	free(buffer_out);
	free(name_disassembled);

	printf("------ Finished ------\n");
	return EXIT_SUCCESS;
}

