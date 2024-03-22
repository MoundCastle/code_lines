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
// 行数排序 大到小 比较函数
bool cmp_line(const struct typeNumber& a, const struct typeNumber& b)
{
    return a.typeLineNum > b.typeLineNum;
}
// 辅助函数计算行数个数和函数相关

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
// 辅助统计函数数据

int findPyFun(const std::string inputFileName, int T, struct funCount funCountNum[])
{
    // 打开要读取的Java文件，替换文件路径为您的实际文件路径
    std::ifstream pyFile(inputFileName);

    if (!pyFile.is_open())
    {
        std::cerr << "无法打开Java文件。" << std::endl;
        return 1;
    }

    std::string line;
    int currentLines = 0;
    bool insideFunction = false, tag = false, isValidFuncPyAll = false;
    // 使用堆栈来跟踪花括号的嵌套
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
    // 关闭py文件
    pyFile.close();

    return 0;
}

// 统计JAVA文件中函数信息
int findJavaFun(const std::string inputFileName,int T, struct funCount funCountNum[])
{
    // 打开要读取的Java文件，替换文件路径为您的实际文件路径
    std::ifstream javaFile(inputFileName);
    // 初始化'fJava'结构体来存储函数统计信息
    struct funCount fJava = { 0, 0, 0, INT_MAX };

    if (!javaFile.is_open())
    {
        std::cerr << "无法打开Java文件。" << std::endl;
        return 1;
    }

    std::string line;
    int currentLines = 0;
    bool insideFunction = false, tag = false, isValidFuncJavaAll = false, isValidFuncJava = false;
    // 使用堆栈来跟踪花括号的嵌套
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
                // 将 '{' 入栈以标记函数开始
                braceStack.push('{');
                continue;
            }
        }

        if (isValidFuncJava && braceStack.empty())
        {
            insideFunction = true;
            currentLines = 1;
            // 将 '{' 入栈以标记函数开始
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
                    // 将 '{' 入栈以跟踪嵌套花括号
                    braceStack.push('{');
                }
                else if (c == '}' && !braceStack.empty())
                {
                    // 弹出栈顶 '{' 表示花括号匹配
                    braceStack.pop();
                }

                if (braceStack.empty())
                {
                    // 函数结束，更新统计信息
                    // 增加函数总数
                    funCountNum[T].allCount++;
                    // 增加函数总行数
                    funCountNum[T].allLinesNum += currentLines;
                    // 更新最大行数
                    funCountNum[T].maxLinesNum = max(funCountNum[T].maxLinesNum, currentLines);
                    // 更新最小行数
                    funCountNum[T].minLinesNum = min(funCountNum[T].minLinesNum, currentLines);
                    //mediumLines[T][++tot2] = currentLines;
                    midLonesNum[T].push_back(currentLines);
                    insideFunction = false;
                    break;
                }
            }
        }
    }

    // 关闭Java文件
    javaFile.close();

    return 0;
}

// 统计CPP/C文件中函数信息
int findCFun(const std::string inputFileName, int T, struct funCount funCountNum[])
{
    // 初始化'fJava'结构体来存储函数统计信息
    struct funCount fC = { 0, 0, 0, INT_MAX };
    // 打开要读取的C文件，替换文件路径为您的实际文件路径
    std::ifstream CFile(inputFileName);

    if (!CFile.is_open())
    {
        std::cerr << "无法打开C文件。" << std::endl;
        return 1;
    }

    std::string line;
    int currentLines = 0;
    bool insideFunction = false, tag = false, isValidFuncCAll = false, isValidFuncC = false;
    // 使用堆栈来跟踪花括号的嵌套
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
                // 将 '{' 入栈以标记函数开始
                braceStack.push('{');
                continue;
            }
        }
        if (isValidFuncC && braceStack.empty())
        {
            insideFunction = true;
            currentLines = 1;
            // 将 '{' 入栈以标记函数开始
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
                    // 将 '{' 入栈以跟踪嵌套花括号
                    braceStack.push('{');
                }
                else if (c == '}' && !braceStack.empty())
                {
                    // 弹出栈顶 '{' 表示花括号匹配
                    braceStack.pop();
                }

                if (braceStack.empty())
                {
                    // 函数结束，更新统计信息
                    // 增加函数总数
                    funCountNum[T].allCount++;
                    // 增加函数总行数
                    funCountNum[T].allLinesNum += currentLines;
                    // 更新最大行数
                    funCountNum[T].maxLinesNum = max(funCountNum[T].maxLinesNum, currentLines);
                    // 更新最小行数
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

    // 关闭C文件
    CFile.close();

    return 0;
}

