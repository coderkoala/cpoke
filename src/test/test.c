/* -*- mode: c; -*- */

/* ========================================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "test.h"
#include "util/test_util.h"

/* ------------------------------------------------------------------------- */

  bool
test_all( void )
{
  bool rsl = true;
  rsl &= do_test( json );
  rsl &= do_test( pokemon );
  rsl &= do_test( ptypes );
  rsl &= do_test( parse_gm );
  return rsl;
}


/* ------------------------------------------------------------------------- */

#ifdef MK_TEST_BINARY
  int
main( int argc, char * argv[], char ** envp )
{
  return test_all() ? EXIT_SUCCESS : EXIT_FAILURE;
}
#endif


/* ------------------------------------------------------------------------- */



/* ========================================================================= */

/* vim: set filetype=c : */
