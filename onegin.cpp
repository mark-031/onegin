#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cassert>
#include "utils.h"

const char* FILEPATH = "hamlet.txt";
const char* OUTPUTFILE = "out.txt";

void  openFileOrExit(FILE* &fileptr, const char* path);
int   getFileSize(FILE* file);
int   readFile(FILE* file, char* &buffer);
int   standardizeContent(char* start);
void  parseContent(char* start, char** strIndex);
void  processFile(char* &buffer, char** &strIndex, int &strIndexLen);
char* getStartOfLine(char* item);
int   compare(const void* first, const void* second);

int main()
{
	FILE* output = freopen(OUTPUTFILE, "w", stdout);

	char*  buffer = nullptr;
	char** strIndex = nullptr;
	int    strIndexLen = -1;

	processFile(buffer, strIndex, strIndexLen);

	assert(buffer      != nullptr);
	assert(strIndex    != nullptr);
	assert(strIndexLen != -1);

	utils::qSort(strIndex, strIndex + strIndexLen - 1, sizeof(char*), compare);

	char** strs = strIndex;
	while (strs < strIndex + strIndexLen)
	{
		printf("%s\n", getStartOfLine((char*) *strs));
		strs++;
	}

	fclose(output);
}

void openFileOrExit(FILE* &fileptr, const char* path)
{
	fileptr = fopen(path, "r");

	while (fileptr == nullptr)
	{
		//puts("Unable to open file\n");
		exit(1);
	}

	//printf(">>> successfuly open file \"%s\" \n", path);
}

int getFileSize(FILE* file)
{
	int size = 0;

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);

	assert(size > 0);
	//printf(">>> successfuly get file size (%d)\n", size);
	return size;
}

int readFile(FILE* file, char* &buffer)
{
	//puts("\nStart read file");

	int size = getFileSize(file) + 2;
	buffer = (char*) calloc(size, sizeof(char));

	assert(buffer != nullptr);

	*(buffer) = *(buffer + size - 1) = '\0';
	
	fread(buffer + 1, sizeof(char), size, file);

	//puts("End read file\n");

	return size;
}

int standardizeContent(char* start)
{
	int linesCount = 1;
	char* newStart = start;

	while (*start == '\n' || *start == ' ' || *start == '\t')
		start++;

	while (*start != '\0')
	{
		if (*start == '\n')
		{
			if (*(newStart - 1) != '\n')
			{
				if(*(newStart - 1) != ' ')
					*(newStart++) = '\n';

				else
					*(newStart - 1) = '\n';

				linesCount++;
			}

			start++;
			continue;
		}

		if (*start == '\t' || *start == ' ')
		{
			if (*(newStart - 1) != ' ' && *(newStart - 1) != '\n')
				*(newStart++) = ' ';

			start++;
			continue;
		}

		*(newStart++) = *(start++);
	}

	if (*(newStart - 1) == '\n' || *(newStart - 1) == ' ')
	{
		*(newStart - 1) = '\0';
		linesCount--;
	}
	else
		*newStart = '\0';

	//printf(">>> successfuly standardize content (lines: %d)\n", linesCount);
	return linesCount;
}

void parseContent(char* start, char** strIndex)
{
	while (*start != '\0')
	{
		if (*start == '\n')
		{
			*(strIndex++) = start - 1;
			*start = '\0';
		}
		start++;
	}
	*strIndex = start - 1;

	//printf(">>> successfuly parsed content\n");
}

void processFile(char* &buffer, char** &strIndex, int &strIndexLen)
{
	FILE* file;
	openFileOrExit(file, FILEPATH);

	readFile(file, buffer);
	fclose(file);
	strIndexLen = standardizeContent(buffer + 1);

	strIndex = (char**) calloc(strIndexLen, sizeof(char*));
	parseContent(buffer + 1, strIndex);
}

char* getStartOfLine(char* item)
{
	while (*item != '\0')
		item--;
	return ++item;
}

int compare(const void* first, const void* second)
{
	char* _first  = *(char**) first;
	char* _second = *(char**) second;

	_first  = getStartOfLine(_first);
	_second = getStartOfLine(_second);

	while (*_first == *_second && *_first != '\0')
	{
		++_first;
		++_second;
	}

	return *_first - *_second;
}