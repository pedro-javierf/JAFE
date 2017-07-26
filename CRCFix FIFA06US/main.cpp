// Checkpatch
// Custom checksum patcher for FIFA06 NDS

#include <stdio.h>
#include <malloc.h>
#include <inttypes.h>
#include <fstream>  //File I/O
#include <iostream>

int main()
{
	printf("               Checkpatch\n");
	printf("     by pedro-javierf and ChampionLeake\n");
	printf("heavily based on FIX94's crc calculation code\n\n");

	//Open Savegame in read mode
	FILE *f = fopen("savegame.dsv", "rb");
	//Seek to the end of the file to measure the filesize
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
	saveStream.open("savegame.dsv", std::ios::in | std::ios::out | std::ios::binary);
	if (!saveStream.is_open())
	{
		std::cout << "[!] Can't Open Transformers.sav";
		exit(EXIT_FAILURE);
	}

	saveStream.seekg(0x10);//Checksum address
	saveStream.write(reinterpret_cast<const char *>(&crc), sizeof(crc)); 

	/*FUCK C
	//Open Savegame in write mode
	FILE *S = fopen("savegame.dsv", "wb");

	printf("Patching savegame...\n\n");
	fseek(S, 0x10, SEEK_SET);
	
	unsigned short *pointer;
	pointer = &crc;

	fwrite(pointer, 1, 2, f);
	
	//fprintf(S, "%04x", crc);
	*/


	printf("Done!\n");

	saveStream.close();
	//fclose(S);
	return 0;
}
