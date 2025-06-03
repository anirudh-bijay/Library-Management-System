/**
 * @file terminal_colours.h
 * Macros that expand to extended-ASCII escape sequences to
 * obtain coloured output on the console/terminal.
 * 
 * @ref https://stackoverflow.com/a/51947317
 * @ref https://en.wikipedia.org/wiki/ANSI_escape_code#SGR_(Select_Graphic_Rendition)_parameters
 * 
 * @example
 * ```c
 *  #include "terminal_colours.h"
 *  #include <stdio.h>
 * 
 *  int main(int argc, char const *argv[])
 *  {
 *      printf(ANSI_COLOR_RED     "This text is RED!"     ANSI_COLOR_RESET "\n");
 *      printf(ANSI_COLOR_GREEN   "This text is GREEN!"   ANSI_COLOR_RESET "\n");
 *      printf(ANSI_COLOR_YELLOW  "This text is YELLOW!"  ANSI_COLOR_RESET "\n");
 *      printf(ANSI_COLOR_BLUE    "This text is BLUE!"    ANSI_COLOR_RESET "\n");
 *      printf(ANSI_COLOR_MAGENTA "This text is MAGENTA!" ANSI_COLOR_RESET "\n");
 *      printf(ANSI_COLOR_CYAN    "This text is CYAN!"    ANSI_COLOR_RESET "\n");
 *
 *      return 0;
 *  }
 * ```
 */

#pragma once

#define ANSI_COLOR_RED     "\x1b[31m"   /** @brief ANSI SGR escape code for red foreground colour. */
#define ANSI_COLOR_GREEN   "\x1b[32m"   /** @brief ANSI SGR escape code for green foreground colour. */
#define ANSI_COLOR_YELLOW  "\x1b[33m"   /** @brief ANSI SGR escape code for yellow foreground colour. */
#define ANSI_COLOR_BLUE    "\x1b[34m"   /** @brief ANSI SGR escape code for blue foreground colour. */
#define ANSI_COLOR_MAGENTA "\x1b[35m"   /** @brief ANSI SGR escape code for magenta foreground colour. */
#define ANSI_COLOR_CYAN    "\x1b[36m"   /** @brief ANSI SGR escape code for cyan foreground colour. */
#define ANSI_COLOR_RESET   "\x1b[0m"    /** @brief ANSI SGR escape code to reset foreground colour to default. */