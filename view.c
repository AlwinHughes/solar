#include "view.h"

int view_single(char* box, unsigned int width, unsigned int height){
	Display* d = XOpenDisplay(0);

	if(!d){
		return 1;
	}

	int black_pixel = BlackPixel(d, DefaultScreen(d));
	int white_pixel = WhitePixel(d, DefaultScreen(d));
	unsigned int window_width = width;
	unsigned int window_height = height;
	
	XSetWindowAttributes attributes;
	attributes.background_pixel = black_pixel;
	attributes.border_pixel = white_pixel;

	int x_pos = 0;
	int y_pos = 0;
	int border_width = 5;

	Window w = XCreateWindow(
			d,
			DefaultRootWindow(d),
			x_pos, y_pos,
			window_width, window_height,
			border_width,
			CopyFromParent,	CopyFromParent, CopyFromParent, /* depth, class, visual*/
			CWBackPixel | CWBorderPixel,
			&attributes
			);

	XSelectInput(d, w, KeyPressMask | StructureNotifyMask);

	XMapWindow(d,w);

	//wait for us to be mapped
	for(;;) {
		XEvent e; 
		XNextEvent(d, &e);
		if(e.type == MapNotify) {
			break;
			printf("mapped\n");
		}
	}


	GC gc = XCreateGC(d, w, 0,0);
  int depth = DefaultDepth(d, DefaultScreen(d));
	printf("default depth: %i\n", depth);
	printf("viewing area %i by %i\n", width, height);

	Pixmap pm = XCreatePixmap(d, w, width, height, depth);

	size_t index;
	for(size_t y = 0; y < height; y++){
		for(size_t x = 0; x < width; x++){
			index = (x + height * y)*3;
			XSetForeground(d, gc, black_pixel + box[index] + 100);
			XDrawPoint(d, pm, gc, x, y);
		}
	}

	unsigned int key_q = 0x18;

	while(1){
		XEvent e;
		XNextEvent(d, &e);

		if(e.type == ConfigureNotify) {
			printf("configure\n");
			XConfigureEvent xce = e.xconfigure;

			if(xce.width != width || xce.height != height) {

				XClearWindow(d, w);
				window_width = xce.width;
				window_height = xce.height;
				printf("copying area %i by %i\n", width, height);
				XCopyArea(d, pm, w, gc, 0,0, width, height, 0,0);

			}
		}

		if(e.type == KeyPress) {

			if(e.xkey.keycode == key_q) {
				break;
			}
		}
	}

	XFreePixmap(d, pm);

	XCloseDisplay(d);

	return 0;
};



int view_all(unsigned char** boxes, size_t box_width, size_t box_height, size_t width, size_t height, size_t num_boxes_h){

	Display* d = XOpenDisplay(0);

	if(!d){
		return 1;
	}

	unsigned int black_pixel = BlackPixel(d, DefaultScreen(d));
	unsigned int white_pixel = WhitePixel(d, DefaultScreen(d));
	unsigned int window_width = width;
	unsigned int window_height = height;
	
	XSetWindowAttributes attributes;
	attributes.background_pixel = black_pixel;
	attributes.border_pixel = white_pixel;

	int x_pos = 0;
	int y_pos = 0;
	int border_width = 5;

	Window w = XCreateWindow(
			d,
			DefaultRootWindow(d),
			x_pos, y_pos,
			window_width, window_height,
			border_width,
			CopyFromParent,	CopyFromParent, CopyFromParent, /* depth, class, visual*/
			CWBackPixel | CWBorderPixel,
			&attributes
			);

	XSelectInput(d, w, KeyPressMask | StructureNotifyMask);

	XMapWindow(d,w);

	//wait for us to be mapped
	for(;;) {
		XEvent e; 
		XNextEvent(d, &e);
		if(e.type == MapNotify) {
			break;
			printf("mapped\n");
		}
	}

	GC gc = XCreateGC(d, w, 0,0);
  int depth = DefaultDepth(d, DefaultScreen(d));
	printf("default depth: %i\n", depth);
	printf("black_pixel %u, white pixel %u\n", black_pixel, white_pixel);

	Pixmap pm = XCreatePixmap(d, w, width, height, depth);


	int box_x, box_y;
	unsigned char* box;
	int curr_box_width;
	unsigned int index, t1, t2;
	for(int y = 0; y < height; y++ ){
		box_y = y / box_height;
		for(int x = 0; x < width; x++){

			box_x = x / box_width;
			curr_box_width = fmin(box_width, width - box_x * box_width);
			box = boxes[box_x + num_boxes_h * box_y];

			//printf("x: %i, y: %i\n", x,y);
			//printf("box_x %i, box_y %i width %i box_index %i index %i\n", box_x, box_y, curr_box_width, box_x + num_boxes_h * box_y, index);

			//curr_box_width = fmin(box_width,width - box_x * box_width);
			index = (curr_box_width * (y % box_height) + (x % box_width)) * sizeof(char) ;

			//box[index];
			t1 = box[index * 3+2];
			t2 = t1;
			t1 = box[index * 3 +1];
			t1 <<= 8;
			t2 += t1;
			t1 =  box[index * 3];
			t1 <<= 16;
			t2 += t1;

			
			XSetForeground(d, gc, black_pixel +t2);
			XDrawPoint(d, pm, gc, x, y);

			//printf("---\n");

		}
	}

	unsigned int key_q = 0x18;

	while(1){
		XEvent e;
		XNextEvent(d, &e);

		if(e.type == ConfigureNotify) {
			printf("configure\n");
			XConfigureEvent xce = e.xconfigure;

			if(xce.width != width || xce.height != height) {
				XClearWindow(d, w);
				window_width = xce.width;
				window_height = xce.height;
				XCopyArea(d, pm, w, gc, 0,0, width, height, 0,0);

			}
		}

		if(e.type == KeyPress) {

			if(e.xkey.keycode == key_q) {
				break;
			}
		}
	}

	XFreePixmap(d, pm);
	XCloseDisplay(d);

	return 0;
}
