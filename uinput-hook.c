#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <time.h>
#include <stdint.h>

void emit(int fd, int type, int code, int val) {
	struct input_event ie;

	ie.type = type;
	ie.code = code;
	ie.value = val;
	/* timestamp values below are ignored */
	ie.time.tv_sec = 0;
	ie.time.tv_usec = 0;

	write(fd, &ie, sizeof(ie));

  //  printf("keyboard event: type %d code %d value %d  \n", ie.type, ie.code, ie.value);
}

void emit_key(int fd, struct input_event ie, int code) {
  emit(fd, ie.type, code, ie.value);
}

int main(int argc, char* argv[]){ 
  sleep(1);
  int rcode = 0;

  char keyboard_name[256] = "Unknown";
  int keyboard_fd = 0;
  if(argc != 2 )
    keyboard_fd = open("/dev/input/event1", O_RDONLY | O_NONBLOCK);
  keyboard_fd = open(argv[1], O_RDONLY | O_NONBLOCK);

  ioctl(keyboard_fd, EVIOCGRAB, 0);
  ioctl(keyboard_fd, EVIOCGRAB, 1);

  if ( keyboard_fd == -1 ) {
    printf("Failed to open keyboard.\n");
    exit(1);
  }
  rcode = ioctl(keyboard_fd, EVIOCGNAME(sizeof(keyboard_name)), keyboard_name);
  printf("Reading From : %s \n", keyboard_name);

  printf("Getting exclusive access: ");
  rcode = ioctl(keyboard_fd, EVIOCGRAB, 1);
  printf("%s\n", (rcode == 0) ? "SUCCESS" : "FAILURE");
  struct input_event keyboard_event;

  char mouse_name[256] = "Unknown";
  int mouse_fd = open("/dev/input/event2", O_RDONLY | O_NONBLOCK);
  if ( mouse_fd == -1 ) {
    printf("Failed to open mouse.\n");
    exit(1);
  }
  rcode = ioctl(mouse_fd, EVIOCGNAME(sizeof(mouse_name)), mouse_name);
  printf("Reading From : %s \n", mouse_name);

  printf("Getting exclusive access: ");
  rcode = ioctl(mouse_fd, EVIOCGRAB, 1);
  printf("%s\n", (rcode == 0) ? "SUCCESS" : "FAILURE");
  struct input_event mouse_event;
#define FAKE_INPUT
#ifdef FAKE_INPUT // Setting up a fake keyboard
  /* Set up a fake keyboard device */
  int virt_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK); // or /dev/input/uinput
  ioctl(virt_fd, UI_SET_EVBIT, EV_KEY);

#if 0
  ioctl(virt_fd, UI_SET_KEYBIT, KEY_COMMA);
  ioctl(virt_fd, UI_SET_KEYBIT, KEY_W);
  ioctl(virt_fd, UI_SET_KEYBIT, KEY_DOT);
  ioctl(virt_fd, UI_SET_KEYBIT, KEY_E);
#endif

  for (int key = KEY_ESC; key <= KEY_SPACE; ++key) {
    ioctl(virt_fd, UI_SET_KEYBIT, key);
  }
  //ioctl(virt_fd, UI_SET_KEYBIT, KEY_W);
  //ioctl(virt_fd, UI_SET_KEYBIT, KEY_W);
  //ioctl(virt_fd, UI_SET_KEYBIT, KEY_W);

        struct uinput_setup usetup;
    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = 0x1234; /* sample vendor */
    usetup.id.product = 0x5678; /* sample product */
    strcpy(usetup.name, "Example device");


    struct uinput_user_dev uidev;
    write(virt_fd, &uidev, sizeof(uidev));
    ioctl(virt_fd, UI_DEV_CREATE);
