#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define MAX_WORDS 2048
#define NUM_FILES 1

int main(int argc, char const *argv[])
{
    char words[][18] = {"bem",
                    "computador",
                    "cpu",
                    "escalonador",
                    "filosofos",
                    "kernel",
                    "legal",
                    "nucleo",
                    "ocimar",
                    "oi",
                    "operacional",
                    "pipeline",
                    "processador",
                    "sistema",
                    "so",
                    "ste",
                    "trhead",
                    "usuario"};

    int randon_num;
    int num_words;
    char* file_name;

    FILE* out_file;
    FILE* log_file = fopen ("./test_num_words/log.txt", "w");;

    srand( (unsigned)time(NULL) );


    int words_counters[18] = {};
    for (int file_index = 0; file_index < NUM_FILES; ++file_index)
    {
        sprintf(file_name, "./test_num_words/generated_%d.txt", file_index+1);
        out_file = fopen (file_name, "w");
        num_words = 0;
        while (num_words < MAX_WORDS)
        {
            randon_num = rand()%18;
            
            fprintf(out_file, "%s", words[randon_num]);
            if (rand()%10 == 0)
                fprintf(out_file, "\n");
            else if (rand()%3 ==0)
                fprintf(out_file, ", ");
            else
                fprintf(out_file, " ");

            words_counters[randon_num]++;
            num_words++;
        }

        fprintf(log_file, "Quantidade de palavras no arquivo:\n");
        for (int i = 0; i < 18; ++i)
        {
            fprintf(log_file, "%11s: %d\n", words[i], words_counters[i]);
        }
        fclose(out_file);

        
    }

    fclose(log_file);
    return 0;
} 
