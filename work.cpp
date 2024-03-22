#include <graphics.h> // 包含EGE的头文件

#include <dirent.h>
#include <windows.h>  
#include <cstdlib>  
#include <cstring>  
#include <ege/sys_edit.h> // sys_edit.h 头文件需要在 graphics.h 或 ege.h 之后包含
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>  
#include <cmath>
#include <regex>
#include <chrono>
#include <stack>
#include <vector>
#include "json/json.h" //jsoncpp的头文件
#include "Shlobj.h"
#include "tchar.h" 
#include <shellapi.h>  

#include "FileCount.h"
#include "toJosn.h"
#include "toCsv.h"
#include "toXml.h"
extern struct typeNumber TN[6] = { {"dir", 0, 0}, {"c", 0, 0}, {"cpp", 0, 0}, {"py", 0, 0}, {"java", 0, 0}, {"h", 0, 0} };
extern struct funCount funCountNum[6] = { {0, 0, 0, INT_MAX, 0, "C"}, {0, 0, 0, INT_MAX, 0, "Cpp"},
 {0, 0, 0, INT_MAX, 0, "Py"}, {0, 0, 0, INT_MAX, 0, "Java"},{0, 0, 0, INT_MAX, 0, "H"},{0, 0, 0, INT_MAX, 0, "NULL"} };

// 定义按钮结构体
struct RectButton
{
    int x, y;
    int width, height;
};

// 查看是否点击是否位于按钮内
bool insideRectButton(const RectButton* button, int x, int y)
{
    return (x >= button->x) && (y >= button->y) && (x < button->x + button->width) && (y < button->y + button->height);
}
// 画按钮
void drawRectButton(const RectButton* button)
{
    setfillcolor(0xFFFFFFFF);
    ege_fillrect(button->x, button->y, button->width, button->height);
    outtextrect(button->x, button->y, button->width, button->height, "开始");
}

// 在 x y 处输出字符串 最大200个 返回值:字符串长度
int xyStrCout(int x, int y, std::string str)
{
    char temp1[200] = "";
    sprintf(temp1, "%s", str.data());
    xyprintf(x, y, temp1);
    return str.length();
}

// 初始化界面
void init()
{
    int WEIGHT = 1000, HEIGHT = 600;
    initgraph(WEIGHT, HEIGHT, INIT_RENDERMANUAL);
    // 对窗口开启抗锯齿
    ege_enable_aa(true);

    setbkcolor(EGEARGB(0xFF, 196, 203, 207));
    setcolor(0xFF323777);
    setfont(24, 0, "楷体");
    setbkmode(TRANSPARENT);
}

void drawCircle(struct typeNumber TN[])
{
    // 画扇形图 totalNum 存放总文件个数；  totalLineNum 存放总文件行数
    int totalNum = 0, totalLineNum = 0;
    float begin = 0, end = 0;
    // 设置每块区域颜色
    unsigned int color[6] = { 0x99FFB6C1, 0x99DC143C, 0x9900BFFF, 0x998B008B, 0x993CB371, 0x99FFFF00 };
    for (int i = 1; i < 6; i++)
    {
        totalNum += TN[i].typeNum;
        totalLineNum += TN[i].typeLineNum;
    }

    // 行数/个数全为空画一个灰色的圆
    if (TN[0].typeNum == 0)
    {
        std::sort(TN + 1, TN + 6, cmp_num);
        for (int i = 1; i < 6; i++)
        {
            setfillcolor(color[i]);
            xyStrCout(940, 300 + i * 30, TN[i].typeMy);
            ege_fillrect(920, 300 + i * 30, 20, 20);
            setfillcolor(0xFFd1c2d3);
            ege_fillpie(620, 310, 280, 280, 0, 360);
        }
    }
    else
    {
        // 行数/个数不是全空按照大小顺序画扇形图
        for (int i = 1; i < 6; i++)
        {
            setfillcolor(color[i]);
            xyStrCout(940, 300 + i * 30, TN[i].typeMy);
            ege_fillrect(920, 300 + i * 30, 20, 20);
            if (TN[i].typeNum != 0)
            {
                end = (TN[i].typeNum * 360 / (float)totalNum);
                ege_fillpie(620, 310, 280, 280, begin, end);
                begin += end;
            }
        }
    }
}

