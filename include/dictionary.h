#ifndef DICTIONARY_H
#define DICTIONARY_H

# include <stdlib.h>

#define DICTIONARY_DECLARE(key_type, value_type, name) \
    typedef struct name ## _entry name ## _entry; \
    typedef struct name name; \
                              \
    struct name ## _entry{ \
        key_type key; \
        value_type value; \
    }; \
               \
    struct name{ \
        size_t entry_size; \
        size_t length; \
        name ## _entry * begin; \
        name ## _entry * end; \
        int (*insert) (name *, key_type key, value_type value); \
    };

#define DICTIONARY_DEFINE(key_type, value_type, name) \
    int name ## _insert(name * dic, key_type key, value_type value){ \
        (dic -> end).key = key; \
        (dic -> end).value = value; \
        ++(dic -> lenght) \
        dic -> begin = realloc(dic -> begin, (dic -> lenght) * (dic -> entry_size) ); \
        dic -> end = dic -> begin + dic -> lenght; \
        return (dic -> begin) != NULL ? 1 : 0; \
    } \
    int name ## _init(name * dic){ \
        dic -> entry_size = sizeof(name ## _entry); \
        dic -> length = 1; \
        dic -> begin = malloc( dic -> entry_size ); \
        dic -> end = dic -> begin; \
        return (dic -> begin) != NULL ? 0 : 1; \
    }
#endif
