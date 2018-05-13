/*
  Scott Little
  CPSC 3500
  Homework 2
*/

#include <pthread.h>
#include <unistd.h>
#include <stdexcept>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 256
#define NUMBER_OF_FILES 20
#define NUMBER_OF_THREADS 20

static const char*
FILE_NAME[20] = {
"/file1.txt",
"/file2.txt",
"/file3.txt",
"/file4.txt",
"/file5.txt",
"/file6.txt",
"/file7.txt",
"/file8.txt",
"/file9.txt",
"/file10.txt",
"/file11.txt",
"/file12.txt",
"/file13.txt",
"/file14.txt",
"/file15.txt",
"/file16.txt",
"//file17.txt",
"/file18.txt",
"/file19.txt",
"/file20.txt"
};

static char file_location[BUFFER_SIZE];

typedef struct vowels
{
  int a = 0;
  int e = 0;
  int i = 0;
  int o = 0;
  int u = 0;
}vowels;

typedef struct args_t
{
  int tid;
  const char* file_name;
}args_t;

pthread_t threads[NUMBER_OF_THREADS];

// closes the file
inline static void
close_file(FILE* file_ptr)
{
  fclose(file_ptr);
}

// reads file, counts the number of values
inline static void*
find_vowls(void* thread_args)
{
  char c;
  const char* file_name = ((args_t*)thread_args)->file_name;
  delete (args_t*)thread_args;
  vowels* p_vowels = new vowels;
  FILE* fp;

  if ((fp = fopen(file_name, "r")) == NULL)
    throw std::invalid_argument("File does not exist.\n");

  do
    {
      c = tolower(getc(fp));
      switch (c)
        {
        case 'a': ++(p_vowels->a); break;
        case 'e': ++(p_vowels->e); break;
        case 'i': ++(p_vowels->i); break;
        case 'o': ++(p_vowels->o); break;
        case 'u': ++(p_vowels->u); break;
        default: break;
        }
    }
  while (c != EOF);

  close_file(fp);

  return (void*)p_vowels;
}

int main()
{
  vowels vowel_counter;
  vowels* counter;

  for (int i = 0; i < NUMBER_OF_THREADS; i++)
    {
      args_t* thread_args = new args_t;
      thread_args->tid = i;
      thread_args->file_name = FILE_NAME[i];

      if (pthread_create(&threads[i], NULL, find_vowls, (void*)thread_args))
        throw std::invalid_argument("pthread_create returned an error.\n");
    }

  for (int i = 0; i < NUMBER_OF_THREADS; i++)
    {
      if (pthread_join(threads[i], (void**)&counter))
        throw std::invalid_argument("pthread_join returned an error.\n");
      vowel_counter.a += counter->a;
      vowel_counter.e += counter->e;
      vowel_counter.i += counter->i;
      vowel_counter.o += counter->o;
      vowel_counter.u += counter->u;
      delete counter;
    }

  printf("Number of A's: %i\n", vowel_counter.a);
  printf("Number of E's: %i\n", vowel_counter.e);
  printf("Number of I's: %i\n", vowel_counter.i);
  printf("Number of O's: %i\n", vowel_counter.o);
  printf("Number of U's: %i\n", vowel_counter.u);

  return 0;
}
