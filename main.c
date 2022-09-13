#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_LEN 32

/**
 * Global variables to store arguments passed by to user, and used by every function
 */
int matches = 0;
char* included_chars = NULL;
char* wordlist = NULL;
char* included_chars_missing;
char debug = 'f'; // n for oN f for oFF
int len = 0;

/**
 * prints the help message
 */
static void print_help(void)
{
	printf("Words_On_Stream Solver:\n"
		   "-h prints this message\n"
		   "-d enables debug printouts\n"
		   "-l [Path to file] input file\n"
		   "-c <chars> up to 32 chars that the words is composed of\n");
}

/**
 * Parses the user arguments and sets global variables
 * May also exit the program with error message if there is faulty input
 * @param argc number of arguments
 * @param argv vector with strings of arguments
 */
static void read_arguments(int argc, char* argv[])
{
	// Loop over every argument
	int i;
	for (i = 1; i < argc; i++)
	{
		// if it starts with an '-' it's an option and start checken wich one it is
		if (argv[i][0] == '-')
		{
			// -h
			if (argv[i][1] == 'h')
			{
				print_help();
				exit(EXIT_FAILURE);
			}
			// -d
			else if (argv[i][1] == 'd')
			{
				debug = 'n';
			}
			// -l
			else if (argv[i][1] == 'l')
			{
				i++;
				len = atoi(argv[i]);
			}
			// -c check also if there is a next entry and that it isn't an option
			else if (argv[i][1] == 'c' && i + 1 < argc && argv[i + 1][0] != '-')
			{
				i++;
				included_chars = argv[i];
			}
				// -w check also if there is a next entry and that it isn't an option
			else if (argv[i][1] == 'w' && i + 1 < argc && argv[i + 1][0] != '-')
			{
				i++;
				wordlist = argv[i];
			}
				// if no match was found print error message and exit
			else
			{
				fprintf(stderr, "Unknown parameter %s", argv[i]);
				exit(EXIT_FAILURE);
			}
		}
	}
	/**
	 * Checks the input after every necessary argument was parsed to allow random order
	 */
	// check if wordlist was passed
	if (wordlist == NULL)
	{
		fprintf(stderr, "missing wordlist\n");
		print_help();
		exit(EXIT_FAILURE);
	}
	// check if chars were passed
	if (included_chars == NULL)
	{
		fprintf(stderr, "missing chars\n");
		print_help();
		exit(EXIT_FAILURE);
	}
}

/**
 * checks if a given char in in the included list and replaces in in the list so no duplicates are a positive check
 * @param curr_char character to be tested
 * @return true if in the list, false otherwise
 */
bool is_included_char(const char* curr_char)
{
	int i = 0;
	while (included_chars_missing[i] != '\0')
	{
		if (included_chars_missing[i] == *curr_char)
		{
			// replace char in list and exit this function
			included_chars_missing[i] = '_';
			return true;
		}
		i++;
	}
	return false;
}

/**
 * Iterates over the wordlist and prints every word that has all the given chars
 */
void find_words(void)
{
	/**
	 * initial search
	 */

	FILE* wordlist_stream = fopen(wordlist, "r");
	char* current_word = calloc(MAX_WORD_LEN, sizeof(char));
	included_chars_missing = (char*) malloc(sizeof(char) * MAX_WORD_LEN);

	// check if file could be opened and memory for the word storage was allocated
	if (wordlist_stream == NULL)
	{
		fprintf(stderr, "Couldn't open Wordlist-file %s", wordlist);
		exit(EXIT_FAILURE);
	}
	if (current_word == NULL)
	{
		fprintf(stderr, "Couldn't allocate memory");
		exit(EXIT_FAILURE);
	}
	if (included_chars_missing == NULL)
	{
		fprintf(stderr, "Couldn't allocate memory");
		exit(EXIT_FAILURE);
	}

	/**
	* find words
	 */
	while (fgets(current_word, MAX_WORD_LEN, wordlist_stream))
	{
		strcpy(included_chars_missing, included_chars);

		int ccount = 0;
		int i = 0;
		while (current_word[i] != '\0')
		{
			if (is_included_char(&current_word[i]))
			{
				ccount++;
			}
			i++;
		}
		if (ccount == --i && ccount >= len)
		{
			fprintf(stdout, "Word found: %s", current_word);
			matches++;
		}

	}

	fclose(wordlist_stream);
}

int main(int argc, char* argv[])
{
	// start timer for total runtime
	clock_t prg_start = clock();

	// parse arguments
	read_arguments(argc, argv);

	// debug info
	if (debug == 'n')
	{
		printf("\nincluded_chars: %s\nword_list: %s\nminimum length: %d \n",
				included_chars, wordlist, len);
	}

	// find words
	find_words();

	// stop timer for total runtime
	clock_t prg_end = clock();
	// print total runtime timer
	printf("total time to find %d word(s): %.2f Seconds", matches, (float)(prg_end - prg_start) / CLOCKS_PER_SEC);

	return EXIT_SUCCESS;
}
