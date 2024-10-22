#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h> // 添加资源使用库

// 使用冒泡排序算法对数组进行排序
void bubbleSort(float arr[], int n) {
    int i, j;
    for (i = 0; i < n-1; i++) {
        for (j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                // 交换 arr[j] 和 arr[j+1]
                float temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

// 打印数组中的元素
void printArray(float arr[], int size) {
    int i;
    for (i = 0; i < size; i++)
        printf("%f ", arr[i]);
    printf("\n");
}

// 主函数，程序的入口
int main() {
    FILE *file;
    char filename[] = "random_data_100000.txt"; // 文件名
    float *arr; // 使用动态内存分配
    int n = 0; // 数组中元素的数量
    clock_t start, end; // 用于记录时间的变量
    double cpu_time_used; // 用于存储执行时间
    struct rusage usage_start, usage_end; // 用于记录资源使用情况

    // 打开文件
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("无法打开文件 %s\n", filename);
        return 1;
    }

    // 动态分配内存
    arr = (float *)malloc(100000 * sizeof(float));
    if (arr == NULL) {
        printf("内存分配失败\n");
        fclose(file);
        return 1;
    }

    // 读取文件中的浮点数
    while (fscanf(file, "%f", &arr[n]) != EOF) {
        n++;
    }

    // 关闭文件
    fclose(file);

    // 记录开始时间
    start = clock();

    // 记录开始时的资源使用情况
    getrusage(RUSAGE_SELF, &usage_start);

    // 对数组进行排序
    bubbleSort(arr, n);

    // 记录结束时间
    end = clock();

    // 记录结束时的资源使用情况
    getrusage(RUSAGE_SELF, &usage_end);

    // 计算执行时间
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // 打印排序后的数组
    printf("Sorted array: \n");
    printArray(arr, n);

    // 打印执行时间
    printf("执行时间: %f 秒\n", cpu_time_used);

    // 打印资源使用情况
    printf("用户 CPU 时间: %ld.%06ld 秒\n", usage_end.ru_utime.tv_sec - usage_start.ru_utime.tv_sec, usage_end.ru_utime.tv_usec - usage_start.ru_utime.tv_usec);
    printf("系统 CPU 时间: %ld.%06ld 秒\n", usage_end.ru_stime.tv_sec - usage_start.ru_stime.tv_sec, usage_end.ru_stime.tv_usec - usage_start.ru_stime.tv_usec);
    printf("最大驻留集大小: %ld KB\n", usage_end.ru_maxrss - usage_start.ru_maxrss);
    printf("页面错误数: %ld\n", usage_end.ru_majflt - usage_start.ru_majflt);

    // 打印内存占用情况
    printf("内存占用: %ld KB\n", usage_end.ru_maxrss);

    // 释放内存
    free(arr);

    return 0;
}