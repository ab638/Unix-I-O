// CS375 Project 2
// Austin Brummett
// Due: Sept. 20, 2018
// Question: How many file descriptors may a process have open?
// Answer: 1021, 1024 when counting 3 taken up by system

#include <iostream> // cout
#include <unistd.h> // close
#include <fcntl.h> // open, O_RDONLY


using namespace std;

int main(int argc, char *argv[]) {
    int32_t count = 0;
    // open first instance
    int fd = open(argv[0], O_RDONLY);
    count++; // count first instance
    // While open doesn't fail
    while(open(argv[0], O_RDONLY)!=-1) {
        count++;
    } // Keep counting
    cout << "Count: " << count << endl; // print count (expected 1021)
    close(fd); // close the file

}
