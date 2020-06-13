#include <jlib/draw.h>

#define SetColor(renderer, r, g, b, a)
#define DrawLine(renderer, x1, y1, x2, y2)

void draw_triangle(Color c, int x1, int y1, int x2, int y2, int x3, int y3)
{
    SetColor(renderer, c.r, c.g, c.b, c.a);

    // https://www.youtube.com/watch?v=PahbNFypubE ~10:30

    // find the top point
    int y, t;

    // sort points
    if (y2 < y1) {
        t = x1; x1 = x2; x2 = t;
        t = y1; y1 = y2; y2 = t;
    }
    if (y3 < y1) {
        t = x1; x1 = x3; x3 = t;
        t = y1; y1 = y3; y3 = t;
    }
    if (y3 < y2) {
        t = x2; x2 = x3; x3 = t;
        t = y2; y2 = y3; y3 = t;
    }

    // triangle has no area
    if (y1 == y3) return;

    if (y1 < y2) {
        for (y = y1; y < y2; y++) {
            DrawLine(renderer, x1 + (x2 - x1) * (y - y1) / (y2 - y1), y, x1 + (x3 - x1) * (y - y1) / (y3 - y1), y);
        }
    }
    if (y2 < y3) {
        for (y = y2; y < y3; y++) {
            DrawLine(renderer, x2 + (x3 - x2) * (y - y2) / (y3 - y2), y, x1 + (x3 - x1) * (y - y1) / (y3 - y1), y);
        }
    }
}
