#include "solver.h"
struct solver_args {
	char** sub_puzzle;
	int subpuzzle_rows;
	int subpuzzle_cols;
	int max_len;
	int min_len;
	int sorted;
};
hashset dict;
tnode* root;
//pthread_mutex_t root_lock;

void check(char* word, int sorted){
	if(search(dict, word)){
		//pthread_mutex_lock(&root_lock);
		int result = bst_insert(&root, word);
		//pthread_mutex_unlock(&root_lock);
		if (!(sorted) && result)
	 		printf("%s\n", word);
	}
}

void* solve(void* arg){
	//your code here
	struct solver_args* args = (struct solver_args*)arg;
	for(int i = 0; i < args->subpuzzle_rows; i++){
		for(int j = 0; j < args->subpuzzle_cols; j++){
			//check right
			for(int k = 0; k < args->subpuzzle_cols - j; k++){
				char* word = (char*)malloc(args->max_len + 1);
				strncpy(word, args->sub_puzzle[i] + j, k + 1);
				word[k + 1] = '\0';
				if(strlen(word) > args->max_len) break;
				if(strlen(word) < args->min_len) continue;
				check(word, args->sorted);
			}
			//check left
			for(int k = 0; k < j + 1; k++){
				char* word = (char*)malloc(args->max_len + 1);
				strncpy(word, args->sub_puzzle[i] + j - k, k + 1);
				word[k + 1] = '\0';
				if(strlen(word) > args->max_len) break;
				if(strlen(word) < args->min_len) continue;
				check(word, args->sorted);
			}
			//check down
			for(int k = 0; k < args->subpuzzle_rows - i; k++){
				char* word = (char*)malloc(args->max_len + 1);
				for(int l = 0; l < k + 1; l++){
					word[l] = args->sub_puzzle[i + l][j];
				}
				word[k + 1] = '\0';
				if(strlen(word) > args->max_len) break;
				if(strlen(word) < args->min_len) continue;
				check(word, args->sorted);
			}
			//check up
			for(int k = 0; k < i + 1; k++){
				char* word = (char*)malloc(args->max_len + 1);
				for(int l = 0; l < k + 1; l++){
					word[l] = args->sub_puzzle[i - l][j];
				}
				word[k + 1] = '\0';
				if(strlen(word) > args->max_len) break;
				if(strlen(word) < args->min_len) continue;
				check(word, args->sorted);
			}
			//check down right
			for(int k = 0; k < args->subpuzzle_rows - i && k < args->subpuzzle_cols - j; k++){
				char* word = (char*)malloc(args->max_len + 1);
				for(int l = 0; l < k + 1; l++){
					word[l] = args->sub_puzzle[i + l][j + l];
				}
				word[k + 1] = '\0';
				if(strlen(word) > args->max_len) break;
				if(strlen(word) < args->min_len) continue;
				check(word, args->sorted);
			}
			//check down left
			for(int k = 0; k < args->subpuzzle_rows - i && k < j + 1; k++){
				char* word = (char*)malloc(args->max_len + 1);
				for(int l = 0; l < k + 1; l++){
					word[l] = args->sub_puzzle[i + l][j - l];
				}
				word[k + 1] = '\0';
				if(strlen(word) > args->max_len) break;
				if(strlen(word) < args->min_len) continue;
				check(word, args->sorted);
			}
			//check up right
			for(int k = 0; k < i + 1 && k < args->subpuzzle_cols - j; k++){
				char* word = (char*)malloc(args->max_len + 1);
				for(int l = 0; l < k + 1; l++){
					word[l] = args->sub_puzzle[i - l][j + l];
				}
				word[k + 1] = '\0';
				if(strlen(word) > args->max_len) break;
				if(strlen(word) < args->min_len) continue;
				check(word, args->sorted);
			}
			//check up left
			for(int k = 0; k < i + 1 && k < j + 1; k++){
				char* word = (char*)malloc(args->max_len + 1);
				for(int l = 0; l < k + 1; l++){
					word[l] = args->sub_puzzle[i - l][j - l];
				}
				word[k + 1] = '\0';
				if(strlen(word) > args->max_len) break;
				if(strlen(word) < args->min_len) continue;
				check(word, args->sorted);
			}
		}
	}
	
}
void print_buffer(char** sub_puzzle, int subpuzzle_rows, int subpuzzle_cols){
	//this function is used for printing the content of each buffer cell.
	//do NOT call this function anywhere in your final submission.
	printf("%d by %d\n", subpuzzle_rows, subpuzzle_cols);
	for(int i = 0; i < subpuzzle_rows;i++)
		for(int j = 0; j < subpuzzle_cols;j++)
			printf("%c%s", sub_puzzle[i][j], j == subpuzzle_cols - 1?"\n":"");
}
int main(int argc, char** argv){
	if (argc < 11)
		error("Fatal Error. Usage: solve -dict dict.txt -input puzzle1mb.txt -size 1000 -nbuffer 64 -len 4:7 [-s]", 1);
	int puzzle_size = 0, buf_cells = 0, i = 0,j, fd, min_len = 0, max_len = 0, sorted = 0, buf_dimension;
	char* filename = NULL;
	FILE* dictionary_file;
	char*** buffer;
	while(++i < argc){
		if(argv[i][0] != '-')
			error("Fatal Error. Invalid CLA", 2);
		if(!strcmp(argv[i], "-size")){
			puzzle_size = atoi(argv[++i]);
			if(puzzle_size < 15 || puzzle_size > 46340)
				error("Fatal Error. Illegal value passed after -size",3);
		}
		else if(!strcmp(argv[i], "-nbuffer")){
			buf_cells = atoi(argv[++i]);
			if(buf_cells != 1 && buf_cells != 4 && buf_cells != 16
				&& buf_cells != 64)
				error("Fatal Error. Illegal value passed after -nbuffer",4);
			buf_dimension = (int)sqrt(MEM_LIMIT / buf_cells);
			//buf_cells = 1 -> buf_dimension = 8000
			//buf_cells = 4 -> buf_dimension = 4000
			//buf_cells = 16 -> buf_dimension = 2000
			//buf_cells = 64 -> buf_dimension = 1000
		}
		else if(!strcmp(argv[i], "-input")){
			filename = strdup(argv[++i]);
			fd = open(filename, O_RDONLY, 0);
			if(fd < 0)
				error("Fatal Error. Illegal value passed after -input",5);
		}else if(!strcmp(argv[i], "-dict")){
			dictionary_file = fopen(argv[++i], "r");
			if(!dictionary_file)
				error("Fatal Error. Illegal value passed after -dict",6);
		}else if(!strcmp(argv[i], "-len")){
			char* min_max = strdup(argv[++i]);
			char* max_str;
			if(strrchr(min_max, ':') != (max_str = strchr(min_max, ':'))
				|| !strchr(min_max, ':'))
				error("Fatal Error. Illegal value passed after -len",7);
			max_len = atoi(max_str+1);
			min_max[max_str - min_max] = '\0';
			min_len = atoi(min_max);
			if(min_len < 3 || max_len > 14 || min_len > max_len)
				error("Fatal Error. Illegal value passed after -len",7);
		}else if(!strcmp(argv[i], "-s"))
			sorted = 1;
		else{
			error("Fatal Error. Usage: solve -dict dict.txt -input puzzle1mb.txt -size 1000 -nbuffer 64 -len 4:7 [-s]", 1);
			break;
		}
	}
	//Read and move all words from dictionary_file to a new hash table (hashset)
	//Your code here...
	dict = set_init();
	//pthread_mutex_init(&root_lock, root);
	char* word = (char*)malloc(100);
	while(fscanf(dictionary_file, "%s", word) != EOF){
		insert(&dict, word);
	}
	
	//allocate 64MB of buffer in the heap
	//buffer is a 3D array
	//on the outermost dimension, we have buf_cells elements
	//each element is given to one consumer thread
	//each element is going to store a (square-shape) sub-puzzle
	//each element is a square-shape 2D array of characters
	//dimension of this square-shape array is buf_dimension by buf_dimension
	buffer = (char***)malloc(buf_cells * sizeof(char**));
	for(i = 0;i < buf_cells;i++){
		buffer[i] = (char**)malloc(buf_dimension * sizeof(char*));
		for(j = 0; j < buf_dimension;j++)
			buffer[i][j] = (char*) malloc(buf_dimension);
	}
	int buf_index = 0;
	pthread_t t_id[buf_cells];
	for(i = 0; i < buf_cells;i++)
		t_id[i] = NULL;
	for(int row = 0; row + max_len - 1 < puzzle_size; row += (buf_dimension - max_len + 1)){
		int subpuzzle_rows = (row + buf_dimension <= puzzle_size)?
				 buf_dimension:	puzzle_size - row;
		for(int column = 0; column + max_len - 1 < puzzle_size;column += (buf_dimension - max_len + 1)){
			long start = (long)row * (puzzle_size+1) + column;
			lseek(fd,start,SEEK_SET);
			int subpuzzle_cols = (column + buf_dimension <= puzzle_size)?
				 buf_dimension:	puzzle_size - column;
			if(t_id[buf_index])//if there is a busy consumer/solver, 
				pthread_join(t_id[buf_index], NULL);//wait for it to finish the job before manipulating the buffer[buffer_index]
			for(i = 0; i < subpuzzle_rows;i++){
				int n_read = read(fd, buffer[buf_index][i], subpuzzle_cols);
				if(n_read < subpuzzle_cols)
					error("Fatal Error. Bad read from input file", 10);
				if(subpuzzle_cols < buf_dimension)
					buffer[buf_index][i][subpuzzle_cols] = '\0';
				lseek(fd, puzzle_size-subpuzzle_cols+1, SEEK_CUR);
			}
			if(subpuzzle_rows < buf_dimension)
				buffer[buf_index][subpuzzle_rows] = NULL;
			
			//modify these lines so that you can create and start a solver thread 
			
			//after passing the right information to it...
			fprintf(stderr, "Consuming buffer #%d\n", buf_index);
		
			struct solver_args* args = (struct solver_args*)malloc(sizeof(struct solver_args));
			args->sub_puzzle = buffer[buf_index];
			args->subpuzzle_rows = subpuzzle_rows;
			args->subpuzzle_cols = subpuzzle_cols;
			args->max_len = max_len;
			args->min_len = min_len;
			args->sorted = sorted;
			pthread_create(t_id + buf_index, NULL, solve, args);
			//print_buffer(buffer[buf_index], subpuzzle_rows, subpuzzle_cols);
			
			//end of modification
			buf_index = (buf_index == buf_cells - 1)?0: buf_index + 1;
		}
	}
	for(i = 0; i < buf_cells;i++)
		if(t_id[i])
			pthread_join(t_id[i], NULL);
	
	if(sorted){
		//print the binary search tree using in-order traversal...
		//your code here...
		inorder_print(root);
	}
}