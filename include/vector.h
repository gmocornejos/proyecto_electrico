#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

#define VECTOR_DECLARE(type, name) \
    typedef type * name ## _itr ; \
    typedef struct name name; \
                              \
    struct name{ \
        size_t type_size; \
        size_t length; \
        size_t capacity; \
        type * begin; \
        type * end; \
        type * (*append) (name *, type); \
        type * (*pop) (name *); \
        type * (*last) (name *); \
        type * (*search) (name *, type, int (*comp)(type, type) );\
        void (*clean) (name *); \
        void (*destroy) (name *); \
    }; \
    \
    type * name ## _init(name * _vec, int size); \


#define VECTOR_DEFINE(type, name) \
\
type * name ## _append(name * _vec, type value){ \
    *(_vec -> end)++ = value; \
    ++(_vec -> length); \
    if( (_vec -> length) == (_vec -> capacity) ){ \
        ++(_vec -> capacity); \
        _vec -> capacity *= 1.2; \
        _vec -> begin = realloc(_vec -> begin, (_vec -> type_size) * (_vec -> capacity) ); \
        _vec -> end = _vec -> begin + _vec -> length; \
    } \
    return _vec -> begin; \
} \
\
type * name ## _pop(name * _vec){ \
    if( _vec -> length == 0 ) \
        return _vec -> begin; \
    --(_vec -> end); \
    --(_vec -> length); \
    if( 1.2 * (_vec -> length) < _vec -> capacity ){ \
        _vec -> capacity = 1.2 * (_vec -> length + 1); \
        _vec -> begin = realloc(_vec -> begin, (_vec -> type_size) * (_vec -> capacity) ); \
        _vec -> end = _vec -> begin + _vec -> length; \
    } \
    return _vec -> begin; \
} \
\
type * name ## _last(name * _vec){ \
    return (_vec -> end) - 1; \
} \
\
type * name ## _search(name * _vec, type value, int (*comp) (type, type) ){ \
    type * i; \
    for(i = _vec -> begin; i != _vec -> end; ++i) \
        if( comp(*i, value) ) \
            return i; \
    return NULL; \
} \
\
void name ## _clean(name * _vec){ \
    _vec -> length = 0; \
    _vec -> end = _vec -> begin; \
} \
\
void name ## _destroy(name * _vec){ \
    _vec -> length = 0; \
    _vec -> capacity = 0; \
    free(_vec -> begin); \
    _vec -> end = NULL; \
} \
\
type * name ## _init(name * _vec, int size){ \
    size = size > 0 ? size : 1; \
    _vec -> type_size = sizeof(type); \
    _vec -> length = 0; \
    _vec -> capacity = size; \
    _vec -> begin = malloc(size * (_vec -> type_size) ); \
    _vec -> end = _vec -> begin; \
    _vec -> append = name ## _append; \
    _vec -> pop = name ## _pop; \
    _vec -> last = name ## _last; \
    _vec -> search = name ## _search; \
    _vec -> clean = name ## _clean; \
    _vec -> destroy = name ## _destroy; \
    return _vec -> begin; \
} 

#define VECTOR_LOCAL_DEFINE(type, name) \
VECTOR_DECLARE(type, name) \
VECTOR_DEFINE(type, name)

#endif 
