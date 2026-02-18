//Done by Aaron

#include "next_box.h"

void init_next_box(NextBox *box, int type) {
    box->next_type = type;
}

int get_next_type(const NextBox *box) {
    return box->next_type;
}

void set_next_type(NextBox *box, int type) {
    box->next_type = type;
}