//寻找当前打开的文件夹下的文件所拥有的行数
//int findAll(const std::string pre, struct typeNumber TN[], struct funCount funCountNum[])
//{
//    // 具体代码量计算
//    std::string temp, tempFile;
//    // 目录流
//    DIR* dir;
//    // 读取目录名用
//    struct dirent* ptr;
//    // string A类型转为const char * B,给opendir()函数用
//    const char* di = pre.c_str();
//
//    if (dir = opendir(di))
//    {
//        TN[0].typeNum++;
//        while ((ptr = readdir(dir)) != NULL)
//        {
//            // 取出当前所指的文件名称
//            temp = ptr->d_name;
//            // 判断不是 .  .. 即当前目录和上级目录
//            if (temp[0] != '.' || temp.length() > 2)
//            {
//                // 获取后缀名
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
//                        // 是否为所需的文件类型
//                        if (TN[i].typeMy == strLast)
//                        {
//                            // 当前文件路径
//                            tempFile = pre + "/" + temp;
//                            if (i == 1)
//                            { // C文件
//                                findCFun(tempFile, 2,funCountNum);
//                            }
//                            else if (i == 2)
//                            { // CPP文件
//                                findCFun(tempFile, 0, funCountNum);
//                            }
//                            else if (i == 3)
//                            { // PY 文件
//                                findPyFun(tempFile, funCountNum);
//                            }
//                            else if (i == 4)
//                            { // Java文件
//                                findJavaFun(tempFile, funCountNum);
//                            }
//
//                            std::ifstream outFile;  // 执行文件操作
//                            outFile.open(tempFile); // 打开文件
//                            if (!outFile.is_open())
//                            { // 不能打开文件
//                                std::cout << "Failed to open File: " << tempFile << std::endl;
//                            }
//                            else
//                            {
//                                // 用来存储一行数据
//                                char buf[1021] = { 0 };
//                                while (outFile.getline(buf, sizeof(buf)))
//                                {
//                                    // 成功读取一行
//                                    TN[i].typeLineNum += 1;
//                                }
//                            }
//                            outFile.close();
//
//                            tagCn = 1;
//                            // 文件类型个数加一
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
    // 具体代码量计算
    std::string temp, tempFile;
    // 目录流
    std::vector<std::string> subdirs;
    std:: string post = "\\*";
    std::string search_path = pre + post;
    WIN32_FIND_DATA find_data;
    HANDLE find_handle = FindFirstFile(search_path.c_str(), &find_data); // 首个文件

    if (find_handle != INVALID_HANDLE_VALUE)
    {
        TN[0].typeNum++;
        do
        {
            if (find_data.cFileName[0] != '.')  // 跳过隐藏文件
            { 
                tempFile = pre + "\\" + find_data.cFileName;
                if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // 是文件夹
                {
                    subdirs.push_back(tempFile); // 入栈
                }
                else
                {
                    temp =  find_data.cFileName;// 获取文件名称

                    std::string strLast = temp.substr(temp.find_last_of('.') + 1);// 获取后缀名
                    for (int i = 0; i < strLast.length(); i++) // 转小写字符
                    {
                        strLast[i] = tolower(strLast[i]);
                    }
                    for (int i = 1; i < 6; i++)
                    {
                        // 是否为所需的文件类型
                        if (TN[i].typeMy == strLast)
                        {
                            // 当前文件路径
                            //tempFile = pre + "/" + temp;

                            
                            if (i == 1)
                            { // C文件   T  =  0
                                findCFun(tempFile, 0, funCountNum);
                            }
                            else if (i == 2)
                            { // CPP文件   T  =  1
                                findCFun(tempFile, 1, funCountNum);
                            }
                            else if (i == 3)
                            { // PY 文件   T  =  2
                                findPyFun(tempFile, 2, funCountNum);
                            }
                            else if (i == 4)
                            { // Java文件    T  =  3
                                findJavaFun(tempFile, 3, funCountNum);
                            }

                            std::ifstream outFile;  // 执行文件操作
                            outFile.open(tempFile); // 打开文件
                            if (!outFile.is_open())
                            { // 不能打开文件
                                std::cout << "Failed to open File: " << tempFile << std::endl;
                            }
                            else
                            {
                                // 用来存储一行数据
                                char buf[1021] = { 0 };
                                while (outFile.getline(buf, sizeof(buf)))
                                {
                                    // 成功读取一行
                                    TN[i].typeLineNum += 1;
                                }
                            }
                            outFile.close();

                            // 文件类型个数加一
                            TN[i].typeNum++;
                            break;
                        }
                    }
                    
                }
            }
        }while (FindNextFile(find_handle, &find_data));  // 查找下一个文件
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
