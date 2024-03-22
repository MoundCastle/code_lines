#include <graphics.h> // ����EGE��ͷ�ļ�

#include <dirent.h>
#include <windows.h>  
#include <cstdlib>  
#include <cstring>  
#include <ege/sys_edit.h> // sys_edit.h ͷ�ļ���Ҫ�� graphics.h �� ege.h ֮�����
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>  
#include <cmath>
#include <regex>
#include <chrono>
#include <stack>
#include <vector>
#include "json/json.h" //jsoncpp��ͷ�ļ�
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

// ���尴ť�ṹ��
struct RectButton
{
    int x, y;
    int width, height;
};

// �鿴�Ƿ����Ƿ�λ�ڰ�ť��
bool insideRectButton(const RectButton* button, int x, int y)
{
    return (x >= button->x) && (y >= button->y) && (x < button->x + button->width) && (y < button->y + button->height);
}
// ����ť
void drawRectButton(const RectButton* button)
{
    setfillcolor(0xFFFFFFFF);
    ege_fillrect(button->x, button->y, button->width, button->height);
    outtextrect(button->x, button->y, button->width, button->height, "��ʼ");
}

// �� x y ������ַ��� ���200�� ����ֵ:�ַ�������
int xyStrCout(int x, int y, std::string str)
{
    char temp1[200] = "";
    sprintf(temp1, "%s", str.data());
    xyprintf(x, y, temp1);
    return str.length();
}

// ��ʼ������
void init()
{
    int WEIGHT = 1000, HEIGHT = 600;
    initgraph(WEIGHT, HEIGHT, INIT_RENDERMANUAL);
    // �Դ��ڿ��������
    ege_enable_aa(true);

    setbkcolor(EGEARGB(0xFF, 196, 203, 207));
    setcolor(0xFF323777);
    setfont(24, 0, "����");
    setbkmode(TRANSPARENT);
}

