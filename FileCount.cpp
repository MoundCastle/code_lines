#include <windows.h>
#include <dirent.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cmath>
#include <regex>
#include <stack>
#include <vector>

#include "FileCount.h"
const int maxn = 100005;
std::vector<int> midLonesNum[6];

bool cmp_num(const struct typeNumber& a, const struct typeNumber& b)
{
    return a.typeNum > b.typeNum;
}
// �������� ��С �ȽϺ���
bool cmp_line(const struct typeNumber& a, const struct typeNumber& b)
{
    return a.typeLineNum > b.typeLineNum;
}
// ���������������������ͺ������

void clearNum(struct typeNumber TN[], struct funCount funCountNum[])
{
    for (int i = 0; i < 6; i++)
    {
        midLonesNum[i].clear();
        TN[i].typeLineNum = 0;
        TN[i].typeNum = 0;
    }
    for (int i = 0; i < 4; i++)
    {
        funCountNum[i].allCount = 0;
        funCountNum[i].allLinesNum = 0;
        funCountNum[i].maxLinesNum = 0;
        funCountNum[i].minLinesNum = INT_MAX;
    }
}
// ����ͳ�ƺ�������

int findPyFun(const std::string inputFileName, int T, struct funCount funCountNum[])
{
    // ��Ҫ��ȡ��Java�ļ����滻�ļ�·��Ϊ����ʵ���ļ�·��
    std::ifstream pyFile(inputFileName);

    if (!pyFile.is_open())
    {
        std::cerr << "�޷���Java�ļ���" << std::endl;
        return 1;
    }

    std::string line;
    int currentLines = 0;
    bool insideFunction = false, tag = false, isValidFuncPyAll = false;
    // ʹ�ö�ջ�����ٻ����ŵ�Ƕ��
    std::stack<char> braceStack;

    while (std::getline(pyFile, line))
    {
        std::regex functionPatternAll("\\s*(def)(\\s+\\w+)+\\s*\\([^)]*\\)\\s*:\\s*$");
        isValidFuncPyAll = std::regex_match(line, functionPatternAll);
        if (tag)
            currentLines++;
        if (isValidFuncPyAll)
        {
            funCountNum[T].allCount++;
            if (tag)
            {
                funCountNum[T].allLinesNum += currentLines;
                midLonesNum[T].push_back(currentLines);
                //mediumLines[T][++tot1] = currentLines;
                funCountNum[T].maxLinesNum = max(currentLines, funCountNum[T].maxLinesNum);
                funCountNum[T].minLinesNum = min(currentLines, funCountNum[T].minLinesNum);
            }
            currentLines = 0;
            tag = true;
        }
    }
    if (tag)
    {
        funCountNum[T].allLinesNum += currentLines;
        midLonesNum[T].push_back(currentLines);
        //mediumLines[T][++tot1] = currentLines;
        funCountNum[T].maxLinesNum = max(currentLines, funCountNum[T].maxLinesNum);
        funCountNum[T].minLinesNum = min(currentLines, funCountNum[T].minLinesNum);
    }
    // �ر�py�ļ�
    pyFile.close();

    return 0;
}

