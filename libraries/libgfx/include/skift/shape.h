#pragma once

typedef PACKED(struct)
{
    int X;
    int Y;
} point_t;

#define point_zero ((point_t){0, 0})

static inline point_t point_clamp(point_t p, point_t pmin, point_t pmax)
{
    p.X = max(pmin.X, min(pmax.X, p.X));
    p.Y = max(pmin.Y, min(pmax.Y, p.Y));

    return p;
}

static inline point_t point_add(point_t a, point_t b)
{
    return (point_t){a.X + b.X, a.Y + b.Y};
}

static inline point_t point_sub(point_t a, point_t b)
{
    return (point_t){a.X - b.X, a.Y - b.Y};
}

static inline point_t point_scale(point_t a, double scale)
{
    return (point_t){a.X * scale, a.Y * scale};
}

static inline point_t point_x(point_t p)
{
    return (point_t){.X = p.X, .Y = 0};
}

static inline point_t point_y(point_t p)
{
    return (point_t){.X = 0, .Y = p.Y};
}

typedef PACKED(union) {
    struct
    {
        int X;
        int Y;
        int width;
        int height;
    };

    struct
    {
        point_t position;
        point_t size;
    };
} rectangle_t;

static inline rectangle_t rectangle_child(rectangle_t rectangle, rectangle_t child_rectangle)
{
    int x = max(rectangle.X + child_rectangle.X, rectangle.X);
    int y = max(rectangle.Y + child_rectangle.Y, rectangle.Y);

    int width = min(rectangle.X + child_rectangle.X + child_rectangle.width, rectangle.X + rectangle.width) - x;
    int height = min(rectangle.Y + child_rectangle.Y + child_rectangle.height, rectangle.Y + rectangle.height) - y;

    return (rectangle_t){x, y, width, height};
}

static inline bool rectangle_containe_position(rectangle_t rectange, point_t position)
{
    return (rectange.X <= position.X && (rectange.X + rectange.width)  > position.X) &&
           (rectange.Y <= position.Y && (rectange.Y + rectange.height) > position.Y);
}