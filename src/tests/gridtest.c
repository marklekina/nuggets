#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include "tester.h"
#include "mem.h"
#include "file.h"
#include "point.h"
#include "grid.h"

int
main(const int argc, char* argv[]) {
  // initialize testing
  srand(strlen(argv[0]));
  printf("%s: running tests...\n", argv[0]);
  testerInit();

  // specify map file
  FILE* fp = fopen("../../maps/big.txt", "r");
  assert(fp);

  // get map information
  char* first_line = file_readLine(fp);
  rewind(fp);

  int ncols = strlen(first_line);
  int nrows = file_numLines(fp);
  mem_free(first_line);

  char* mapString = file_readFile(fp);
  assert(mapString[strlen(mapString) - 1] == '\n');  // ensure mapfile is newline-terminated
  int size = nrows * ncols;
  rewind(fp);

  // 1. grid_new; 2. build_grid
  grid_t* grid;
  TRY { grid = grid_new(fp); build_grid(grid); } ENDTRY;
  TEST_ASSERT(grid);

  // close map file
  fclose(fp);

  // 3. get_map_string
  TEST(strcmp(get_map_string(grid), mapString), 0);

  // 4. get_size
  TEST(get_size(grid), size);

  // 5. get_nrows
  TEST(get_nrows(grid), nrows);

  // 6. get_ncols
  TEST(get_ncols(grid), ncols);

  // get grid points information
  // first gridpoint
  int first_str_idx = 0;
  int row_A = first_str_idx / (ncols + 1) + 1;
  int col_A = first_str_idx % (ncols + 1) + 1;
  point_t* gridpoint_A = point_new(row_A, col_A, mapString[first_str_idx]);

  // last gridpoint
  int last_str_idx = strlen(mapString) - 2;       // taking into account the closing newline
  int row_B = last_str_idx / (ncols + 1) + 1;
  int col_B = last_str_idx % (ncols + 1) + 1;
  point_t* gridpoint_B = point_new(row_B, col_B, mapString[last_str_idx]);

  // 7. get_gridpoint
  point_t* gridpoint_C = get_gridpoint(grid, row_B, col_B);
  TEST_ASSERT(gridpoint_C);

  TEST_ASSERT(is_same_location(get_gridpoint(grid, row_A, col_A), gridpoint_A));
  TEST_ASSERT(get_symbol(get_gridpoint(grid, row_A, col_A)) == get_symbol(gridpoint_A));

  TEST_ASSERT(is_same_location(get_gridpoint(grid, row_B, col_B), gridpoint_B));
  TEST_ASSERT(get_symbol(get_gridpoint(grid, row_B, col_B)) == get_symbol(gridpoint_B));

  // 8. get_gridpoints
  int first_grid_idx = 0;
  int last_grid_idx = size - 1;
  point_t** gridPoints;

  TRY { gridPoints = get_gridpoints(grid); } ENDTRY;
  TEST_ASSERT(is_same_location(gridPoints[first_grid_idx], gridpoint_A));
  TEST_ASSERT(get_symbol(gridPoints[first_grid_idx]) == get_symbol(gridpoint_A));

  TEST_ASSERT(is_same_location(gridPoints[last_grid_idx], gridpoint_B));
  TEST_ASSERT(get_symbol(gridPoints[last_grid_idx]) == get_symbol(gridpoint_B));

  // reconstruct map string from gridpoints array
  char* mapString_copy = mem_malloc(sizeof(char) * strlen(mapString) + 1);
  int i, idx = 0;

  // loop through gridpoints array and append symbols to mapString_copy
  for (i = 0; i < size; i++) {
    mapString_copy[idx] = get_symbol(gridPoints[i]);
    idx += 1;

    // append newline at the end of each row
    if ((i + 1) % ncols == 0) {
      mapString_copy[idx] = '\n';
      idx += 1;
    }
  }

  // ensure string is null terminated
  mapString_copy[strlen(mapString)] = '\0';

  // compare reconstructed copy with original mapstring
  TEST(strcmp(mapString_copy, mapString), 0);
  TEST(strlen(mapString_copy), (int) strlen(mapString));

  // 9. compute_visibility
  // same row visible
  point_t* A = get_gridpoint(grid, 15, 110);
  point_t* B = get_gridpoint(grid, 15, 88);
  TEST_ASSERT(compute_visibility(grid, A, B));

  // same column visible
  point_t* C = get_gridpoint(grid, 20, 37);
  point_t* D = get_gridpoint(grid, 15, 37);
  TEST_ASSERT(compute_visibility(grid, C, D));

  // different row & column visible
  point_t* E = get_gridpoint(grid, 36, 56);
  point_t* F = get_gridpoint(grid, 37, 62);
  TEST_ASSERT(compute_visibility(grid, E, F));

  point_t* G = get_gridpoint(grid, 16, 140);
  point_t* H = get_gridpoint(grid, 13, 135);
  TEST_ASSERT(compute_visibility(grid, G, H));

  // different row & column invisible
  point_t* I = get_gridpoint(grid, 18, 10);
  point_t* J = get_gridpoint(grid, 20, 32);
  TEST_ASSERT(!compute_visibility(grid, I, J));

  // same row invisible
  point_t* K = get_gridpoint(grid, 15, 117);
  point_t* L = get_gridpoint(grid, 15, 64);
  TEST_ASSERT(!compute_visibility(grid, K, L));

  // same column invisible
  point_t* M = get_gridpoint(grid, 13, 142);
  point_t* N = get_gridpoint(grid, 35, 142);
  TEST_ASSERT(!compute_visibility(grid, M, N));

  // different rooms invisible
  point_t* O = get_gridpoint(grid, 37, 110);
  point_t* P = get_gridpoint(grid, 10, 47);
  TEST_ASSERT(!compute_visibility(grid, O, P));

  // 10. grid_delete
  TRY { grid_delete(grid); } ENDTRY;

  // clean-up
  point_delete(gridpoint_A);
  point_delete(gridpoint_B);

  // free map strings
  mem_free(mapString_copy);
  mem_free(mapString);

  // complete testing
  testerReport(stdout, argv[0]);

  // print memory report if net malloc-free count is unbalanced
  if (mem_net() > 0) {
    mem_report(stderr, argv[0]);
  }

  printf("%s: testing complete\n", argv[0]);
  return testerResult();
}
