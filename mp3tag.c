#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mp3tag.h"

int readID3FromMP3(const char *filename, struct ID3Info_s *ID3Info)
{
	FILE *file;
	file = fopen(filename, "r");
	char ID[4];
	if (!file) {
		printf("%s: Error open file:%s.\n", __func__, filename);
		return -1;
	}
	// setting 'cursor' to 128bytes before EOF
	fseek(file, -128, SEEK_END);
	// checking to see wether it is an actual MP3 file
	fread(ID,sizeof(char),3,file);
	ID[3] = '\0';
	if (strcmp(ID,"TAG")) {
		fclose(file);
		return -1;
	}
	// parse info
	fread(ID3Info->songTitle,sizeof(char),30,file);
	ID3Info->songTitle[30] = '\0';
	fread(ID3Info->artist,sizeof(char),30,file);
	ID3Info->artist[30] = '\0';
	fread(ID3Info->album,sizeof(char),30,file);
	ID3Info->album[30] = '\0';
	fread(ID3Info->year,sizeof(char),30,file);
	ID3Info->year[30] = '\0';
	fread(ID3Info->comment,sizeof(char),30,file);
	ID3Info->comment[30] = '\0';
	fread(&(ID3Info->genre),sizeof(char),1,file);
	fclose(file);
	return 0;
}

void printID3Tag(struct ID3Info_s *ID3Info)
{
	// printf("%-10s\t\t%d\t\t\t\t%f\t\t\t\t\t%d\n"
	printf("%-30s\t%-30s\t%-30s\n",ID3Info->artist,ID3Info->songTitle,ID3Info->album);
}
