#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "IInvFile.h"

//struct _post {
//	int docid;
//	int freq;
//} post;

IInvFile::IInvFile() {
	hsize = 0;
	htable = NULL;
	MaxDocid = 0;
	Files = NULL;
	result = NULL;
	runID = 0;
	outputFile = NULL;
}

IInvFile::~IInvFile() {
	Clear();
}

// Determine the hash value of the string s given the hash table size is h
// hash value is stored in hvalue
int IInvFile::hash(char * s, int h) {
	char * t = s;
	hvalue = 3;
	if (s == NULL) return 0;

	while (*t != '\0') {
		hvalue = (31*hvalue + 57*((int) (unsigned char) *t));
		if (hvalue < 0) hvalue = -1 * hvalue;
		hvalue = hvalue % h;
		t++;
	}

	return hvalue;
}


// Create a hash table of size h
// Initialize this hash table of pointers to NULL
void IInvFile::MakeHashTable(int h) {
	if (h > 0) {
		hsize = h;
		htable = (hnode **) calloc(h, sizeof(hnode *));
		for(int i=0; i < h; i++) {
			htable[i] = NULL;
		}
	}
}

// Delete the hash table entries and the hash table itself
void IInvFile::Clear() {
	hnode * w;
	hnode * oldw;
	post * k;
	post * oldk;
	if ((hsize > 0) && (htable != NULL)) {
		for(int i=0; i < hsize; i++) {
			w = htable[i];
			while (w != NULL) {
				oldw = w;
				k = w->posting;
				while (k != NULL) {
					oldk = k;
					k = k->next;
					free(oldk);
				}
				free(w->stem);
				w = w->next;
				free(oldw);
			}
		}
		free(htable); // delete the entire hash table
		htable = NULL;
		hsize = 0;
	}

	MaxDocid = -1;
	free(Files);
	Files = NULL;
}

// Count the number of postings as the document frequency
int IInvFile::CountDF(post * p) {
	int cnt = 0;
	post * r = p;
	while (r != NULL) {
		cnt++;
		r = r->next;
	}
	return cnt;
}

// Save the data into the file
void IInvFile::Save(char * f) {
	FILE * fp = fopen(f, "wb");		// open the file for writing
	hnode * w;
	post * k;
	if ((hsize > 0) && (htable != NULL)) {	// Is there a hash table?
		for(int i=0; i < hsize; i++) {	// For each hash table entry do
			w = htable[i];		// Go through each hash node
			while (w != NULL) {
				fprintf(fp,"%s %d:", w->stem, w->df);
				k = w->posting;	// Go through each post node
				while (k != NULL) {
					fprintf(fp,"%d %d,", k->docid, k->freq);
					k = k->next;	// next post node
				}
				fprintf(fp,"\r\n");
				w = w->next;		// next hash node
			}
		}
	}
	fclose(fp);				// close the file
}

// Load the data into RAM
void IInvFile::Load(char * f) {
	FILE * fp = fopen(f, "rb");
	hnode * w;
	post * k;
	char c;
	bool next;
	int state = 0;
	int cnt;
	char line[1000];
	char stem[1000];
	int df;
	int i;
	int docid;
	int freq;

	if (fp == NULL) {
		printf("Aborted: file not found for <%s>\r\n",f);
		return;
	}
	if ((hsize > 0) && (htable != NULL)) {
		i = 0;
		cnt = 0;
		do {	next = true;
		if (fread(&c,1,1,fp) > 0) { // read a character
			switch(state) {
			case 0:	if (c != ':') line[i++] = c;
					else {
						line[i] = '\0';
						sscanf(line,"%s %d",stem,&df);
						w = Find(stem);
						if (w == NULL) {
							w = MakeHnode(stem);
							w->df = df;
						}
						i=0;
						state = 1;
						//printf("Read [%s,%d]\r\n",stem, df);
					}
					break;
			case 1:
				if (c == '\r') i=0;
				else
					if (c == '\n') {
						cnt=0;
						i=0;
						state = 0;
					}
					else if (c == ',') {
						line[i] = '\0';
						cnt++;
						sscanf(line,"%d %d",&docid,&freq);
						k = w->posting; // push the data into the posting field
						w->posting = new post;
						w->posting->docid = docid; 
						w->posting->freq = freq;
						w->posting->next = k;
						if (MaxDocid < docid) MaxDocid = docid;
						//printf("[%d] %d %d,\r\n", cnt, docid, freq);
						i=0;
					}
					else line[i++] = c;
					break;				
			}
		}
		else next = false;
		} while (next == true);
	}
	else printf("Aborted: no hash table\r\n");
	fclose(fp);
}

