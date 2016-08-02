#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdlib.h>
#include <stdio.h>

#define DICTIONARY_DECLARE(key_type, value_type, name) \
    typedef struct{ \
        key_type key; \
        value_type value; \
    } name ## _entry ; \
    typedef name ## _entry * name ## _itr;\
                       \
    typedef struct name name; \
    struct name{ \
        size_t entry_size; \
        size_t length; \
        name ## _entry * begin; \
        name ## _entry * end; \
        int (*cmp) (const key_type, const key_type); \
        name ## _entry * (*insert) (name *, key_type, value_type); \
        name ## _entry * (*remove) (name *, key_type); \
        value_type * (*get) (name*, key_type); \
        void (*destroy) (name *); \
    }; \
    \
    name ## _entry * name ## _init(name * _dic, int (*cmp) (const key_type, const key_type) ); 


#define DICTIONARY_DEFINE(key_type, value_type, name) \
\
\
value_type * name ## _get(name * _dic, key_type _key){ \
    name ## _entry *p; \
    for(p = _dic -> begin; p != _dic -> end; ++p) \
        if( (_dic -> cmp) (_key, p -> key) == 0 ) \
            return &(p -> value); \
    fprintf(stderr, "Could'n find entry\n"); \
    return NULL; \
} \
\
name ## _entry * name ## _remove(name * _dic, key_type _key){ \
    name ## _entry *p, *i; \
    for(p = _dic -> begin; p != _dic -> end; ++p) \
        if( (_dic -> cmp) (_key, p -> key) == 0 ) \
            break; \
    if( p == _dic -> end ) \
        return 0; /* key not found */ \
    for(i = _dic -> begin; i != p; ++i); \
    for(; i != _dic -> end; ++i) \
        *i = *(i+1); \
    --(_dic -> length); \
    _dic -> begin = realloc(_dic -> begin, (_dic -> entry_size) * (_dic -> length + 1) ); \
    _dic -> end = _dic -> begin + _dic -> length; \
    return _dic -> begin; \
}\
\
name ## _entry * name ## _insert(name * _dic, key_type _key, value_type _value){ \
    name ## _entry *p; \
    for(p = _dic -> begin; p != _dic -> end; ++p) \
        if( (_dic -> cmp) (_key, p -> key) == 0 ){ \
            p -> value = _value; \
            return p; \
        } \
    (_dic -> end) -> key = _key; \
    (_dic -> end) -> value = _value; \
    ++(_dic -> length); \
    _dic -> begin = realloc(_dic -> begin, (_dic -> entry_size) * (_dic -> length + 1) ); \
    _dic -> end = _dic -> begin + _dic -> length; \
    return _dic -> begin; \
} \
\
void name ## _destroy(name * _dic){ \
    _dic -> length = 0; \
    free(_dic -> begin); \
    _dic -> end = NULL; \
} \
\
name ## _entry * name ## _init(name * _dic, int (*cmp) (const key_type, const key_type) ){ \
    _dic -> entry_size = sizeof(name ## _entry); \
    _dic -> length = 0; \
    _dic -> begin = malloc( _dic -> entry_size ); \
    _dic -> end = _dic -> begin; \
    _dic -> cmp = cmp; \
    _dic -> get = name ## _get; \
    _dic -> insert = name ## _insert; \
    _dic -> remove = name ## _remove; \
    _dic -> destroy = name ## _destroy; \
    return _dic -> begin; \
}

#define DICTIONARY_LOCAL_DEFINE(key_type, value_type, name) \
    DICTIONARY_DECLARE(key_type, value_type, name) \
    DICTIONARY_DEFINE(key_type, value_type, name)
    
#endif
