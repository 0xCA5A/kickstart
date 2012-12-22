
#include "MouseMover.h"





MouseMover::MouseMover()
{
    printf("MouseMover constructor!\n");
    leftClicked = false;
}

MouseMover::~MouseMover()
{
    printf("MouseMover destructor!\n");  
}


void MouseMover::leftClickChecker(int y_pos)
{
 
      static bool lastClickState = false;

      
//       printf("y_pos: %0d\n", y_pos);
      
  
      if (y_pos > (430/4*3))
      {
	leftClicked = true;
      }
      else
      {
	leftClicked = false;
      }
  
  
//      any modifications??
      if (lastClickState != leftClicked)
      {
	if (leftClicked == true)
	{
	  clickLeft();
	}
	else
	{

	  releaseLeft();
	}

      }

      lastClickState = leftClicked;

}



int MouseMover::setupUinputDevice() 
{ 
            int retcode, i;

            uinp_fd = open("/dev/input/uinput", O_WRONLY | O_NDELAY );
            printf("[%s] open /dev/input/uinput returned %d.\n", __func__, uinp_fd);
            if (uinp_fd == 0) {
                        printf("[%s] Could not open uinput\n", __func__);
                        return -1;
            }

            memset(&uinp, 0, sizeof(uinp));
            strncpy(uinp.name, "kinect mouse", 20);
            uinp.id.version = 4;
            uinp.id.bustype = BUS_USB;

            ioctl(uinp_fd, UI_SET_EVBIT, EV_KEY);
            ioctl(uinp_fd, UI_SET_EVBIT, EV_REL);
            ioctl(uinp_fd, UI_SET_RELBIT, REL_X);
            ioctl(uinp_fd, UI_SET_RELBIT, REL_Y);
	    ioctl(uinp_fd, UI_SET_KEYBIT, BTN_LEFT); 

            for (i=0; i<256; i++) {
                        ioctl(uinp_fd, UI_SET_KEYBIT, i);
            }

            ioctl(uinp_fd, UI_SET_KEYBIT, BTN_MOUSE);

            // create input device in input subsystem
            retcode = write(uinp_fd, &uinp, sizeof(uinp));
            printf("[%s] First write returned %d.\n", __func__, retcode);

            retcode = (ioctl(uinp_fd, UI_DEV_CREATE));
            printf("[%s] ioctl UI_DEV_CREATE returned %d.\n", __func__, retcode);
            if (retcode) {
                        printf("[%s] Error create uinput device %d.\n", __func__, retcode);
                        return -1;
            }
    
    return 1; 
} 


//pseude center...
//read from input device would be better...
void MouseMover::centerCursor()
{

//     memset(&event, 0, sizeof(event)); 
//     gettimeofday(&event.time, NULL); 
//     
//     event.type = EV_REL; 
//     event.code = REL_X; 
//     event.value = 0;
//     write(uinp_fd, &event, sizeof(event)); 
//     
//     event.type = EV_REL; 
//     event.code = REL_Y; 
//     event.value = 0; 
//     write(uinp_fd, &event, sizeof(event)); 
//     
//     event.type = EV_SYN; 
//     event.code = SYN_REPORT; 
//     event.value = 0; 
//     write(uinp_fd, &event, sizeof(event)); 
  
      //hacky way to center cursor... bad implementation!
      moveCursor(0, 0);
      moveCursor(0, 0);
  
}



void MouseMover::moveCursor(int kinect_x, int kinect_y) 
{

    static int old_x_pos = 0;
    static int old_y_pos = 0;
  
    int x = (kinect_x - old_x_pos) * ((1280*2) / (620 / 2));
    int y = (kinect_y - old_y_pos) * (1024 / (440 / 2));

    
//     printf("[%s] move cursor to (%0d / %0d)! \n", __func__, x , y);
//     printf("x diff: %0d\n", (kinect_x - old_x_pos));
//     printf("x diff: %0d\n", (kinect_x - old_x_pos));    
     
    old_x_pos = kinect_x;
    old_y_pos = kinect_y;
 
    
    
    memset(&event, 0, sizeof(event)); 
    gettimeofday(&event.time, NULL); 
    
    event.type = EV_REL; 
    event.code = REL_X; 
    event.value = x;
    write(uinp_fd, &event, sizeof(event)); 
    
    event.type = EV_REL; 
    event.code = REL_Y; 
    event.value = y; 
    write(uinp_fd, &event, sizeof(event)); 
    
    event.type = EV_SYN; 
    event.code = SYN_REPORT; 
    event.value = 0; 
    write(uinp_fd, &event, sizeof(event)); 
}



void MouseMover::clickLeft()
{
	printf("[%s] clicked!\n", __func__); 
	
	memset(&event, 0, sizeof(event)); 
	gettimeofday(&event.time, NULL); 
    

    
	// Report BUTTON CLICK - PRESS event
	memset(&event, 0, sizeof(event));
	gettimeofday(&event.time, NULL);
	event.type = EV_KEY;
	event.code = BTN_LEFT;
	event.value = 1;
	write(uinp_fd, &event, sizeof(event));
	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	write(uinp_fd, &event, sizeof(event));

    
    

}

void MouseMover::releaseLeft()
{
	printf("[%s] released!\n", __func__);
  
	// Report BUTTON CLICK - RELEASE event
	memset(&event, 0, sizeof(event));
	gettimeofday(&event.time, NULL);
	event.type = EV_KEY;
	event.code = BTN_LEFT;
	event.value = 0;
	write(uinp_fd, &event, sizeof(event));
	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	write(uinp_fd, &event, sizeof(event));

}


