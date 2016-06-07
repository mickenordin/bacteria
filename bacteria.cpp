/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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
