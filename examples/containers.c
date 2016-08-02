// \begin{verbatim}
/*---- Header file for containers.c ----*/
#include <vector.h>
#include <dictionary.h>
#include <stdio.h>

VECTOR_DECLARE(int, int_vec);
DICTIONARY_DECLARE(char, int_vec *, series_dic);

/*---- Source file for containers.c ----*/
VECTOR_DEFINE(int, int_vec);
DICTIONARY_DEFINE(char, int_vec *, series_dic);

int comp_char(char a, char b){
    return a - b;
}

int main(int argc, char * argv[]){
    int i, f;
    series_dic_itr s_itr;
    int_vec_itr int_itr;
    // Crea una variable series_dic e inicializa el dicccionario 
    series_dic s;
    series_dic_init( &s, comp_char);
    // Crea una variable int_vec y inicialize el vector
    int_vec fibonacci; 
    int_vec_init( &fibonacci, 10);

    fibonacci.append( &fibonacci, 0);
    fibonacci.append( &fibonacci, 1);
    for(i = 1; i < 45; ++i){
        f = fibonacci.begin[i-1] + fibonacci.begin[i];
        fibonacci.append( &fibonacci, f);
    }
    s.insert( &s, 'f', &fibonacci);

    printf("Length of fibonacci %ld\n", (*s.get( &s, 'f')) -> length);
    for(s_itr = s.begin; s_itr != s.end; ++s_itr){
        for(int_itr = s_itr -> value -> begin; 
              int_itr != s_itr -> value -> end; ++int_itr)
            printf(" %d ", *int_itr);
        printf("\n");
    }
    return 0;
} // \end{verbatim}
