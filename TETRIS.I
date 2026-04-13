# 1 "tetris.c" 1
# 1 "C:/c68/include/osbind.h" 1
# 1 "C:/c68/include/compiler.h" 1
# 9 "C:/c68/include/osbind.h" 2
# 1 "C:/c68/include/ostruct.h" 1
# 22 "C:/c68/include/ostruct.h"
typedef struct {
    long b_free;
    long b_total;
    long b_secsiz;
    long b_clsiz;
} _DISKINFO;


typedef struct {
  short time;
  short date;
} _DOSTIME;


typedef struct
{
        unsigned char maxlen;
        unsigned char actuallen;
        char    buffer[255];
} _CCONLINE;
# 48 "C:/c68/include/ostruct.h"
typedef struct _dta {
    char 	    dta_buf[21];
    char            dta_attribute;
    unsigned short  dta_time;
    unsigned short  dta_date;
    long            dta_size;
    char            dta_name[14];
} _DTA;
# 98 "C:/c68/include/ostruct.h"
typedef struct {
  short recsiz;
  short clsiz;
  short clsizb;
  short rdlen;
  short fsiz;
  short fatrec;
  short datrec;
  short numcl;
  short bflags;
} _BPB;




typedef struct _md {
    struct _md	*md_next;
    long	 md_start;
    long	 md_length;
    long	 md_owner;
} _MD;


typedef struct {
    _MD *mp_free;
    _MD *mp_used;
    _MD *mp_rover;
} _MPB;
# 141 "C:/c68/include/ostruct.h"
typedef struct {
    char    *ibuf;
    short   ibufsiz;
    volatile short   ibufhd;
    volatile short   ibuftl;
    short   ibuflow;
    short   ibufhi;
} _IOREC;


typedef struct {
	char	topmode;
	char	buttons;
	char	xparam;
	char	yparam;
	short	xmax;
	short	ymax;
	short	xstart;
	short	ystart;
} _PARAM;


typedef struct {
    void    (*midivec)	(void) ;
    void    (*vkbderr)	(void) ;
    void    (*vmiderr)	(void) ;
    void    (*statvec)	(void *) ;
    void    (*mousevec)	(void *) ;
    void    (*clockvec)	(void *) ;
    void    (*joyvec)	(void *) ;
    long    (*midisys)	(void) ;
    long    (*ikbdsys)	(void) ;
    char    kbstate;
} _KBDVECS;


typedef struct {
    void *unshift;
    void *shift;
    void *caps;
} _KEYTAB;


typedef struct
{
        void    *pb_scrptr;
        int     pb_offset;
        int     pb_width;
        int     pb_height;
        int     pb_left;
        int     pb_right;
        int     pb_screz;
        int     pb_prrez;
        void    *pb_colptr;
        int     pb_prtype;
        int     pb_prport;
        void    *pb_mask;
} _PBDEF;
# 17 "C:/c68/include/osbind.h" 2
# 33 "C:/c68/include/osbind.h"
 long _trap_1_w		(short) ;
 long _trap_1_ww	(short,short) ;
 long _trap_1_wl	(short,long) ;
 long _trap_1_wlw	(short,long,short) ;
 long _trap_1_wwll	(short,short,long,long) ;
 long _trap_1_wlww	(short,long,short,short) ;
 long _trap_1_www	(short,short,short) ;
 long _trap_1_wll	(short,long,long) ;
 long _trap_1_wwlll	(short,short,long,long,long) ;
 long _trap_1_wwwll	(short,short,short,long,long) ;
 long _trap_13_wl	(short,long) ;
 long _trap_13_w	(short) ;
 long _trap_13_ww	(short,short) ;
 long _trap_13_www	(short,short,short) ;
 long _trap_13_wwlwww	(short,short,long,short,short,short) ;
 long _trap_13_wwl	(short,short,long) ;
 long _trap_14_wwl	(short,short,long) ;
 long _trap_14_wwll	(short,short,long,long) ;
 long _trap_14_ww	(short,short) ;
 long _trap_14_w	(short) ;
 long _trap_14_wllw	(short,long,long,short) ;
 long _trap_14_wl	(short,long) ;
 long _trap_14_www	(short,short,short) ;
 long _trap_14_wllwwwww (short,long,long,short,short,short,short,short) ;
 long _trap_14_wllwwwwlw (short,long,long,short,short,short,short,long,short) ;
 long _trap_14_wllwwwwwlw (short,long,long,short,short,short,short,short,long,short) ;
 long _trap_14_wwwwwww	(short,short,short,short,short,short,short) ;
 long _trap_14_wlll	(short,long,long,long) ;
 long _trap_14_wllww	(short,long,long,short,short) ;
 long _trap_14_wwwwl	(short,short,short,short,long) ;
 long _trap_14_wwwl	(short,short,short,long) ;
 long _trap_14_wlwlw	(short,long,short,long,short) ;