// ͳ��JAVA�ļ��к�����Ϣ
int findJavaFun(const std::string inputFileName,int T, struct funCount funCountNum[])
{
    // ��Ҫ��ȡ��Java�ļ����滻�ļ�·��Ϊ����ʵ���ļ�·��
    std::ifstream javaFile(inputFileName);
    // ��ʼ��'fJava'�ṹ�����洢����ͳ����Ϣ
    struct funCount fJava = { 0, 0, 0, INT_MAX };

    if (!javaFile.is_open())
    {
        std::cerr << "�޷���Java�ļ���" << std::endl;
        return 1;
    }

    std::string line;
    int currentLines = 0;
    bool insideFunction = false, tag = false, isValidFuncJavaAll = false, isValidFuncJava = false;
    // ʹ�ö�ջ�����ٻ����ŵ�Ƕ��
    std::stack<char> braceStack;

    while (std::getline(javaFile, line))
    {
        std::regex functionPatternAll("\\s*(public|private|protected|static)?(\\s+[\\w\\[\\]]+)+\\s*\\([^)]*\\)\\s*$");
        std::regex functionPattern("\\s*(public|private|protected|static)?(\\s+[\\w\\[\\]]+)+\\s*\\([^)]*\\)\\s*\\{[^\\{]*");
        if (braceStack.empty())
        {
            isValidFuncJavaAll = std::regex_match(line, functionPatternAll);
            if (!isValidFuncJavaAll)
                isValidFuncJava = std::regex_match(line, functionPattern);
        }
        if (tag)
        {
            tag = false;
            std::regex functionPattern("^\\s*\\{[^\\{]*");
            if (std::regex_match(line, functionPattern))
            {
                insideFunction = true;
                currentLines = 2;
                // �� '{' ��ջ�Ա�Ǻ�����ʼ
                braceStack.push('{');
                continue;
            }
        }

        if (isValidFuncJava && braceStack.empty())
        {
            insideFunction = true;
            currentLines = 1;
            // �� '{' ��ջ�Ա�Ǻ�����ʼ
            braceStack.push('{');
            continue;
        }
        else if (isValidFuncJavaAll && braceStack.empty())
        {
            tag = true;
        }

        if (insideFunction)
        {
            currentLines++;

            for (char c : line)
            {
                if (c == '{')
                {
                    // �� '{' ��ջ�Ը���Ƕ�׻�����
                    braceStack.push('{');
                }
                else if (c == '}' && !braceStack.empty())
                {
                    // ����ջ�� '{' ��ʾ������ƥ��
                    braceStack.pop();
                }

                if (braceStack.empty())
                {
                    // ��������������ͳ����Ϣ
                    // ���Ӻ�������
                    funCountNum[T].allCount++;
                    // ���Ӻ���������
                    funCountNum[T].allLinesNum += currentLines;
                    // �����������
                    funCountNum[T].maxLinesNum = max(funCountNum[T].maxLinesNum, currentLines);
                    // ������С����
                    funCountNum[T].minLinesNum = min(funCountNum[T].minLinesNum, currentLines);
                    //mediumLines[T][++tot2] = currentLines;
                    midLonesNum[T].push_back(currentLines);
                    insideFunction = false;
                    break;
                }
            }
        }
    }

    // �ر�Java�ļ�
    javaFile.close();

    return 0;
}

// ͳ��CPP/C�ļ��к�����Ϣ
int findCFun(const std::string inputFileName, int T, struct funCount funCountNum[])
{
    // ��ʼ��'fJava'�ṹ�����洢����ͳ����Ϣ
    struct funCount fC = { 0, 0, 0, INT_MAX };
    // ��Ҫ��ȡ��C�ļ����滻�ļ�·��Ϊ����ʵ���ļ�·��
    std::ifstream CFile(inputFileName);

    if (!CFile.is_open())
    {
        std::cerr << "�޷���C�ļ���" << std::endl;
        return 1;
    }

    std::string line;
    int currentLines = 0;
    bool insideFunction = false, tag = false, isValidFuncCAll = false, isValidFuncC = false;
    // ʹ�ö�ջ�����ٻ����ŵ�Ƕ��
    std::stack<char> braceStack;

    while (std::getline(CFile, line))
    {
        std::regex functionPatternAll(R"(\s*[\w<>]+(\s+[\w\:]+)+\s*\([^)]*\)\s*$)");
        std::regex functionPattern(R"(\s*[\w<>]+(\s+[\w\:]+)+\s*\([^)]*\)\s*\{\s*)");
       /* std::regex functionPatternAll(R"(\s*[\w<>]+(\s+[\w\:\[\]]+)+\s*\([^)]*\)\s*$)");
        std::regex functionPattern(R"(\s*[\w<>]+(\s+[\w\:\[\]]+)+\s*\([^)]*\)\s*\{\s*)");*/
        if (braceStack.empty())
        {
            isValidFuncCAll = std::regex_match(line, functionPatternAll);
            if(!isValidFuncCAll)
                isValidFuncC = std::regex_match(line, functionPattern);
        }

        if (tag)
        {
            tag = false;
            std::regex functionPattern("^\\s*\\{[^\\{]*");
            if (std::regex_match(line, functionPattern))
            {
                insideFunction = true;
                currentLines = 2;
                // �� '{' ��ջ�Ա�Ǻ�����ʼ
                braceStack.push('{');
                continue;
            }
        }
        if (isValidFuncC && braceStack.empty())
        {
            insideFunction = true;
            currentLines = 1;
            // �� '{' ��ջ�Ա�Ǻ�����ʼ
            braceStack.push('{');
            continue;
        }
        else if (isValidFuncCAll && braceStack.empty())
        {
            tag = true;
        }

        if (insideFunction)
        {
            currentLines++;
            for (char c : line)
            {
                if (c == '\'' || c == '\"')
                    break;
                if (c == '{')
                {
                    // �� '{' ��ջ�Ը���Ƕ�׻�����
                    braceStack.push('{');
                }
                else if (c == '}' && !braceStack.empty())
                {
                    // ����ջ�� '{' ��ʾ������ƥ��
                    braceStack.pop();
                }

                if (braceStack.empty())
                {
                    // ��������������ͳ����Ϣ
                    // ���Ӻ�������
                    funCountNum[T].allCount++;
                    // ���Ӻ���������
                    funCountNum[T].allLinesNum += currentLines;
                    // �����������
                    funCountNum[T].maxLinesNum = max(funCountNum[T].maxLinesNum, currentLines);
                    // ������С����
                    funCountNum[T].minLinesNum = min(funCountNum[T].minLinesNum, currentLines);
                    /*if (T == 0)
                     mediumLines[T][++tot3] = currentLines;
                    if (T == 1)
                        mediumLines[T][++tot4] = currentLines;*/
                    midLonesNum[T].push_back(currentLines);
                    insideFunction = false;
                    break;
                }
            }
        }
    }

    // �ر�C�ļ�
    CFile.close();

    return 0;
}

