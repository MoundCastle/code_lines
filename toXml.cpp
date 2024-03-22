#include <iostream>
#include <fstream>
#include "FileCount.h"
#include "toXml.h"

void drawXml(const std::string filename, struct typeNumber TN[], struct funCount funCountNum[])
{
    std::ofstream outFile;
    outFile.open(filename, std::ios::out | std::ios::trunc);

    // 写入文件相关标题行
    outFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;

    outFile << "<root1>" << std::endl;

    for (int i = 1; i < TN_LONG; i++) {
        int meanNum = 0,minNum = 0;
        if(funCountNum[i - 1].allCount != 0) {
            meanNum = funCountNum[i - 1].allLinesNum / funCountNum[i - 1].allCount;
            minNum = funCountNum[i - 1].minLinesNum;
        }
         
        outFile << "<table1>" << std::endl; 

        outFile << "<type>" << TN[i].typeMy << "</type>" << std::endl;                                // 文件类型
        outFile << "<typeNum>" <<TN[i].typeNum << "</typeNum>" << std::endl;                          //文件个数
        outFile << "<typeLineNum>" << TN[i].typeLineNum << "</typeLineNum>" << std::endl;             //文件总行数
        outFile << "<allFuncsNum>" << funCountNum[i - 1].allCount<< "</allFuncsNum>" << std::endl;    //文件函数 总数
        outFile << "<allLinesNum>" << funCountNum[i - 1].allLinesNum << "</allLinesNum>" << std::endl;//文件函数 总行数
        outFile << "<maxLinesNum>" << funCountNum[i - 1].maxLinesNum << "</maxLinesNum>" << std::endl;//函数行数 最大行数
        outFile << "<minLinesNum>" << minNum<< "</minLinesNum>" << std::endl;                         //函数函数 最小行数
        outFile << "<mediumLinesNum>" << funCountNum[i - 1].mediumLinesNum << "</mediumLinesNum>" << std::endl;//函数行数 中位数
        outFile << "<meanLinesNum>" << meanNum << "</meanLinesNum>" << std::endl;                     //函数行数 平均数

        outFile << "</table1>" << std::endl;
    }

    outFile << "</root1>" << std::endl;

    outFile << std::endl;

    outFile.close();
}