# 103 "C:/c68/include/osbind.h"
 long _trap_1_ 	(short,...) ;
 long _trap_14_ 	(short,...) ;
 long _trap_13_ 	(short,...) ;
# 5 "tetris.c" 2
# 1 "./input.h" 1
# 19 "./input.h"
int has_input();
# 28 "./input.h"
char get_input();

char get_scan_code();

int get_mouse_x();

int get_mouse_y();

int get_mouse_buttons();

void update_mouse(int dx, int dy);

void set_mouse_buttons(int buttons);
# 6 "tetris.c" 2
# 1 "./synch.h" 1
# 1 "./model.h" 1
# 1 "./gstate.h" 1
# 1 "./types.h" 1





typedef unsigned int  UINT16;
typedef unsigned char UINT8;
typedef unsigned long UINT32;
# 10 "./gstate.h" 2



typedef struct GameState {
    UINT16 score;
    UINT16 level;
    UINT16 lines_cleared;
    UINT8  is_game_over;
} GameState;




void init_game_state(GameState *gameState, UINT16 score, UINT8 game_over);




UINT16 map_line_base(const UINT16 lines_cleared);





void update_state(GameState *state, UINT16 lines_cleared);
# 6 "./model.h" 2
# 1 "./matrix.h" 1
# 1 "./tetrom.h" 1
# 18 "./tetrom.h"
typedef struct {
    int type;
    int row;
    int col;
    int rotation;
} Tetromino;

extern const int tetromino_shapes[7][4][4][4];
# 39 "./tetrom.h"
void init_tetromino(Tetromino *piece, int type, int spawn_col);
# 48 "./tetrom.h"
void move_tetromino_left(Tetromino *piece);
# 57 "./tetrom.h"
void move_tetromino_right(Tetromino *piece);
# 66 "./tetrom.h"
void move_tetromino_down(Tetromino *piece);
# 75 "./tetrom.h"
void rotate_tetromino_cw(Tetromino *piece);
# 86 "./tetrom.h"
int get_cell(const Tetromino *piece, int local_row, int local_col);
# 10 "./matrix.h" 2
# 1 "./types.h" 1
# 11 "./matrix.h" 2



typedef struct {
    UINT8 cell[20 ][10 ];
} Matrix;
# 27 "./matrix.h"
void create_matrix(Matrix *gameGrid);
# 38 "./matrix.h"
void output_matrix(Matrix *gameGrid);
# 49 "./matrix.h"
UINT16 clear_full_lines(Matrix *gameGrid,UINT16 lines_cleared);
# 60 "./matrix.h"
void drop_rows_down(Matrix *gameGrid);
# 71 "./matrix.h"
void lock_piece(Matrix *board, const Tetromino *piece);
# 7 "./model.h" 2
# 1 "./tetrom.h" 1
# 8 "./model.h" 2
# 1 "./hold_box.h" 1
# 1 "./tetrom.h" 1
# 7 "./hold_box.h" 2


typedef struct {
    unsigned int contains;
    Tetromino piece_held;
} HoldBox;
# 23 "./hold_box.h"
void init_hold_box(HoldBox *heldbox);
# 34 "./hold_box.h"
unsigned int hold_box_contains(HoldBox *heldbox);
# 45 "./hold_box.h"
Tetromino hold_tetromino(HoldBox *heldbox, Tetromino *piece);
# 56 "./hold_box.h"
Tetromino release_tetromino(HoldBox *heldbox);
# 9 "./model.h" 2
# 1 "./next_box.h" 1
# 8 "./next_box.h"
typedef struct {
    int next_type;
} NextBox;
# 20 "./next_box.h"
void init_next_box(NextBox *box, int type);
# 29 "./next_box.h"
int get_next_type(const NextBox *box);
# 39 "./next_box.h"
void set_next_type(NextBox *box, int type);
# 10 "./model.h" 2
# 1 "./types.h" 1
# 11 "./model.h" 2


