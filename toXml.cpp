#include <iostream>
#include <fstream>
#include "FileCount.h"
#include "toXml.h"

void drawXml(const std::string filename, struct typeNumber TN[], struct funCount funCountNum[])
{
    std::ofstream outFile;
    outFile.open(filename, std::ios::out | std::ios::trunc);

    // д���ļ���ر�����
    outFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;

    outFile << "<root1>" << std::endl;

    for (int i = 1; i < TN_LONG; i++) {
        int meanNum = 0,minNum = 0;
        if(funCountNum[i - 1].allCount != 0) {
            meanNum = funCountNum[i - 1].allLinesNum / funCountNum[i - 1].allCount;
            minNum = funCountNum[i - 1].minLinesNum;
        }
         
        outFile << "<table1>" << std::endl; 

        outFile << "<type>" << TN[i].typeMy << "</type>" << std::endl;                                // �ļ�����
        outFile << "<typeNum>" <<TN[i].typeNum << "</typeNum>" << std::endl;                          //�ļ�����
        outFile << "<typeLineNum>" << TN[i].typeLineNum << "</typeLineNum>" << std::endl;             //�ļ�������
        outFile << "<allFuncsNum>" << funCountNum[i - 1].allCount<< "</allFuncsNum>" << std::endl;    //�ļ����� ����
        outFile << "<allLinesNum>" << funCountNum[i - 1].allLinesNum << "</allLinesNum>" << std::endl;//�ļ����� ������
        outFile << "<maxLinesNum>" << funCountNum[i - 1].maxLinesNum << "</maxLinesNum>" << std::endl;//�������� �������
        outFile << "<minLinesNum>" << minNum<< "</minLinesNum>" << std::endl;                         //�������� ��С����
        outFile << "<mediumLinesNum>" << funCountNum[i - 1].mediumLinesNum << "</mediumLinesNum>" << std::endl;//�������� ��λ��
        outFile << "<meanLinesNum>" << meanNum << "</meanLinesNum>" << std::endl;                     //�������� ƽ����

        outFile << "</table1>" << std::endl;
    }

    outFile << "</root1>" << std::endl;

    outFile << std::endl;

    outFile.close();
}
