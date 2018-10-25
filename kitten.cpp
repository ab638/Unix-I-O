#include <unistd.h>
#include <string> // string
#include <map>	  // map
#include <iomanip> // setw, right
#include <iostream> // cin, cout
#include <cstdlib> // exit
#include <fstream> // fistream
#include <signal.h> // used to catch ctrl-c
#include <sstream> // sstream
using namespace std;
using std::string;

void my_handler(int s) {
    exit(1);

}
int main(int argc, char* argv[]) {
    signal(SIGINT, my_handler);
    int opt;
    bool opt_n = false;
    bool opt_E = false;
    bool opt_s = false;

    opterr = 0; // supress default error message

    while((opt = getopt(argc, argv, "Ens")) != -1) {
        switch(opt) {
        case 'E':
            opt_E = true;
            break;
        case 'n':
            opt_n = true;
            break;
        case 's':
            opt_s = true;
            break;
        case '?':
            cout << "unknown option: " << static_cast<char>(optopt) << "\n";
            break;
        }

    }
    // if no arguments
    string tmp;
    if(argv[optind] == NULL) {
        while(1) {
            cin >> tmp;
            if(opt_E == true)
                tmp+="$";
            if(opt_n == true)
                tmp = "1 " + tmp;
            if(opt_s == true)
                tmp = tmp;
            cout << tmp << endl;
        }
    }


    for(; optind < argc; optind++) {
        cout << "argument: " << argv[optind] << "\n";
        ifstream in(argv[optind]);
        stringstream buffer;
        string tmp;
        if(in) {
            buffer << in.rdbuf();
            in.close();
        }
        //if no options selected
        if((opt_E ==false)&& (opt_n==false) && (opt_s==false)) {
            //ifstream in(argv[optind]);
            cout << buffer.rdbuf();

        }
        // supress unnecessary repeated empty lines
        else if((opt_s == true)&&(opt_E ==false)&& (opt_n==false) ) {
            map<string,int> mymap; // this is so necessary
            string tmp2;	// output string
            int tmpint = 0; // keeps track of spaces
            mymap[tmp] = 0; // default to 0
            while(getline(buffer, tmp)) { // look at that line man
		// if at position 0 or postion greater than 1 or at 2 spaces
                if(mymap[tmp] == 0 || mymap[tmp] > 1 || tmpint == 2) {
                    mymap[tmp] = 1; // set to position 1
                    tmp2 = tmp+'\n'; // add to output string
                    cout << tmp2; // output string

                }

                else {
                    mymap[tmp]++; //  increment position due to spaces
                    tmpint = mymap[tmp]; // save that position for next iteration

                }

            }


        }

        // supress and add numbers
        else if((opt_n == true)&&(opt_E ==false)&& (opt_s==true)) {
            map<string,int> mymap;
            string tmp2;
            int tmpint = 0;
            int i = 1;
            while(getline(buffer, tmp)) {
                if(mymap[tmp] == 0 || mymap[tmp] > 1|| tmpint == 2) {
                    mymap[tmp] = 1;
                    tmp2 = tmp+'\n';
                    cout << setw(2) << right << to_string(i) << " " << tmp2;
                    i++;

                }
                else {
                    mymap[tmp]++;
                    tmpint = mymap[tmp];

                }

            }


        }
        // supress and add $
        else if((opt_n == false)&&(opt_E == true) && (opt_s == true)) {
            map<string,int> mymap;
            string tmp2;
            int tmpint = 0;
            mymap[tmp] = 0;
            while(getline(buffer, tmp)) {
                if(mymap[tmp] == 0 || mymap[tmp] > 1 || tmpint == 2) {
                    mymap[tmp] = 1;
                    tmp2 = tmp+'$'+'\n';
                    cout << tmp2;

                }

                else {
                    mymap[tmp]++;
                    tmpint = mymap[tmp];

                }

            }


        }


        //appends $ to end of each line
        else if((opt_E == true)&&(opt_s ==false)&& (opt_n==false)) {
            string str;
            while(getline(buffer, str)) {
                tmp += str + "$" + "\n" ;
            }
            cout << tmp;
        }



        // add numbered lines
        else if((opt_n == true)&&(opt_E ==false)&& (opt_s==false)) {
            int i = 1;
            string str;
            while(getline(buffer, str)) {
                tmp = " " + str + "\n";
                cout << setw(2) << right << i << tmp;
                i++;
            }

        }



        // add numbered lines and $
        else if((opt_n == true)&&(opt_E ==true)&& (opt_s==false)) {
            int i = 1;
            string str;
            while(getline(buffer, str)) {

                tmp =  " " + str + "$" + "\n";
                cout << setw(2) << right << i << tmp;
                i++;
            }
        }


        // add numbered lines, suppress lines, and add $
        else if((opt_n == true)&&(opt_E == true)&& (opt_s==true)) {
            map<string,int> mymap;
            string tmp2;
            int tmpint = 0;
            int i = 1;
            mymap[tmp] = 0;
            while(getline(buffer, tmp)) {
                if(mymap[tmp] == 0 || mymap[tmp] > 1 || tmpint == 2) {
                    mymap[tmp] = 1;
                    tmp2 = tmp+'$'+'\n';
                    cout << setw(2) << right << i << tmp2;
                    i++;

                }

                else {
                    mymap[tmp]++;
                    tmpint = mymap[tmp];

                }

            }


        }


    }

}
