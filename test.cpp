#include <stdio.h>
#include <string>
#include "casecorrectpath.h"

void CheckCase(const char* path){
    FILE *file = fopen(path, "rb");
    if(file){
        fclose(file);
    } else {
        printf("There is no file: %s\n", path);
        return;
    }
    std::string corrected;
    if(IsPathCaseCorrect(path, &corrected)){
        printf("Case is correct: %s\n", corrected.c_str());
    } else {
        printf("Case is not correct: %s\n", path);
        printf("The correct case is: %s\n", corrected.c_str());
    }
}

int main(int argc, char* argv[]){
    CheckCase("casecorrectpath.cpp");
    CheckCase("CaseCorrectPath.H");
    CheckCase("CASECORRECTPATH.CPP");
    #ifdef _WIN32
        printf("\nTest completed, press 'enter' to quit\n");
        getchar();
    #endif
}