//Ѱ�ҵ�ǰ�򿪵��ļ����µ��ļ���ӵ�е�����
//int findAll(const std::string pre, struct typeNumber TN[], struct funCount funCountNum[])
//{
//    // �������������
//    std::string temp, tempFile;
//    // Ŀ¼��
//    DIR* dir;
//    // ��ȡĿ¼����
//    struct dirent* ptr;
//    // string A����תΪconst char * B,��opendir()������
//    const char* di = pre.c_str();
//
//    if (dir = opendir(di))
//    {
//        TN[0].typeNum++;
//        while ((ptr = readdir(dir)) != NULL)
//        {
//            // ȡ����ǰ��ָ���ļ�����
//            temp = ptr->d_name;
//            // �жϲ��� .  .. ����ǰĿ¼���ϼ�Ŀ¼
//            if (temp[0] != '.' || temp.length() > 2)
//            {
//                // ��ȡ��׺��
//                std::string strLast = temp.substr(temp.find_last_of('.') + 1);
//                if (strLast != temp)
//                {
//                    int tagCn = 0;
//                    for (int i = 0; i < strLast.length(); i++)
//                    {
//                        strLast[i] = tolower(strLast[i]);
//                    }
//                    for (int i = 1; i < 6; i++)
//                    {
//                        // �Ƿ�Ϊ������ļ�����
//                        if (TN[i].typeMy == strLast)
//                        {
//                            // ��ǰ�ļ�·��
//                            tempFile = pre + "/" + temp;
//                            if (i == 1)
//                            { // C�ļ�
//                                findCFun(tempFile, 2,funCountNum);
//                            }
//                            else if (i == 2)
//                            { // CPP�ļ�
//                                findCFun(tempFile, 0, funCountNum);
//                            }
//                            else if (i == 3)
//                            { // PY �ļ�
//                                findPyFun(tempFile, funCountNum);
//                            }
//                            else if (i == 4)
//                            { // Java�ļ�
//                                findJavaFun(tempFile, funCountNum);
//                            }
//
//                            std::ifstream outFile;  // ִ���ļ�����
//                            outFile.open(tempFile); // ���ļ�
//                            if (!outFile.is_open())
//                            { // ���ܴ��ļ�
//                                std::cout << "Failed to open File: " << tempFile << std::endl;
//                            }
//                            else
//                            {
//                                // �����洢һ������
//                                char buf[1021] = { 0 };
//                                while (outFile.getline(buf, sizeof(buf)))
//                                {
//                                    // �ɹ���ȡһ��
//                                    TN[i].typeLineNum += 1;
//                                }
//                            }
//                            outFile.close();
//
//                            tagCn = 1;
//                            // �ļ����͸�����һ
//                            TN[i].typeNum++;
//                            break;
//                        }
//                    }
//                    if (tagCn == 1)
//                        continue;
//                }
//                temp = pre + "/" + temp;
//                findAll(temp, TN, funCountNum);
//            }
//        }
//        closedir(dir);
//    }
//    return 0;
//}

