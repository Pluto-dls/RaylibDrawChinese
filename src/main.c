#include "raylib.h"
#include "chinesetext.h"

int main() {
    InitWindow(1280, 720, "动态字库示例");
    SetTargetFPS(60);

    // 1. 初始化字体系统，包含基础字符集
    const char* baseChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789,./?;:!";
    FontSystem fontSystem = InitFontSystem("resources/font/fangzheng12.TTF", 32, baseChars);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);

        // 2. 绘制文本（会自动处理新字符的添加）
        DrawChineseText(&fontSystem, "欢迎使用DrawChineseText", 50, 50, 32, BLACK);
        DrawChineseText(&fontSystem, "Hello World! Try to Down Space to 测试新字符：", 50, 100, 24, BLUE);

        // 3. 可以手动添加一批新字符
        if (IsKeyDown(KEY_SPACE)) {
            DrawChineseText(&fontSystem, "新字符已添加：", 50, 150, 60, RED);
            DrawChineseText(&fontSystem, "魑魅魍魉", 150, 350, 60, BLACK);
        }

        EndDrawing();
    }

    // 4. 程序退出前清理资源
    CleanupFontSystem(&fontSystem);
    CloseWindow();

    return 0;
}