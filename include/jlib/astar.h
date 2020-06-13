#ifndef JLIB_ASTAR_H
#define JLIB_ASTAR_H

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief
 *      Give the width and height of the walkable area.
 * @param width
 *      The width of the grid
 * @param height
 *      The height of the grid
 * @param check_obstacle_cb
 *      returns true if the item at i and j is an obstacle, or NOT walkable
 */
void astar_init(size_t width, size_t height, bool (*check_obstacle_cb)(int i, int j));
void astar_cleanup(void);

/**
 * Give an array of x and y to copy the calculated path IN REVERSE ORDER
 * size is the max you want to copy. This is problematic because if you
 * don't make size large enough, you can't get the next available node.
 * This is for safety, and will assert if size isn't big enough.
 * 
 * It returns the number of steps total there were, regardless of size
 * si/sj = start i and start j (y, x)
 * ei/ej = end i and end j (y, x)
 * 
 * Example:
 * 
 * #define SIZE 10
 * 
 * char grid[SIZE][SIZE] = {
 *     "          ",
 *     "######### ",
 *     "          ",
 *     " #########",
 *     "          ",
 *     "######### ",
 *     "          ",
 *     " #########",
 *     "          ",
 *     "######### ",
 * };
 * 
 * bool obstacle_cb(int i, int j)
 * {
 *     return grid[i][j] == '#';
 * }
 * 
 * int main(void)
 * {
 *     int xs[100] = { 0 }:
 *     int ys[100] = { 0 };
 *     size_t steps;
 *     int i, j;
 *     
 *     astar_init(SIZE, SIZE, obstacle_cb);
 *     
 *     // path find from top left corner to bottom right
 *     steps = astar_path(xs, ys, SIZE * SIZE, 0, 0, 9, 9);
 *     
 *     // add '.' as a step in 'grid'
 *     for (i = 0; i < steps; i++) {
 *         grid[ys[i]][xs[i]] = '.';
 *     }
 *     
 *     // print the grid with the found path
 *     for (i = 0; i < SIZE; i++) {
 *         for (j = 0; j < SIZE; j++) {
 *             printf("%c ", grid[i][j]);
 *         }
 *         printf("\n");
 *     }
 * }
 * 
 * // done
 * astar_cleanup();
 */
size_t astar_path(int *xs, int *ys, size_t size, int si, int sj, int ei, int ej);

#endif // JLIB_ASTAR_H
