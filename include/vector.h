#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

#define VECTOR_DEFINE(type, name) \
    typedef struct name name; \
                              \
    struct name{ \
        size_t type_size; \
        size_t length; \
        size_t capacity; \
        type * begin; \
        type * end; \
        int (*append) (name *, type); \
        int (*pop) (name *); \
        type * (*search) (name *, type, int (*comp)(type, type) );\
        void (*destroy) (name *); \
    }; \
    \
    int name ## _append(name * vector, type value){ \
        *(vector -> end)++ = value; \
        ++(vector -> length); \
        if( (vector -> length) == (vector -> capacity) ){ \
            vector -> capacity *= 1.2; \
            vector -> begin = realloc(vector -> begin, (vector -> type_size) * (vector -> capacity) ); \
            vector -> end = vector -> begin + vector -> length; \
        } \
        return (vector -> begin) != NULL ? 1 : 0; \
    } \
    \
    int name ## _pop(name * vector){ \
        --(vector -> end); \
        --(vector -> length); \
        if( (vector -> length) < 0.8 * (vector -> capacity) ){ \
            vector -> capacity *= 0.8; \
            vector -> begin = realloc(vector -> begin, (vector -> type_size) * (vector -> capacity) ); \
            vector -> end = vector -> begin + vector -> length; \
        } \
        return (vector -> begin) != NULL ? 1 : 0; \
    } \
    \
    type * name ## _search(name * vector, type value, int (*comp) (type, type) ){ \
        type * i; \
        for(i = vector -> begin; i != vector -> end; ++i) \
            if( comp(*i, value) ) \
                return i; \
        return NULL; \
    } \
    \
    void name ## _destroy(name * vector){\
        vector -> length = 0; \
        vector -> capacity = 0; \
        free(vector -> begin); \
        vector -> end = NULL; \
    } \
    \
    int name ## _init(name * vector, int size){ \
        vector -> type_size = sizeof(type); \
        vector -> length = 0; \
        vector -> capacity = size; \
        vector -> begin = malloc(size * (vector -> type_size) ); \
        vector -> end = vector -> begin; \
        vector -> append = name ## _append; \
        vector -> pop = name ## _pop; \
        vector -> search = name ## _search; \
        vector -> destroy = name ## _destroy; \
        return (vector -> begin) != NULL ? 1 : 0; \
    } 

#endif
