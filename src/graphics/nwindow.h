#ifndef NWINDOW_H_
#define NWINDOW_H_

#include <cstdlib>
#include <ncurses.h>

class NWindow {
    void swap(NWindow&) noexcept;

protected:
    WINDOW *win;

    void reframe(size_t wx, size_t wy, size_t x, size_t y, size_t w, size_t h);

    // Resizes the ncurses pad to the specified dimensions
    void resize(size_t w, size_t h);

    size_t _wx, _wy;
    size_t _x, _y, _w, _h;
public:
    NWindow();
    virtual ~NWindow() = 0;

    NWindow(NWindow&&);
    NWindow(const NWindow&) = delete;
    NWindow& operator=(const NWindow&) = delete;
    NWindow& operator=(NWindow&&);

    size_t wx() const { return _wx; }                 // pad x
    size_t wy() const { return _wy; }                 // pad y
    size_t x() const { return _x; }                 // frame x
    size_t y() const { return _y; }                 // frame y
    size_t w() const { return getmaxx(win) - x(); }                 // frame width
    size_t h() const { return getmaxy(win) - y(); }                 // frame height
    size_t cx() const { return getcurx(win); }      // cursor x
    size_t cy() const { return getcury(win); }      // cursor y
    size_t maxx() const { return getmaxx(stdscr); } // stdscr width
    size_t maxy() const { return getmaxy(stdscr); } // stdscr height

    void move(size_t x, size_t y) { wmove(win, y, x); }
    int get() const { return ::wgetch(win); }
    void nodelay(bool b) { ::nodelay(win, b); }

    // Redraw the ncurses window to screen
    void refresh() const;

    virtual void onDraw(bool isActive) const {}
    virtual void onInput(int key) {}
    virtual void onResize() {}
};

#endif
