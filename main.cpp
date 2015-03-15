// SetEvent();
//Sleep();
#include"email.h"
#include"mailbox.h"
#include <windows.h>
#include <fstream>
#include <sstream>
#include <process.h>
#include <iostream>
#define MAXNUMOFMAILS 100
using namespace std;

CRITICAL_SECTION critical;
CRITICAL_SECTION rec_cr;
int recieved_no = 0, mail_no = 0;
void mailboxthread(LPDWORD x);
void mailmenAthread(LPDWORD x);
void mailmenBthread(LPDWORD x);
email* MergeSort2(email* d,int n){ //Ye Merge sorte khub ke email ha ro ruye timeshun soudi sort mikone...
	if(n == 1)
		return d;
	email* dsort = new email[n];
	email* dtemp1;
	email* dtemp2;
	dtemp1 = MergeSort2(d,n/2);
	dtemp2 = MergeSort2(d+n/2,n-n/2);
	int p1 = 0;
	int p2 = 0;
	while(p1 != n/2 && p2 != n-n/2)
		if(dtemp1[p1].timeofsend <= dtemp2[p2].timeofsend){
			dsort[p1+p2].senderID = dtemp1[p1].senderID; 	
			dsort[p1+p2].timeofsend = dtemp1[p1].timeofsend;
			dsort[p1+p2].receiverID = dtemp1[p1].receiverID;
			dsort[p1+p2].textmail = dtemp1[p1].textmail;
			p1++;
		}
		else{
			dsort[p1+p2].senderID = dtemp2[p2].senderID; 	
			dsort[p1+p2].timeofsend = dtemp2[p2].timeofsend;
			dsort[p1+p2].receiverID = dtemp2[p2].receiverID;
			dsort[p1+p2].textmail = dtemp2[p2].textmail;
			p2++;
		}
	if(p1 == n/2)
		while(p2 != n-n/2){
			dsort[p1+p2].senderID = dtemp2[p2].senderID; 	
			dsort[p1+p2].timeofsend = dtemp2[p2].timeofsend;
			dsort[p1+p2].receiverID = dtemp2[p2].receiverID;
			dsort[p1+p2].textmail = dtemp2[p2].textmail;
			p2++;
		}
	else
		while(p1 != n/2){
			dsort[p1+p2].senderID = dtemp1[p1].senderID; 	
			dsort[p1+p2].timeofsend = dtemp1[p1].timeofsend;
			dsort[p1+p2].receiverID = dtemp1[p1].receiverID;
			dsort[p1+p2].textmail = dtemp1[p1].textmail;
			p1++;
		}
	return &dsort[0];
}
email* MergeSort(email* d,int n){//Ye Merge sorte khub ke email ha ro ruye sender-ID soudi sort mikone...
	if(n == 1)
		return d;
	email* dsort = new email[n];
	email* dtemp1;
	email* dtemp2;
	dtemp1 = MergeSort(d,n/2);
	dtemp2 = MergeSort(d+n/2,n-n/2);
	int p1 = 0;
	int p2 = 0;
	while(p1 != n/2 && p2 != n-n/2)
		if(dtemp1[p1].senderID <= dtemp2[p2].senderID){
			dsort[p1+p2].senderID = dtemp1[p1].senderID; 	
			dsort[p1+p2].timeofsend = dtemp1[p1].timeofsend;
			dsort[p1+p2].receiverID = dtemp1[p1].receiverID;
			dsort[p1+p2].textmail = dtemp1[p1].textmail;
			p1++;
		}
		else{
			dsort[p1+p2].senderID = dtemp2[p2].senderID; 	
			dsort[p1+p2].timeofsend = dtemp2[p2].timeofsend;
			dsort[p1+p2].receiverID = dtemp2[p2].receiverID;
			dsort[p1+p2].textmail = dtemp2[p2].textmail;
			p2++;
		}
	if(p1 == n/2)
		while(p2 != n-n/2){
			dsort[p1+p2].senderID = dtemp2[p2].senderID; 	
			dsort[p1+p2].timeofsend = dtemp2[p2].timeofsend;
			dsort[p1+p2].receiverID = dtemp2[p2].receiverID;
			dsort[p1+p2].textmail = dtemp2[p2].textmail;
			p2++;
		}
	else
		while(p1 != n/2){
			dsort[p1+p2].senderID = dtemp1[p1].senderID; 	
			dsort[p1+p2].timeofsend = dtemp1[p1].timeofsend;
			dsort[p1+p2].receiverID = dtemp1[p1].receiverID;
			dsort[p1+p2].textmail = dtemp1[p1].textmail;
			p1++;
		}
	return &dsort[0];
}
email** inputfilescanner(vector<int>& numofmailbesend,int *numofbox,int *numofmenA,int *numofmenB){//info.txt ra scan mikonad...
	fstream in;
	in.open("info.txt",fstream::in);
	email** mailshouldbesend;
	int i,j,k,l,n;
	in >> numofbox[0];			//number of mailboxes...
	in >> numofmenA[0];			//number of mailmen by type A...
	in >> numofmenB[0];			//number of mailmen by type B...
	email* mail = new email[MAXNUMOFMAILS];
	i = 0;
	char* a = new char[200];
	while(!in.eof()){			//Scan lines 4 to ...
		in >> mail[i].senderID >> mail[i].timeofsend >> mail[i].receiverID;	
		mail_no++;
		in.getline(a,40);
		mail[i].textmail = "";
		for(j = 0;a[j] != NULL;j++){
			mail[i].textmail += a[j];
		}
		i++;
	}
	mail = MergeSort(MergeSort2(mail,i),i);//Sort mails by sender ID and time sent...
	l = 0;
	mailshouldbesend = new email*[numofbox[0]];
	int* _numofmailbesend = new int[numofbox[0]];
	
	for(n = 1;n < mail[0].senderID;n++){
		mailshouldbesend[n-1] = NULL;
		_numofmailbesend[n-1] = 0;
	}
	k = mail[0].senderID;
	for(j = 0;j < i;j++)					//Move mails from email* mails to email** mailshouldbesend...
		if(mail[j].senderID == k){
			l++;
			continue;
		}
		else{
			if(l){
				mailshouldbesend[k-1] = new email[l];
				for(n = j-1;n >= j-l;n--){
					mailshouldbesend[k-1][n-j+l].senderID = mail[n].senderID;
					mailshouldbesend[k-1][n-j+l].receiverID = mail[n].receiverID;
					mailshouldbesend[k-1][n-j+l].textmail = mail[n].textmail;
					mailshouldbesend[k-1][n-j+l].timeofsend = mail[n].timeofsend;
				}				
			}
			else
				mailshouldbesend[k-1] = NULL;
			_numofmailbesend[k-1] = l;
			l = 0;
			k++;
			j--;
		}
	if(l){
		mailshouldbesend[k-1] = new email[l];
		for(n = j-1;n >= j-l;n--){
			mailshouldbesend[k-1][n-j+l].senderID = mail[n].senderID;
			mailshouldbesend[k-1][n-j+l].receiverID = mail[n].receiverID;
			mailshouldbesend[k-1][n-j+l].textmail = mail[n].textmail;
			mailshouldbesend[k-1][n-j+l].timeofsend = mail[n].timeofsend;
		}
		_numofmailbesend[k-1] = l;
	}
	k = mail[i-1].senderID;
	for(n = k+1;n <= numofbox[0];n++){
		mailshouldbesend[n-1] = NULL;
		_numofmailbesend[n-1] = 0;
	}

	for(i = 0; i < numofbox[0]; i++)
		numofmailbesend.push_back(_numofmailbesend[i]);

	return mailshouldbesend;
}

