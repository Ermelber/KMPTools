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

typedef unsigned char uchar;

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

float bytesToFloat(uchar b0, uchar b1, uchar b2, uchar b3)
{
    float output;

    *((uchar*)(&output) + 3) = b0;
    *((uchar*)(&output) + 2) = b1;
    *((uchar*)(&output) + 1) = b2;
    *((uchar*)(&output) + 0) = b3;

    return output;
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
             out << "#KMP Written by KMPTools by MKGirlism and Ermelber" << endl << endl;

#define NICEHEX setw(2) << setfill('0')
#define FCAST (unsigned)(unsigned char)

		int numpt,pos,length,j;
		     for (j = 0; j < 18; j++)
             {
			     pos = keiempi.find(Names[j],0)+8;
			     length = keiempi.find(Names[j+1],0)-pos;
			     switch (j)
			     {
                        //TPTK
                        case 0:
                            out << "@KTPT" << endl << "#Kart Points" << endl;
                            break;
                        //TPNE
                        case 1:
                            numpt = length/24;
                            //unsigned int x,y,z,scale;
                            out << "@ENPT" << endl << "#Enemy Points" << endl << "#Number of Points="<< numpt << endl;
                            out << "#ID:	X:	    	Y:	     	Z:	     	SCALE:	 	UNKNOWN1:	UNKNOWN2:" << endl;
                            for (int i = 0; i < numpt; i++)
                            {
                                out << i << "\t";
                                //X
                                out << left << setw(8) << bytesToFloat(keiempi[pos+(24*i)+3],keiempi[pos+(24*i)+2],keiempi[pos+(24*i)+1],keiempi[pos+(24*i)]) << "\t";
                                //Y
                                out << left << setw(8) << bytesToFloat(keiempi[pos+(24*i)+7],keiempi[pos+(24*i)+6],keiempi[pos+(24*i)+5],keiempi[pos+(24*i)+4])<< "\t";
                                //Z
                                out << left << setw(8) << bytesToFloat(keiempi[pos+(24*i)+11],keiempi[pos+(24*i)+10],keiempi[pos+(24*i)+9],keiempi[pos+(24*i)+8])<< "\t";
                                //scale
                                out << left << setw(8) << bytesToFloat(keiempi[pos+(24*i)+15],keiempi[pos+(24*i)+14],keiempi[pos+(24*i)+13],keiempi[pos+(24*i)+12])<< "\t";
                                //unknown1
                                out << hex << uppercase << NICEHEX << FCAST keiempi[pos+(24*i)+16] << NICEHEX << FCAST keiempi[pos+(24*i)+17] << NICEHEX << FCAST keiempi[pos+(24*i)+18] << NICEHEX << FCAST keiempi[pos+(24*i)+19] << "\t";
                                //unknown2
                                out << hex << uppercase << NICEHEX << FCAST keiempi[pos+(24*i)+20] << NICEHEX << FCAST keiempi[pos+(24*i)+21] << NICEHEX << FCAST keiempi[pos+(24*i)+22] << NICEHEX <<  FCAST keiempi[pos+(24*i)+23];
				out << endl << dec;
                            }
                            break;
                        //HPNE
                        case 2:
                            out << "@ENPH" << endl << "Enemy Points (Sections)" <<endl;
                            break;
                        //TPTI
                        case 3:
                            out << "@ITPT" << endl << "#Item Points" <<endl;
                            break;
                        //HPTI
                        case 4:
                            out << "@ITPH" << endl << "#Item Points (Sections)" <<endl;
                            break;
                        //TPKC
                        case 5:
                            out << "@CKPT" << endl << "#Checkpoints" <<endl;
                            break;
                        //HPKC
                        case 6:
                            out << "@CKPH" << endl << "#Checkpoints (Sections)" <<endl;
                            break;
                        //JBOG
                        case 7:
                            out << "@GOBJ" << endl << "#Global Objects" <<endl;
                            break;
                        //ITOP
                        case 8:
                            out << "@POTI" << endl << "#Point Information" <<endl;
                            break;
                        //AERA
                        case 9:
                            out << "@AREA" << endl << "#Area Section" <<endl;
                            break;
                        //EMAC
                        case 10:
                            out << "@CAME" << endl << "#Cameras" <<endl;
                            break;
                        //TPGJ
                        case 11:
                            out << "@JGPT" << endl << "#Jugem Points (Respawns)" <<endl;
                            break;
                        //TPNC
                        case 12:
                            out << "@CNPT" << endl << "#Cannon Points" <<endl;
                            break;
                        //TPSM
                        case 13:
                            out << "@MSPT" << endl << "#Minigame S Points" <<endl;
                            break;
                        //IGTS
                        case 14:
                            out << "@STGI" << endl << "#Stage Points" <<endl;
                            break;
                        //SROC
                        case 15:
                            out << "@CORS" << endl << "#Course" <<endl;
                            break;
                        //TPLG
                        case 16:
                            out << "@GLPT" << endl << "#Glider Points" <<endl;
                            break;
                        //HPLG
                        case 17:
                            out << "@GLPH" << endl << "#Glider Points (Sections)" <<endl;
                            break;
                    }
                    out << endl;
              }
		      printf("KMP to TXT success.\n");
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
	return 0;
}