typedef struct {
    GameState game_state;
    Matrix    Matrix;
    Tetromino piece;
    NextBox   nbox;
    HoldBox   hbox;


    int request_move_left;
    int request_move_right;
    int request_rotate;
    int request_soft_drop;
    int request_hold;


    int bag[7];
    int bag_index;

    int request_hard_drop;

    int gravity_counter;

    int hold_used;


    int redraw_matrix;
    int redraw_next_box;
    int redraw_hold_box;
    int redraw_score;
} Model;

extern Model initial_model;




void init_model(Model *model);
# 61 "./model.h"
UINT8 check_game_over (Model *model);
# 73 "./model.h"
UINT32 get_time();
# 85 "./model.h"
void init_bag(Model *model);
# 98 "./model.h"
int next_piece_from_bag(Model *model);
# 110 "./model.h"
int peek_bag(Model *model);
# 6 "./synch.h" 2
# 1 "./matrix.h" 1
# 7 "./synch.h" 2
# 1 "./tetrom.h" 1
# 8 "./synch.h" 2
# 1 "./types.h" 1
# 9 "./synch.h" 2
# 1 "./next_box.h" 1
# 10 "./synch.h" 2
# 26 "./synch.h"
UINT16 check_collision(const Matrix *board, const Tetromino *piece, int offset_x, int offset_y);





UINT16 can_move_down(Model *model);
# 42 "./synch.h"
void handle_tick(Model *model);
# 7 "tetris.c" 2
# 1 "./cond.h" 1
# 1 "./gstate.h" 1
# 4 "./cond.h" 2
# 1 "./tetrom.h" 1
# 5 "./cond.h" 2
# 1 "./synch.h" 1
# 6 "./cond.h" 2
# 14 "./cond.h"
int handle_spawn_piece(Model *model);





void handle_piece_lock(Model *model);





void handle_line_clear(Model *model);
# 8 "tetris.c" 2
# 1 "./renderer.h" 1
# 1 "./tetrom.h" 1
# 5 "./renderer.h" 2
# 1 "./next_box.h" 1
# 6 "./renderer.h" 2
# 1 "./types.h" 1
# 7 "./renderer.h" 2
# 1 "./raster.h" 1
# 1 "./types.h" 1
# 4 "./raster.h" 2
# 15 "./raster.h"
void clear_screen(UINT32 *base);
# 30 "./raster.h"
void clear_region(UINT32 *base, UINT16 row, UINT16 col, UINT16 length, UINT16 width);
# 43 "./raster.h"
void plot_pixel(UINT8 *base, UINT16 row, UINT16 col);
# 56 "./raster.h"
void plot_horizontal_line(UINT32 *base, UINT16 row, UINT16 col, UINT16 length);
# 69 "./raster.h"
void plot_vertical_line(UINT32 *base, UINT16 row, UINT16 col, UINT16 length);
# 82 "./raster.h"
void plot_line(UINT32 *base, UINT16 start_row, UINT16 start_col, UINT16 end_row, UINT16 end_col);
# 96 "./raster.h"
void plot_rectangle(UINT32 *base, UINT16 row, UINT16 col, UINT16 length, UINT16 width);
# 109 "./raster.h"
void plot_square(UINT32 *base, UINT16 row, UINT16 col, UINT16 side);
# 128 "./raster.h"
void plot_triangle(UINT32 *base, UINT16 row, UINT16 col, UINT16 width, UINT16 height, UINT8 direction);
# 141 "./raster.h"
void plot_bitmap_8(UINT8 *base, UINT16 row, UINT16 col, UINT16 height, UINT8 *bitmap);
# 154 "./raster.h"
void plot_bitmap_16(UINT16 *base, UINT16 row, UINT16 col, UINT16 height, UINT16 *bitmap);
# 167 "./raster.h"
void plot_bitmap_32(UINT32 *base, UINT16 row, UINT16 col, UINT16 height, const UINT32 *bitmap);
# 180 "./raster.h"
void plot_character(UINT8 *base, UINT16 row, UINT16 col, char ch);
# 193 "./raster.h"
void plot_string(UINT8 *base, UINT16 row, UINT16 col, char *ch);
# 201 "./raster.h"
UINT16 *get_video_base();
# 210 "./raster.h"
void set_video_base(UINT16 *base);
# 10 "./renderer.h" 2
# 1 "./matrix.h" 1
# 11 "./renderer.h" 2
# 1 "./hold_box.h" 1
# 12 "./renderer.h" 2
# 1 "./cond.h" 1
# 15 "./renderer.h" 2
# 41 "./renderer.h"
void render_next_box(UINT32 *base, const NextBox *box);
# 55 "./renderer.h"
void render_matrix(UINT32 *base, const Matrix *gameGrid);
# 66 "./renderer.h"
void render_hold_box(UINT32 *base, const HoldBox *heldbox);
# 80 "./renderer.h"
void render_piece(const Tetromino *piece, UINT32 *base);
# 91 "./renderer.h"
void render_raw_piece(const Tetromino *piece, UINT32 *base);




