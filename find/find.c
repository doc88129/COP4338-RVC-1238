#include"find.h"
char*lineptr[MAX_LINES];
int readlines(){
	int len, nlines;
	char*p, line[MAX_LEN];
	nlines = 0;
	while(fgets(line, MAX_LEN, stdin)){
		len = strlen(line);
		if(nlines >= MAX_LINES || (p = malloc(len + 1)) == NULL)
			return -1;
		else{
			if(line[len - 1] == '\n')//droping the newline at the end...
				line[len - 1] = '\0';
			strcpy(p, line);
			lineptr[nlines++] = p;
		}
	}
	return nlines;
}
void error(int error_code){
	if (error_code == 5) {
		printf("fatal error: can't use -p and -x in the same execution\n");
	} else {
		printf("find: fatal error - Illegal usage. Error code: %d. Usage: \"%s\"\n",
	error_code, "find [-n] [-x] [-s] [-r] [-m] [-c] [-f] [-p] pattern");
	}
	exit(error_code);
}
flags set_flags(int argc, char** argv){
	flags option = 0;//By default, all flags are down!
	for(int i = 1; i < argc - 1;i++){
		if(argv[i][0] != '-')//if argument doesn't start with -
			error(2);
		for(int j = 1; argv[i][j] != '\0';j++)
			switch(argv[i][j]){
				case 'n':case 'N':
				option |= NUMBERED;
				break;
				case 'x':case 'X':
				option |= EXCEPT;
				break;
				case 's':case 'S':
				option |= SORTED;
				break;
				case 'r':case 'R':
				option |= REVERSED;
				break;
				case 'm':case 'M':
				option |= MATCHED;
				break;
				case 'c':case 'C':
				option |= CASE;
				break;
				case 'f':case 'F':
				option |= FIRST;
				break;
				case 'p':case 'P':
				option |= PARTIAL;
				break;
				default: 
				error(3);
			}
	}
	return option;
}
/*
* Program find is used to find the occurrences of a given pattern in stdin
* Assumption is that stdin is made of multiple lines (no more than 1000)
* Program tries finding the pattern in each line.
* The pattern is given to the program as the last command-line argument
* There are multiple options for running this program, all passed in CLAs
* Usage: find [-n] [-x] [-s] [-r] [-m] [-c] [-f] [-p] pattern

* -f: prints out the index of first occurrence of pattern found in each line - ?
* -p: partially prints out the input lines. Uses ellipsis to shorten lines. - TODO

******e.g. if we look for "apple" in "my favorite dessert is apple pie with coffee"
****** then the output is "my favorit...apple...offee"
*/
int main(int argc, char** argv)
{
	if(argc < 2)
		error(1);
	char* pattern = strdup(argv[argc - 1]);// the last CLA is considered to be the pattern
	flags option = set_flags(argc, argv);
	if((option & REVERSED) && (option & SORTED))
		error(4);//cannot print the output using both sorted and reversed options...
	if((option & PARTIAL) && (option & EXCEPT))
		error(5);

	int nlines = readlines();

	if(option & SORTED)
		quicksort(lineptr, 0, nlines - 1);

	char initial[10] = "";

	if(option & REVERSED)
		for(int i = nlines - 1; i >= 0;i--)//last in first out
			print_out(i, initial, pattern, lineptr, option);
	else
		for(int i = 0; i < nlines;i++)//first in first out
			print_out(i, initial, pattern, lineptr, option);
    return 0;
}

#define FIRST_MAX 10
#define LAST_MAX 5

char* partial_line(char* line, char* first_occurrence, char* pattern, flags option){
	if (!(option & PARTIAL) || strlen(line) < 15 + strlen(pattern))
		return line;
	int occurence_index = strlen(line) - strlen(first_occurrence) - strlen(pattern);
	char buffer [100];

	if (occurence_index <= FIRST_MAX) {
		sprintf(buffer, "%.*s...%.*s", occurence_index, line, LAST_MAX, line + strlen(line) - LAST_MAX);
		return strdup(buffer);
	} else if (strlen(line) - LAST_MAX - strlen(pattern) <= occurence_index + strlen(pattern)) {
		sprintf(buffer, "%.*s...%.*s", FIRST_MAX, line, strlen(line) - occurence_index, first_occurrence);
		return strdup(buffer);
	} else {
		sprintf(buffer, "%.*s...%.*s...%.*s", FIRST_MAX, line, strlen(pattern), first_occurrence, LAST_MAX, line + strlen(line) - LAST_MAX);
		return strdup(buffer);
	}
	
	return line;
}

void print_out(int i, char* initial, char* pattern, char* lineptr[], flags option){
	if(option & NUMBERED)
		sprintf(initial, "%d. ", i + 1);
	//search for pattern in current line
	char* first_occurrence = strstr_w_option(lineptr[i], pattern, option);
	//if except flag is up and pattern cannot be found in current line
	//or if except flag is down and pattern can be found in the current line
	if(((option & EXCEPT) != 0) != (first_occurrence != NULL)) {
		char* partial = partial_line(lineptr[i], first_occurrence, pattern, option);
		if (option & FIRST)
			printf("%s@%d: %s\n",initial , strlen(lineptr[i]) - strlen(first_occurrence), partial);
		else
			printf("%s%s\n", initial, partial);//print the line w/ number if numbered flag is raised
	}
}
