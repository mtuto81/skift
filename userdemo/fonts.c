/* Copyright © 2018-2019 N. Van Bossuyt.                                      */
/* This code is licensed under the MIT License.                               */
/* See: LICENSE.md                                                            */

#include <skift/iostream.h>
#include <skift/cstring.h>
#include <skift/error.h>
#include <skift/bitmap.h>
#include <skift/painter.h>
#include <skift/logger.h>
#include <skift/assert.h>

#include <hjert/devices/framebuffer.h>

framebuffer_mode_info_t mode_info = {true, 800, 600};

typedef struct
{
    point_t start;
    point_t finish;
    color_t color;
} a_line_t;

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    iostream_t *framebuffer_device = iostream_open(FRAMEBUFFER_DEVICE, IOSTREAM_READ);

    assert(framebuffer_device);

    assert(iostream_ioctl(framebuffer_device, FRAMEBUFFER_IOCTL_SET_MODE, &mode_info) >= 0);

    bitmap_t *fb = bitmap(800, 600);

    assert(fb);

    font_t *mono = font("mono");

    assert(mono);

    painter_t *paint = painter(fb);

    assert(paint);

    painter_fill_rect(paint, bitmap_bound(fb), COLOR_BLACK);
    painter_fill_rect(paint, (rectangle_t){{16, 16, 16, 16}}, COLOR_RED);

    painter_draw_text(paint, mono, "The quick brown fox jumps over the lazy dog.", (point_t){16, 16}, 16, COLOR_WHITE);

    iostream_ioctl(framebuffer_device, FRAMEBUFFER_IOCTL_BLIT, fb->buffer);

    return 0;
}