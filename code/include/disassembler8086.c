#include <stdint.h>
#include <stdlib.h>

#include "file_utils.c"

const	char * REGs[8][2] = {
	{"al", "ax"},
	{"cl", "cx"},
	{"dl", "dx"},
	{"bl", "bx"},
	{"ah", "sp"},
	{"ch", "bp"},
	{"dh", "si"},
	{"bh", "di"},
};

typedef struct DataLookup_def
{
	char *data;
	int pos;
} DataLookup;


int disassembly8086(DataLookup *dataLookup, char *buffer_out)
{
	uint8_t byte1 = dataLookup->data[dataLookup->pos++];

	if (byte1 >> 4 << 4 == 0b10110000) // 1011 w reg | data | data if w =1
	{
		char w =  (byte1 & 0b00001000) >> 3;

		uint16_t byte2;
		
		if (w) {
			byte2 = ((dataLookup->data[dataLookup->pos + 1] << 8) | (uint8_t)(dataLookup->data[dataLookup->pos] & 0b11111111));
			dataLookup->pos += 2;
		}
		else {
			byte2 = dataLookup->data[dataLookup->pos++];
		}

		char data[10]; itoa(byte2, data, 10);

		char reg = byte1 & 0b00000111;
		strncat(buffer_out, "mov ", 4);
		strncat(buffer_out, REGs[reg][w], 2);
		strncat(buffer_out, ", ", 2);
		strncat(buffer_out, data, strlen(data));
	
		strncat(buffer_out, "\n", 1);
	} 
	else if (byte1 >> 2 << 2 == 0b10001000) // MOV // 0b100010DW | mode reg r/m | (DISP-LO) | (DISP-H)
	{
		char w =  (byte1 & 0b00000001) == 0b00000001;

		char byte2 = dataLookup->data[dataLookup->pos++];

		char d =  (byte1 & 0b00000010) == 0b00000010;

		char reg_displacement;
		char mem_reg_displacement;

		reg_displacement = d ? 0b00111000 : 0b00000111;
		mem_reg_displacement = d ? 0b00000111 : 0b00111000;

		char reg = (byte2 & reg_displacement);
		char mem_reg = ((byte2 & mem_reg_displacement) >> 3);

		strncat(buffer_out, "mov ", 4);
		strncat(buffer_out, REGs[reg][w], 2);
		if (REGs[mem_reg][w] != NULL) {
			strncat(buffer_out, ", ", 2);
			strncat(buffer_out, REGs[mem_reg][w], 2);
		}
		strncat(buffer_out, "\n", 1);
	}
	return 0;
}
