#ifndef STATUS_CODES_H
#define STATUS_CODES_H

typedef enum {
    STATUS_OK = 0,
    INVALID_INPUT, 
    MEMORY_ERROR,
    READ_ERROR,
    WRITE_ERROR,
    OPEN_ERROR,

} StatusCode;

#endif