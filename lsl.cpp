// CS375
// Austin Brummett
// Project 2: lsl.cpp

/* Requirements
	1) zero or one directory names as arguments
		a) if no argument output long directory listing on current
		   directory
	2. output same info as "ls -l" command
*/

#include <errno.h> // errno
#include <dirent.h> // DIR
#include <string.h> // string 
#include <iostream> // cout
#include <iomanip> // setw, right
#include <pwd.h> // passwd
#include <grp.h> // grp
#include <sys/types.h> //size_t, uid_t, etc.
#include <sys/stat.h>// stat
#include <unistd.h>
#include <stdlib.h> // exit
#include <ctime> // time, strftime
#include <algorithm> //transform
using namespace std;
// TODO Figure out alphabetical listing
struct info_t {
    string permissions;
    string owner;
    string dateModified;
    string timeModified;
    int hardlinks;
    string group;
    string fileName;
    int blocksize;
    long long size;
};

// outputs type of file and permissions in a single string
string filepermissions(struct stat sb) {
    string out; // output string
    switch (sb.st_mode & S_IFMT) { // if mode and file type
    case S_IFBLK:
        out += "b" ;
        break; // block device
    case S_IFCHR:
        out += "c" ;
        break; // character device
    case S_IFDIR:
        out += "d" ;
        break; // directory
    case S_IFIFO:
        out += "p" ;
        break; // pipe
    case S_IFLNK:
        out += "l" ;
        break; // symbolic link
    case S_IFREG:
        out += "-" ;
        break; // regular file
    case S_IFSOCK:
        out += "s" ;
        break; // socket
    default:
        out+= "?"  ;
        break;	 // unknown
    }
    out += (sb.st_mode & S_IRUSR) ? "r" : "-"; // usr read
    out += (sb.st_mode & S_IWUSR) ? "w" : "-"; // usr write
    out += (sb.st_mode & S_IXUSR) ? "x" : "-"; // usr exec
    out += (sb.st_mode & S_IRGRP) ? "r" : "-"; // group read
    out += (sb.st_mode & S_IWGRP) ? "w" : "-"; // group write
    out += (sb.st_mode & S_IXGRP) ? "x" : "-"; // group exec
    out += (sb.st_mode & S_IROTH) ? "r" : "-"; // other read
    out += (sb.st_mode & S_IWOTH) ? "w" : "-"; // other write
    out += (sb.st_mode & S_IXOTH) ? "x" : "-"; // other exec

    return out; // send back file type and permissions
} // end filepermissions

// displays the output of the stored struct info_t
void displaylsl(struct info_t info[], int i) {
    int blksize = 0; // block size
    struct info_t temp, temp1, temp2; // temp for swap, temp1 & 2 for comparison
    for (int s = 0; s < i; s++) {
        blksize +=info[s].blocksize; // add up block size
    }

    cout << "total " << blksize/2 <<endl; // needs divided by 2 for some reason
    for(int a = 0; a < i; a++) {
        for(int b=a+1; b < i; b++) {
            // use transform in order to use tolower for accuracte comparision
            transform(info[a].fileName.begin(), info[a].fileName.end(), temp1.fileName.begin(), ::tolower);
            transform(info[b].fileName.begin(), info[b].fileName.end(), temp2.fileName.begin(), ::tolower);
            // check if temp1 or temp2 comes first based on 2 initial character
            if(strncmp(temp1.fileName.c_str(),temp2.fileName.c_str(), 2) >0) {
                // basic swap routine
                temp = info[a];
                info[a] = info[b];
                info[b] = temp;
            }
        }
    }

    // output the information
    for(int j =0; j < i; j++) {
        cout <<  info[j].permissions << " " << info[j].hardlinks << " "<< info[j].owner << " " << info[j].group << " ";
        cout << setw(6) << right << info[j].size;
        cout << " " << info[j].dateModified << " " << info[j].timeModified;
        cout << " " << info[j].fileName << right << endl;
    }

}

// store information in struct info_t
void store(struct stat sb, struct dirent* de, int i, struct info_t* info) {

    string out = filepermissions(sb); // get file type and permissions

    struct passwd *pws = getpwuid(sb.st_uid); // get username
    struct group *grp = getgrgid(sb.st_gid); // get group name
    tm* curr_tm; // current time
    char date[100], times[100];
    time_t t = sb.st_mtime; // epoch time of occurence
    curr_tm = localtime(&t); // convert to local time

    strftime(date, sizeof(date), "%b %d", curr_tm); //  convert month and day
    strftime(times, sizeof(times), "%H:%M", curr_tm);  // convert hours:minutes

    // set info at the current index
    info[i].permissions = out;
    info[i].owner = pws->pw_name;
    info[i].group = grp->gr_name;
    info[i].dateModified = date;
    info[i].timeModified = times;
    info[i].hardlinks = sb.st_nlink;
    info[i].fileName = de->d_name;
    info[i].blocksize = sb.st_blocks;
    info[i].size = sb.st_size;
}
// Define error handling routines
int errmsg(string str) {
    cerr << str << endl;
    exit(1);
}

int uerrmsg(string str) {
    cerr << str << ":" << strerror(errno) << endl;
    exit(errno);
}

int main(int argc, char* argv[]) {

    string dirname; // directory name
    struct stat sb; // stat buffer
    info_t* file = new info_t [1000];
    if(argc == 1)
        dirname = "."; // current directory
    else if (argc == 2) {
        dirname = argv[1]; // specified directory

        string word = dirname + '/'; // test string for comparison
        if(strcmp(word.c_str(), dirname.c_str()))

            dirname += '/';	// adds slash to end of directory
    }
    else
        errmsg("usage: lsl [directory]"); // usage info

    DIR *dp; // Directory

    if((dp = opendir(dirname.c_str())) == NULL)
        uerrmsg("opendir");	// directory doesn't exist

    struct dirent *de; // directory struct

    int i = 0;
    while((de = readdir(dp)) != NULL) { // loop through all entries

        if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..")) // check if these
            continue;	// skip them
        else if(de->d_name[0] != '.') { // check if file/directory is hidden
            if((stat(de->d_name, &sb)) == 0) {
                store(sb, de, i, file); // store info
                i++; // iterate
            }
            else if ((stat((dirname+de->d_name).c_str(), &sb)) == 0) {
                store(sb, de, i, file);	// store info
                i++; // iterate
            }
        }

    }

    // output info
    displaylsl(file, i);

    if(errno != 0)
        uerrmsg("readdir");
    closedir(dp);
    return 0;
}
