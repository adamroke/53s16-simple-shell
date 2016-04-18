//Andrew Light
//argument passing tester
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
	int count = argc;
	int min;
	printf("program has run, num args is: %d\n",count);
	for(min=1;min<argc;min++){
		printf("arg%d:|%s|\n",min,argv[min]);
	}
	 return 0;
}

