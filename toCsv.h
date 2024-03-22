#pragma once

#ifndef MY_CSV //放在XX.h被重复引用
#define MY_CSV

#include <iostream>

void drawCsv(const std::string filename, struct typeNumber TN[], struct funCount funCountNum[]);

#endif
