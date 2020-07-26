
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUF 100
#define INIT 0

//helper function for swapping two boards
void swap(double*** board_1, double*** board_2);
//helper function for computing L, given i,j, and k
double compute_L(int i, int j, int k, double sigma, double tau);
//helper function for initializing boards where k=0,1
double** init_board(int size);
//helper function for parsing fraction from string
double compute_frac(char* str);
//helper function for opening input file and initializing the boards accordingly
void open_file(char* input, double** prev_board, double** cur_board, double** future_board, int size);
//helper function for saving output results
void save_output( char* output, double** prev_board, double** cur_board, double** future_board, int size, int k_val);
//helper function for deallocating memory
void free_boards(double** prev_board, double** cur_board, double** future_board, int size);

void usage(){
  printf("./main -k k_value -s sigma -t tau -b boardsize -i inputfile -o outputfile -h help\n");
}

void swap(double*** board_1, double*** board_2){
  double** temp = *board_1;
  *board_1 = *board_2;
  *board_2 = temp;
}

double compute_L(int i, int j, int k, double sigma, double tau){
  int u = k/2;
  double l = 0;
  if ((i+j+k)%2 == 0) {
    i = abs((i-u)%2);
    j = abs((j-u)%2);
    k = k % 2;
    if (i == 1 && j == 0 && k == 1) {
      l = sigma;
    }
    else if (i == 0 && j == 1 && k == 1) {
      l = 1 - sigma;
    }
    else if (i == 1 && j == 1 && k == 0) {
      l = tau;
    }
    else if (i == 0 && j == 0 && k == 0) {
      l = 1 - tau;
    }
  }

  return l;
}

double** init_board(int size){
  double** board = malloc(size* sizeof(double*));
  for (int i = 0; i < size; i++){
    board[i] = malloc(size*sizeof(double));
      for (int j = 0; j < size ; j++){
        board[i][j] = INIT;
      }
  }
  return board;
}

double compute_frac(char* str){
  char* astr;
  char* bstr;
  int a, b;
  int size = 0;
  int slash =0;
  while (str[size] != '\0'){
    if (str[size]== '/'){
      slash = size;
    }
    size++;
  }
  if (slash == 0){
    return atoi(str);
  }
  astr = malloc(sizeof(char) * slash+1);
  bstr = (char*) str+slash+1;
  memcpy(astr, str, sizeof(char)*slash);
  *(astr + slash) = '\0';
  a = atoi(astr);
  b = atoi(bstr);

  free(astr);
  return ( a/(double)b);
}

void open_file(char* input, double** prev_board, double** cur_board, double** future_board, int size){
  FILE* infile;
  int row, col,i,j,k;
  int num_init;
  char initstr[MAX_BUF];
  double init;

  infile = fopen(input, "r");
  if (infile == NULL) {
      free_boards(prev_board, cur_board, future_board, size);
      perror("fopen\n");
      exit(1);
  }

  if (fscanf(infile, "%d", &num_init)!=1){
      free_boards(prev_board, cur_board, future_board, size);
      printf("Invalid file\n");
      exit(1);
  }

  for (int i = 0; i < num_init; i++) {
    if (fscanf(infile, "%d %d %d %s", &i, &j, &k, initstr)!=4){
      free_boards(prev_board, cur_board, future_board, size);
      printf("Invalid coordinates\n");
      exit(1);
    }
    row = i+ size/2;
    col = j+ size/2;
    k = k % 2;
    if (row < 0 || col < 0 || col > size || row > size){
      free_boards(prev_board, cur_board, future_board, size);
      printf("Invalid input\n");
      exit(1);
    }
    init = compute_frac(initstr);
    if (k ==1){
      cur_board[row][col] = init;
    }
    else {
      prev_board[row][col] = init;
    }

  }
}


void save_output( char* output, double** prev_board, double** cur_board, double** future_board, int size, int k_val ){
  FILE* outfile;
  int i,j,k;
  double init;
  outfile = fopen(output, "w");
  if (outfile == NULL) {
      free_boards(prev_board, cur_board, future_board, size);
      perror("fopen\n");
      exit(1);
  }

  for (int row = 1; row < size-1; row ++){
    for (int col =1; col < size-1; col++){
        i = row - size/2;
        j = col - size/2;
        if ((i+j+k_val)%2 ==1 || (i+j+k_val)%2 == -1){
          fprintf(outfile,"%.8f %.8f %.8f\n", i/(float)k_val, j/(float)k_val, cur_board[row][col]);
        }
    }
  }

}

void free_boards(double** prev_board, double** cur_board, double** future_board, int size){
  for (int i = 0; i < size; i++){
    free(prev_board[i]);
  }
  for (int i = 0; i < size; i++){
    free(cur_board[i]);
  }
  for (int i = 0; i < size; i++){
    free(future_board[i]);
  }
  free(prev_board);
  free(cur_board);
  free(future_board);
}

int main(int argc, char** argv){
  int c, arg_given, k_val, size, i, j = 0;
  double l, sigma, tau  = 0;
  char* sigstr = "0";
  char* taustr;
  char* input = NULL;
  char* output =NULL;
  double** prev_board;
  double** cur_board;
  double** future_board;

  //parse command line arguments
  while(1) {
    c = getopt(argc, argv, "hs:t:b:i:o:k:b:");
    if(c == -1) {
      break; // nothing left to parse
    }
    switch(c) {
      case 'k': k_val = atoi(optarg);
                arg_given += 1;
                break;
      case 's': sigstr = (char*) optarg;
                arg_given += 1;
                break;
      case 't': taustr = (char*) optarg;
                arg_given += 1;
                break;
      case 'b': size = atoi(optarg);
                arg_given += 1;
                break;
      case 'h': usage();

      case 'i': input = (char*) optarg;
                arg_given += 1;
                break;
      case 'o': output = (char*) optarg;
                arg_given += 1;
                break;

      default: printf("optopt: %c\n", optopt);

    }
  }
  /***************************************************************/

  if((arg_given < 5)) {
    printf("Invalid number of arguments\n");
    usage();
    exit(1);
  }
  else if (k_val <=0 ){
    printf("the number of iterations has to be greater than 0\n");
    usage();
    exit(1);
  }
  else if (input == NULL || output == NULL ){
    printf("Input and output files must be given.\n");
    usage();
    exit(1);
  }
  //if size is not given, by default,
  //set size to twice the number of iterations with some padding
  if (size <=0){
    size = k_val*2 + 3;
  }

  //initialize the board
  prev_board = init_board(size);
  cur_board = init_board(size);
  future_board = init_board(size);

  //open file and initialize accordingly
  open_file(input, prev_board, cur_board, future_board, size);
  //parse sigma and tau values
  sigma = compute_frac(sigstr);
  tau = compute_frac(taustr);

  //perform main recursion
  for (int k = 2; k < k_val+1; k++){
    for (int row = 1; row < size-1; row ++){
      for (int col =1; col < size-1; col++){
          i = row - size/2;
          j = col - size/2;
          l = compute_L(i,j,k-1,sigma,tau);
          future_board[row][col] = l*(cur_board[row-1][col]+cur_board[row+1][col])
                                +(1-l)*(cur_board[row][col-1]+cur_board[row][col+1])
                                -prev_board[row][col];


      }
    }
    swap(&future_board, &prev_board);
    swap(&prev_board, &cur_board);
  }
  //save the results to output file
  save_output(output, prev_board, cur_board, future_board, size, k_val);
  //deallocate memory
  free_boards(prev_board, cur_board, future_board, size);
  printf("Output file saved to %s\n", output);
  return 0;
}