void drawCircle(struct typeNumber TN[])
{
    // ������ͼ totalNum ������ļ�������  totalLineNum ������ļ�����
    int totalNum = 0, totalLineNum = 0;
    float begin = 0, end = 0;
    // ����ÿ��������ɫ
    unsigned int color[6] = { 0x99FFB6C1, 0x99DC143C, 0x9900BFFF, 0x998B008B, 0x993CB371, 0x99FFFF00 };
    for (int i = 1; i < 6; i++)
    {
        totalNum += TN[i].typeNum;
        totalLineNum += TN[i].typeLineNum;
    }

    // ����/����ȫΪ�ջ�һ����ɫ��Բ
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
        // ����/��������ȫ�հ��մ�С˳������ͼ
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
//    TCHAR szBuffer[MAX_PATH] = { 0 };   // ���ѡ���ļ���·��   
//    BROWSEINFO bi;
//    ZeroMemory(&bi, sizeof(BROWSEINFO));
//    bi.hwndOwner = NULL;
//    bi.pszDisplayName = szBuffer;
//    bi.lpszTitle = _T("������ѡ���ļ����ļ���:");   // _T() ��ͷ�ļ� tchar.h �µ�һ���궨�塣   
//    bi.ulFlags = BIF_BROWSEINCLUDEFILES;
//    LPITEMIDLIST idl = SHBrowseForFolder(&bi);   // ��ʼѡ���ļ����ļ���  
//    if (NULL == idl)
//    {
//        return "";
//    }
//    SHGetPathFromIDList(idl, szBuffer);  // ��ȡ����·�������� szBuffer ֻ��洢��ǰѡ����ļ����ļ�������  
//
//    // �����ַ� szBuffer ת��Ϊ���ַ��ַ��������� std::string  
//    char narrowBuffer[MAX_PATH];
//    int numChars = WideCharToMultiByte(CP_ACP, 0, (LPCWCH)szBuffer, -1, narrowBuffer, MAX_PATH, NULL, NULL);
//    if (numChars > 0)
//    {
//        std::string narrowString(narrowBuffer);
//        std::cout << "��ѡ����ļ����ļ��е�·��Ϊ��" << narrowString << std::endl;
//        return narrowString;
//    }
//    else
//    {
//        return "";
//    }
//}

int draw()
{
    // ���������
    sys_edit editBox;
    // ����߶�
    int editTextHeight = 18;
    // ������false��ʾ����
    editBox.create(false);
    // ���������λ��
    editBox.move(100, 100);
    // ���óߴ磬�߶�һ��Ҫ���ֵĸ߶�һ�㣬��Ϊ�����ı߿�Ҳռһ����С��
    editBox.size(editTextHeight * 20, 32);
    // ���ñ�����ɫ
    editBox.setbgcolor(EGERGB(249, 236, 195));
    editBox.setcolor(BLACK);
    editBox.setfont(editTextHeight, 0, "����");
    // ʹ�ɼ�
    editBox.visible(true);
    // ��ȡ����
    editBox.setfocus();

    const int buffSize = 200; // �������뻺����
    char strBuffer[200] = "";
    //std::string path_str = "";
    setfont(24, 0, "����");
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
        xyprintf(100, 70, "�����ļ���·���������ʼ");
        drawRectButton(&button);

        // ��⵽�س�������ͱ�����
        while (mousemsg())
        {
            mouse_msg msg = getmouse();
            // �ж�������������������ȷ��λ�ã�̧��Ϊִ��ʱ�̣�
            if (msg.is_left())
            {
                if (msg.is_down())
                { // ������İ�ť
                    clickButton = insideRectButton(&button, msg.x, msg.y);
                }
                else
                {
                    // ���̧���ж��Ƿ���Ҫִ���¼���Ӧ
                    if (clickButton)
                    {
                        // ���� ������ �����
                        cleardevice();
                        //xyprintf(630, 40, "�����ĵȴ�");
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

            // ��ȡ����������ַ��� char*
            editBox.gettext(buffSize, strBuffer);
            // ��ȡ���ݺ������������������Ϊ���ַ���
            editBox.settext("");
            if (strBuffer[0] != '\0')
            {
                outtextxy(100, 150, "��������:");
                rectprintf(100, 180, 700, 220, "%s", strBuffer);
                std::string pre = strBuffer;

                findAllcode(pre,TN, funCountNum);
                // д�ļ��������
                for (int i = 1; i < 6; i++)
                {
                    sprintf(strBuffer, "%-5s �ļ�����:%-10d ������: %d", TN[i].typeMy.data(), TN[i].typeNum, TN[i].typeLineNum);
                    xyprintf(50, 30 * i + 200, strBuffer);
                }
                
                // д����ͳ���������
                xyprintf(20, 30 * 5 + 240, "�ļ�����:");
                xyprintf(20, 30 * 6 + 240, "�ܺ�������:");
                xyprintf(20, 30 * 7 + 240, "�ܺ�������:");
                xyprintf(20, 30 * 8 + 240, "���������:");
                xyprintf(20, 30 * 9 + 240, "��С��������:");
                xyprintf(20, 30 * 10 + 240, "����������λ��:");
                xyprintf(20, 30 * 11 + 240, "ƽ����������:");
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
                
                // ���ɲ����json�ļ��� �����޸ġ����򴴽�
                drawJson("D:\\VScode\\visual\\Project1\\OutPut\\OutJson.json", TN, funCountNum);
                drawCsv("D:\\VScode\\visual\\Project1\\OutPut\\OutCsv.csv", TN, funCountNum);
                drawXml("D:\\VScode\\visual\\Project1\\OutPut\\OutXml.xml", TN, funCountNum);
                
                
                // ��������ͼ
                drawCircle(TN);

                auto end = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

                xyprintf(630, 100, "������ʱ��");
                sprintf(strBuffer, "%.3fs", elapsed.count() * 1e-9);
                xyprintf(760 , 100, strBuffer);

            }
            isOutput = false;
        }
    }
    // ����˳�ѭ����ֱ��getch()���޷������ģ���Ϊ�����ʱ��Ҳ�ڲ���������Ϣ��Ҫ����հ�����Ϣ
    flushkey();
    // ��������Ϣ
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
