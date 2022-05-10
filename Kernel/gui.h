#ifndef MocaGUI
#define MocaGUI
#include "memory.h"
 
typedef struct FramebufferInfo
{
        uint32 *framebuffer;
        uint32 *antibuffer;
} _FB_INFO;
 
typedef struct Window {
        uint8   *title;
        uint32  id;
        uint32  x;
        uint32  y;
        uint32  x_size;
        uint32  y_size;
        uint32  *buffer;
        struct Window *next;
} _Window;
 
typedef struct buttons {
        uint8           *button_title;
        uint32          button_id;
        // Might not need?
        /*
         * is_activated:
         *      Set to zero if button is clicked, by default set to 1.
         *      (TRUE = 0, FALSE = 1)
         * */
        bool            is_activated;
        struct buttons  *next_button;
} _Buttons;
 
 
static _Window *windows;
static _FB_INFO fb_info;
static _Buttons top_bar_buttons;
static uint32   top_bar_buttons_amnt = 0;
 
#define RESET_FB \
        fb_info.framebuffer = (uint32 *) FB
#define PADDING         2
 
void plot_px(uint32 x, uint32 y, uint32 color)
{
        uint32 fb_i = x + (pitch / sizeof(uint32)) * y;
        fb_info.framebuffer[fb_i] = color;
}
 
void plot_char(uint8 c, uint32 x, uint32 y, uint32 fg, uint32 bg)
{
        if(y == 0)
                y = 1;
        uint32 orig_x = x;
        uint8 *font = (uint8 *)FONT;
 
        for(uint32 i = 0; i < 16; i++)
        {
                for(uint32 j = 0; j < 8; j++)
                {
                        if((font[c * 16 + i] >> ((8 - 1) - j)) & 1)
                                plot_px(x++, y, fg);
                        else
                                plot_px(x++, y, bg);
                }
                y++;
                x = orig_x;
        }
        RESET_FB;
}
 
void vertical_line(uint32 length, uint32 x, uint32 y, uint32 color)
{
        for(uint32 i = 0; i < length; i++)
                plot_px(x, y++, color);
}
 
void generate_top_bar()
{
        uint32 color = 0x0;
        uint32 x = 32;
        uint32 y = 32;
        uint32 x1 = 0;
 
        for(uint32 x = 0; x < HEIGHT; x++)
                for(uint32 y = 0; y < WIDTH; y++)
                {
                        if(x < 20)
                                color = WHITE;
                        else
                                color = BLACK;
                        *fb_info.framebuffer = color;
                        fb_info.framebuffer++;
                }
        RESET_FB;
 
        uint8 a[5+(PADDING*2)] = "  HOME  ";
        for(uint8 i = 0; i < SIZEOF(a); i++)
        {
                plot_char(a[i], x1, 0, BLACK, WHITE);
                x1+=8;
        }
        x1-=8;
        vertical_line(20, x1, 0, BLACK);
 
        top_bar_buttons.button_title = (uint8 *)"home";
        top_bar_buttons.button_id    = top_bar_buttons_amnt;
        top_bar_buttons.is_activated = FALSE;
        top_bar_buttons.next_button = moca_alloc(1, 4);
 
        top_bar_buttons_amnt++;
}
 
void refresh_screen() {
        _Window *w = moca_alloc(1, 4);
 
        for(uint32 x = 0; x < HEIGHT; x++)
                for(uint32 y = 0; y < WIDTH; y++)
                {
                        *fb_info.framebuffer = WHITE;
                        fb_info.framebuffer++;
                }
        RESET_FB;
        generate_top_bar();
        RESET_FB;
}
 
 
void init_GUI() {
        fb_info = (_FB_INFO) {
                .framebuffer = (uint32 *)FB,
                .antibuffer = (uint32 *)0x200
        };
 
        refresh_screen();
}
 
 
#endif