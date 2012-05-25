#include <cstdio>
#include <string>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <algorithm>
    #include <cstdlib>
    #include <sstream>
#endif

namespace {

// Return the index of the nth instance of the_char in str, starting from the back
// Returns -1 if there is no nth instance
int FindNthCharFromBack(std::string& str, char the_char, int n){
    int count = 0;
    int str_length = str.length();
    for(int i = str_length-1; i>=0; --i){
        if(str[i] == the_char){
            ++count;
            if(count == n){
                return i;
            }
        }
    }
    return -1;
}


std::string GetCaseCorrectPath(std::string input_path){
    #ifdef _WIN32 // Correct case by converting to short path and back to long
        long kPathBufferSize = GetLongPathName(input_path.c_str(), NULL, 0);
        char* short_path = new char[kPathBufferSize];
        char* correct_case_buf = new char[kPathBufferSize];
        GetShortPathName(input_path.c_str(), short_path, kPathBufferSize);
        GetLongPathName(short_path, correct_case_buf, kPathBufferSize);
        std::string corrected = std::string(correct_case_buf);
        delete short_path;
        delete correct_case_buf;
        return corrected;
    #else  // Ask /proc for a symlink to the actual file and use real_path to extract
        FILE* f = fopen(input_path.c_str(), "r");
        pid_t pid = getpid();
        std::ptrdiff_t num_dirs = std::count(input_path.begin(), input_path.end(), '/');
        std::stringstream proc_path;
        char* actual_path;
        proc_path << "/proc/" << pid << "/fd/" << fileno(f);
        actual_path = realpath(proc_path.str().c_str(), NULL);
        fclose(f);
        std::string path(actual_path);
        free(actual_path);
        return path.substr(FindNthCharFromBack(path,'/',num_dirs+1)+1);
    #endif
}

} // namespace ""

bool IsPathCaseCorrect(const std::string& input_path, std::string *correct_case){
    *correct_case = GetCaseCorrectPath(input_path);
    return (input_path == *correct_case);
}
