/*
 * MultithreadPasswordEncrypt.cpp
 */
#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include <pthread.h>
#include <cstring>
using namespace std;
vector<string> encrypted;

int main()
{
	void *encryptPass(void*);
	void savePasswordList(vector<string> s);
	//void readPasswordList(string s);


	int numThreads=0;
	string filename, line;
	ifstream passwordfile;
	vector<string> passwordlist;


	char tid[10]={0,1,2,3,4,5,6,7,8,9};

	cout<<"please enter file name: "<<endl;
	cin>>filename;
	passwordfile.open(filename);
	if(passwordfile.is_open())
	{
		cout<<"file opened successfully"<<endl;
	}
	else
	{
		cerr<<"file not found"<<endl;
		return 0;
	}
	while ( getline (passwordfile,line) )
	{
		passwordlist.push_back(line);
	}
	passwordfile.close();

	cout<<"how many threads?"<<endl;
	cin>>numThreads;

	int numPasswords=					passwordlist.size();
	int numPasswordsPerThread=			numPasswords/numThreads;
	int numRemainder=					numPasswords%numThreads;

	cout<<" numThreads: "<< numThreads<< " numPasswords: "<<numPasswords<<" numPasswordsPerThread: "<<numPasswordsPerThread<<" numRemainder: "<<numRemainder<<endl;

	while(numPasswords>1)
	{
		int tidNum=numPasswords%numThreads;
		cout<<"Thread id: "<<numPasswords%numThreads<<" Password: "<<passwordlist.size()-numPasswords<<endl;

		//create a thread id & attributes
		pthread_t tid[tidNum];
		pthread_attr_t attr;
		pthread_attr_init(&attr);   //set attributes
		pthread_create(&tid[tidNum], &attr, encryptPass,(void *) passwordlist[passwordlist.size()-numPasswords].c_str()); //pw);   //start thread
		pthread_join(tid[tidNum],NULL);

		numPasswords--;
	}
	cout<<"Saving password list..."<<endl;
	savePasswordList(encrypted);
	cout<<"Done"<<endl;
	return 0;
}


void *encryptPass(void *param)
{
    //cast the incoming void pointer (param) to our struct data
	char * pw = static_cast<char*>(param);
	cout<<"encrypting "<< pw<<"..."<<endl;

	encrypted.push_back(pw);
	int pwLength=strlen(pw);
	char temp=pw[0];
	for( int i=0; i<pwLength; i++)
	{
		pw[i]=pw[i+1];
		pw[i]=pw[i]+5;
	}
	pw[pwLength-1]=temp+5;

    cout<<"finished... new password: "<< pw<<endl;
    encrypted.push_back(pw);

    pthread_exit(0);
}

void savePasswordList(vector<string> s)
{
	ofstream file;
	file.open("encryptedlist");
	for(int i=0;i<s.size();++i)
	{
		file<<s[i]<<endl;
	}
	file.close();
}
//void readPasswordList(string s)
//{
//}

