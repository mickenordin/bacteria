#include <fstream>
#include <cstdlib>
#include <climits>
#include <ctime>
#include <iostream>

using namespace std;

int file_size(const char *filename){
        ifstream ifile;
        ifile.open(filename, ios_base::binary | ios_base::ate);
        int size = ifile.tellg();
        ifile.close();
        return size;
}

int main(int argc,char** argv) {
        for (int i = 0; i < 2; i++) {
                // Read my own program
                ifstream ifile;
                ifile.open(argv[0], ios::binary) ;
                int filesize = file_size(argv[0]);
                srand( time(NULL) );
                int rand_byte = rand() %  ( filesize + 1 );
                
                // Devide
                ofstream ofile;
                string filename = tempnam(".", "bact-");
                cout << "Creating new bacteria called: "<< filename << " with size:" << file_size(argv[0]) << '\n';;
                ofile.open(filename, ios::binary | ios::out);

                if ( ifile.fail() or ofile.fail() ) {
                   return 1; //error
                }

                int byte_counter = 0;
                while (!ifile.eof()) {
                        unsigned char c;

                        c = ifile.get();

                        if (ifile.fail()) {
                                break;
                        }
                        if (byte_counter == rand_byte and i == 1) {
                                int bit = rand() % CHAR_BIT;
                                cout << "Mutating bit: " << bit << " of byte: " << byte_counter << '\n';
                                c ^= (1u << bit);
                        }
                        ofile.put(c);
                        byte_counter++;
                }

                ofile.close();
                string executable = "/bin/chmod +x " + filename;
                string run = filename;
                system(executable.c_str());
                //system(run.c_str());
        }
        return 0;
}
