#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extract_file_name_from_path(char * path, int len, char * file_name)
{
  if (!path)
	{
		perror("no path was provided");
    return EXIT_FAILURE;
  }

  char split_string = '\\';

  int str_start_point = -1;

  for (int i = 0; i < len; i++)
  {
    if (split_string == path[i])
    {
      str_start_point = i;
    }
  }

  if (str_start_point >= 0)
  {
    strcat(file_name, &path[str_start_point + 1]);
  }
  return EXIT_SUCCESS;
}