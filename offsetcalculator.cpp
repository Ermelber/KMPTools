#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <streambuf>
#include <cerrno>
#include <cstdlib>
#include <ctype.h>

using namespace std;



string get_file_contents(const char *filename) {
	ifstream in (filename, ios::in | ios::binary);
	if (in) {
		ostringstream contents;
		contents << in.rdbuf();
		in.close();
		return (contents.str());
	}  else {
				cout << "There isn't a valid file called ''course.kmp'' in your directory.\nMake sure it's there.\n";
				cin.ignore();			
				exit(1);
			}
}

/*
    inserts KMP sections names in the array
*/
void fillNames(string array[])
{
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

void getOffsets(int ofs[],const string nams[],const string kmp)
{
    for (int i=0;i<18;i++)
        ofs[i]=kmp.find(nams[i],0)-88;
        
}


//Patches Header with right values
void patchHeader(string &hd,const int ofs[],int size)
{
    //Patches Filesize
    hd[4]=size%256;
    hd[5]=size/256;
    //Patches Offsets
    int i;
    int j;
    for (i=16,j=0;j<18;i=i+4,j++)
    {
        hd[i]=ofs[j]%256;
        hd[i+1]=ofs[j]/256;
        hd[i+2]=0;
        hd[i+3]=0;
    }
}

int main()
{
    string Names[18];
    fillNames(Names);
    int Offsets[18];
    string Header;
    string keiempi = get_file_contents("course.kmp");
    Header=keiempi.substr(0,88);
    getOffsets(Offsets,Names,keiempi);
    for (int i=0;i<88;i++)
    {
        if ((i%16)==0)
            printf("\n");
        printf("%0.2X ",Header[i]&255);
    }
    printf("\n");
    patchHeader(Header,Offsets,keiempi.size());
    for (int i=0;i<88;i++)
    {
        if ((i%16)==0)
            printf("\n");
        printf("%0.2X ",Header[i]&255);
    }
    cin.ignore();
}
