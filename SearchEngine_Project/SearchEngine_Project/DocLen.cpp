///////////////////////////////////////////////////////////////////////////////////////
//
// Author: Robert Luk
// Year: 2010
// Robert Luk (c) 2010 
//
// Compute Document Lengths: (make sure the InvFile.txt is available)
// This software is made available only to students studying COMP433 (Information
// Retreieval). It should not be used or distributed without consent by the author.
//
////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "IInvFile.h"

// Integrated Inverted Index (see lecture notes on Implementation)
IInvFile InvFile2;

int main2() {
	char tmp[10000];
	char str[1000];
	int docid;
	int loc;
	int cnt=0;

	// Initialize the Hash Table
	InvFile2.MakeHashTable(13023973);

	printf("Loading Inverted File\r\n");
	InvFile2.Load("InvFile.txt");
	printf("Creating Document Records (size = %d)\r\n", InvFile2.MaxDocid+1);
	InvFile2.MakeDocRec();	// allocate document records
	printf("Compute Document Lengths...\r\n");
	InvFile2.DocLen(InvFile2.Files);
	printf("Save Document Lengths\r\n");
	InvFile2.SaveDocRec("InvFile.doc");

	return 0;
}
