#include <stdlib.h>
#include <string.h>

namespace utils
{
	/**
	 * Quick sort
	 * @param start pointer on first element
	 * @param end pointer on last element
	 * @param itemSize size of element
	 * @param compare comparator
	*/
	void qSort(void* const start, void* const end, size_t itemSize, int (*compare)(const void*, const void*));

	/**
	 * Swap two elements by void pointer
	 * @param item1 first element pointer
	 * @param item2 second element pointer
	 * @param itemSize size of element
	 */
	void voidSwap(void* item1, void* item2, size_t itemSize);

	/**
	 * Move void pointer
	 * @param item pointer to moveable void pointer
	 * @param itemSize size of element
	 * @param count count of elements to move the pointer
	 */
	void voidMove(void** item, size_t itemSize, int count);

	/**
	 * Default comparator
	 * @param first first element pointer
	 * @param second second element pointer
	 * @return 0 if equal, > 0 if first element larger, < 0 if first elent lower
	 */
	int intCompare(const void* first, const void* second);

	/**
	 * Replace stdin and stdout with file
	 * @param input link to input file
	 * @param output link to output file
	 * @param argc count of argv elements
	 * @param argv array of args
	 */
	void load_iofiles(FILE* &input, FILE* &output, int argc, char* argv[]);

	/**
	 * Close input and output file if it was opened
	 * @param input input file
	 * @param output output file
	 */
	void close_iofiles(FILE* input, FILE* output);

	void qSort(void* const start, void* const end, size_t itemSize, int (*compare)(const void*, const void*))
	{
		void* low  = (void*) end;
		void* high = (void*) end;

		while (low > start)
		{
			if (compare(low, start) > 0)
			{
				voidSwap(low, high, itemSize);
				voidMove(&high, itemSize, -1);
			}
			voidMove(&low, itemSize, -1);
		}

		voidSwap(high, (void*) start, itemSize);

		low = high;
		voidMove(&low,  itemSize, -1);
		voidMove(&high, itemSize,  1);

		if (start < low) qSort(start, low, itemSize, compare);
		if (high  < end) qSort(high,  end, itemSize, compare);
	}

	void voidSwap(void* item1, void* item2, size_t itemSize)
	{
		char* first  = (char*) item1;
		char* second = (char*) item2;
		char  tmp    = 0;

		while (itemSize--)
		{
			tmp = *first;
			*(first++) = *second;
			*(second++) = tmp;
		}
	}

	void voidMove(void** item, size_t itemSize, int count)
	{
		*(char**) item += itemSize * count;
	}

	int intCompare(const void* first, const void* second)
	{
		return *(int*) first - *(int*) second;
	}

	void load_iofiles(FILE* &input, FILE* &output, int argc, char* argv[])
	{
		input  = nullptr;
		output = nullptr;

		for(int i = 1; i < argc; ++i)
		{
			if(!strcmp(argv[i], "-i") || !strcmp(argv[i], "--input"))
			{
				if(input != nullptr)
					fclose(input);

				input = freopen(argv[++i], "r", stdin);

				continue;
			}

			if(!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output"))
			{
				if(output != nullptr)
					fclose(output);

				output = freopen(argv[++i], "w", stdout);

				continue;
			}
		}
	}

	void close_iofiles(FILE* input, FILE* output)
	{
		if (input != nullptr)
			fclose(input);
		
		if (output != nullptr)
			fclose(output);
	}
}