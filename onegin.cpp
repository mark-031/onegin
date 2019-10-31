#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cassert>
#include "utils.h"

int   getFileSize(FILE* file);
int   readFile(FILE* file, char* &buffer);
int   standardizeContent(char* start);
void  parseContent(char* parsUnit, char** textLines);
char* processFile(char** &textLines, int &linesCount, FILE* file);
char* getStartOfLine(char* item);
int   compare(const void* first, const void* second);

int main(int argc, char* argv[])
{
	FILE* input  = nullptr;
	FILE* output = nullptr;

	utils::load_iofiles(input, output, argc, argv);

	char** textLines  = nullptr;
	int    linesCount = -1;

	char* buffer = processFile(textLines, linesCount, input);

	assert(textLines  != nullptr);
	assert(linesCount != -1);

	utils::qSort(textLines, textLines + linesCount - 1, sizeof(char*), compare);

	for(int i = 0; i < linesCount; ++i)
	{
		printf("%s\n", getStartOfLine(textLines[i]));
	}

	free(buffer);

	utils::close_iofiles(input, output);
}

int getFileSize(FILE* file)
{
	int size = 0;

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);

	assert(size > 0);
	return size;
}

int readFile(FILE* file, char* &buffer)
{
	int size = getFileSize(file) + 2;
	buffer = (char*) calloc(size, sizeof(char));

	assert(buffer != nullptr);

	*(buffer) = *(buffer + size - 1) = '\0';
	
	fread(buffer + 1, sizeof(char), size, file);

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

	return linesCount;
}

void parseContent(char* parsUnit, char** textLines)
{
	while (*parsUnit != '\0')
	{
		if (*parsUnit == '\n')
		{
			*(textLines++) = parsUnit - 1;
			*parsUnit = '\0';
		}
		parsUnit++;
	}
	*textLines = parsUnit - 1;
}

char* processFile(char** &textLines, int &linesCount, FILE* file)
{
	char* buffer = nullptr;

	readFile(file, buffer);
	linesCount = standardizeContent(buffer + 1);

	textLines = (char**) calloc(linesCount, sizeof(char*));
	parseContent(buffer + 1, textLines);

	return buffer;
}

char* getStartOfLine(char* item)
{
	while (*item != '\0')
		item--;
	return ++item;
}

int compare(const void* first, const void* second)
{
	char* first_elem  = *(char**) first;
	char* second_elem = *(char**) second;

	first_elem  = getStartOfLine(first_elem);
	second_elem = getStartOfLine(second_elem);

	while (*first_elem == *second_elem && *first_elem != '\0')
	{
		++first_elem;
		++second_elem;
	}

	return *first_elem - *second_elem;
}