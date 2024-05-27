
//#include <graphics.h>
#include "../HiEasyX.h"
#include <functional>
#include <string>
#include <vector>
#include <iostream>

using namespace std;



class Button
{
private:
    int x;
    int y;
    int width;
    int height;
    float scale;
    bool isMouseOver;
    string text;
    function<void()> onClick;

public:
    Button(int x, int y, int width, int height, const string& text, const function<void()>& onClick)
            : x(x), y(y), width(width), height(height), text(text), onClick(onClick), scale(1.0f), isMouseOver(false)
    {
    }

    void checkMouseOver(int mouseX, int mouseY)
    {
        isMouseOver = (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height);

        if (isMouseOver) {
            scale = 0.9f;
        }
        else {
            scale = 1.0f;
        }
    }

    bool checkClick(int mouseX, int mouseY)
    {
        if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height)
        {
            onClick();
            isMouseOver = false;
            scale = 1.0f;
            return true;
        }
        return false;
    }

    void draw()
    {
        int scaledWidth = width * scale;
        int scaledHeight = height * scale;
        int scaledX = x + (width - scaledWidth) / 2;
        int scaledY = y + (height - scaledHeight) / 2;

        if (isMouseOver)
        {
            setlinecolor(RGB(0, 120, 215));
            setfillcolor(RGB(229, 241, 251));

        }
        else
        {
            setlinecolor(RGB(173, 173, 173));
            setfillcolor(RGB(225, 225, 225));
        }

        fillrectangle(scaledX, scaledY, scaledX + scaledWidth, scaledY + scaledHeight);
        settextcolor(BLACK);
        setbkmode(TRANSPARENT);
        settextstyle(20 * scale, 0, _T("微软雅黑"));
        int textX = scaledX + (scaledWidth - textwidth(text.c_str())) / 2;
        int textY = scaledY + (scaledHeight - textheight(_T("微软雅黑"))) / 2;
        outtextxy(textX, textY, text.c_str());
    }
};

class TextBox
{
private:
    int x;
    int y;
    int width;
    int height;
    int maxWord;    //最长单词个数
    string text;   //文本
    bool isSelected;    //是否被选中
    bool showCursor;    //是否显示光标
    int cursorPos;      //光标位置

public:
    TextBox(int x, int y, int width, int height, int maxWord)
            : x(x), y(y), width(width), height(height), maxWord(maxWord), isSelected(false), showCursor(false), cursorPos(0)
    {
    }

    const string& getText() const
    {
        return text;
    }

    bool getSelect() const
    {
        return isSelected;
    }

    bool isEmpty() const
    {
        return text.empty();
    }

    void clear()
    {
        text = "";
    }

    void setText(const string& newText)
    {
        text = newText;
        cursorPos = text.length();
    }

    void draw()
    {
        setfillcolor(WHITE);
        if (isSelected)
        {
            setlinecolor(RGB(0, 120, 215));
            fillrectangle(x, y, x + width, y + height);
        }
        else
        {
            setlinecolor(RGB(122, 122, 122));
            fillrectangle(x, y, x + width, y + height);
        }

        settextcolor(BLACK);
        setbkmode(TRANSPARENT);
        settextstyle(height * 3 / 4, 0, _T("微软雅黑"));

        outtextxy(x + 5, y + (height - textheight(_T("微软雅黑"))) / 2, text.c_str());

        setlinecolor(BLACK);
        if (isSelected && showCursor)
        {
            int cursorX = x + 5 + textwidth(text.substr(0, cursorPos).c_str());
            line(cursorX, y + 2 + height / 8, cursorX, y + height * 7 / 8 - 2);
        }
    }

