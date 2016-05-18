#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdlib.h>
#include <stdio.h>

#define DICTIONARY_DECLARE(key_type, value_type, name) \
    typedef struct name name; \
                              \
    typedef struct{ \
        key_type key; \
        value_type value; \
    } name ## _entry ; \
                       \
    struct name{ \
        size_t entry_size; \
        size_t length; \
        name ## _entry * begin; \
        name ## _entry * end; \
        int (*cmp) (const key_type, const key_type); \
        int (*insert) (name *, key_type, value_type); \
        value_type * (*get) (name*, key_type); \
    };

#define DICTIONARY_DEFINE(key_type, value_type, name) \
\
\
value_type * name ## _get(name * _dic, key_type _key){ \
    name ## _entry *p; \
    for(p = _dic -> begin; p != _dic -> end; ++p) \
        if( (_dic -> cmp) (_key, p -> key) == 0 ) \
            return &(p -> value); \
    return NULL; \
} \
\
int name ## _remove(name * _dic, key_type _key){ \
    name ## _entry *p, tmp; \
    for(p = _dic -> begin; p != _dic -> end; ++p) \
        if( (_dic -> cmp) (_key, p -> key) == 0) \
            break; \
    if( p == _dic -> end) \
        return 1; \
    --(_dic -> length); \
    tmp.key = p -> key; \
    tmp.value = p -> value; \
    _dic -> begin = realloc( _dic -> begin, (_dic -> entry_size) * (_dic -> length + 1) ); \
    for(p = _dic -> begin; p != _dic -> end; ++p) \
        if( (_dic -> cmp) (_key, p -> key) == 0){ \
            p -> key = tmp.key; \
            p -> value = tmp.value; \
            break; \
        } \
    return (_dic -> begin) != NULL ? 0 : 1; \
}\
\
int name ## _insert(name * _dic, key_type _key, value_type _value){ \
    value_type * v; \
    if( ( v = (_dic -> get) (_dic, _key) ) != NULL ){ \
        *v = _value; \
        return 1; \
    } \
    (_dic -> end) -> key = _key; \
    (_dic -> end) -> value = _value; \
    ++(_dic -> length); \
    _dic -> begin = realloc(_dic -> begin, (_dic -> entry_size) * (_dic -> length + 1) ); \
    _dic -> end = _dic -> begin + _dic -> length; \
    return (_dic -> begin) != NULL ? 0 : 1; \
} \
\
int name ## _init(name * _dic, int (*_cmp) (const key_type, const key_type) ){ \
    _dic -> entry_size = sizeof(name ## _entry); \
    _dic -> length = 0; \
    _dic -> begin = malloc( _dic -> entry_size ); \
    _dic -> end = _dic -> begin; \
    _dic -> cmp = _cmp; \
    _dic -> get = name ## _get; \
    _dic -> insert = name ## _insert; \
    return (_dic -> begin) != NULL ? 0 : 1; \
}
#endif
