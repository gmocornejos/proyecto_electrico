#include <mas.h>

#include <stdio.h>
#include <string.h>

DICTIONARY_DECLARE(char *, int, dic_type);
DICTIONARY_DEFINE(char *, int, dic_type);

int main(int argc, char * argv[]){

    char * names[] = {"cero", "uno", "dos", "tres", "cuatro", "cinco"};
    int i;
    dic_type my_dic;
    dic_type_entry *j;

    dic_type_init( &my_dic, strcmp);

    for(i = 0; i != 6; ++i)
        my_dic.insert(&my_dic, names[i], i);

    for(j = my_dic.begin; j != my_dic.end; ++j)
        printf("%d es %s\n", j -> value, j -> key);

    my_dic.insert(&my_dic, "cuatro", 9);
    printf("cuatro es %d\n", *(my_dic.get(&my_dic, "cuatro")) );

    my_dic.remove(&my_dic, "dos");

    for(j = my_dic.begin; j != my_dic.end; ++j)
        printf("%d es %s\n", j -> value, j -> key);

    return 0;
}
