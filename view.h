#ifndef _VIEW_H
#define _VIEW_H
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int view_single(char* box, unsigned int width, unsigned int height);

int view_all(char** boxes, size_t box_width, size_t box_height, size_t width, size_t height, size_t num_boxes_h);

#endif
