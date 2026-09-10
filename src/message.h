#ifndef MESSAGE_HEAD
#define MESSAGE_HEAD

extern const Rectangle MESSAGE_WINDOW_R; // const.h
extern const int message_speeds[3];
extern int message_speed;

void message_box_wait();
void message_box(char* text);

#endif
