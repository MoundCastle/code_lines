#include <iostream>
#include <fstream>
#include "FileCount.h"
#include "toCsv.h"

void drawCsv(const std::string filename, struct typeNumber TN[], struct funCount funCountNum[])
{
    char temp1[2000] = "";
    std::string temp[9] = {"�ļ�����", "�ļ�����", "�ļ�������",  "�ܺ�������", "�ܺ�������", "���������", "��С��������","����������λ��", "ƽ����������" };

    //writebuild["emitUTF8"] = true;  //utf8֧��,�����,utf8�������ַ�����
    //д���ļ�
    /*std::ofstream ofile(filename);
    ofile << document;
    ofile.close();*/

    std::ofstream outFile;
    outFile.open(filename, std::ios::out | std::ios::trunc);

     // д���ļ���ر�����
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
