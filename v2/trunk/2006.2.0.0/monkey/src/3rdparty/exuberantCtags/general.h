#ifndef  _GENERAL_H
#define  _GENERAL_H
#include "string.h"

# define errout	stderr
# define __unused__  __attribute__((unused))
#  define strcasecmp(s1,s2) strcmp(s1,s2)  //possible source of bugs!!!!


#include <stdlib.h>
#include "ctype.h" //need for parsers

#ifndef __cplusplus
typedef enum { FALSE =0, TRUE =1 } boolean;
#else
typedef enum { BFALSE =0, BTRUE =1 } boolean;
#endif


#endif  /* _GENERAL_H */