struct mailboxthreadparam
{
	mailbox* mbox;
	email* shouldsend;
	int n;
};

struct menparam
{
	mailbox* box;
	vector<email>* sharedq;
	int n;
}postmenparam;

void main(){									//Main function of the program...
	InitializeCriticalSectionAndSpinCount(&critical,0x80000400);
	InitializeCriticalSectionAndSpinCount(&rec_cr,0x80000400);
	
	int *numofbox = new int[1];
	int*numofmenA = new int[1];
	int*numofmenB = new int[1];
	HANDLE* handle;
	DWORD dwret;
	int i,j;
	vector<email> postofficequeue;
	email** mailshouldbesend = NULL;
	vector<int> numofmailsbesend;
	mailshouldbesend = inputfilescanner(numofmailsbesend,numofbox,numofmenA,numofmenB);
	int numbox = numofbox[0],numA = numofmenA[0],numB = numofmenB[0];
	handle = new HANDLE[numbox+numA+numB];
	DWORD* tid = new DWORD[numbox+numA+numB];
	mailbox* box = new mailbox[numbox];

	struct mailboxthreadparam* myparam = new struct mailboxthreadparam[numbox];
	
	for(i = 0; i < numbox; i++)				//creating mailbox thread...
		if(numofmailsbesend[i]){
			myparam[i].n = numofmailsbesend[i];
			myparam[i].mbox = (box+i);
			myparam[i].shouldsend = mailshouldbesend[i];
			handle[i] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)mailboxthread,&myparam[i],0,&tid[i]);
		}

	Sleep(1);
	postmenparam.box = box;
	postmenparam.sharedq = &postofficequeue;
	postmenparam.n = numbox;


	for(i = numbox; i < numbox + numA; i++)//creating Mailmen type A...
		handle[i] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)mailmenAthread,&postmenparam,0,&tid[i]);

	Sleep(1);
	for(i = numbox + numA ; i < numbox + numA + numB; i++)//creating Mailmen type A...
		handle[i] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)mailmenBthread,&postmenparam,0,&tid[i]);
	
	Sleep(1500);
	dwret = WaitForMultipleObjects(numbox+numA+numB,handle,TRUE,INFINITE);		//waits for all the thread created above...
	//for(i = 0; i < numbox + numA + numB; i++)
	//	CloseHandle(handle[i]);
	
	fstream* out = new fstream[numbox];
	string filename;
	for(i = 0; i < numbox; i++)				//writing inboxes of mailboxes to files "#.txt"...
	{
		filename = "";
		stringstream maziar;
		maziar << i+1 << ".txt";
		filename = maziar.str();
		out[i].open(filename.c_str(), fstream::out);
		for(j = 0; j < (int)box[i].inbox.size(); j++)
			out[i] << box[i].inbox[j].senderID << " " << box[i].inbox[j].timeofsend << " " << box[i].inbox[j].receiverID << " " << box[i].inbox[j].textmail << endl;
		out[i].close();
	}
	
}

