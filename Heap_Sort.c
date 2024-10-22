#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>

void swap(float* a, float* b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(float arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(float arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

void printArray(float arr[], int size) {
    for (int i = 0; i < size; i++)
        printf("%.2f ", arr[i]);
    printf("\n");
}

int main() {
    FILE *file;
    char filename[] = "random_data_100000.txt";
    float *arr;
    int n = 0;
    int capacity = 1000000; // 初始容量设置为 1000000

    // 动态分配内存
    arr = (float *)malloc(capacity * sizeof(float));
    if (arr == NULL) {
        printf("内存分配失败\n");
        return 1;
    }

    // 打开文件
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("无法打开文件 %s\n", filename);
        free(arr);
        return 1;
    }

    // 读取文件中的浮点数
    while (fscanf(file, "%f", &arr[n]) != EOF) {
        n++;
        if (n >= capacity) {
            capacity *= 2;
            arr = (float *)realloc(arr, capacity * sizeof(float));
            if (arr == NULL) {
                printf("内存重新分配失败\n");
                fclose(file);
                return 1;
            }
        }
    }

    // 关闭文件
    fclose(file);

    // 记录开始时间
    clock_t start_time = clock();

    // 记录开始时的资源使用情况
    struct rusage start_usage, end_usage;
    getrusage(RUSAGE_SELF, &start_usage);

    // 对数组进行排序
    heapSort(arr, n);

    // 记录结束时间
    clock_t end_time = clock();

    // 记录结束时的资源使用情况
    getrusage(RUSAGE_SELF, &end_usage);

    // 计算执行时间
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    // 计算资源使用情况
    double user_time = (end_usage.ru_utime.tv_sec - start_usage.ru_utime.tv_sec) +
                       (end_usage.ru_utime.tv_usec - start_usage.ru_utime.tv_usec) / 1e6;
    double system_time = (end_usage.ru_stime.tv_sec - start_usage.ru_stime.tv_sec) +
                         (end_usage.ru_stime.tv_usec - start_usage.ru_stime.tv_usec) / 1e6;
    long max_rss = end_usage.ru_maxrss; // 最大驻留集大小（以KB为单位）

    // 打印排序后的数组
    printf("排序后的数组: \n");
    printArray(arr, n);

    // 打印执行时间
    printf("执行时间: %f 秒\n", time_taken);

    // 打印资源使用情况
    printf("用户时间: %f 秒\n", user_time);
    printf("系统时间: %f 秒\n", system_time);
    printf("最大驻留集大小: %ld KB\n", max_rss);

    // 释放内存
    free(arr);

    return 0;
}