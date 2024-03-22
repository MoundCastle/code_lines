#include <fstream>
#include "json/json.h"
#include "FileCount.h"
#include "toJosn.h"

Json::Value writeJson( struct typeNumber TN[], struct funCount funCountNum[])
{
	Json::Value Json_TN;
	Json::Value Json_funCountNum;

	for (int i = 1; i < TN_LONG; i++) {
		Json_TN[TN[i].typeMy + "文件类型数量"] = TN[i].typeNum;
		Json_TN[TN[i].typeMy + "文件总行数"] = TN[i].typeLineNum;
	}
	for (int i = 0; i < FunCOUNTNUM_LONG; i++)
	{
		Json_funCountNum[funCountNum[i].typeMy + "文件 总函数个数"] = funCountNum[i].allCount;
		Json_funCountNum[funCountNum[i].typeMy + "文件 函数总行数"] = funCountNum[i].allLinesNum;
		Json_funCountNum[funCountNum[i].typeMy + "文件 中最大函数行数"] = funCountNum[i].maxLinesNum;
        if (funCountNum[i].allCount == 0)
        {
		    Json_funCountNum[funCountNum[i].typeMy + "文件 中最小函数行数"] = 0;
            Json_funCountNum[funCountNum[i].typeMy + "文件 中函数行数中位数"] = 0;
            Json_funCountNum[funCountNum[i].typeMy + "文件 中平均函数行数"] = 0;
        }
        else
        {
            Json_funCountNum[funCountNum[i].typeMy + "文件 中最小函数行数"] = funCountNum[i].minLinesNum;
            Json_funCountNum[funCountNum[i].typeMy + "文件 中函数行数中位数"] = funCountNum[i].mediumLinesNum;
            Json_funCountNum[funCountNum[i].typeMy + "文件 中平均函数行数"] = funCountNum[i].allLinesNum / (float)funCountNum[i].allCount;
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
    Json::Value root;// 最后的JSON
    char temp1[2000] = "";
    std::string temp;

    root = writeJson(TN, funCountNum);
    temp = jsonToString(root);

    Json::StreamWriterBuilder writebuild;
    writebuild["emitUTF8"] = true;  //utf8支持,加这句,utf8的中文字符会编程
    std::string document = Json::writeString(writebuild, root); //把json对象转变为字符串

    //写入文件
    std::ofstream ofile(filename);
    ofile << document;
    ofile.close();
}