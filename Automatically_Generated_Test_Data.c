#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 生成随机数据的函数
void generate_random_data(int size, const char *filename, int data_type) {
    FILE *file = fopen(filename, "w"); // 打开文件以写入数据
    if (file == NULL) {
        perror("Failed to open file"); // 如果文件打开失败，输出错误信息
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        if (data_type == 0) {
            // 生成随机浮点数
            float random_float = (float)rand() / RAND_MAX * 2000.0f - 1000.0f;
            fprintf(file, "%f\n", random_float); // 将浮点数写入文件
        } else {
            // 生成随机整数
            int random_int = rand() % 2001 - 1000;
            fprintf(file, "%d\n", random_int); // 将整数写入文件
        }
    }

    fclose(file); // 关闭文件
}

int main() {
    srand(time(NULL)); // 初始化随机数生成器

    // 定义不同规模的数据集
    int data_sizes[] = {100, 1000, 10000, 100000, 1000000};
    int data_type = 0; // 0 表示浮点数，1 表示整数

    for (int i = 0; i < sizeof(data_sizes) / sizeof(data_sizes[0]); i++) {
        int size = data_sizes[i];
        char filename[256];
        snprintf(filename, sizeof(filename), "random_data_%d.txt", size); // 生成文件名
        generate_random_data(size, filename, data_type); // 生成随机数据并保存到文件
        printf("Generated and saved %d random data points to %s\n", size, filename); // 输出提示信息
    }

    return 0;
}