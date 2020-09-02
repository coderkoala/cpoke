/* -*- mode: c; -*- */

/* ========================================================================= */

#include <assert.h>
#include "util/files.h"
#include "util/json_util.h"
#include "ext/jsmn.h"
#include <limits.h>
#include <math.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>

/* ------------------------------------------------------------------------- */

jsmnerr_t
jsmn_parse_realloc( jsmn_parser_t *  parser,
                    const char    *  js,
                    size_t           len,
                    jsmntok_t     ** tokens,
                    size_t        *  num_tokens
                  ) {
  size_t tmp_numtok = 128;
  jsmnerr_t r;

  if ( ( * tokens ) == NULL ) {
    num_tokens = &tmp_numtok;
    ( * tokens ) = malloc( ( * num_tokens ) * sizeof( ( * tokens )[0] ) );
    if ( ( * tokens ) == NULL ) return JSMN_ERROR_NOMEM;
  }

  while( true ) {  /* break inside */
    jsmn_init( parser );
    r = jsmn_parse(parser, js, len, *tokens, *num_tokens);
    if ( r == JSMN_ERROR_NOMEM ) {
      unsigned int want_num_tokens = 2 * ( * num_tokens );
      jsmntok_t *  newtok = realloc( ( * tokens ),
                                     want_num_tokens * sizeof( ( * tokens )[0] )
                                   );
      if ( newtok == NULL ) break;
      ( * num_tokens ) = want_num_tokens;
      ( * tokens )     = newtok;
    } else {
      break;
    }
  }
  return r;
}


/* ------------------------------------------------------------------------- */

void
free_jsmn_file_parser( jsmn_file_parser_t * parser ) {
  if ( parser == NULL ) return;

  free( parser->buffer );
  parser->buffer = NULL;

  free( parser->fpath );
  parser->fpath = NULL;

  free( parser->tokens );
  parser->tokens = NULL;

  parser->buffer_len = 0;
  parser->tokens_cnt = 0;

  jsmn_init( &( parser->jparser ) );
}


/* ------------------------------------------------------------------------- */

size_t
jsmn_file_parser_init( const char * fpath, jsmn_file_parser_t * f_parser ) {
  assert( fpath != NULL );
  assert( strlen( fpath ) != 0 );
  assert( f_parser != NULL );

  /* Set sane defaults for parser */
  f_parser->fpath      = NULL; /* Wait until file is read to set actual. */
  f_parser->buffer     = NULL;
  f_parser->buffer_len = 0;
  f_parser->tokens     = NULL;
  f_parser->tokens_cnt = 0;
  jsmn_init( &( f_parser->jparser ) );

  /* Read file into buffer. `fread_malloc' handles allocation. */
  f_parser->buffer_len = fread_malloc( fpath, &( f_parser->buffer ) );
  if ( f_parser->buffer_len == 0 ) return 0;
  /* Copy fpath */
  f_parser->fpath = strdup( fpath );
  if ( f_parser->fpath == NULL ) {
    perror( __func__ );
    free( f_parser->buffer );
    f_parser->buffer_len = 0;
    return 0;
  }

  return f_parser->buffer_len;
}


/* ------------------------------------------------------------------------- */

size_t
toklen( const jsmntok_t * token ) {
  assert( token != NULL );
  assert( token->start <= token->end );
  assert( ( token->end - token->start ) <= UINT_MAX );
  return (size_t) token->end - token->start;
}


/* ------------------------------------------------------------------------- */

int
fprint_tok( FILE * stream, const char * json, const jsmntok_t * token ) {
  assert( json != NULL );
  assert( token != NULL );
  char r = '\0';
  int  i = 0;
  for ( const char * c = json + token->start;
        ( c < json + token->end ) && ( r != EOF ) && ( *c != '\0' );
        c++
        ) r = putc( *c, stream );
  return toklen( token );
}


int
print_tok( const char * json, const jsmntok_t * token ) {
  return fprint_tok( stdout, json, token );
}


/* ------------------------------------------------------------------------- */

bool
jsoneq( const char * json, const jsmntok_t * token, const char * str ) {
  assert( json != NULL );
  assert( token != NULL );
  assert( str != NULL );
  size_t token_len = toklen( token );
  return ( ( strlen( str ) == token_len )
           && ( strncmp( json + token->start, str, token_len ) == 0 )
         );
}


bool
jsoneq_str( const char * json, const jsmntok_t * token, const char * str ) {
  assert( token != NULL );
  return ( ( token->type == JSMN_STRING ) && jsoneq( json, token, str ) );
}


bool
jsoneq_int( const char * json, const jsmntok_t * token, const int i ) {
  size_t tok_len;
  char * buffer;
  int    val;
  assert( token != NULL );
  /* Confirm type */
  if ( token->type != JSMN_PRIMITIVE ) return false;
  tok_len = toklen( token );
  /* Compare length */
  if ( tok_len != ceil( log10( i ) ) ) return false;
  /* Copy and call `atoi' */
  buffer  = (char *) malloc( tok_len * sizeof( char ) );
  strncpy( buffer, json + token->start, tok_len );
  val = atoi( buffer );
  /* Because 0 is the fallback for `atoi', explicitly check for it. */
  if ( ( val == 0 ) && ( tok_len == 1 ) && ( buffer[0] == '0' ) ) return true;
  free( buffer );
  return val == i;
}


/* ------------------------------------------------------------------------- */

bool
jsonmatch_str( const char * json, const jsmntok_t * token, regex_t * regex ) {
  assert( json != NULL );
  assert( token != NULL );
  assert( regex != NULL );
  if ( token->type != JSMN_STRING ) return false;
  return ( regexec( regex, json + token->start, 0, NULL, 0 ) == 0 );
}


/* ------------------------------------------------------------------------- */



/* ========================================================================= */

/* vim: set filetype=c : */