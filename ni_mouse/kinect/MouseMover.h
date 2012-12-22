
#ifndef MOUSEMOVER_H_
#define MOUSEMOVER_H_



// General headers
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>



class MouseMover
{

	// uinput stuff
	struct uinput_user_dev uinp; // uInput device structure 
	struct input_event event; // Input device structure 
	int uinp_fd;
	bool leftClicked;

	void clickLeft();
	void releaseLeft();
	
public:
	MouseMover();
	virtual ~MouseMover();
	
	void moveCursor(int x, int y);

	int setupUinputDevice();
	void centerCursor();
	void leftClickChecker(int y_pos);
	

};

#endif
