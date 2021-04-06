#ifndef __GETLINE_H__
#define __GETLINE_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parse_info_struc.h"

#define OK              0
#define ERR_GETLINE     (-1)

#define CALLOC_CONST    1000

typedef int err_t;

ssize_t cgetline(char **const linep, size_t *const linelenp, FILE *const stream);

#endif  // __GETLINE_H__
