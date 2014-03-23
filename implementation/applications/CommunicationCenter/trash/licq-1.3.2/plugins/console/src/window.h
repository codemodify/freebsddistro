#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
extern "C" {
#include <cdk.h>
}

#define COLOR_YELLOW_BLUE COLOR_YELLOW + 8
#define COLOR_WHITE_BLUE  COLOR_WHITE + 8
#define COLOR_CYAN_BLUE   COLOR_CYAN + 8

#define SCROLLBACK_OVERLAP 10

enum InputState { STATE_COMMAND, STATE_PENDING, STATE_MLE, STATE_LE, STATE_QUERY };

class CLicqConsole;
class CData;

struct SContact
{
  char *szId;
  unsigned long nPPID;
};


class CWindow
{
public:
  CWindow(int _rows, int _cols, int _y, int _x, int _scrollback, int _useCDK = false);
  ~CWindow();
  void RefreshWin();
  void ScrollUp();
  void ScrollDown();
  void SetActive(bool _active) { active = _active; RefreshWin(); }
  bool Active()  { return active; }
  CWindow& operator<<(char d);
  CWindow& operator<<(unsigned char d);
  CWindow& operator<<(const char *d);
  CWindow& operator<<(unsigned long d);
  CWindow& operator<<(unsigned short d);
  void wprintf(char *formatIn, ...);
  WINDOW *Win()  { return win; }
  CDKSCREEN *CDKScreen() { return cdkscreen; } 
  static void StartScreen();
  static void EndScreen();
  int Rows() { return rows; }
  int Cols() { return cols; }

  void (CLicqConsole::*fProcessInput)(int);
  unsigned long event;
  InputState state;
  CData *data;
  unsigned long nLastUin;
  struct SContact sLastContact;
  unsigned short nLastHistory;
protected:
  WINDOW *win;
  CDKSCREEN *cdkscreen;
  bool pad, active;
  int rows, cols, x, y, height, cur_y;
};

#endif