void IInvFile::MakeDocRec() {

	if (MaxDocid > 0) {
		Files = (DocRec *) calloc(MaxDocid+1,sizeof(DocRec));
		for (int i=0;i<=MaxDocid;i++) {
			Files[i].TRECID = NULL;
			Files[i].len = 0.0;
		}
	}
}

float IInvFile::GetIDF(int df) {
	float idf;
	double N = 1 + (double) MaxDocid;
	idf = (float) log10(N / ((double) df)); // Compute IDF
	return idf;
}

// Compute documemt length
void IInvFile::DocLen(DocRec File[]) {
	float idf;
	float idf2;
	hnode * w;
	post * k;

	if ((hsize > 0) && (htable != NULL)) {	// if there is a hash table (for open hashing)
		for(int i=0; i < hsize; i++) {	// Loop through every entry in the hash table
			w = htable[i];		// Get the ith hash table entry
			while (w != NULL) {	// Loop through each hash node (hnode) in the linked list
				idf = GetIDF(w->df);	// Get the IDF value based on the document frequency, df
				idf2 = idf * idf;	// square of IDF
				k = w->posting;		// Loop through each posting in the posting list
				while (k != NULL) {
					if (k->docid <= MaxDocid)
						File[k->docid].len += idf2 * (float) (k->freq * k->freq); // TF*IDF square
					else
						printf("DocLen Error: Docid = %d > Max = %d\r\n",k->docid, MaxDocid);
					k = k->next;	// next posting
				}
				w = w->next;		// next hash node in the linked list
			}
		}
	}
	else printf("Doclen aborted: no hash table\r\n");
	for(int i=0; i <=MaxDocid; i++) File[i].len = (float) sqrt((double) File[i].len);
}

// Save document records
void IInvFile::LoadDocRec(char * f) {
	FILE * fp;
	char line[10000];
	int i =0;
	char str[1000];
	float doclen;

	if ((MaxDocid > 0) && (Files != NULL)) {
		printf("LoadDocRec error: already has document records\r\n");
		return;
	}

	fp = fopen(f, "rb");
	if (fp == NULL) {
		printf("LoadDocRec error: Cannot find file [%s]\r\n",f);
		return;
	}
	if (fgets(line,10000,fp) != NULL) {
		sscanf(line,"%d", &MaxDocid);
		MaxDocid--;
		MakeDocRec();
		while (fgets(line,10000,fp) != NULL) {
			sscanf(line,"%s %e",str, &doclen);
			Files[i].TRECID = strdup(str);
			Files[i].len = doclen;
			if (i > MaxDocid) printf("LoadDocRec error: MaxDocid incorrect [%d,MaxDocid=%d]\r\n", i,MaxDocid);
			i++;
		}
	}
	fclose(fp);
}

// Save document records
void IInvFile::SaveDocRec(char * f) {
	FILE * fp;
	if ((MaxDocid > 0) && (Files != NULL)) {
		fp = fopen(f, "wb");
		fprintf(fp,"%d\r\n",MaxDocid+1);
		for(int i=0;i<=MaxDocid;i++) {
			if (Files[i].TRECID == NULL)
				fprintf(fp,"%d %e\r\n",i,Files[i].len);
			else
				fprintf(fp,"%s %e\r\n",Files[i].TRECID, Files[i].len);
		}
		fclose(fp);
	}

}

// Find the hnode structure of the string s
// If no such structure exist, return NULL
hnode * IInvFile::Find(char * s) {
	hnode * r;
	int h = hash(s, hsize);		// Get the hash value
	if (htable != NULL) {		// there is a hashtable
		r = htable[h];		// Get the first pointer to the linked list
		while (r != NULL) {	// Loop through each hash node
			if (strcmp(r->stem,s) == 0)
				return r;	// found the hash node with same stem
			else r = r->next;	// next hash node
		}
		return r;
	}
	else printf("No hash table! \r\n");
	return NULL;
}

// Insert an hnode structure into the hash table
// Initialize the structure with the string s
hnode * IInvFile::MakeHnode(char * s) {
	hnode * r = new hnode;
	r->stem = strdup(s);
	r->posting = NULL;
	r->df = 0;
	r->next = htable[hvalue];
	htable[hvalue]=r;
	return r;
}


// Find the posting that has the document id
// Assume postings are in reverse order of document ids (i.e., 5, 4, 3, ...)
post * IInvFile::FindPost(hnode * w, int docid) {
	post * k = w->posting;

	if (k->docid == docid) return k;
	else return NULL;

	//while (k != NULL) {
	//	if (k->docid == docid) return k;
	//	else if (k->docid > docid) return NULL;
	//	else k = k->next;
	//}
	//return k;
}

