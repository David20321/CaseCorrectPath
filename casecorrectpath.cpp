#include <stdio.h>
#include <string>

namespace {

static const int kPathBufferSize = 1024;

// Return the number of instances of the_char in str
int CountCharsInString(const char* str, char the_char){
    int count = 0;
    int str_length = strlen(str);
    for(int i=0; i<str_length; ++i){
        if(str[i] == '/'){
            ++count;
        }
    }
    return count;
}

// Return the index of the nth instance of the_char in str, starting from the back
// Returns -1 if there is no nth instance
int FindNthCharFromBack(const char* str, char the_char, int n){
    int count = 0;
    int str_length = strlen(str);
    for(int i= str_length-1; i>=0; --i){
        if(str[i] == '/'){
            ++count;
            if(count == n){
                return i;
            }
        }
    }
    return -1;
}

void GetCaseCorrectPath(const char* input_path, char* correct_path){
    #ifdef _WIN32 // Correct case by converting to short path and back to long
        char short_path[kPathBufferSize];
        GetShortPathName(input_path, short_path, kPathBufferSize);
        GetLongPathName(short_path, correct_path, kPathBufferSize);
    #else  // Correct case using realpath() and cut off working directory
        int num_dirs = CountCharsInString(input_path, '/');
        char path[kPathBufferSize];
        realpath(input_path, path);
        char *cut_path = &path[FindNthCharFromBack(path,'/',num_dirs+1)+1];
        strcpy(correct_path, cut_path);
    #endif
}

} // namespace ""

bool IsPathCaseCorrect(const std::string& input_path, std::string *correct_case){
    char correct_case_buf[kPathBufferSize];
    GetCaseCorrectPath(input_path.c_str(), correct_case_buf);
    *correct_case = correct_case_buf;
    if(input_path != *correct_case){
        return false;
    } else {
        return true;
    }
}