    bool checkClick(int mouseX, int mouseY)
    {
        if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height)
        {
            isSelected = true;
            return true;
        }
        else
        {
            isSelected = false;
            cursorPos = text.length();
            return false;
        }
    }

    void keyInput(wchar_t ch)
    {
        if (isSelected)
        {
            switch (ch)
            {
                case '\b':				// 用户按退格键，删掉一个字符
                    if (!text.empty() && cursorPos > 0)
                    {
                        text.erase(cursorPos - 1, 1);
                        cursorPos--;
                    }
                    break;
                case '\r':
                case '\n':
                    cursorPos = text.length();
                    isSelected = false;
                    break;
                default:				// 用户按其它键，接受文本输入
                    if (text.length() < maxWord)
                    {
                        text.insert(cursorPos, 1, ch);
                        cursorPos++;
                    }
            }
        }
    }

    void updateCursor()
    {
        static DWORD lastTick = GetTickCount();
        DWORD currentTick = GetTickCount();
        if (currentTick - lastTick >= 500) // 每500毫秒切换光标状态
        {
            showCursor = !showCursor;
            lastTick = currentTick;
        }
    }
};

class Widget
{
private:
    int width;
    int height;
    int currentIndex;
    vector<IMAGE*> pages;
    vector<vector<Button*>> buttons;
    vector<vector<TextBox*>>textBoxs;

    void addPage(IMAGE* page)
    {
        pages.push_back(page);
        buttons.push_back({});
        textBoxs.push_back({});
    }

    void addButton(int index, Button* button)
    {
        if (index >= 0 && index < buttons.size())
        {
            buttons[index].push_back(button);
        }
    }

    void addTextBox(int index, TextBox* textBox)
    {
        if (index >= 0 && index < textBoxs.size())
        {
            textBoxs[index].push_back(textBox);
        }
    }


    void setCurrentIndex(int index)
    {
        if (index >= 0 && index < pages.size())
        {
            currentIndex = index;
        }
    }

    void mouseClick(int mouseX, int mouseY)
    {
        if (currentIndex >= 0 && currentIndex < buttons.size())
        {
            for (Button* button : buttons[currentIndex])
            {
                if (button->checkClick(mouseX, mouseY))
                {
                    break;
                }
            }
        }
        if (currentIndex >= 0 && currentIndex < textBoxs.size())
        {
            for (auto textbox: textBoxs[currentIndex])
            {
                textbox->checkClick(mouseX, mouseY);
            }
        }
    }

    void mouseMove(int mouseX, int mouseY)
    {

        if (currentIndex >= 0 && currentIndex < buttons.size())
        {
            for (Button* button : buttons[currentIndex])
            {
                button->checkMouseOver(mouseX, mouseY);
            }
        }
    }

    void keyInput(wchar_t ch)
    {
        if (currentIndex >= 0 && currentIndex < textBoxs.size())
        {
            for (auto textbox:textBoxs[currentIndex])
            {
                if (textbox->getSelect())
                {
                    textbox->keyInput(ch);
                }
            }
        }
    }

    void draw()
    {
        if (currentIndex >= 0 && currentIndex < pages.size())
        {
            putimage(0, 0, pages[currentIndex]);

            if (currentIndex >= 0 && currentIndex < buttons.size())
            {
                for (Button* button : buttons[currentIndex])
                {
                    button->draw();
                }
            }

            if (currentIndex >= 0 && currentIndex < textBoxs.size())
            {
                for (auto textbox: textBoxs[currentIndex])
                {
                    if (textbox->getSelect())
                    {
                        textbox->updateCursor();
                    }
                    textbox->draw();
                }
            }
        }
    }

public:
    Widget(int width, int height)
            :width(width), height(height), currentIndex(-1)
    {
    }
    ~Widget() {}