//std::string selectDir()
//{
//    TCHAR szBuffer[MAX_PATH] = { 0 };   // 存放选择文件的路径   
//    BROWSEINFO bi;
//    ZeroMemory(&bi, sizeof(BROWSEINFO));
//    bi.hwndOwner = NULL;
//    bi.pszDisplayName = szBuffer;
//    bi.lpszTitle = _T("从下面选择文件或文件夹:");   // _T() 是头文件 tchar.h 下的一个宏定义。   
//    bi.ulFlags = BIF_BROWSEINCLUDEFILES;
//    LPITEMIDLIST idl = SHBrowseForFolder(&bi);   // 开始选择文件或文件夹  
//    if (NULL == idl)
//    {
//        return "";
//    }
//    SHGetPathFromIDList(idl, szBuffer);  // 获取完整路径，否则 szBuffer 只会存储当前选择的文件或文件夹名称  
//
//    // 将宽字符 szBuffer 转换为多字符字符串并返回 std::string  
//    char narrowBuffer[MAX_PATH];
//    int numChars = WideCharToMultiByte(CP_ACP, 0, (LPCWCH)szBuffer, -1, narrowBuffer, MAX_PATH, NULL, NULL);
//    if (numChars > 0)
//    {
//        std::string narrowString(narrowBuffer);
//        std::cout << "您选择的文件或文件夹的路径为：" << narrowString << std::endl;
//        return narrowString;
//    }
//    else
//    {
//        return "";
//    }
//}

