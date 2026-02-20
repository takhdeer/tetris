// Author: Aaron

#ifndef NEXT_BOX_H
#define NEXT_BOX_H


typedef struct {
    int next_type;
} NextBox;

void init_next_box(NextBox *box, int type);
int get_next_type(const NextBox *box);
void set_next_type(NextBox *box, int type);

#endif