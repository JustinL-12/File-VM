// A simple, .txt file-based vm that runs a proprietary instruction set
// Written by Justin Long, 2020

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

#define MAX_INSTR_SIZE 20
#define MAX_INSTR_ARG_SIZE 5 // max num of args in instruction including the instr itself

// Path of directory to run 
const char* PATH = "";

int get_num_lines(char* file_name);

void run(char** instrs, int n, int* registers, node* root);

char** parse(char* line, char** tokens);

int execute(int* registers, char** instr, node* root);

void start(char* file_path, int* registers, node* root);

int main(int argc, char*argv[]) {
	// general purpose registers
	int registers[8];
	// memory stack 
	node* root = NULL;
	
	// Get full file_path to start
	char str[200];
	strcpy(str, PATH);
	strcat(str, "main.txt");	
	
	// Start running
	start(str, registers, root);
}

void start(char* file_path, int* registers, node* root) {
	// Get number of lines in file
	int num_lines = get_num_lines(file_path);
	//printf("Number of lines: %d", num_lines);
	
	//read each instruction to buffer
	FILE* fp = fopen(file_path, "r");
	char** all_input;
	all_input = malloc(num_lines * sizeof(char*));
	for (int i=0; i < num_lines; i++) {
		all_input[i] = malloc(MAX_INSTR_SIZE * sizeof(char));
		fgets(all_input[i], MAX_INSTR_SIZE, fp);
	}
	fclose(fp);
	// test
	//for (int i=0; i<num_lines; i++)
	//	printf("%s", all_input[i]);
	
	
	// run
	run(all_input, num_lines, registers, root);
	
	
	// free memory
}

int get_num_lines(char* file_name) {
	int count = 0;
	char c;
	FILE* fp = fopen(file_name, "r");
	if (fp == NULL) {
		printf("Failed to open file %s", file_name);
		return 0;
	}

	for(c = getc(fp); c != EOF; c = getc(fp))
		if (c == '\n')
			count++;
	fclose(fp);
	return count + 1;
}

// -- Instruction List --
/*
	movi i r1 - move immediate int to r1
	movr r1 r2 - copy val from r1 to r2
	log str r1 - print str and contents of r1 to console
	addi i r1 - add immediate i to r1
	addr r1 r2 - add contents of r1 to r2
	multi i r1 - multiply r1 by i
	multr r1 r2 - multiply contents of r2 by contents of r1
	jmp i - jump to instruction at line i
	run str - run .txt file 
	push i - push int to stack
	pop r1 - pop top value from stack to r1
	
	// - comment out that entire line
*/

void run(char** instrs, int n, int* registers, node* root) {
	
	char** tokens = (char**)malloc(MAX_INSTR_ARG_SIZE * sizeof(char*));
	
	// begin running 
	for (int i=0; i<n; i++) {
		
		// parse instruction
		tokens = parse(instrs[i], tokens);
		
		// execute instruction
		int ret = execute(registers, tokens, root);
		
		// check exec return for jump instrucion
		if (ret != -1) {
			i = ret -1;
		}
		
		// free tokens
		for (int j=0; j < MAX_INSTR_ARG_SIZE; j++) {
			if (tokens[j] != NULL) {
				free(tokens[j]);
			}
		}
		
	}
	
	free(tokens);
	
}

char** parse(char* str, char** tokens) {
	// allocate locations for tokens
	int i = 0;
	
	char* token = strtok(str, " ");
	while (token != NULL) {
		token[strcspn(token, "\n")] = 0;
		
		tokens[i] = malloc(MAX_INSTR_SIZE * sizeof(char));
		strcpy(tokens[i], token);
		
		token = strtok(NULL, " ");
		i++;
	} 
	
	return tokens;
}

int execute(int* registers, char** instr, node* root) {
	
	// test for comment to ignore
	if (instr[0][0] == '/' && instr[0][1] == '/')
		return -1;
	
	// generate op code
	int i = 0; 
	char temp = instr[0][0];
	int sum = 0;
	while (temp != '\0') {
		sum += (int)instr[0][i];
		i++;
		temp = instr[0][i];
	}
	
	// execute instruction
	switch (sum) {
		// log
		case 322:
			printf("%s %d\n", instr[1], registers[atoi(instr[2])]);
			break;
		
		// movi
		case 443:
			registers[atoi(instr[2])] = atoi(instr[1]);
			break;
			
		// movr
		case 452:
			registers[atoi(instr[2])] = registers[atoi(instr[1])];
			break;
			
		// addi 
		case 402:
			registers[atoi(instr[2])] += atoi(instr[1]);
			break;
		
		// addr
		case 411:
			registers[atoi(instr[2])] += registers[atoi(instr[1])];
			break;
			
		// multi	
		case 555:
			registers[atoi(instr[2])] *= atoi(instr[1]);
			break;
		
		// multr
		case 564: 
			registers[atoi(instr[2])] *= registers[atoi(instr[1])];
			break;
			
		// jmp
		case 327:
			return atoi(instr[1]);	
			
		// run
		case 341: ;
			char str[200];
			strcpy(str, PATH);
			strcat(str, instr[1]);
			start(str, registers, root);
			break;
		
		// push	
		case 448:
			printf("push\n");
			push(&root, atoi(instr[1]));
			break;
		
		// pop
		case 335:
			registers[atoi(instr[1])] = pop(&root);
			break;
		
		// Unrecognized command
		default:
			printf("Unknown command: %s - %d\n", instr[0], sum);
			
	}
	
	return -1;
} 