int findAll(const std::string pre, struct typeNumber TN[], struct funCount funCountNum[])
{
    // �������������
    std::string temp, tempFile;
    // Ŀ¼��
    std::vector<std::string> subdirs;
    std:: string post = "\\*";
    std::string search_path = pre + post;
    WIN32_FIND_DATA find_data;
    HANDLE find_handle = FindFirstFile(search_path.c_str(), &find_data); // �׸��ļ�

    if (find_handle != INVALID_HANDLE_VALUE)
    {
        TN[0].typeNum++;
        do
        {
            if (find_data.cFileName[0] != '.')  // ���������ļ�
            { 
                tempFile = pre + "\\" + find_data.cFileName;
                if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // ���ļ���
                {
                    subdirs.push_back(tempFile); // ��ջ
                }
                else
                {
                    temp =  find_data.cFileName;// ��ȡ�ļ�����

                    std::string strLast = temp.substr(temp.find_last_of('.') + 1);// ��ȡ��׺��
                    for (int i = 0; i < strLast.length(); i++) // תСд�ַ�
                    {
                        strLast[i] = tolower(strLast[i]);
                    }
                    for (int i = 1; i < 6; i++)
                    {
                        // �Ƿ�Ϊ������ļ�����
                        if (TN[i].typeMy == strLast)
                        {
                            // ��ǰ�ļ�·��
                            //tempFile = pre + "/" + temp;

                            
                            if (i == 1)
                            { // C�ļ�   T  =  0
                                findCFun(tempFile, 0, funCountNum);
                            }
                            else if (i == 2)
                            { // CPP�ļ�   T  =  1
                                findCFun(tempFile, 1, funCountNum);
                            }
                            else if (i == 3)
                            { // PY �ļ�   T  =  2
                                findPyFun(tempFile, 2, funCountNum);
                            }
                            else if (i == 4)
                            { // Java�ļ�    T  =  3
                                findJavaFun(tempFile, 3, funCountNum);
                            }

                            std::ifstream outFile;  // ִ���ļ�����
                            outFile.open(tempFile); // ���ļ�
                            if (!outFile.is_open())
                            { // ���ܴ��ļ�
                                std::cout << "Failed to open File: " << tempFile << std::endl;
                            }
                            else
                            {
                                // �����洢һ������
                                char buf[1021] = { 0 };
                                while (outFile.getline(buf, sizeof(buf)))
                                {
                                    // �ɹ���ȡһ��
                                    TN[i].typeLineNum += 1;
                                }
                            }
                            outFile.close();

                            // �ļ����͸�����һ
                            TN[i].typeNum++;
                            break;
                        }
                    }
                    
                }
            }
        }while (FindNextFile(find_handle, &find_data));  // ������һ���ļ�
        FindClose(find_handle);
    }
    for (const auto& tempDir : subdirs)
    {
        findAll(tempDir, TN, funCountNum);
    }
    return 0;
}

int findAllcode(const std::string pre, struct typeNumber TN[], struct funCount funCountNum[])
{
    clearNum(TN, funCountNum);
    findAll(pre, TN, funCountNum);
    for (int i = 0; i <= 3; i++)
    {
        if (midLonesNum[i].size() == 0) {
            funCountNum[i].mediumLinesNum = 0;
        }
        else {
            std::sort(midLonesNum[i].begin(),midLonesNum[i].end());
            funCountNum[i].mediumLinesNum = midLonesNum[i][midLonesNum[i].size() / 2];
        }
    }
    return 1;
}