// Add a posting
post * IInvFile::Add(char * s, int docid, int freq) {
	hnode * w = Find(s);	// Does the stem exist in the dictionary?
	post * k = NULL;

	if (w == NULL) w = MakeHnode(s); // If not exist, create a new hash node
	else k = FindPost(w, docid);	// if exists, is the first posting the wanted one?

	if (k == NULL) {			// no posting has the same docid
		k = w->posting; 		// push the data into the posting field
		w->posting = new post;		// create a new posting record
		w->posting->docid = docid;	// save the document id
		w->posting->freq = freq;	// save the term frequency
		w->df += 1;			// keep track of the document frequency
		w->posting->next = k;		// push the data into the posting field
	}
	else	k->freq += freq;	// The posting exists, so add the freq to the freq field

	return k;
}

// Get next query term
char * IInvFile::GotoNextWord(char * s) {
	char * q = s;
	if ((s == NULL) || (*s == '\0')) return NULL;
	while ((*q == ' ') && (*q == '\t')) q++;
	while ((*q != ' ') && (*q != '\0')) q++;
	if (*q == ' ') {
		*q = '\0';
		q++;
	}
	return q;
}

// Combine partial retrieval results
void IInvFile::CombineResult(RetRec * r, post * kk, float idf) {
	post * k = kk;				// Get the pointer to the posting list
	int docid;

	while (k != NULL) {
		docid = k->docid;
		if (docid > MaxDocid) printf("CombineResult error: Docid = %d > MaxDocID = %d\r\n", k->docid, MaxDocid);
		r[docid].docid = docid;			// make sure we store the document id
		r[docid].sim += idf * (float) k->freq;	// add the partial dot product score	
		k = k->next;				// next posting
	}

}

