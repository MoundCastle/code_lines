#include <fstream>
#include "json/json.h"
#include "FileCount.h"
#include "toJosn.h"

Json::Value writeJson( struct typeNumber TN[], struct funCount funCountNum[])
{
	Json::Value Json_TN;
	Json::Value Json_funCountNum;

	for (int i = 1; i < TN_LONG; i++) {
		Json_TN[TN[i].typeMy + "�ļ���������"] = TN[i].typeNum;
		Json_TN[TN[i].typeMy + "�ļ�������"] = TN[i].typeLineNum;
	}
	for (int i = 0; i < FunCOUNTNUM_LONG; i++)
	{
		Json_funCountNum[funCountNum[i].typeMy + "�ļ� �ܺ�������"] = funCountNum[i].allCount;
		Json_funCountNum[funCountNum[i].typeMy + "�ļ� ����������"] = funCountNum[i].allLinesNum;
		Json_funCountNum[funCountNum[i].typeMy + "�ļ� �����������"] = funCountNum[i].maxLinesNum;
        if (funCountNum[i].allCount == 0)
        {
		    Json_funCountNum[funCountNum[i].typeMy + "�ļ� ����С��������"] = 0;
            Json_funCountNum[funCountNum[i].typeMy + "�ļ� �к���������λ��"] = 0;
            Json_funCountNum[funCountNum[i].typeMy + "�ļ� ��ƽ����������"] = 0;
        }
        else
        {
            Json_funCountNum[funCountNum[i].typeMy + "�ļ� ����С��������"] = funCountNum[i].minLinesNum;
            Json_funCountNum[funCountNum[i].typeMy + "�ļ� �к���������λ��"] = funCountNum[i].mediumLinesNum;
            Json_funCountNum[funCountNum[i].typeMy + "�ļ� ��ƽ����������"] = funCountNum[i].allLinesNum / (float)funCountNum[i].allCount;
        }
	}
	Json_TN["Function:"] = Json_funCountNum;
	return Json_TN;
}

std::string jsonToString(Json::Value  temp)
{
	return temp.toStyledString();
}

void drawJson(const std::string filename, struct typeNumber TN[], struct funCount funCountNum[]) {
    Json::Value root;// ����JSON
    char temp1[2000] = "";
    std::string temp;

    root = writeJson(TN, funCountNum);
    temp = jsonToString(root);

    Json::StreamWriterBuilder writebuild;
    writebuild["emitUTF8"] = true;  //utf8֧��,�����,utf8�������ַ�����
    std::string document = Json::writeString(writebuild, root); //��json����ת��Ϊ�ַ���

    //д���ļ�
    std::ofstream ofile(filename);
    ofile << document;
    ofile.close();
}