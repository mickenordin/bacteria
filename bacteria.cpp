#include <bitset>
#include <climits>
#include <chrono>         // std::chrono::seconds
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
 

using namespace std;

int file_size(const char *filename){
	ifstream ifile;
	ifile.open(filename, ios_base::binary | ios_base::ate);
	int size = ifile.tellg();
	ifile.close();
	return size;
}

int main(int argc,char** argv) {
	for (int i = 0; i < 2; i++) {
		// Read my own program
		ifstream ifile;
		ifile.open(argv[0], ios::binary) ;
		
		//Get some randomness
		srand( time(NULL) );
		int filesize = file_size(argv[0]);
		int rand_byte = rand() %  ( filesize + 1 );

		// Devide
		ofstream ofile;
		string filename = tempnam(".", "bact-");
		cout << "Creating new bacteria called: "<< filename << " with size:" << filesize << '\n';;
		ofile.open(filename, ios::binary | ios::out);

		// We count our bytes
		int byte_counter = 0;
		
		// Loop through binary file byte by byte
		while (!ifile.eof()) {
			unsigned char c;

			c = ifile.get();

			if (byte_counter == rand_byte ) { 
				int insert = rand() % 6;
				if (insert == 0) {
					char byte = (char) rand() % 256;
					bitset<8> binbyte(byte);
					cout << "Inserting byte with value: " << binbyte << " after byte: " << byte_counter << '\n';
					ofile.put(c);
					ofile.put(byte);
				} else if (insert == 1)  {
					int bit = rand() % CHAR_BIT;
					cout << "Mutating bit: " << bit << " of byte: " << byte_counter << '\n';
					c ^= (1u << bit);
					ofile.put(c);
				} else if (insert == 2){
					cout << "Witholding byte: " << byte_counter << '\n';
				} else { 
					cout << "Just letting this one pass."<< '\n';
					ofile.put(c);
				}
			} else {
				ofile.put(c);
			}
			byte_counter++;
		}

		ofile.close();
		string executable = "/bin/chmod +x " + filename;
		string run = filename;
		system(executable.c_str());
		this_thread::sleep_for (chrono::seconds(1));
		system(run.c_str()); // This is the dangerous part ^^
	}
	return 0;
}
