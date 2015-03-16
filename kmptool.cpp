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

#define NICEHEX setw(2) << setfill('0')
#define FCAST (unsigned)(unsigned char)

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

float bytesToFloat(uchar b0, uchar b1, uchar b2, uchar b3) {
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
	for (int i = 0; i < 18; i++)
		ofs[i] = kmp.find(nams[i],0)-88;
}


// Patches Header with right values
void patchHeader(string &hd,const int ofs[],int size) {
	// Patches Filesize
	hd[4] = size%256;
	hd[5] = size/256;
	
	// Patches Offsets
	int i;
	int j;
	for (i = 16, j = 0; j < 18; i = i+4, j++) {
		hd[i] = ofs[j]%256;
		hd[i+1] = ofs[j]/256;
		hd[i+2] = 0;
		hd[i+3] = 0;
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
			
			int numpt,pos,length,j,a;
			for (j = 0; j < 18; j++) {
				pos = keiempi.find(Names[j],0)+8;
				length = keiempi.find(Names[j+1],0)-pos;
				switch (j) {
					//TPTK
					case 0:
						out << "@KTPT" << endl << "#Kart Points" << endl;
						break;
					//TPNE
					case 1:
						numpt = length/24;
						out << "@ENPT" << endl << "#Enemy Points" << endl << "#Number of Points="<< numpt << endl;
						out << "#ID:	X:	    	Y:	     	Z:	     	SCALE:	 	UNKNOWN1:	UNKNOWN2:" << endl;
						for (int i = 0; i < numpt; i++) {
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
							out << endl << dec << setfill(' ');
						}
						break;
					//HPNE
					case 2:
						numpt = length/72;
						out << "@ENPH" << endl << "#Enemy Points (Sections)" <<endl << "#Number of Sections="<< numpt << endl;
						out << "#ID:    START:	LENGTH:	PR1:	PR2:	PR3:	PR4:	PR5:	PR6:	PR7: 	PR8: 	PR9: 	PR10: 	PR11: 	PR12: 	PR13: 	PR14: 	PR15: 	PR16: 	NX1: 	NX2: 	NX3: 	NX4: 	NX5: 	NX6: 	NX7: 	NX8: 	NX9: 	NX10: 	NX11: 	NX12: 	NX13: 	NX14: 	NX15: 	NX16: 	UNKNOWN:" << endl;
						for (int i = 0; i < numpt; i++) {
							//ID
							out << i << "\t";
							//START
							a = 0;
							out << (unsigned short)keiempi[pos+(72*i)+a]+((unsigned short)keiempi[pos+(72*i)+a+1]*256) << "\t";
							a += 2;
							//LENGTH
							out << setw(3) << (unsigned short)keiempi[pos+(72*i)+a]+((unsigned short)keiempi[pos+(72*i)+a+1]*256) << "\t";
							//PREVIOUSNEXT
							for (a+=2;a<68;a+=2) {
								if ((((uchar)keiempi[pos+(72*i)+a])&255==255) && (((uchar)keiempi[pos+(72*i)+a+1])&255==255))
									out << setw(3)<< "-1" << "\t";
								else
									out << setw(3)<< (unsigned short)keiempi[pos+(72*i)+a]+((unsigned short)keiempi[pos+(72*i)+a+1]*256) << "\t";
							}
							//UNKNOWN
							out << endl ;
						}
						break;
					//TPTI
					case 3:
						numpt = length/20;
						out << "@ITPT" << endl << "#Item Routes" << endl << "#Number of Points="<< numpt << endl;
						out << "#ID:	X:	    	Y:	     	Z:	     	SCALE:	 	UNKNOWN:" << endl;
						for (int i = 0; i < numpt; i++) {
							out << i << "\t";
							//X
							out << left << setw(8) << bytesToFloat(keiempi[pos+(20*i)+3],keiempi[pos+(20*i)+2],keiempi[pos+(20*i)+1],keiempi[pos+(20*i)]) << "\t";
							//Y
							out << left << setw(8) << bytesToFloat(keiempi[pos+(20*i)+7],keiempi[pos+(20*i)+6],keiempi[pos+(20*i)+5],keiempi[pos+(20*i)+4])<< "\t";
							//Z
							out << left << setw(8) << bytesToFloat(keiempi[pos+(20*i)+11],keiempi[pos+(20*i)+10],keiempi[pos+(20*i)+9],keiempi[pos+(20*i)+8])<< "\t";
							//scale
							out << left << setw(8) << bytesToFloat(keiempi[pos+(20*i)+15],keiempi[pos+(20*i)+14],keiempi[pos+(20*i)+13],keiempi[pos+(20*i)+12])<< "\t";
							//unknown1
							out << hex << uppercase << NICEHEX << FCAST keiempi[pos+(20*i)+16] << NICEHEX << FCAST keiempi[pos+(20*i)+17] << NICEHEX << FCAST keiempi[pos+(20*i)+18] << NICEHEX << FCAST keiempi[pos+(20*i)+19];
							out << endl << dec << setfill(' ');
						}
						break;
					//HPTI
					case 4:
						numpt = length/28;
						out << "@ITPH" << endl << "#Item Points (Sections)" <<endl << "#Number of Sections="<< numpt << endl;
						out << "#ID:    START:	LENGTH:	PR1:	PR2:	PR3:	PR4:	PR5:	PR6:	NX1: 	NX2: 	NX3: 	NX4: 	NX5: 	NX6:" << endl;
						for (int i = 0; i < numpt; i++) {
							//ID
							out << i << "\t";
							//START
							a = 0;
							out << (unsigned short)keiempi[pos+(28*i)+a]+((unsigned short)keiempi[pos+(28*i)+a+1]*256) << "\t";
							a += 2;
							//LENGTH
							out << setw(3)<< (unsigned short)keiempi[pos+(28*i)+a]+((unsigned short)keiempi[pos+(28*i)+a+1]*256) << "\t";
							//PREVIOUSNEXT
							for (a += 2; a < 28; a += 2) {
								if ((((uchar)keiempi[pos+(28*i)+a])&255==255) && (((uchar)keiempi[pos+(28*i)+a+1])&255==255))
									out << setw(3)<< "-1" << "\t";
								else
									out << setw(3)<< (unsigned short)keiempi[pos+(28*i)+a]+((unsigned short)keiempi[pos+(28*i)+a+1]*256) << "\t";
							}
							out << endl;
						}
						break;
					//TPKC
					case 5:
						numpt=length/24;
						out << "@CKPT" << endl << "#Checkpoints" <<endl << "#Number of Points:" << numpt << endl;
						out << "#ID:	X1:	    	Z1:	     	X2:	     	Z2:       	RESPAWNID:  	TYPE:	PREV:	NEXT:	UNKNOWN:" << endl;
						for (int i = 0; i < numpt; i++)
						{
							out << i << "\t";
							//X1
							out << left << setw(8) << bytesToFloat(keiempi[pos+(24*i)+3],keiempi[pos+(24*i)+2],keiempi[pos+(24*i)+1],keiempi[pos+(24*i)]) << "\t";
							//Z1
							out << left << setw(8) << bytesToFloat(keiempi[pos+(24*i)+7],keiempi[pos+(24*i)+6],keiempi[pos+(24*i)+5],keiempi[pos+(24*i)+4])<< "\t";
							//X2
							out << left << setw(8) << bytesToFloat(keiempi[pos+(24*i)+11],keiempi[pos+(24*i)+10],keiempi[pos+(24*i)+9],keiempi[pos+(24*i)+8])<< "\t";
							//Z2
							out << left << setw(8) << bytesToFloat(keiempi[pos+(24*i)+15],keiempi[pos+(24*i)+14],keiempi[pos+(24*i)+13],keiempi[pos+(24*i)+12])<< "\t";    
							//RESPAWN
							out << left << setw(8) << (unsigned int)keiempi[pos+(24*i)+16] << "\t";
							//TYPE
							if ((keiempi[pos+(24*i)+17]&255)==255)
								out << left << setw(6) << -1 << "\t";
							else
								out << left << setw(6) << (unsigned int)keiempi[pos+(24*i)+17] << "\t";
							//PREVIOUS
							if ((keiempi[pos+(24*i)+18]&255)==255)
								out << left << setw(6) << -1 << "\t";
							else
								out << left << setw(6) << (unsigned int)keiempi[pos+(24*i)+18] << "\t";
							//NEXT
							if ((keiempi[pos+(24*i)+19]&255)==255)
								out << left << setw(6) << -1 << "\t";
							else
								out << left << setw(6) << (unsigned int)keiempi[pos+(24*i)+19] << "\t";
							//Unknown
							//out << left << setw(8) << keiempi[pos+(24*i)+20] << "\t";
							out << endl;
						}
						break;
					//HPKC
					case 6:
						numpt = length/16;
						out << "@CKPH" << endl << "#Checkpoints (Sections)" <<endl << "#Number of Sections="<< numpt << endl;
						out << "#ID:    START:	LENGTH:	PR1:	PR2:	PR3:	PR4:	PR5:	PR6:	NX1: 	NX2: 	NX3: 	NX4: 	NX5: 	NX6:     UNKNOWN:" << endl;
						for (int i = 0; i < numpt; i++)
						{
							//ID
							out << i << "\t";
							//START
							out << (unsigned short)keiempi[pos+(16*i)] << "\t";
							a=1;
							//LENGTH
							out << setw(3)<< (unsigned short)keiempi[pos+(16*i)+a]+((unsigned short)keiempi[pos+(16*i)+a+1]*256) << "\t";
							//PREVIOUSNEXT
							for (a++;a<14;a++)
							{
								if ((keiempi[pos+(16*i)+a]&255)==255)
									out << setw(3)<< "-1" << "\t";
								else
									out << setw(3)<< (unsigned int)keiempi[pos+(16*i)+a] << "\t";
							}
							//UNKNOWN
							a++;
							out << setw(3)<< (unsigned short)keiempi[pos+(16*i)+a]+((unsigned short)keiempi[pos+(16*i)+a+1]*256) << "\t";
							out << endl;
						}
						break;
					//JBOG
					case 7:
						numpt=length/64;
						out << "@GOBJ" << endl << "#Global Objects" << endl << "#Number of Objects="<< numpt <<endl;
						out << "#ID:    OBJECT ID: 	UNKNOWN: 	X: 		Y: 		Z: 		X-ANGLE: 	Y-ANGLE: 	Z-ANGLE: 	X-SCALE: 	Y-SCALE: 	Z-SCALE: 	ROUTE ID:" << endl;
						for (int i = 0; i < numpt; i++)
						{
							out << i << "\t";
							//Object ID
							out << setw(8)<< (unsigned short)keiempi[pos+(64*i)]+((unsigned short)keiempi[pos+(64*i)+1]*256) << "\t";
							//Unknown
							out << setw(8)<< (unsigned short)keiempi[pos+(64*i)+2]+((unsigned short)keiempi[pos+(64*i)+3]*256) << "\t";
							//X
							out << left << setw(8) << bytesToFloat(keiempi[pos+(64*i)+7],keiempi[pos+(64*i)+6],keiempi[pos+(64*i)+5],keiempi[pos+(64*i)+4])<< "\t";
							//Y
							out << left << setw(8) << bytesToFloat(keiempi[pos+(64*i)+11],keiempi[pos+(64*i)+10],keiempi[pos+(64*i)+9],keiempi[pos+(64*i)+8])<< "\t";
							//Z
							out << left << setw(8) << bytesToFloat(keiempi[pos+(64*i)+15],keiempi[pos+(64*i)+14],keiempi[pos+(64*i)+13],keiempi[pos+(64*i)+12])<< "\t";
							//X Angle
							out << left << setw(8) << bytesToFloat(keiempi[pos+(64*i)+19],keiempi[pos+(64*i)+18],keiempi[pos+(64*i)+17],keiempi[pos+(64*i)+16])<< "\t";
							//Y Angle
							out << left << setw(8) << bytesToFloat(keiempi[pos+(64*i)+23],keiempi[pos+(64*i)+22],keiempi[pos+(64*i)+21],keiempi[pos+(64*i)+20])<< "\t";
							//Z Angle
							out << left << setw(8) << bytesToFloat(keiempi[pos+(64*i)+27],keiempi[pos+(64*i)+26],keiempi[pos+(64*i)+25],keiempi[pos+(64*i)+24])<< "\t";
							//X Scale
							out << left << setw(8) << bytesToFloat(keiempi[pos+(64*i)+31],keiempi[pos+(64*i)+30],keiempi[pos+(64*i)+29],keiempi[pos+(64*i)+28])<< "\t";
							//Y Scale
							out << left << setw(8) << bytesToFloat(keiempi[pos+(64*i)+35],keiempi[pos+(64*i)+34],keiempi[pos+(64*i)+33],keiempi[pos+(64*i)+32])<< "\t";
							//Z Scale
							out << left << setw(8) << bytesToFloat(keiempi[pos+(64*i)+39],keiempi[pos+(64*i)+38],keiempi[pos+(64*i)+37],keiempi[pos+(64*i)+36])<< "\t";
							//Route ID
							if (((uchar)keiempi[pos+(64*i)+40])==255)
								out << setw(8)<< -1 << "\t";
							else
								out << setw(8)<< (unsigned short)keiempi[pos+(64*i)+40] << "\t";
							out << endl;
						}
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
