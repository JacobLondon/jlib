#ifndef JLIB_DRAW_H
#define JLIB_DRAW_H

// just compile
#ifndef Color
#define Color int
#endif

/**
 * Draw a filled triangle using scanline drawing by
 * providing 3 points in 2D space and a color.
 * 
 * Step 1)
 *     *
 *    *   *
 *   *       *
 *  *           *
 * * * * * * * * * *
 * 
 * Step 2)
 *     *
 *    * * *
 *   *       *
 *  *           *
 * * * * * * * * * *
 * 
 * Step 3)
 *     *
 *    * * *
 *   * * * * *
 *  *           *
 * * * * * * * * * *
 * 
 * Step 4)
 *     *
 *    * * *
 *   * * * * *
 *  * * * * * * *
 * * * * * * * * * *
 * 
 * Done!
 */
void draw_triangle(Color c, int x1, int y1, int x2, int y2, int x3, int y3);

#endif /* JLIB_DRAW_H */
