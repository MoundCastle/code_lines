#include <iostream>
#include <fstream>
#include "FileCount.h"
#include "toCsv.h"

void drawCsv(const std::string filename, struct typeNumber TN[], struct funCount funCountNum[])
{
    char temp1[2000] = "";
    std::string temp[9] = {"文件类型", "文件个数", "文件总行数",  "总函数个数", "总函数行数", "最大函数行数", "最小函数函数","函数行数中位数", "平均函数行数" };

    //writebuild["emitUTF8"] = true;  //utf8支持,加这句,utf8的中文字符会编程
    //写入文件
    /*std::ofstream ofile(filename);
    ofile << document;
    ofile.close();*/

    std::ofstream outFile;
    outFile.open(filename, std::ios::out | std::ios::trunc);

     // 写入文件相关标题行
     outFile << temp[0] << ','; 
     for (int i = 1; i < TN_LONG; i++)
     {
         outFile << TN[i].typeMy << ','; 
     }
     outFile << std::endl;

     outFile << temp[1] << ',';
     for (int i = 1; i < TN_LONG; i++)
     {
         outFile << TN[i].typeNum << ',';
     }
     outFile << std::endl;

     outFile << temp[2] << ',';
     for (int i = 1; i < TN_LONG; i++)
     {
         outFile << TN[i].typeLineNum << ',';
     }
     outFile << std::endl;

     outFile << temp[3] << ',';
     for (int i = 1; i < TN_LONG; i++)
     {
         outFile << funCountNum[i - 1].allCount << ',';
     }
     outFile << std::endl;

     outFile << temp[4] << ',';
     for (int i = 1; i < TN_LONG; i++)
     {
         outFile << funCountNum[i - 1].allLinesNum << ',';
     }
     outFile << std::endl;

     outFile << temp[5] << ',';
     for (int i = 1; i < TN_LONG; i++)
     {
         outFile << funCountNum[i - 1].maxLinesNum << ',';
     }
     outFile << std::endl;

     outFile << temp[6] << ',';

     for (int i = 1; i < TN_LONG; i++)
     {
         if (funCountNum[i - 1].allCount == 0)
         {
             outFile << 0 << ',';
         }
         else
         {
             outFile << funCountNum[i - 1].minLinesNum << ',';
         }
     }
     outFile << std::endl;

     outFile << temp[7] << ',';
     for (int i = 1; i < TN_LONG; i++)
     {
         outFile << funCountNum[i - 1].mediumLinesNum << ',';
     }

     outFile << std::endl;
     outFile << temp[8] << ',';
     for (int i = 1; i < TN_LONG; i++)
     {
         if (funCountNum[i - 1].allCount == 0)
         {
             outFile << 0 << ',';
         }
         else
         {
             outFile << funCountNum[i - 1].allLinesNum / (float)funCountNum[i - 1].allCount << ',';
         }
     }
     outFile << std::endl;

     outFile.close();
}
