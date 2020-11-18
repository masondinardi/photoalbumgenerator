#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <libgen.h>
#include <sys/wait.h>

#include "readline.h"

//struct to keep track of pics and user input
struct Photos {
	char med_pic[30];
	char thumbnail[30];
	char caption[30];
}pics[30];

int n = 0;

//function declarations
void display(char prefix[], char photo[]);
void convert(char size[], char photo[], char prefix[], char rotate_percentage[]);
void gen_thumbnail(int argc, char* argv[]);
void get_inp(char photo[]);
void create_page(struct Photos *pic_arr, int argc);

//main function that takes care of executions
int main(int argc, char* argv[]){
	
	if (argc < 2){
		fprintf(stderr, "No path to photo.\n");
		exit(-1);
    }

	fprintf(stdout, "Using %d photos from file.\n", argc - 1);
	
	gen_thumbnail(argc - 1, argv + 1);
	create_page(pics, argc - 1);

	return 0;
}

//function that takes care of conversions
void convert(char size[], char photo[], char prefix[], char rotate_percentage[]){
	strcat(prefix, basename(photo));
	execlp("convert", "convert", photo, "-geometry", size, "-rotate", rotate_percentage, prefix, NULL);	
}

//function to display thumbnail to user
void display(char prefix[], char photo[]){	
	strcat(prefix, basename(photo));
	execlp("display", "display", prefix, NULL);
}

//function to generate thumbnails. calls convert and display in forked children processes
void gen_thumbnail(int argc, char *argv[]){
	int status, dpid, i;
	pid_t pid[argc];
	char prefix[30] = "tn_";
	
	for (i = 0; i < argc; i++){
		if((pid[i] = fork()) == 0){
			convert("25%", argv[i], prefix, "0%");
		}
	}
	for(i = 0; i < argc; i++){
		if(waitpid(pid[i], &status, 0) != 0){
			if((dpid = fork()) == 0){
				display(prefix, argv[i]);
			}
			get_inp(argv[i]);
			waitpid(dpid, &status, 0);
		}
	}
}

/*Function gets user input and then uses fork() and call to conver function to create properly oriented 
medium size versions. I am aware the use of the global n variable is bad practice however I spent
over 6 hours working on just this part and that is the only working solution I found.*/
void get_inp(char photo[]){

	char ans[30];
	char med_prefix[30] = "med_";
	char tn_prefix[30] = "tn_";
	char rotate[30];
	int pid;
	
	printf("Would you like %s to be rotated? y or n\n", basename(photo));
	readLine(ans, 2);
	if((strcmp(ans, "y")) == 0){
		printf("enter a rotation percentage. format: ###%%\n");
		readLine(rotate, 4);
		
		if(strlen(rotate) < 5){
			printf("Enter a caption for %s that is max 25 characters. When done, press enter and close thumbnail\n", basename(photo));
			readLine(pics[n].caption, 25);
		}
		else{
			printf("Invalid input, photo will not be rotated\n");
			readLine(pics[n].caption, 25);
		}
	}
	else if((strcmp(ans, "n")) == 0){
		strcpy(rotate, "0%");
		printf("Enter a caption for %s that is max 25 characters. When done, press enter and close thumbnail\n", basename(photo));
		readLine(pics[n].caption, 25);
	}
	
	if((pid = fork()) == 0){
		convert("75%", photo, med_prefix, rotate);
	}
	
	strcat(med_prefix, basename(photo));
	strcpy(pics[n].med_pic, med_prefix);
	strcat(tn_prefix, basename(photo));
	strcpy(pics[n].thumbnail, tn_prefix);
	
	n++;
}

//called after all photos are processed and generates html page for photos and captions
void create_page(struct Photos pic_arr[], int argc){
	
	FILE *fp;
	
	fp = fopen("index.html", "w");
	if(fp == NULL){
		fprintf(stderr, "CANNOT OPEN FILE\n");
		exit(1);
	}
	
	char* title = "Your photo album!";
	
	fprintf(fp, "<!DOCTYPE html>\n");
    fprintf(fp, "<html>\n");
    fprintf(fp, "<head>\n");
    fprintf(fp, "<title>%s</title>\n", title);
    fprintf(fp, "</head>\n");
    fprintf(fp, "<body>\n");
	
	fprintf(fp, "<h1>%s</h1>\n", title);
    fprintf(fp, "<h2>Click the thumbnails for a properly oriented version!</h2>\n");
	fprintf(fp, "<ul>\n");
	
	for(int i = 0; i < argc; i++){
		fprintf(fp, "<li>\n");
		fprintf(fp, "<h3>%s</h3>\n", pic_arr[i].caption);
		fprintf(fp, "<a href=\"%s\"><img src=\"%s\"></a>\n",pic_arr[i].med_pic, pic_arr[i].thumbnail);
		fprintf(fp, "</li>\n");
	}
	fprintf(fp, "</ul>\n");
	
	fprintf(fp, "</body>\n");
    fprintf(fp, "</html>\n");
	
	fclose(fp);
	printf("Enjoy your photo album!\n");
}
	
	
	
	
	