void mailmenAthread(LPDWORD x)				//pointer e in function be thread e mail men e type A pass dade mishavad...  
{
	cout << "A sakht" << endl;
	int i;
	email mymail;
	struct menparam a = *((struct menparam*)x);

	while(TRUE)
	{
		Sleep(1);
		for(i = 0; i < a.n; i++)
		{
			EnterCriticalSection(&a.box[i].outcr);
			if(!a.box[i].outbox.empty())
			{
				mymail = a.box[i].outbox[0];
				a.box[i].outbox.erase(a.box[i].outbox.begin());

				EnterCriticalSection(&critical);
				(*a.sharedq).push_back(mymail);
				LeaveCriticalSection(&critical);
				
			}
			LeaveCriticalSection(&a.box[i].outcr);
		}

		if(mail_no == recieved_no)
			break;
	}
}

void mailmenBthread(LPDWORD x)							//pointer e in function be thread e mail men e type B pass dade mishavad...
{
//	int i;
	cout << "B sakht" << endl;
	email mymail;
	struct menparam a = *((struct menparam*)x);

	while(TRUE)
	{
		Sleep(1);
		EnterCriticalSection(&critical);						//Start critical sections
		
		if(!(*a.sharedq).empty())
		{
			cout << "receiver id" << (*a.sharedq)[0].receiverID << endl;
			//Sleep(2);
			mymail = (*a.sharedq)[0];
			(*a.sharedq).erase((*a.sharedq).begin());

			EnterCriticalSection(&a.box[mymail.receiverID-1].incr);//Start critical sections
			a.box[mymail.receiverID-1].inbox.push_back(mymail);
			EnterCriticalSection(&rec_cr);							//Start critical sections
			recieved_no++;
			LeaveCriticalSection(&rec_cr);							//Finish critical sections
			LeaveCriticalSection(&a.box[mymail.receiverID-1].incr);//Finish critical sections
		}
		LeaveCriticalSection(&critical);							//Finish critical sections
		if(mail_no == recieved_no)
			break;
	}
}

void mailboxthread(LPDWORD x)//pointer e in function be thread e mailbox pass dade mishavad...
{
	int i = 0;
	time_t t;
	struct mailboxthreadparam a = *((struct mailboxthreadparam*)x);
	cout << "radife" << a.n << endl;
	mailbox* mb = a.mbox;
	time_t pt = 0;
	
	for(i = 0; i < a.n; i++)
	{
		t = a.shouldsend[i].timeofsend;
		Sleep((DWORD)(t-pt));
		(mb->outbox).push_back(a.shouldsend[i]);
		pt = t;
	}

}