// Comparison function used by qsort(.): see below
int compare(const void * aa, const void * bb) {
	RetRec * a = (RetRec *) aa;
	RetRec * b = (RetRec *) bb;

	if (a->sim > b->sim) 
	{
		return -1;
	}
	else if (a->sim < b->sim)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

// Print top N results
void IInvFile::PrintTop(int queryID, RetRec * r, int top) {
	int i = MaxDocid + 1;
	qsort(r, MaxDocid + 1, sizeof(RetRec), compare); // qsort is a C function: sort results
	i = 0;
	printf("Search Results:\r\n");

	while (i < top)
	{
		if (((r[i].docid == -1) && (r[i].sim == 0)) || !(i < MaxDocid + 1))
		{
			return; // no more results; so exit
		}
		fprintf(outputFile, "%d\tQ0\t%s\t%d\t%e\tHKPU-%d\r\n",queryID, Files[r[i].docid].TRECID, i + 1, r[i].sim, runID);
		i++;
	}
}

// Perform retrieval
void IInvFile::Search(char * q)
{
	int i = -1;
	char * s = q;
	char * w;
	bool next = true;
	hnode * h;

	float qsize = 0.0;	//query size

	int queryID = -1;

	// Initialize the result set
	if (result != NULL)
	{
		free(result);
	}
	result = (RetRec *) calloc(MaxDocid+1, sizeof(RetRec));

	for (int i = 0;i < MaxDocid + 1; ++i)
	{
		result[i].docid = -1;
		result[i].sim = 0;
	}

	char line[5000] = "\0";

	for (i = 0;q[i] != ' ';++i)
	{
		line[i] = q[i];
	}

	queryID = atoi(line);

	strcpy(line,&(q[i + 1]));

	s = line;

	do {
		w = s;					// Do searching
		s = GotoNextWord(s);			// Delimit the term
		if (s == NULL)
		{
			next = false;		// If no more terms, exit
		}
		else 
		{
			if (*s != '\0') 
			{
				*(s-1) = '\0';	// If not the last term, delimit the term
			}
			Stemming.Stem(w);		// Stem the term w
			h = Find(w);			// Find it in the integrated inverted index
			if (h != NULL)			// Add the scores to the result set
			{
				CombineResult(result, h->posting, GetIDF(h->df));
				qsize += 1.0;
			}
			else if (strlen(w) > 0) 
			{
				printf("Query term does not exist <%s>\r\n",w);
			}
		}
	} while (next == true);				// More query terms to handle?

	normalize(result, qsize);

	PrintTop(queryID, result, 1000);				// Print top 10 retrieved results
}

void IInvFile::normalize(RetRec* r, float qsize)
{
	float qlen = sqrt(qsize);
	int docid;

	for (int i = 0;i < MaxDocid + 1; ++i)
	{
		docid = r[i].docid;
		r[i].sim = r[i].sim / Files[docid].len / qlen;
	}
}

bool IInvFile::BuildInvFile(char* postfilepath)
{
	char tmp[10000];
	char str[1000];
	int docid;
	int loc;
	int cnt=0;

	FILE * fp = fopen(postfilepath, "rb");
	if (fp == NULL) {
		printf("Cannot open file \r\n");
		return false;
	}

	// Initialize the Hash Table
	this->MakeHashTable(13023973);

	while(fgets(tmp,10000,fp) != NULL) {
		// Get the stem, the document identifier and the location
		sscanf(tmp,"%s %d %d", &(str[0]), &docid, &loc);

		// Add posting into the Integrated Inverted index
		// See lecture notes on Implementation
		this->Add(str, docid, 1);

		// Keep us informed about the progress
		cnt++;
		if ((cnt % 100000) == 0) printf("Added [%d]\r\n",cnt);
	}

	printf("Saving inverted file ...\r\n");
	this->Save("InvFile.txt");

	this->Clear();		//It is unnessary to clear all hashtable
	fclose(fp);
	return true;
}

void IInvFile::getRetrieval()
{
	bool next = true;
	char line[10000];
	int queryID = -1;
	char cmd[10000];

	// Initialize the Hash Table
	this->MakeHashTable(13023973);		//HashTable has been made in the function of building inverted file

	printf("Loading Inverted File\r\n");	//Hash Table has been created when building inverted file
	this->Load("InvFile.txt");

	printf("Load Document Lengths\r\n");
	this->LoadDocRec("InvFile.doc");

	printf("Read TREC File\r\n");
	this->ReadTRECID("E:\\Material of Exchange Student\\course\\COMP433 Information Retrieval\\All\\1. Assignment\\1.1. Assignment\\1.1.7. FileID-to-DocumentID mapping\\assign\\file.txt");
	
	FILE * queryFile;

	//queryFile = fopen("queryT","rb");

	queryFile = fopen("E:\\Material of Exchange Student\\course\\COMP433 Information Retrieval\\All\\1. Assignment\\1.1. Assignment\\1.1.4. Query files and Judgment File\\queryT","rb");

	if (queryFile == NULL)
	{
		printf("Error : query file can not open!\n");
		return;
	}

	outputFile = fopen("searchOutput.txt","w");

	while (fgets(cmd, 10000, queryFile) != NULL)
	{
		while (10 == cmd[strlen(cmd) - 1] || 13 == cmd[strlen(cmd) - 1])
		{
			cmd[strlen(cmd) - 1] = '\0';
		}

		next = this->Retrieval(cmd);
	}


	//do {
	//	//printf("Type the query or \"_quit\" to exit\r\n");
	//	// Start interactive retrieval

	//	fgets(cmd, 10000, queryFile);

	//	while (10 == cmd[strlen(cmd) - 1] || 13 == cmd[strlen(cmd) - 1])
	//	{
	//		cmd[strlen(cmd) - 1] = '\0';
	//	}

	//	next = this->Retrieval(cmd);
	//} while (next == true);

	runID++;

	this->Clear();
}

// Interactive retrieval
bool IInvFile::Retrieval(char* query) {
	if (/*strcmp(query,"_quit") == 0*/ query == NULL)
	{
		return false;
	}
	else
	{
		Search(query);
		return true;
	}
}

void IInvFile::CalculateDocLen()
{
	this->MakeHashTable(13023973);

	printf("Loading Inverted File\r\n");
	this->Load("InvFile.txt");

	printf("Creating Document Records (size = %d)\r\n", this->MaxDocid+1);
	this->MakeDocRec();	// allocate document records

	printf("Compute Document Lengths...\r\n");
	this->DocLen(this->Files);

	printf("Save Document Lengths\r\n");
	this->SaveDocRec("InvFile.doc");

	this->Clear();
}

void IInvFile::ReadTRECID(char* f)
{
	char line[10000];
	char str[1000];
	char TRECID[1000];

	int docid = -1;
	int len = -1;

	FILE * fp = fopen(f,"rb");
	if (fp == NULL)
	{
		printf("Error : no file <%s>", f);
		return;
	}

	for (int i = 0;i < MaxDocid + 1 && fgets(line, 10000, fp) != NULL ; ++i)
	{
		sscanf(line,"%d %d %s %s", &docid,&len,str,TRECID);
		Files[docid].TRECID = strdup(TRECID);
	}

	fclose(fp);
}