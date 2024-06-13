#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

char** SplitString(const char* str, char delimiter) {
    int count = 0;
    const char* ptr = str;
    while (*ptr != '\0') {
        if (*ptr++ == delimiter) {
            count++;
        }
    }

    char** tokens = (char**)malloc(sizeof(char*) * (count + 2));
    int i = 0;
    ptr = str;
    char* token = (char*)malloc(strlen(str) + 1);
    int j = 0;
    while (*ptr != '\0') {
        if (*ptr == delimiter) {
            token[j] = '\0';
            tokens[i++] = strdup(token);
            j = 0;
        } else {
            token[j++] = *ptr;
        }
        ptr++;
    }
    token[j] = '\0';
    tokens[i++] = strdup(token);
    tokens[i] = NULL;
    free(token);
    return tokens;
}


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

void convertDataFromCsvToXml(FILE* stream) {
    char line[1024];
    tradeRecord objects[1024];
    int lineCount = 0;
    int objectCount = 0;

    while (fgets(line, sizeof(line), stream)) {
        char* fields[3];
        int fieldCount = 0;
        char* token = strtok(line, ",");
        while (token != NULL) {
            fields[fieldCount++] = token;
            token = strtok(NULL, ",");
        }

        if (fieldCount != 3) {
            fprintf(stderr, "WARN: Line %d malformed. Only %d field(s) found.\n", lineCount + 1, fieldCount);
            continue;
        }

        if (strlen(fields[0]) != 6) {
            fprintf(stderr, "WARN: Trade currencies on line %d malformed: '%s'\n", lineCount + 1, fields[0]);
            continue;
        }

        int TradeAmount;
        if (!tryToConvertStringToInterger(fields[1], &TradeAmount)) {
            fprintf(stderr, "WARN: Trade amount on line %d not a valid integer: '%s'\n", lineCount + 1, fields[1]);
        }

        double TradePrice;
        if (!tryToConvertStringToDouble(fields[2], &TradePrice)) {
            fprintf(stderr, "WARN: Trade price on line %d not a valid decimal: '%s'\n", lineCount + 1, fields[2]);
        }

        strncpy(objects[objectCount].SourceCurrency, fields[0], 3);
        strncpy(objects[objectCount].DestibationCurrency, fields[0] + 3, 3);
        objects[objectCount].Lots = TradeAmount / LotSize;
        objects[objectCount].Price = TradePrice;
        objectCount++;
        lineCount++;
    }

    FILE* outFile = fopen("output.xml", "w");
    fprintf(outFile, "<TradeRecords>\n");
    for (int i = 0; i < objectCount; i++) {
        fprintf(outFile, "\t<TradeRecord>\n");
        fprintf(outFile, "\t\t<SourceCurrency>%s</SourceCurrency>\n", objects[i].SourceCurrency);
        fprintf(outFile, "\t\t<DestinationCurrency>%s</DestinationCurrency>\n", objects[i].DestibationCurrency);
        fprintf(outFile, "\t\t<Lots>%d</Lots>\n", objects[i].Lots);
        fprintf(outFile, "\t\t<Price>%f</Price>\n", objects[i].Price);
        fprintf(outFile, "\t</TradeRecord>\n");
    }
    fprintf(outFile, "</TradeRecords>");
    fclose(outFile);
    printf("INFO: %d trades processed\n", objectCount);
}

