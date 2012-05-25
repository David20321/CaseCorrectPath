#include <cstdio>
#include <string>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <algorithm>
    #include <cstdlib>
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
        delete short_path;
        return std::string(correct_case_buf);
    #else  // Correct case using realpath() and cut off working directory
        std::ptrdiff_t num_dirs = std::count(input_path.begin(), input_path.end(), '/');
        std::string path(realpath(input_path.c_str(), NULL));
        return path.substr(FindNthCharFromBack(path,'/',num_dirs+1)+1);
    #endif
}

} // namespace ""

bool IsPathCaseCorrect(const std::string& input_path, std::string *correct_case){
    *correct_case = GetCaseCorrectPath(input_path);
    return (input_path == *correct_case);
}
