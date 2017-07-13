// Checkpatch
// Custom checksum patcher for FIFA06 NDS

#include <stdio.h>  //C I/O
#include <malloc.h> //Memory Allocation
#include <inttypes.h>
#include <fstream>  //File I/O


int main(int argc, char *argv[])
{
	printf("               F6USA_Patch\n");
	printf("FIFA06 (USA) NDS Savegame Checksum Patcher\n\n");

	//Open Savegame in read mode
	FILE *f = fopen(argv[1], "rb");
	//Seek to the end of the file to measure the size
	fseek(f, 0, SEEK_END);
	size_t fsize = ftell(f);
	//Done, set the file pointer back at the top
	rewind(f);

	/*
	uint8_t = unsigned char
	uint16_t = unsigned short
	*/

	//Allocate memory and read the file
	uint8_t *data = (uint8_t*)malloc(fsize);
	fread(data, 1, fsize, f);
	
	//Starting value for the CRC
	unsigned short crc = 11;

	//Calculation code
	uint32_t i;
	for (i = 0x000003B8; i < 0x00001150; i++) {
		crc += data[i] * (0x00001150 - (i & ~0x3));
	}

	
	if (crc == (uint16_t)(data[0x10] | (data[0x11] << 8)))
		printf("CRC Good\nPatching anyways!");
	else
		printf("CRC Bad!\nPatching...");

	printf("\nCalculated CRC:%04x\n\n", crc);
	free(data);
	fclose(f);
	

	//File stream 'fstream' can both read and write to files
	std::fstream saveStream;
	saveStream.open(argv[1], std::ios::in | std::ios::out | std::ios::binary);
	if (!saveStream.is_open())
	{
		printf("[!] Can't Open %s", argv[1]);
		exit(EXIT_FAILURE);
	}

	saveStream.seekg(0x10);//Checksum address
	saveStream.write(reinterpret_cast<const char *>(&crc), sizeof(crc)); 

	printf("Done!\n");

	saveStream.close();
	fgetchar();
	//fclose(S);
	return 0;
}
