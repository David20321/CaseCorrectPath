#include <cstdio>
#include <string>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <algorithm>
    #include <cstdlib>
    #include <dirent.h> 
#endif

namespace {


std::string BuildRealPath(std::string& input_path) {
    std::string path = "";
    size_t i = 0;
    std::string next_child = ".";
    bool first = true;
    
    if(input_path.compare(0, next_child.length(), "./") == 0) {
        i += 2;
        path = "./";
    } else if(input_path[0] == '/') {
        i += 1;
        path = "/";
        next_child = "/";
    }
    
    bool done = false;
    for(;!done && i < input_path.length();) {
        DIR *dp;
        struct dirent *ep;
        
        std::string dir;

        if(first) {
            dir = next_child;
            first = false;
        } else {
            dir = path;
        }
        dp = opendir(dir.c_str());
        
        if(!dp) {
            return ""; // Unexpected, raise exception?
        }
        
        size_t dir_end = input_path.find_first_of('/', i + 1);
        
        if(dir_end == std::string::npos) {
            next_child = input_path.substr(i);
            done = true;
        } else {
            next_child = input_path.substr(i, dir_end - i) ;
            i = dir_end + 1;
        }
        
        std::transform(next_child.begin(), next_child.end(), next_child.begin(), ::tolower);
        
        bool found = false;
        while(!found && (ep = readdir (dp))) {
            std::string cur(ep->d_name);
            std::transform(cur.begin(), cur.end(), cur.begin(), ::tolower);
            if(next_child == cur) {
                found = true;
                path += ep->d_name;
            }
            free(ep);
        }
        closedir(dp);
        free(dp);
        
        
        if(!found) {
            return ""; // Unexpected, raise exception?
        }
        
        if(!done) {
            path += "/";
        }
    }
    
    return path;    
    
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
    #else  // Correct case by traversing the directory(s) manually
        return BuildRealPath(input_path);
    #endif
}

} // namespace ""

bool IsPathCaseCorrect(const std::string& input_path, std::string *correct_case){
    *correct_case = GetCaseCorrectPath(input_path);
    return (input_path == *correct_case);
}
