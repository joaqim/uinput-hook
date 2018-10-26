rm uinput-hook >/dev/null&

gcc uinput-hook.c -o uinput-hook && \
 sudo	./uinput-hook /dev/input/by-id/usb-KB_USB_KB-event-kbd 
