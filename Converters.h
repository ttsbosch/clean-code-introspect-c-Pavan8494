#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

boolean tryToConvertStringToInterger(const char* str, int* intergerValue) {
    char* endptr;
    boolean returnValue = TRUE;
    *intergerValue = strtol(str, &endptr, 10);
    if (endptr == str) {
        returnValue = FALSE;
    }
    return returnValue;
}

boolean tryToConvertStringToDouble(const char* str, double* doubleValue) {
    char* endptr;
    boolean returnValue = TRUE;
    *doubleValue = strtod(str, &endptr);
    if (endptr == str) {
        returnValue = FALSE;
    }
    return returnValue;
}
