#pragma once

#ifndef MY_JSON //����XX.h���ظ�����

#define MY_JSON
#include <iostream>
#include "json/json.h"

Json::Value writeJson(struct typeNumber TN[], struct funCount funCountNum[]);
std::string jsonToString(Json::Value  temp);
void drawJson(const std::string filename, struct typeNumber TN[], struct funCount funCountNum[]);

#endif