#pragma once
#include <iostream>
#include <vector>

#ifndef MY_COUNT //放在XX.h被重复引用

#define MY_COUNT
#define TN_LONG 6
#define FunCOUNTNUM_LONG 4
// 定义文件统计结构体
struct typeNumber
{
    std::string typeMy;
    // 该类型文件个数
    int typeNum;
    // 该类型文件总行数
    int typeLineNum;
};

// 定义函数统计结构体
 struct funCount
{
    int allCount;    // 函数总个数
    int allLinesNum; // 函数总行数
    int maxLinesNum; // 最大行数
    int minLinesNum; // 最小行数
    int mediumLinesNum; // 统计中位数(新增)
    std::string typeMy;
};


// 个数排序 大到小 比较函数
bool cmp_num(const struct typeNumber& a, const struct typeNumber& b);
bool cmp_line(const struct typeNumber& a, const struct typeNumber& b);

void clearNum(struct typeNumber TN[], struct funCount funCountNum[]);

int findPyFun(const std::string inputFileName, int T,struct funCount funCountNum[]);
int findJavaFun(const std::string inputFileName,int T, struct funCount funCountNum[]);
int findCFun(const std::string inputFileName, int T, struct funCount funCountNum[]);
int findAll(const std::string pre, struct typeNumber TN[], struct funCount funCountNum[]);
int findAllcode(const std::string pre, struct typeNumber TN[], struct funCount funCountNum[]);

#endif