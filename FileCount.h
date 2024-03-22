#pragma once
#include <iostream>
#include <vector>

#ifndef MY_COUNT //����XX.h���ظ�����

#define MY_COUNT
#define TN_LONG 6
#define FunCOUNTNUM_LONG 4
// �����ļ�ͳ�ƽṹ��
struct typeNumber
{
    std::string typeMy;
    // �������ļ�����
    int typeNum;
    // �������ļ�������
    int typeLineNum;
};

// ���庯��ͳ�ƽṹ��
 struct funCount
{
    int allCount;    // �����ܸ���
    int allLinesNum; // ����������
    int maxLinesNum; // �������
    int minLinesNum; // ��С����
    int mediumLinesNum; // ͳ����λ��(����)
    std::string typeMy;
};


// �������� ��С �ȽϺ���
bool cmp_num(const struct typeNumber& a, const struct typeNumber& b);
bool cmp_line(const struct typeNumber& a, const struct typeNumber& b);

void clearNum(struct typeNumber TN[], struct funCount funCountNum[]);

int findPyFun(const std::string inputFileName, int T,struct funCount funCountNum[]);
int findJavaFun(const std::string inputFileName,int T, struct funCount funCountNum[]);
int findCFun(const std::string inputFileName, int T, struct funCount funCountNum[]);
int findAll(const std::string pre, struct typeNumber TN[], struct funCount funCountNum[]);
int findAllcode(const std::string pre, struct typeNumber TN[], struct funCount funCountNum[]);

#endif