#include"utils.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define test(n)                        \
if(test##n())                          \
    printf("Test " #n " faild\n");     \
else                                   \
    printf("Test " #n  " passed\n")

int test1()
{
    int a = 0;
    int b = 0;

    if (utils::intCompare(&a, &b) != 0)
        return 1;

    a = 23;
    b = -8;

    if (utils::intCompare(&a, &b) <= 0)
        return 1;
    
    a = 92;
    b = 2123;

    if (utils::intCompare(&a, &b) >= 0)
        return 1;
    
    return 0;
}

int test2()
{
    int test_list [10] = {1, 2, 9, 8, 3, 5, 4, 7, 6, 0};
    int check_list [10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    utils::qSort(test_list, test_list + 9, sizeof(int), utils::intCompare);

    for (int i = 0; i < 10; ++i)
        if (test_list[i] != check_list[i])
            return 1;
    
    return 0;
}

int test3()
{
    int test_list[10]  = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int check_list[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    utils::qSort(test_list, test_list + 9, sizeof(int), utils::intCompare);

    for(int i = 0; i < 10; i++)
        if(test_list[i] != check_list[i])
            return 1;

    return 0;
}

clock_t timeTest1()
{
    const int COUNT = 250000;
    int test_list[COUNT] = {0};
    int check_list[COUNT] = {0};
    clock_t test_time = {0};

    for (int i = 0; i < COUNT; ++i)
    {
        test_list[i] = rand() % 1000;
        check_list[i] = test_list[i];
    }

    qsort(check_list, COUNT, sizeof(int), utils::intCompare);

    test_time = clock();
    utils::qSort(test_list, test_list + COUNT - 1, sizeof(int), utils::intCompare);
    test_time = clock() - test_time;

    return test_time;
}

int main()
{
    // Compare test
    test(1);

    // Basic test
    test(2);

    // Sorted test
    test(3);

    // Time test
    printf("Time test end (%.2f seconds)\n",
        (float) timeTest1() / CLOCKS_PER_SEC
    );
}