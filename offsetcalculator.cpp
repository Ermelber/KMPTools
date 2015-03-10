#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <streambuf>
#include <cerrno>
#include <cstdlib>
#include <ctype.h>
#include <iomanip>

using namespace std;

string get_file_contents(const char *filename) {
	ifstream in (filename, ios::in | ios::binary);
	if (in) {
		ostringstream contents;
		contents << in.rdbuf();
		in.close();
		return (contents.str());
	}  else {
		cout << "KMP file not found.\n";
		cin.ignore();
		exit(1);
	}
}

/*
    inserts KMP sections names in the array
*/
void fillNames(string array[]) {
	array[0]="TPTK";
	array[1]="TPNE";
	array[2]="HPNE";
	array[3]="TPTI";
	array[4]="HPTI";
	array[5]="TPKC";
	array[6]="HPKC";
	array[7]="JBOG";
	array[8]="ITOP";
	array[9]="AERA";
	array[10]="EMAC";
	array[11]="TPGJ";
	array[12]="TPNC";
	array[13]="TPSM";
	array[14]="IGTS";
	array[15]="SROC";
	array[16]="TPLG";
	array[17]="HPLG";
}

void getOffsets(int ofs[],const string nams[], const string kmp) {
	for (int i=0;i<18;i++)
		ofs[i]=kmp.find(nams[i],0)-88;
}


// Patches Header with right values
void patchHeader(string &hd,const int ofs[],int size) {
	// Patches Filesize
	hd[4]=size%256;
	hd[5]=size/256;
	
	// Patches Offsets
	int i;
	int j;
	for (i=16,j=0;j<18;i=i+4,j++) {
		hd[i]=ofs[j]%256;
		hd[i+1]=ofs[j]/256;
		hd[i+2]=0;
		hd[i+3]=0;
	}
}

void Help(char *arg) {
	cout << "Usage:\n\n";
	
	cout << arg << " -h\t\t-\tDisplay help.\n";
	cout << arg << " -r [FILE].kmp\t-\tRead a KMP File.\n";
	cout << arg << " -w txt [FILE].kmp\t-\tWrite a KMP File to TXT.\n";
	cout << arg << " -w kmp [FILE].txt\t-\tWrite a TXT File to KMP.\n";
}

void Read(const char *file) {
	string Names[18];
	fillNames(Names);
	int Offsets[18];
	string Header;
	
	string keiempi = get_file_contents(file);
	Header = keiempi.substr(0, 88);
	getOffsets(Offsets, Names, keiempi);
	for (int i = 0; i < 88; i++) {
		if ((i%16) == 0)
			printf("\n");
		printf("%0.2X ", Header[i]&255);
	}
		
	printf("\n");
	patchHeader(Header, Offsets, keiempi.size());
	for (int i = 0; i < 88; i++) {
		if ((i%16) == 0)
			printf("\n");
		printf("%0.2X ", Header[i]&255);
	}
}

void Write(const char *file, char *ext) {
	string Names[18];
	fillNames(Names);
	int Offsets[18];
	string Header;
	
	string keiempi = get_file_contents(file);
	Header = keiempi.substr(0, 88);
	getOffsets(Offsets, Names, keiempi);
	
	patchHeader(Header, Offsets, keiempi.size());
	
	string fname;
	stringstream str;
	
	if (string(ext) == "txt") {
		str << file;
		fname = str.str();
		fname += ".txt";
		
		ofstream out(fname.c_str());
		
		if (out.fail()) {
			printf("Failed to create TXT.\n");
		}
		else {
			printf("KMP to TXT success.\n");
		}
		
		for (int i = 0; i < 88; i++) {
			if ((i%16) == 0)
				out << endl;
			out << hex << uppercase << setw(2) << setfill('0') << int(Header[i]&255);
		}
	}
	else if (string(ext) == "kmp") {
		printf("KMP Writing coming soon.\n");
	}
	else {
		printf("Invalid File Format. Only available as \"kmp\" or \"txt\".\n");
	}
}

int main(int argc, char *argv[]) {
	printf("KMPTools 0.1 by MKGirlism and Ermelber\n\n");
	
	while (true) {
		if (argv[1] == NULL) {
			Help(argv[0]);
			break;
		}
		else if (string(argv[1]) == "-h") {
			argv[1] = NULL;
			continue;
		}
		else if (string(argv[1]) == "-r") {
			Read(argv[2]);
			break;
		}
		else if (string(argv[1]) == "-w") {
			Write(argv[3], argv[2]);
			break;
		}
		else {
			argv[1] = NULL;
			continue;
		}
	}
	
	printf("\nPress any key to continue...\n");
	cin.ignore();
}
