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

int getOffsets(int ofs[],const string nams[],const string kmp,int size)
{
    int j=0;
    string tempstr;
    for (int i=0;i<size;i++)
    {   
        tempstr=(kmp[i]+kmp[i+1]+kmp[i+2]+kmp[i+3]);
        if (tempstr==nams[j])
        {
            ofs[j]=i;
            j++;
        } 
    }
}

int main()
{
    string Names[18];
    int Offsets[18];
    fillNames(Names);
    string keiempi = get_file_contents("course.kmp");
    //getOffsets(Offsets,Names,keiempi,filesize);
    cout << (int)keiempi[88] << "\n";
    cout << keiempi.size() << "\n";
    cin.ignore();
}
