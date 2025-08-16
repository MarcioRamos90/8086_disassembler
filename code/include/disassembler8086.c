#include <stdint.h>

#define uint8 	uint8_t
#define int8 		uint8_t

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

void disassembly8086(char *buffer_in, int size, char *buffer_out)
{
	long long int byte1 = (buffer_in[0] - 0xffffff00);
	char d =  (byte1 & 0b00000010) == 0b00000010;
	char w =  (byte1 & 0b00000001) == 0b00000001;

	// second byte
	long long int byte2 = buffer_in[1] - 0xffffff00;
	printf("%lld %lld\n",  byte1, byte2);

	long long int byte1_1 = byte1 & 0b11111100;
	if (true && byte1_1 == 0b10001000) // MOV // 0b100010DW | mode reg r/m | (DISP-LO) | (DISP-H)
	{
		char reg_displacement;
		char mem_reg_displacement;
		
		if (d)
		{
			reg_displacement = 0b00111000;
			mem_reg_displacement = 0b00000111;
		} 
		else
		{
			reg_displacement = 0b00000111;
			mem_reg_displacement = 0b00111000;
		}

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
}
