#ifndef CHINESETEXT_H
#define CHINESETEXT_H

#include "raylib.h"

// 字体系统管理结构
typedef struct {
    Font font;
    unsigned char* fontFileData;
    unsigned int fileSize;
    int* preloadedCodepoints;    // 预加载的码点数组
    int preloadedCount;          // 已预加载的码点数
    int maxCapacity;             // 最大容量
} FontSystem;

// 初始化字体系统（包含基础字符集）
FontSystem InitFontSystem(const char* fontPath, int fontSize, const char* baseChars);

// 动态向字库中添加新字符
void AddCharsToFontSystem(FontSystem* system, const char* newChars);

// 使用当前字库绘制文本
void DrawChineseText(FontSystem* system, const char* text, float x, float y, float fontSize, Color color);

// 清理字体系统资源
void CleanupFontSystem(FontSystem* system);

#endif