void uint_to_str(UINT32 value, char *buf, int start);
# 107 "./renderer.h"
void render_score(const GameState *game, UINT8 *base);
# 118 "./renderer.h"
void render_level(const GameState *game, UINT8 *base);
# 129 "./renderer.h"
void render_game_over(UINT8 *base, int score);
# 9 "tetris.c" 2
# 1 "./model.h" 1
# 10 "tetris.c" 2
# 1 "./gstate.h" 1
# 11 "tetris.c" 2
# 1 "./hold_box.h" 1
# 12 "tetris.c" 2
# 1 "./next_box.h" 1
# 13 "tetris.c" 2
# 1 "./matrix.h" 1
# 14 "tetris.c" 2
# 1 "./music.h" 1
# 1 "./types.h" 1
# 7 "./music.h" 2


void start_music(void);
void update_music(UINT32 time_elapsed);
void stop_music(void);
# 15 "tetris.c" 2
# 1 "./splash.h" 1
# 1 "./types.h" 1
# 9 "./splash.h" 2




static void filled_rect(UINT32 *base, UINT16 top, UINT16 left, UINT16 bottom, UINT16 right);

static void draw_circle(UINT8 *base, UINT16 cy, UINT16 cx, UINT16 r);





static void draw_onion_dome(UINT32 *base, UINT16 top_row, UINT16 centre_col, UINT16 dome_h, UINT16 dome_w);
# 31 "./splash.h"
static void draw_temple(UINT32 *base, UINT16 ground_row, UINT16 anchor_col);

static void draw_moon(UINT8 *base);

static void draw_stars(UINT8 *base);

int show_splash_screen(UINT8 *base);
# 16 "tetris.c" 2
# 1 "./effects.h" 1
# 1 "./types.h" 1
# 7 "./effects.h" 2
# 1 "./psg.h" 1
# 1 "./types.h" 1
# 7 "./psg.h" 2
# 22 "./psg.h"
void write_psg(int reg, UINT8 val);
UINT8 read_psg(int reg);
void set_tone(int channel, int tuning);
void set_volume(int channel, int volume);
void enable_channel(int channel, int tone_on, int noise_on);
void stop_sound();
# 40 "./psg.h"
void set_noise(int tuning);
# 51 "./psg.h"
void set_envelope(int shape, unsigned int sustain);
# 8 "./effects.h" 2
# 21 "./effects.h"
void play_effect_hold();
# 32 "./effects.h"
void play_effect_line_clear();
# 44 "./effects.h"
void play_effect_tetris_clear();
# 17 "tetris.c" 2
# 1 "./vbl_inst.h" 1
# 8 "./vbl_inst.h"
void install_vbl_isr();





void uninstall_vbl_isr();

void vbl_handler();
# 18 "tetris.c" 2
# 1 "./ikbd.h" 1
# 1 "./types.h" 1
# 7 "./ikbd.h" 2





typedef struct {
    UINT8 buf[8 ];
    UINT8 head;
    UINT8 tail;
    UINT8 count;
} ScanQueue;




extern volatile ScanQueue scan_queue;


extern volatile UINT8 key_state[128];


extern volatile long mouse_x;
extern volatile long mouse_y;
extern volatile UINT8 mouse_buttons;


