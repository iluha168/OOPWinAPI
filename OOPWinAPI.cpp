#include "OOPWinAPI.h"
#include <iostream>

MSG msg = ([]{
    WNDCLASSW wc;
    ZeroMemory(&wc, sizeof(wc));
        wc.lpfnWndProc = WndMain;
        wc.lpszClassName = L"OOPWinAPI";
    RegisterClassW(&wc);
    return msg;
})();

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    if(int code = init())
        exit(code);
    do {
        if(GetMessage(&msg, NULL, 0,0)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    } while(msg.message != WM_QUIT);
    return 0;
}

namespace OOPWinAPI{
    time_t unixTime(){
        using namespace std::chrono;
        return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    }

    Window::Window(LPCWSTR title, int x,int y,int w,int h, DWORD style, DWORD exstyle, LPCWSTR className, HWND parent, HMENU menu){
        this->hWnd = CreateWindowExW(
            exstyle, className, title,
            style, x,y,w,h,
            parent, menu,
            GetModuleHandle(NULL), NULL
        );
        this->setWindLong(GWLP_USERDATA, (LONG_PTR)this);
    }

    Window::Window(HWND handle){
        hWnd = handle;
    }

    void Window::hide(){
        ShowWindow(hWnd,SW_HIDE);
    }
    void Window::show(){
        ShowWindow(hWnd,SW_SHOW);
    }
    void Window::restore(){
        ShowWindow(hWnd,SW_RESTORE);
    }
    void Window::minimize(){
        ShowWindow(hWnd,SW_MINIMIZE);
    }
    void Window::maximize(){
        ShowWindow(hWnd,SW_MAXIMIZE);
    }
    void Window::enable(bool b){
        EnableWindow(hWnd,b);
    }
    bool Window::enabled(){
        return IsWindowEnabled(hWnd);
    }
    bool Window::repaint(){
        return InvalidateRect(hWnd, NULL, false)
            && RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE|RDW_UPDATENOW|RDW_INTERNALPAINT|RDW_ALLCHILDREN)
            && UpdateWindow(hWnd);
    }
    void Window::focus(){
        SetFocus(hWnd);
    }
    void Window::redraw(){
        RedrawWindow(hWnd, NULL, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);
    }
    void Window::sysmessage(UINT msg, WPARAM wParam, LPARAM lParam){
        SendMessage(hWnd,msg,wParam,lParam);
    }
    void Window::setWindLong(int index, LONG_PTR Long){
        SetWindowLongPtr(hWnd,index,Long);
    }
    void Window::destroy(){
        DestroyWindow(hWnd);
    }

    tagRECT Window::getRect(){
        tagRECT rect;
        GetWindowRect(hWnd,&rect);
        return rect;
    }
    tagRECT Window::getClientRect(){
        tagRECT rect;
        GetClientRect(hWnd,&rect);
        return rect;
    }
    int Window::x(){ return getRect().left; }
    int Window::y(){ return getRect().top ; }
    int Window::right (){ return getRect().right ; }
    int Window::bottom(){ return getRect().bottom; }
    int Window::width (){ return getRect().right  - getRect().left; }
    int Window::height(){ return getRect().bottom - getRect().top ; }

    void Window::setTitle(LPCWSTR title){
        SetWindowTextW(hWnd,title);
    }

    void Window::move(int x, int y){
        SetWindowPos(hWnd,0, x,y, 0,0, SWP_NOSIZE|SWP_NOACTIVATE|SWP_ASYNCWINDOWPOS);
    }
    void Window::move(POINT p){
        SetWindowPos(hWnd,0, p.x,p.y, 0,0, SWP_NOSIZE|SWP_NOACTIVATE|SWP_ASYNCWINDOWPOS);
    }
    void Window::move(int x, int y, int w, int h){
        SetWindowPos(hWnd,0, x,y,w,h, SWP_NOACTIVATE|SWP_ASYNCWINDOWPOS);
    }
    void Window::move(RECT r){
        SetWindowPos(hWnd,0, r.left,r.top,r.right-r.left,r.bottom-r.top, SWP_NOACTIVATE|SWP_ASYNCWINDOWPOS);
    }

    void Window::resize(int w, int h){
        SetWindowPos(hWnd,0, 0,0, w,h, SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE|SWP_ASYNCWINDOWPOS);
    }

    void Window::setNoResize(bool b){
        long style = GetWindowLong(hWnd, GWL_STYLE);
        style = b? style&~WS_SIZEBOX : style|WS_SIZEBOX;
        SetWindowLong(hWnd, GWL_STYLE, style);
    }

    int Window::alert(LPCWSTR text, LPCWSTR caption, short icon){
        return MessageBoxW(hWnd,text,caption,icon);
    }

    unsigned char Window::screenEdge(){
        Window screen(GetDesktopWindow());
        unsigned char result = Edge::NoEdge;
        if(x()<=0) result |= Edge::Left;
        if(y()<=0) result |= Edge::Top;
        if(right()>=screen.width()) result |= Edge::Right;
        if(bottom()>=screen.height()) result |= Edge::Bottom;
        return result;
    }

    bool Window::operator==(Window o){
        return hWnd==o.hWnd;
    }

    void Window::OnClose(){}
    void Window::OnMove(short&, short&){}
    void Window::OnMoving(RECT*){}
    void Window::OnResize(WPARAM&, short&, short&){}
    void Window::OnResizing(WPARAM&, RECT*){}
    void Window::OnKeyPress(WPARAM&){}
    void Window::OnMousePress(WPARAM&, short&, short&){}
    void Window::OnMouseMove(WPARAM&, short&, short&){}
    void Window::OnMouseWheel(short&, short&, short&, short&){}
    void Window::OnRepaint(){}
    Window::~Window(){}

    POINT Cursor::getPos(){
        POINT pos;
        GetCursorPos(&pos);
        return pos;
    }

    bool Cursor::setPos(POINT p){
        return SetCursorPos(p.x, p.y);
    }
    bool Cursor::setPos(int x, int y){
        return SetCursorPos(x, y);
    }
}

LRESULT CALLBACK WndMain(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
    OOPWinAPI::Window* eventwnd = (OOPWinAPI::Window*) GetWindowLongPtr(hWnd, GWLP_USERDATA);
    if(eventwnd == 0x0 || eventwnd->hWnd == 0x0)
        return DefWindowProc(hWnd, msg, wParam, lParam);
    short* lwords = (short*)&lParam;
    short* wwords = (short*)&wParam;
    switch(msg){
    case WM_CREATE:
        UpdateWindow(hWnd);
        break;
    case WM_CLOSE:
        eventwnd->OnClose();
    case WM_DESTROY:
        break;
    case WM_KEYDOWN:
        eventwnd->OnKeyPress(wParam);
        break;
    case WM_MOVE:
        eventwnd->OnMove(lwords[0], lwords[1]);
        break;
    case WM_MOVING:
        eventwnd->OnMoving((RECT*)lParam);
        break;
    case WM_SIZE:
        eventwnd->OnResize(wParam, lwords[0], lwords[1]);
        break;
    case WM_SIZING:
        eventwnd->OnResizing(wParam, (RECT*)lParam);
        break;
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_XBUTTONDOWN:
        eventwnd->OnMousePress(wParam, lwords[0], lwords[1]);
        break;
    case WM_MOUSEMOVE:
        eventwnd->OnMouseMove(wParam, lwords[0], lwords[1]);
        break;
    case WM_PAINT:
        eventwnd->OnRepaint();
        break;
    case WM_MOUSEWHEEL:
        eventwnd->OnMouseWheel(wwords[0], wwords[1], lwords[0], lwords[1]);
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}
