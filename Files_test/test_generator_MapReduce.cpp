#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define MAX_WORDS 2048
#define NUM_FILES 2048

int main(int argc, char const *argv[])
{
    char words[][18] = {"oi",
                    "ocimar",
                    "ste",
                    "bem",
                    "legal",
                    "processador",
                    "trhead",
                    "filosofos",
                    "cpu",
                    "computador",
                    "sistema",
                    "operacional",
                    "so",
                    "usuario",
                    "nucleo",
                    "kernel",
                    "pipeline",
                    "escalonador"};

    int randon_num;
    int num_words;
    char* file_name;
    FILE* out_file;

    srand( (unsigned)time(NULL) );


    int words_counters[18] = {};
    for (int file_index = 0; file_index < NUM_FILES; ++file_index)
    {
        sprintf(file_name, "./test_MapReduce/generated_%d.txt", file_index+1);
        num_words = 0;
        out_file = fopen(file_name, "w");
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
        fclose(out_file);
    }
    return 0;
} 