void install_ikbd_isr(void);
void remove_ikbd_isr(void);
void disable_midi(void);
void enable_midi(void);
void mask_ikbd(void);
void unmask_ikbd(void);


extern void ikbd_isr(void);
# 19 "tetris.c" 2
# 1 "./gravity.h" 1
# 10 "./gravity.h"
void update_gravity();
# 20 "tetris.c" 2
# 34 "tetris.c"
Model game_model;
volatile int render_request = 0;
volatile int drop_requested = 0;

const char LEFT_ARROW = 0x4B;
const char RIGHT_ARROW = 0x4D;
const char UP_ARROW = 0x48;
const char DOWN_ARROW = 0x50;


UINT8 buffer_space[32256];




int main() {
    UINT8 quit = 0;
    char key;

    Tetromino temp_piece;
    Tetromino released_piece;
    Tetromino old_piece;
    int gravity_threshold;

    int pr, pc;
    UINT16 cell_row, cell_col;
    int old_lines;
    UINT16 lines_this_drop;

    int center_x;
    int center_y;
    int i;

    long old_ssp;


    Tetromino last_piece_buf1;
    Tetromino last_piece_buf2;
    int which_buffer;
    int first_render;


    UINT32 *buffer1;
    UINT32 *buffer2;
    UINT32 *back_buffer;
    UINT32 *front_buffer;
    UINT32 *original_screen;
    UINT32 *temp_buffer;


    original_screen = (UINT32 *)get_video_base();


    buffer1 = original_screen;
    buffer2 = (UINT32 *)(((UINT32)buffer_space + 255) & 0xFFFFFF00);

    front_buffer = buffer1;
    back_buffer = buffer2;


    clear_screen(buffer1);
    clear_screen(buffer2);


    disable_midi();
    install_ikbd_isr();


    if (!show_splash_screen((UINT8 *)buffer1)) {
        remove_ikbd_isr();
        enable_midi();
        return 0;
    }


    clear_screen(buffer1);
    clear_screen(buffer2);


    init_model(&game_model);


    init_next_box(&game_model.nbox, peek_bag(&game_model));
    init_hold_box(&game_model.hbox);


    render_matrix(back_buffer, &game_model.Matrix);
    render_next_box((UINT32 *) back_buffer, &game_model.nbox);
    render_hold_box(back_buffer, &game_model.hbox);
    render_piece(&game_model.piece, back_buffer);
    render_score(&game_model.game_state, (UINT8 *)back_buffer);
    render_level(&game_model.game_state, (UINT8 *)back_buffer);


    old_ssp = _trap_1_wl((short)0x20,(long)(0)) ;

    set_video_base((UINT16 *)back_buffer);
    _trap_1_wl((short)0x20,(long)(old_ssp)) ;


    temp_buffer = front_buffer;
    front_buffer = back_buffer;
    back_buffer = temp_buffer;


    which_buffer = 1;
    last_piece_buf1 = game_model.piece;
    last_piece_buf2 = game_model.piece;
    first_render = 1;


    start_music();


    while (quit != 1) {
        render_request = 1;
        update_gravity();

        update_music(1);


        if (has_input()) {
            key = get_input();

            if (key == 0) {
                char scan = get_scan_code();

                if (scan == LEFT_ARROW) {
                    game_model.request_move_left = 1;
                }
                else if (scan == RIGHT_ARROW) {
                    game_model.request_move_right = 1;
                }
                else if (scan == UP_ARROW) {
                    game_model.request_rotate = 1;
                }
                else if (scan == DOWN_ARROW) {
                    game_model.request_soft_drop = 1;
                }
            }


            else {
                if (key == 'c') {
                    game_model.request_hold = 1;
                }
                else if(key == ' ') {
                    game_model.request_hard_drop = 1;
                }
                else if (key == 'q') {
                    quit = 1;
                }
            }
        }


        if (render_request) {


            if (first_render) {
                old_piece = game_model.piece;
                first_render = 0;
            }
            else {
                old_piece = (which_buffer == 0) ? last_piece_buf1 : last_piece_buf2;
            }


            if (game_model.request_move_left) {
                if (!check_collision(&game_model.Matrix, &game_model.piece,
                                    game_model.piece.col - 1, game_model.piece.row)) {
                    move_tetromino_left(&game_model.piece);
                }
                game_model.request_move_left = 0;
            }

            if (game_model.request_move_right) {
                if (!check_collision(&game_model.Matrix, &game_model.piece,
                                    game_model.piece.col + 1, game_model.piece.row)) {
                    move_tetromino_right(&game_model.piece);
                }
                game_model.request_move_right = 0;
            }

            if (game_model.request_rotate) {
                temp_piece = game_model.piece;
                rotate_tetromino_cw(&temp_piece);

                if (!check_collision(&game_model.Matrix, &temp_piece, temp_piece.col, temp_piece.row)) {
                    game_model.piece = temp_piece;
                }
                game_model.request_rotate = 0;
            }

            if (game_model.request_soft_drop) {
                if (can_move_down(&game_model)) {
                    move_tetromino_down(&game_model.piece);
                }
                game_model.request_soft_drop = 0;
            }

            temp_piece = game_model.piece;

            if (game_model.request_hold) {
                game_model.request_hold = 0;


                if (!game_model.hold_used) {

                    if (hold_box_contains(&game_model.hbox)) {
                        released_piece = release_tetromino(&game_model.hbox);
                        hold_tetromino(&game_model.hbox, &temp_piece);


                        released_piece.row = 0;
                        released_piece.col = 3;
                        released_piece.rotation = 0;

                        game_model.piece = released_piece;
                    }
                    else {
                        hold_tetromino(&game_model.hbox, &temp_piece);

                        init_tetromino(&game_model.piece, next_piece_from_bag(&game_model), 3);
                        game_model.hold_used = 0;
                        init_next_box(&game_model.nbox, peek_bag(&game_model));
                        game_model.redraw_next_box = 1;
                    }
                    game_model.redraw_hold_box = 1;
                    game_model.hold_used = 1;

                }
            }

            if (game_model.request_hard_drop) {
                while (can_move_down(&game_model)) {
                    move_tetromino_down(&game_model.piece);
                }

                lock_piece(&game_model.Matrix, &game_model.piece);
                game_model.redraw_matrix = 1;

                old_lines = game_model.game_state.lines_cleared;
                game_model.game_state.lines_cleared = clear_full_lines(&game_model.Matrix,
                    game_model.game_state.lines_cleared);
                lines_this_drop = game_model.game_state.lines_cleared - old_lines;

                if (game_model.game_state.lines_cleared > old_lines) {
                    game_model.redraw_matrix = 1;
                    game_model.redraw_score = 1;
                    update_state(&game_model.game_state, lines_this_drop);

                    if (lines_this_drop < 4) {
                        play_effect_line_clear();
                    }
                    else {
                        play_effect_tetris_clear();
                    }
                }

                init_tetromino(&game_model.piece, next_piece_from_bag(&game_model), 3);
                game_model.hold_used = 0;
                init_next_box(&game_model.nbox, peek_bag(&game_model));
                game_model.redraw_next_box = 1;

                if (game_model.game_state.is_game_over) {
                    quit = 1;
                }

                game_model.request_hard_drop = 0;
            }


            else {
                game_model.gravity_counter++;

                gravity_threshold = 50 - (game_model.game_state.level * 5);
                if (gravity_threshold < 5) {
                    gravity_threshold = 5;
                }

                if (game_model.gravity_counter >= gravity_threshold) {
                    old_lines = game_model.game_state.lines_cleared;
                    handle_tick(&game_model);
                    game_model.gravity_counter = 0;

                    lines_this_drop = game_model.game_state.lines_cleared - old_lines;

                    if (lines_this_drop > 0) {
                        game_model.redraw_matrix = 1;
                        game_model.redraw_score = 1;

                        if (lines_this_drop < 4) {
                            play_effect_line_clear();
                        }
                        else {
                            play_effect_tetris_clear();
                        }
                    }

                    if(game_model.game_state.is_game_over == 1) {
                        quit = 1;
                    }
                }
            }


            if (game_model.redraw_matrix) {
                UINT32 *back_ptr;
                UINT32 *front_ptr;
                int row, word;
                int bytes_per_row = 80;
                int matrix_pixel_rows = 20  * 16 ;
                int words_per_matrix_row = (10  * 16 ) / 32;

                for (row = 0; row < matrix_pixel_rows; row++) {
                    back_ptr  = (UINT32 *)((UINT8 *)back_buffer  + ((row + 40 ) * bytes_per_row) + (240  / 8));
                    front_ptr = (UINT32 *)((UINT8 *)front_buffer + ((row + 40 ) * bytes_per_row) + (240  / 8));

                    for (word = 0; word < words_per_matrix_row; word++) {
                        back_ptr[word]  = 0;
                        front_ptr[word] = 0;
                    }
                }

                render_matrix(back_buffer, &game_model.Matrix);
                render_matrix(front_buffer, &game_model.Matrix);
                game_model.redraw_matrix = 0;
            }

            if (game_model.redraw_next_box) {
                clear_region(back_buffer, 50 , 500 , 80 , 80 );
                clear_region(front_buffer, 50 , 500 , 80 , 80 );

                render_next_box(back_buffer, &game_model.nbox);
                render_next_box(front_buffer, &game_model.nbox);
                game_model.redraw_next_box = 0;
            }

            if (game_model.redraw_hold_box) {
                clear_region(back_buffer, 320 , 500 , 80 , 80 );
                clear_region(front_buffer, 320 , 500 , 80 , 80 );

                render_hold_box(back_buffer, &game_model.hbox);
                render_hold_box(front_buffer, &game_model.hbox);
                game_model.redraw_hold_box = 0;
            }

            if (game_model.redraw_score) {
                clear_region(back_buffer, 150 , 20 , 16, 150);
                clear_region(front_buffer, 150 , 20 , 16, 150);
                clear_region(back_buffer, 170 , 20 , 16, 150);
                clear_region(front_buffer, 170 , 20 , 16, 150);

                render_score(&game_model.game_state, (UINT8 *)back_buffer);
                render_level(&game_model.game_state, (UINT8 *)back_buffer);
                render_score(&game_model.game_state, (UINT8 *)front_buffer);
                render_level(&game_model.game_state, (UINT8 *)front_buffer);
                game_model.redraw_score = 0;
            }


            for (pr = 0; pr < 4; pr++) {
                for (pc = 0; pc < 4; pc++) {
                    if (get_cell(&old_piece, pr, pc) != 0) {
                        int matrix_row = old_piece.row + pr;
                        int matrix_col = old_piece.col + pc;

                        if (matrix_row >= 0 && matrix_row < 20  &&
                            matrix_col >= 0 && matrix_col < 10 ) {

                            cell_row = (UINT16)(matrix_row * 16 ) + 40 ;
                            cell_col = (UINT16)(matrix_col * 16 ) + 240 ;

                            clear_region(back_buffer, cell_row, cell_col,
                                        (UINT16)16 , (UINT16)16 );

                            if (game_model.Matrix.cell[matrix_row][matrix_col] == 1) {
                                plot_rectangle(back_buffer, cell_row, cell_col, 16 , 16 );

                                center_x = cell_col + (16  / 2) - 6;
                                center_y = cell_row + (16  / 2) - 6;

                                for (i = 0; i < 12; i++) {
                                    plot_horizontal_line(back_buffer, center_y + i, center_x, 12);
                                }
                            }
                        }
                    }
                }
            }


            render_piece(&game_model.piece, back_buffer);


            old_ssp = _trap_1_wl((short)0x20,(long)(0)) ;
            set_video_base((UINT16 *)back_buffer);
            _trap_1_wl((short)0x20,(long)(old_ssp)) ;


            if (which_buffer == 0) {
                last_piece_buf1 = game_model.piece;
            } else {
                last_piece_buf2 = game_model.piece;
            }


            temp_buffer = front_buffer;
            front_buffer = back_buffer;
            back_buffer = temp_buffer;


            which_buffer = 1 - which_buffer;


            update_music(1);

            render_request = 0;
        }
    }

    stop_music();
    remove_ikbd_isr();
    enable_midi();

    old_ssp = _trap_1_wl((short)0x20,(long)(0)) ;

    set_video_base((UINT16 *)original_screen);
    _trap_1_wl((short)0x20,(long)(old_ssp)) ;

    clear_screen(buffer1);
    clear_screen(buffer2);
    render_game_over((UINT8 *)original_screen, game_model.game_state.score);

    return 0;
}