    void init()
    {
        hiex::Window wnd(width, height);

        IMAGE* page1 = new IMAGE;
        loadimage(page1, "../source/ui/OIP-C.png",width,height);

        addPage(page1);

        // 在界面1创建按钮1
        Button* button1_1 = new Button(300, 400, 150, 30, "single player", [&]() {
                                           setCurrentIndex(4);
                                       }
        );
        addButton(0, button1_1);

        Button* button1_2 = new Button(300, 450, 150, 30, "multi player", [&]() {
                                           setCurrentIndex(1);
                                       }
        );
        addButton(0, button1_2);

        IMAGE*page2 = new IMAGE;
        loadimage(page2, "../source/ui/OIP-C.png",width,height);

        addPage(page2);

        Button *button2_1 = new Button(300, 400, 150, 30, "creat game", [&]() {
                                           setCurrentIndex(3);
                                       }
        );
        addButton(1, button2_1);

        Button *button2_2 = new Button(300, 450, 150, 30, "join game", [&]() {
                                           setCurrentIndex(2);
                                       }
        );
        addButton(1, button2_2);

        Button *button2_3 = new Button(300, 500, 150, 30, "back", [&]() {
            setCurrentIndex(0);
        });
        addButton(1, button2_3);

        IMAGE* page3 = new IMAGE;
        loadimage(page3, "../source/ui/OIP-JOIN.png",width,height);
        addPage(page3);

        TextBox* textbox3_1 = new TextBox(220, 300, 300, 30, 10);
        addTextBox(2, textbox3_1);

        TextBox* textbox3_2 = new TextBox(220, 350, 300, 30, 10);
        addTextBox(2, textbox3_2);

        TextBox* textbox3_3 = new TextBox(220, 400, 300, 30, 10);
        addTextBox(2, textbox3_3);

        Button* button3_1 = new Button(200, 500, 100, 30, "back", [&]() {
            setCurrentIndex(1);
        });
        addButton(2, button3_1);

        Button* button3_2 = new Button(400, 500, 100, 30, "confirm", [&]() {

        });
        addButton(2, button3_2);

        // 创建页面2
        IMAGE* page4 = new IMAGE;
        loadimage(page4, "../source/ui/OIP-CREATE.png",width,height);

        addPage(page4);

        TextBox* textbox1 = new TextBox(220, 300, 300, 30, 10);
        addTextBox(3, textbox1);


        TextBox* textbox2 = new TextBox(220, 350, 300, 30, 10);
        addTextBox(3, textbox2);


        // 在界面2创建按钮1
        Button* button4_1 = new Button(200, 400, 100, 30, "back", [&]() {
            setCurrentIndex(1);
        });
        addButton(3, button4_1);

        Button* button4_2 = new Button(400, 400, 100, 30, "confirm", [&]() {

                                       }
        );
        addButton(3, button4_2);

        //创建页面3
        auto* page5 = new IMAGE;
        loadimage(page5, "../source/ui/OIP.png",width,height);
        addPage(page5);

        Button* button5_1 = new Button(225, 276, 100, 40, "tiger", [&]() {

        });
        addButton(4, button5_1);
        Button* button5_2 = new Button(225, 380, 100, 40, "t34_85", [&]() {

                                       }
        );
        addButton(4, button5_2);
        Button* button5_3 = new Button(225, 490, 100, 40, "sherman", [&]() {

                                       }
        );
        addButton(4, button5_3);
        Button* button5_4 = new Button(563, 276, 100, 40, "is2", [&]() {

                                       }
        );
        addButton(4, button5_4);

        Button* button5_5 = new Button(563, 380, 100, 40, "churchill", [&]() {

                                       }
        );
        addButton(4, button5_5);

        setCurrentIndex(0);
    }

    void run()
    {
        ExMessage msg;

        BeginBatchDraw();
        while (true)
        {
            if (peekmessage(&msg))
            {
                int mouseX = msg.x;
                int mouseY = msg.y;
                switch (msg.message)
                {
                    case WM_LBUTTONDOWN:
                        mouseClick(mouseX, mouseY);
                        break;
                    case WM_MOUSEMOVE:
                        mouseMove(mouseX, mouseY);
                        break;
                    case WM_CHAR:
                        keyInput(msg.ch);
                        break;
                }
            }
            draw();
            FlushBatchDraw();
            Sleep(10);
        }
        EndBatchDraw();
    }

    void close()
    {
        closegraph();
    }
};

//int main()
//{
//    Widget widget(800, 600);
//    widget.init();
//    widget.run();
//    widget.close();
//    return 0;
//}