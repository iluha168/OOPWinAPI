#ifndef OOPWinAPI_h
#define OOPWinAPI_h

#ifndef _UNICODE
    #define _UNICODE
#endif
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <Commctrl.h>
#include <thread>
#include <map>
#include <chrono>

#define UNUSED(var) (void)var

#define ifnot(boolean) if(!(boolean))
#define elif else if
#define with(variable) if(typeof variable _ = variable, true)

#define repeat(count,var) for(int var=0; var<count; var++)
#define repeatEx(count,var,pause) for(int var=0; var<count; Sleep(pause),var++)

extern int init(); //MAIN FUNCTION

LRESULT CALLBACK WndMain(HWND, UINT, WPARAM, LPARAM);

enum Edge{
    Left   = 0b0001,
    Right  = 0b0010,
    Top    = 0b0100,
    Bottom = 0b1000,
    NoEdge = 0b0000
};

namespace OOPWinAPI{
    time_t unixTime(); //current time

    LRESULT CALLBACK WndMain(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    class Window{
    public:
        HWND hWnd;

        Window(LPCWSTR = L"", int = CW_USEDEFAULT, int = CW_USEDEFAULT, int = CW_USEDEFAULT, int = CW_USEDEFAULT,
            DWORD = WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN, DWORD = WS_EX_LEFT,
            LPCWSTR = L"OOPWinAPI",
            HWND = NULL, HMENU = NULL
        );
        Window(HWND);

        virtual void OnClose();
        virtual void OnMove(short&, short&);
        virtual void OnMoving(RECT*);
        virtual void OnResize(WPARAM&, short&, short&);
        virtual void OnResizing(WPARAM&, RECT*);
        virtual void OnKeyPress(WPARAM&);
        virtual void OnMousePress(WPARAM&, short&, short&);
        virtual void OnMouseMove(WPARAM&, short&, short&);
        virtual void OnMouseWheel(short&, short&, short&, short&);
        virtual void OnRepaint();

        void hide();
        void show();
        void restore();
        void minimize();
        void maximize();
        void enable(bool);
        bool enabled();
        bool repaint();
        void focus();
        void redraw();
        void sysmessage(UINT,WPARAM = 0,LPARAM = 0);
        void setWindLong(int, LONG_PTR);
        void destroy();

        tagRECT getRect();
        tagRECT getClientRect();
        int x();
        int y();
        int right();
        int bottom();
        int width();
        int height();

        void setTitle(LPCWSTR);
        void move(int, int);
        void move(POINT);
        void move(int, int, int, int);
        void move(RECT);
        void resize(int, int);
        void setNoResize(bool);

        int alert(LPCWSTR, LPCWSTR, short);

        unsigned char screenEdge();
        bool operator==(Window);
        virtual ~Window();
    };

    class Cursor{
    public:
        static POINT getPos();
        static bool setPos(POINT);
        static bool setPos(int, int);
    };
}
#include "OOPWinAPI.cpp"
#endif //OOPWinAPI_h
