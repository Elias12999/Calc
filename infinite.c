#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

// constants
#define EOS	257
#define NUM	258

// prototypes 
int expr( FILE *fpRead, FILE *fpWrite );
int term( FILE *fpRead, FILE *fpWrite );
int factor( FILE *fpRead, FILE *fpWrite );
void error( char * );
int get_token( FILE *fpRead, FILE *fpWrite );
void match( int, FILE *fpRead, FILE *fpWrite );


// global communication variables
int current_token;
int current_attribute;

/* bounded memory calculator */
int main(int argc, char * argv[])
{
	
	FILE *fpRead, *fpWrite;
        if (argc < 3) {
                fprintf(stderr,"Not enough arguments\n");
                exit(1);
        }
        if (!(fpRead = fopen(argv[1],"r"))) {
                fprintf(stderr,"Cannot open file %s\n",argv[1]);
                exit(1);
        }
        if (!(fpWrite = fopen(argv[2],"w"))) {
                fprintf(stderr,"Cannot open file %s\n",argv[2]);
                exit(1);
        }

	

	int value;
	while( (current_token = get_token(fpRead,fpWrite)) != EOF ){

		while ( current_token != EOS ) {
			value = expr(fpRead, fpWrite);
			fprintf( stderr, "\nValue = %d\n", value );
			fprintf( fpWrite, "\nValue = %d\n", value );
		}
	}
}

/* calculator */

// handles addition and subtraction
int expr(FILE *fpRead, FILE *fpWrite)
{
	int value = term(fpRead, fpWrite);
	while (1) {
		if ( current_token == '+' ) {
			match( '+', fpRead, fpWrite);
			value += term(fpRead, fpWrite);
		}
		else if ( current_token == '-' ) {
			match( '-', fpRead, fpWrite);
			value -= term(fpRead, fpWrite);

		}
		else break;
	}
	return value;
}

// handles multiplication and division
int term(FILE *fpRead, FILE *fpWrite)
{
	int value = factor(fpRead, fpWrite);
	while (1) {
		if ( current_token == '*' ) {
			match( '*', fpRead, fpWrite );
			value *= factor(fpRead, fpWrite);
		}
		if ( current_token == '/' ) {
			match( '/', fpRead, fpWrite );
			value /= factor(fpRead, fpWrite);
		}
		else break;
	}
	return value;
}

// handles brackets and numbers
int factor(FILE *fpRead, FILE *fpWrite)
{
	int value;

	if ( current_token == '(' ) {
		match( '(', fpRead, fpWrite );
		value = expr(fpRead, fpWrite);
		match( ')', fpRead, fpWrite );
		return value;
	}
	else if ( current_token == NUM ) {
		value = current_attribute;
		match( NUM, fpRead, fpWrite );
		return value;
	}
	else error( "Unexpected token in factor()" );
}

/* match expected token */
void match( int expected_token, FILE *fpRead, FILE *fpWrite)
{
	if ( current_token == expected_token ) {
		current_token = get_token(fpRead, fpWrite);
	}
	else {
		error("Unexpected token in match" );
	}
}


/* get next token */
int get_token(FILE *fpRead, FILE *fpWrite)
{
	int c;		// current character from the stream
	int value;	// value of a number
	//printf("HERE\n");
	while ( 1 ) {
		//printf("HERE\n");
		c = fgetc(fpRead);
		//c = fgetc(stdin);
				
		if (c == '+' || c == '-' || c == '*' || c == '(' || c == ')' || c == '/' ) {
			fprintf( stderr, "[OP:%c]", c );
			fprintf( fpWrite, "[OP:%c]", c );
			return c;	// return operators and brackets as is
		}
	
		else if ( c == ' ' || c == '\t' )
			continue;	// ignore spaces and tabs
			
		else if ( isdigit(c) ) {
			value = c - '0';
			while ( isdigit( c = fgetc(fpRead) )) {
				value = value * 10 + (c - '0');
			}
			ungetc( c, fpRead );
			fprintf( stderr, "[NUM:%d]", value );
			fprintf( fpWrite, "[NUM:%d]", value );
			current_attribute = value;
			return NUM;
		}
	
		else if ( c == '\n' )
			return EOS;
		else {
			fprintf( stderr, "{%c}", c );
			error( "Unknown token" );
		}
	}
}

// error reporting function
void error( char *message )
{
	fprintf( stderr, "Error: %s\n", message );
	exit(1);
}
