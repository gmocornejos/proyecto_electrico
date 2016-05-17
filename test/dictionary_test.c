#include <stdio.h>

#include <dictionary.h>

DICTIONARY_DECLARE(char *, double, dic_type);
DICTIONARY_DEFINE(char *, double, dic_type);

int main(int argc, char * argv[]){

    dic_type dic_test;

    dic_type_init( & dic_test );
    
    return 0;
}
