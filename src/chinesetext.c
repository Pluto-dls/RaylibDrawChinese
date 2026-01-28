#include "chinesetext.h"
#include <string.h>
#include <stdlib.h>

// 工具函数：合并两个码点数组，去除重复项
static int* MergeCodepoints(const int* codepoints1, int count1, const int* codepoints2, int count2, int* mergedCount) {
    int maxSize = count1 + count2;
    int* merged = malloc(maxSize * sizeof(int));
    int currentCount = 0;

    // 添加第一个数组的所有码点
    for (int i = 0; i < count1; i++) {
        merged[currentCount++] = codepoints1[i];
    }

    // 添加第二个数组中不重复的码点
    for (int i = 0; i < count2; i++) {
        int exists = 0;
        for (int j = 0; j < count1; j++) {
            if (codepoints2[i] == codepoints1[j]) {
                exists = 1;
                break;
            }
        }
        if (!exists) {
            merged[currentCount++] = codepoints2[i];
        }
    }

    *mergedCount = currentCount;
    return merged;
}

FontSystem InitFontSystem(const char* fontPath, int fontSize, const char* baseChars) {
    FontSystem system = {0};

    // 加载字体文件
    system.fontFileData = LoadFileData(fontPath, &system.fileSize);
    if (system.fontFileData == NULL) {
        TraceLog(LOG_ERROR, "无法加载字体文件: %s", fontPath);
        return system;
    }

    // 转换基础字符为码点
    system.preloadedCodepoints = LoadCodepoints(baseChars, &system.preloadedCount);
    system.maxCapacity = system.preloadedCount + 256; // 预留扩展空间

    // 初始加载字体
    system.font = LoadFontFromMemory(".ttf", system.fontFileData, system.fileSize,
                                   fontSize, system.preloadedCodepoints, system.preloadedCount);

    return system;
}

void AddCharsToFontSystem(FontSystem* system, const char* newChars) {
    if (!system || !newChars || strlen(newChars) == 0) return;

    // 获取新字符的码点
    int newCodepointsCount;
    int* newCodepoints = LoadCodepoints(newChars, &newCodepointsCount);

    // 合并码点数组
    int mergedCount;
    int* mergedCodepoints = MergeCodepoints(system->preloadedCodepoints, system->preloadedCount,
                                          newCodepoints, newCodepointsCount, &mergedCount);

    // 如果确实有新字符需要添加
    if (mergedCount > system->preloadedCount) {
        // 卸载旧字体和码点
        UnloadFont(system->font);
        UnloadCodepoints(system->preloadedCodepoints);

        // 更新系统状态
        system->preloadedCodepoints = mergedCodepoints;
        system->preloadedCount = mergedCount;

        // 重新加载包含新字符的字体
        system->font = LoadFontFromMemory(".ttf", system->fontFileData, system->fileSize,
                                         system->font.baseSize, system->preloadedCodepoints,
                                         system->preloadedCount);

        TraceLog(LOG_INFO, "字库已更新，当前字符数: %d", system->preloadedCount);
    } else {
        free(mergedCodepoints); // 没有新字符，释放临时数组
    }

    UnloadCodepoints(newCodepoints);
}

void DrawChineseText(FontSystem* system, const char* text, float x, float y, float fontSize, Color color) {
    if (!system || !text) return;

    // 检查是否需要动态添加新字符
    int textCodepointsCount;
    int* textCodepoints = LoadCodepoints(text, &textCodepointsCount);

    int needsUpdate = 0;
    for (int i = 0; i < textCodepointsCount; i++) {
        int exists = 0;
        for (int j = 0; j < system->preloadedCount; j++) {
            if (textCodepoints[i] == system->preloadedCodepoints[j]) {
                exists = 1;
                break;
            }
        }
        if (!exists) {
            needsUpdate = 1;
            break;
        }
    }

    // 如果发现新字符，动态添加到字库
    if (needsUpdate) {
        AddCharsToFontSystem(system, text);
    }

    UnloadCodepoints(textCodepoints);

    // 绘制文本
    DrawTextEx(system->font, text, (Vector2){x, y}, fontSize, 2, color);
}

void CleanupFontSystem(FontSystem* system) {
    if (!system) return;

    if (system->font.texture.id != 0) {
        UnloadFont(system->font);
    }
    if (system->preloadedCodepoints) {
        UnloadCodepoints(system->preloadedCodepoints);
    }
    if (system->fontFileData) {
        UnloadFileData(system->fontFileData);
    }
}