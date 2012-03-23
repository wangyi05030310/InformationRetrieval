///////////////////////////////////////////////////////////////////////////////////////
//
// Author: Robert Luk
// Year: 2010
// Robert Luk (c) 2010 
//
// Convert data into inverted file format using the Integrated Inverted Index class:
// This software is made available only to students studying COMP433 (Information
// Retreieval). It should not be used or distributed without consent by the author.
//
////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "IInvFile.h"
namespace SearchEngine_Project {
// Integrated Inverted Index (see lecture notes on Implementation)
IInvFile InvFile4;

int main3() {
	char tmp[10000];
	char str[1000];
	int docid;
	int loc;
	int cnt=0;
	FILE * fp = fopen("", "rb");
	if (fp == NULL) {
		printf("Cannot open file \r\n");
		return 1;
		}

	// Initialize the Hash Table
	InvFile4.MakeHashTable(13023973);

	while(fgets(tmp,10000,fp) != NULL) {
		// Get the stem, the document identifier and the location
		sscanf(tmp,"%s %d %d", &(str[0]), &docid, &loc);

		// Add posting into the Integrated Inverted index
		// See lecture notes on Implementation
		InvFile4.Add(str, docid, 1);

		// Keep us informed about the progress
		cnt++;
		if ((cnt % 100000) == 0) printf("Added [%d]\r\n",cnt);
	}

	printf("Saving inverted file ...\r\n");
	InvFile4.Save("InvFile.txt");

	InvFile4.Clear();
	fclose(fp);
	return 0;
}
}