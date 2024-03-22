#pragma once

#ifndef MY_JSON //放在XX.h被重复引用

#define MY_JSON
#include <iostream>
#include "json/json.h"

Json::Value writeJson(struct typeNumber TN[], struct funCount funCountNum[]);
std::string jsonToString(Json::Value  temp);
void drawJson(const std::string filename, struct typeNumber TN[], struct funCount funCountNum[]);

#endif