#endif
    printf("Started reading input");
    int end = time(NULL) + 60;
    while ( time(NULL) < end ) {
      if ( read(keyboard_fd, &keyboard_event, sizeof(keyboard_event)) != -1 ) {
        //printf("keyboard event: type %d code %d value %d  \n", keyboard_event.type, keyboard_event.code, keyboard_event.value);
#if 0
        struct input_event ev;
        memset(&ev, 0, sizeof(ev));
        ev.type = EV_KEY;
        ev.code = KEY_D;
        ev.value = 1;
        //write(keyboard_fd, &ev, sizeof(ev));
#endif

#ifdef FAKE_INPUT
        /* Send an event */
        //			write(virt_fd, &ev, sizeof(ev));
        //emit(virt_fd, EV_KEY, KEY_SPACE, 1);
        //emit(virt_fd, EV_SYN, SYN_REPORT, 0);
        //emit(virt_fd, EV_KEY, KEY_SPACE, 0);
        //emit(virt_fd, EV_SYN, SYN_REPORT, 0);
        struct input_event ev = keyboard_event;
#if 0
        if(ev.code == KEY_W) emit(virt_fd, ev.type, KEY_COMMA, ev.value); 
        else
          emit(virt_fd, ev.type, ev.code, ev.value);
#elif 1
        switch(ev.code) {
          case KEY_Q : emit_key(virt_fd, ev, KEY_X); break;
          case KEY_W : emit_key(virt_fd, ev, KEY_COMMA); break;
          case KEY_E : emit_key(virt_fd, ev, KEY_D); break;
          case KEY_R : emit_key(virt_fd, ev, KEY_O); break;
          case KEY_T : emit_key(virt_fd, ev, KEY_K); break;
          case KEY_Y : emit_key(virt_fd, ev, KEY_T); break;

            //case KEY_S : emit_key(virt_fd, ev, KEY_SEMICOLON); break; //NOTE: Assigns general in EU4
          case KEY_S : emit_key(virt_fd, ev, 39); break;
          case KEY_V : emit_key(virt_fd, ev, 52); break;
          case KEY_MINUS : emit_key(virt_fd, ev, 40); break;
          case KEY_D : emit_key(virt_fd, ev, KEY_H); break;
          case KEY_G : emit_key(virt_fd, ev, KEY_U); break;
          case KEY_H : emit_key(virt_fd, ev, KEY_J); break;
          case KEY_U : emit_key(virt_fd, ev, KEY_F); break;
          case KEY_I : emit_key(virt_fd, ev, KEY_G); break;
          case KEY_O : emit_key(virt_fd, ev, KEY_S); break;
          case KEY_P : emit_key(virt_fd, ev, KEY_R); break;
          case KEY_L : emit_key(virt_fd, ev, KEY_P); break;
          case KEY_F : emit_key(virt_fd, ev, KEY_Y); break;


            //case KEY_Z : emit_key(virt_fd, ev, KEY_MINUS); break;
          case KEY_Z : emit_key(virt_fd, ev, 53); break;
          case KEY_X : emit_key(virt_fd, ev, KEY_B); break;
          case KEY_C : emit_key(virt_fd, ev, KEY_I); break;
          case KEY_K : emit_key(virt_fd, ev, KEY_V); break;
          case KEY_B : emit_key(virt_fd, ev, KEY_N); break;
          case KEY_N : emit_key(virt_fd, ev, KEY_L); break;

#if 0
          case KEY_1 : emit_key(virt_fd, ev, KEY_LEFTBRACE); break;
          case KEY_3 : emit_key(virt_fd, ev, KEY_LEFTBRACE); break;
          case KEY_4 : emit_key(virt_fd, ev, KEY_RIGHTBRACE); break;

          case KEY_6 : emit_key(virt_fd, ev, KEY_EQUAL); break;
            //case KEY_8 : emit_key(virt_fd, ev, KEY_EQUAL & KEY_LEFTSHIFT); break;

#endif
            //case KEY_AMPERSAND : emit_key(virt_fd, ev, KEY_1); break;

          case KEY_GRAVE : end = 0; break;
            //case KEY_M : emit_key(virt_fd, ev, KEY_M); break;
          default:
            if(ev.value == 0 && ev.code != 0)
              printf("\t keyboard event: type %d code %d value %d  \n", ev.type, ev.code, ev.value);

            emit(virt_fd, ev.type, ev.code, ev.value);
        }
#else
        emit_key(virt_fd, ev, ev.code);
#endif
            //emit(virt_fd, ev.type, ev.code, ev.value);
#endif
      }


      int sz = read(mouse_fd, &mouse_event, sizeof(mouse_event));
      if (sz) {
        if(sz != -1) {
          printf("mouse event: type %d code %d value %d  \n", mouse_event.type, mouse_event.code, mouse_event.value);
        }
      }
    }

    printf("Exiting.\n");
    rcode = ioctl(keyboard_fd, EVIOCGRAB, 1);
    close(keyboard_fd);
    rcode = ioctl(mouse_fd, EVIOCGRAB, 1);
    close(mouse_fd);
#ifdef FAKE_INPUT // Setting up a fake keyboard
    /* Clean up */
    ioctl(virt_fd, UI_DEV_DESTROY);
    close(virt_fd);
#endif

    return 0;
}
