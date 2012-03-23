#include <iostream>
#include "IInvFile.h"

using namespace std;

int main ()
{
	IInvFile InvFile;
	char post_file_path[1000];

	cout << "Please input the path of the post file:" << endl;
	gets(post_file_path);

	InvFile.BuildInvFile(post_file_path);
	InvFile.CalculateDocLen();
	InvFile.getRetrieval();

	return 0;
}