int draw()
{
    // 声明输入框
    sys_edit editBox;
    // 字体高度
    int editTextHeight = 18;
    // 创建，false表示单行
    editBox.create(false);
    // 设置输入框位置
    editBox.move(100, 100);
    // 设置尺寸，高度一般要比字的高多一点，因为输入框的边框也占一定大小。
    editBox.size(editTextHeight * 20, 32);
    // 设置背景颜色
    editBox.setbgcolor(EGERGB(249, 236, 195));
    editBox.setcolor(BLACK);
    editBox.setfont(editTextHeight, 0, "楷体");
    // 使可见
    editBox.visible(true);
    // 获取焦点
    editBox.setfocus();

    const int buffSize = 200; // 创建输入缓存区
    char strBuffer[200] = "";
    //std::string path_str = "";
    setfont(24, 0, "楷体");
    ege_line(600, 0, 600, 600);

    bool isOutput = false;
    RectButton button = {
        500, 100, /* x, y */
        50, 30,   /* width, height */
    };
    bool clickButton = false;

    for (; is_run(); delay_fps(60))
    {
        ege_line(600, 0, 600, 600);
        xyprintf(100, 70, "输入文件夹路径，点击开始");
        drawRectButton(&button);

        // 检测到回车键弹起就标记输出
        while (mousemsg())
        {
            mouse_msg msg = getmouse();
            // 判断鼠标左键点击（左键按下确定位置，抬起为执行时刻）
            if (msg.is_left())
            {
                if (msg.is_down())
                { // 检测点击的按钮
                    clickButton = insideRectButton(&button, msg.x, msg.y);
                }
                else
                {
                    // 左键抬起，判断是否需要执行事件响应
                    if (clickButton)
                    {
                        // 清屏 不消除 输入框
                        cleardevice();
                        //xyprintf(630, 40, "请耐心等待");
                        clickButton = false;
                        isOutput = true;
                    }
                }
            }
        }

        if (isOutput )
        {
            // Start measuring time
            auto begin = std::chrono::high_resolution_clock::now();

            // 获取输入框内容字符串 char*
            editBox.gettext(buffSize, strBuffer);
            // 获取内容后后清空输入框，设置内容为空字符串
            editBox.settext("");
            if (strBuffer[0] != '\0')
            {
                outtextxy(100, 150, "输入内容:");
                rectprintf(100, 180, 700, 220, "%s", strBuffer);
                std::string pre = strBuffer;

                findAllcode(pre,TN, funCountNum);
                // 写文件相关内容
                for (int i = 1; i < 6; i++)
                {
                    sprintf(strBuffer, "%-5s 文件个数:%-10d 总行数: %d", TN[i].typeMy.data(), TN[i].typeNum, TN[i].typeLineNum);
                    xyprintf(50, 30 * i + 200, strBuffer);
                }
                
                // 写函数统计相关内容
                xyprintf(20, 30 * 5 + 240, "文件类型:");
                xyprintf(20, 30 * 6 + 240, "总函数个数:");
                xyprintf(20, 30 * 7 + 240, "总函数行数:");
                xyprintf(20, 30 * 8 + 240, "最大函数行数:");
                xyprintf(20, 30 * 9 + 240, "最小函数行数:");
                xyprintf(20, 30 * 10 + 240, "函数行数中位数:");
                xyprintf(20, 30 * 11 + 240, "平均函数行数:");
                for (int i = 0; i < 4; i++)
                {
                    sprintf(strBuffer, "%-4s", funCountNum[i].typeMy.data());
                    xyprintf(200 + i * 100, 30 * 5 + 240, strBuffer);
                    sprintf(strBuffer, "%-4d", funCountNum[i].allCount);
                    xyprintf(200 + i * 100, 30 * 6 + 240, strBuffer);
                    sprintf(strBuffer, "%-4d", funCountNum[i].allLinesNum);
                    xyprintf(200 + i * 100, 30 * 7 + 240, strBuffer);
                    sprintf(strBuffer, "%-4d", funCountNum[i].maxLinesNum);
                    xyprintf(200 + i * 100, 30 * 8 + 240, strBuffer);
                    sprintf(strBuffer, "%-4d", funCountNum[i].mediumLinesNum);
                    xyprintf(200 + i * 100, 30 * 9 + 240, strBuffer);
                    if (funCountNum[i].allCount == 0)
                    {
                        sprintf(strBuffer, "%-4d", 0);
                        xyprintf(200 + i * 100, 30 * 10 + 240, strBuffer);
                        sprintf(strBuffer, "%-4d", funCountNum[i].allLinesNum);
                    }
                    else
                    {
                        sprintf(strBuffer, "%-4d", funCountNum[i].minLinesNum);
                        xyprintf(200 + i * 100, 30 * 10 + 240, strBuffer);
                        sprintf(strBuffer, "%-4.2f", funCountNum[i].allLinesNum / (float)funCountNum[i].allCount);
                    }
                    xyprintf(200 + i * 100, 30 * 11 + 240, strBuffer);
                   
                }
                
                // 生成并输出json文件， 有则修改、无则创建
                drawJson("D:\\VScode\\visual\\Project1\\OutPut\\OutJson.json", TN, funCountNum);
                drawCsv("D:\\VScode\\visual\\Project1\\OutPut\\OutCsv.csv", TN, funCountNum);
                drawXml("D:\\VScode\\visual\\Project1\\OutPut\\OutXml.xml", TN, funCountNum);
                
                
                // 画出扇形图
                drawCircle(TN);

                auto end = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

                xyprintf(630, 100, "运行用时：");
                sprintf(strBuffer, "%.3fs", elapsed.count() * 1e-9);
                xyprintf(760 , 100, strBuffer);

            }
            isOutput = false;
        }
    }
    // 如果退出循环，直接getch()是无法阻塞的，因为输入的时候也在产生按键消息需要先清空按键消息
    flushkey();
    // 清空鼠标消息
    flushmouse();
    closegraph();
    return 0;
}


int main()
{
    std::ios::sync_with_stdio(false);
    init();
    draw();

    system("pause");
    return 0;
}
