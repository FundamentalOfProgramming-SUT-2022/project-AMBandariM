// ~ Be Name Khoda ~
// <Doctor Bandari>
// Powered by .LOVE

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

void backup(char *file_name)
{
	char backup_name[100]; backup_name[0] = '\0';
	strcat(backup_name, file_name); strcat(backup_name, ".bk");
	backup_name[0] = '.'; backup_name[1] = 'a'; backup_name[2] = 'r'; backup_name[3] = 'a';
	
	FILE *input_file	= fopen(file_name, "r");
	FILE *output_file	= fopen(backup_name, "w");
	char ch;
	while((ch = fgetc(input_file)) != EOF){
		fputc(ch, output_file);
	}
	fclose(input_file);
	fclose(output_file);
}

void replace(char *file_name)
{
	FILE *input_file	= fopen(".ara/place", "r");
	FILE *output_file	= fopen(file_name, "w");
	char ch;
	while((ch = fgetc(input_file)) != EOF){
		fputc(ch, output_file);
	}
	fclose(input_file);
	fclose(output_file);
}

char corr_read_out[10000]; int the_length;
void corr_read(char *address)
{
	the_length = 0;
	while(*address == ' '){address++; the_length++;}
	if(address[0] != '"'){
		sscanf(address, "%1000s", corr_read_out);
		the_length = strlen(corr_read_out);
	}
	else{
		address++;
		the_length++;
		int slash = 0;
		int dirptr = 0;
		while(1){
			if(*address == '\\'){
				if(slash){
					corr_read_out[dirptr] = '\\';
					slash = 0;
					dirptr++;
				}
				else slash = 1;
			}
			else if(*address == '"'){
				if(slash){
					corr_read_out[dirptr] = '"';
					slash = 0;
					dirptr++;
				}
				else break;
			}
			else if(*address == 'n' && slash){
				corr_read_out[dirptr] = '\n';
				slash = 0;
				dirptr++;
			}
			else{
				corr_read_out[dirptr] = *address;
				dirptr++;
			}
			address++;
			the_length++;
		}
	}
}

void insertstr(char *file_name, int line_no, int pos_in_line)
{
	backup(file_name);
	FILE *file = fopen(file_name, "r");
	FILE *str_file = fopen(".ara/str", "r");
	FILE *place = fopen(".ara/place", "w");
	char one_line[1000];
	for(int i = 1; i < line_no && fgets(one_line,1000,file) != NULL; i++){
		fprintf(place,"%s",one_line);
	}
	char one_char;
	for(int i = 0; i < pos_in_line && fgets(&one_char,1,file) != NULL; i++){
		fprintf(place, "%c", one_char);
	}
	while(fgets(one_line,1000,str_file) != NULL){
		fprintf(place,"%s",one_line);
	}
	while(fgets(one_line,1000,file) != NULL){
		fprintf(place,"%s",one_line);
	}
	fclose(file);
	fclose(str_file);
	fclose(place);
	replace(file_name);
}

void cat(char *file_name, int mode)
{
	FILE *input_file = fopen(file_name, "r");
	FILE *output_file;
	if(mode == 1){
		output_file = fopen(".ara/str", "w");
	}
	else{
		output_file = stdout;
	}
	char one_line[1000];
	while(fgets(one_line, 1000, input_file) != NULL){
		fprintf(output_file, "%s", one_line);
	}
	if(mode == 0){
		printf("\n");
	}
	fclose(input_file);
	fclose(output_file);
}

void removestr(char *file_name, int line_no, int pos_in_line, int rm_size, char ward)
{
	backup(file_name);
	FILE *file = fopen(file_name, "r");
	FILE *place = fopen(".ara/place", "w");
	char one_line[1000], one_char[2];
	if(ward == 'f')
	{
		for(int i = 1; i < line_no && fgets(one_line,1000,file) != NULL; i++){
			fprintf(place,"%s",one_line);
		}
		for(int i = 0; i < pos_in_line && fgets(one_char,2,file) != NULL; i++){
			fprintf(place,"%c",*one_char);
		}
		fseek(file, rm_size, SEEK_CUR);
		while(fgets(one_line,1000,file) != NULL){
			fprintf(place,"%s",one_line);
		}
	}
	else // 'b'
	{
		fseek(file, 0, SEEK_SET);
		for(int i = 1; i < line_no && fgets(one_line,1000,file) != NULL; i++){
			int x = 0;
		}
		fgets(one_line,pos_in_line+ + 1,file);
		long long where_we_are = ftell(file);
		fseek(file, 0, SEEK_SET);
		for(int i = 0; (i < (where_we_are - rm_size)) && pos_in_line && fgets(one_char,2,file) != NULL; i++){
			fprintf(place, "%c",*one_char);
		}
		fseek(file, where_we_are, SEEK_SET);
		while(fgets(one_char,2,file) != NULL){
			fprintf(place,"%c",*one_char);
		}
	}
	fclose(file);
	fclose(place);
	replace(file_name);
}

void copystr(char *file_name, int line_no, int pos_in_line, int cpy_size, char ward)
{
	FILE *file = fopen(file_name, "r");
	FILE *clipboard = fopen(".ara/clip.board", "w");
	char one_line[1000], one_char[2];
	if(ward == 'f')
	{
		fseek(file, 0, SEEK_SET);
		for(int i = 1; i < line_no && fgets(one_line,1000,file) != NULL; i++){
			int x = 0;
		}
		fgets(one_line,pos_in_line + 1,file);

		for(int i = 0; i < cpy_size && fgets(one_char,2,file) != NULL; i++){
			fprintf(clipboard, "%c", *one_char);
		}
	}
	else // 'b'
	{
		fseek(file, 0, SEEK_SET);
		for(int i = 1; i < line_no && fgets(one_line,1000,file) != NULL; i++){
			int x = 0;
		}
		fgets(one_line,pos_in_line + 1,file);
		long long where_we_are = ftell(file);
		fseek(file, where_we_are - cpy_size, SEEK_SET);
		for(int i = 0; i < cpy_size && pos_in_line && fgets(one_char,2,file) != NULL; i++){
			fprintf(clipboard, "%c",*one_char);
		}
	}
	fclose(file);
	fclose(clipboard);
}

void pastestr(char *file_name, int line_no, int pos_in_line)
{
	backup(file_name);
	FILE *file = fopen(file_name, "r");
	FILE *str_file = fopen(".ara/clip.board", "r");
	FILE *place = fopen(".ara/place", "w");
	char one_line[1000];
	for(int i = 1; i < line_no && fgets(one_line,1000,file) != NULL; i++){
		fprintf(place,"%s",one_line);
	}
	char one_char;
	for(int i = 0; i < pos_in_line && fgets(&one_char,1,file) != NULL; i++){
		fprintf(place, "%c", one_char);
	}
	while(fgets(one_line,1000,str_file) != NULL){
		fprintf(place,"%s",one_line);
	}
	while(fgets(one_line,1000,file) != NULL){
		fprintf(place,"%s",one_line);
	}
	fclose(file);
	fclose(str_file);
	fclose(place);
	replace(file_name);
}

int main(){
	char COMMAND_LINE[1000];
	char command_word[50];
	char *command_line;
	fgets(COMMAND_LINE, 1000, stdin);
	command_line = (char *)&COMMAND_LINE;
	while(1){
		if(-1 == sscanf(command_line, "%50s", command_word)){
			fgets(COMMAND_LINE, 1000, stdin);
			command_line = (char *)&COMMAND_LINE;
			sscanf(command_line, "%50s", command_word);
		}
		
		command_line += strlen(command_word) + 1;
		if(!strcmp(command_word,"exit") || !strcmp(command_word,"x"))
		{
			break;
		}
		else if(!strcmp(command_word,"createfile"))
		{
			sscanf(command_line, "%50s", command_word);
			command_line += strlen(command_word) + 1;
			if(!strcmp(command_word,"--file")){
				corr_read(command_line);
				command_line += the_length + 1;
				FILE *file = fopen(corr_read_out, "r");
				if(file != NULL){
					fclose(file);
					printf("%s exists\n", corr_read_out);
				}
				else{
					FILE *file = fopen(corr_read_out, "w");
					fclose(file);
				}
			}
			else{
				printf("%s\n", "put address after --file");
				*command_line = '\0';
			}
		}
		else if(!strcmp(command_word,"insertstr"))
		{
			sscanf(command_line, "%50s", command_word);
			command_line += strlen(command_word) + 1;
			if(!strcmp(command_word,"--file")){
				char dir[1000]; int line, position;
				corr_read(command_line);
				strcpy(dir, corr_read_out);
				command_line += the_length + 1;

				sscanf(command_line, "%50s", command_word);
				if(!strcmp(command_word,"--str")){
					command_line += strlen(command_word) + 1;
					corr_read(command_line);
					FILE *file = fopen(".ara/str","w");
					fprintf(file, "%s", corr_read_out);
					fclose(file);
					command_line += the_length + 1;
				}
				sscanf(command_line, "%50s", command_word);
				if(!strcmp(command_word,"--pos")){
					command_line += strlen(command_word) + 1;
					sscanf(command_line, "%d:%d", &line, &position);
					char somestringhere[100];
					sprintf(somestringhere, "%d:%d", line, position);
					command_line += strlen(somestringhere) + 1;
					insertstr(dir,line,position);
				}
				else{
					printf("%s\n", "put position after --pos");
					*command_line = '\0';
				}
			}
			else{
				printf("%s\n", "put address after --file");
				*command_line = '\0';
			}
		}
		else if(!strcmp(command_word,"cat"))
		{
			sscanf(command_line, "%50s", command_word);
			command_line += strlen(command_word) + 1;
			if(!strcmp(command_word,"--file")){
				char dir[1000];
				corr_read(command_line);
				strcpy(dir, corr_read_out);
				command_line += the_length + 1;
				sscanf(command_line, "%50s", command_word);
				if(!strcmp(command_word,"=D")){
					command_line += strlen(command_word) + 1;
					cat(dir,1);
				}
				else{
					cat(dir,0);
				}

			}
			else{
				printf("%s\n", "put address after --file");
				*command_line = '\0';
			}
		}
		else if(!strcmp(command_word,"removestr"))
		{
			sscanf(command_line, "%50s", command_word);
			command_line += strlen(command_word) + 1;
			if(!strcmp(command_word,"--file")){
				char dir[1000], bfward; int line, position, size;
				corr_read(command_line);
				strcpy(dir, corr_read_out);
				command_line += the_length + 1;
				sscanf(command_line, "%50s", command_word);
				command_line += strlen(command_word) + 1;
				if(!strcmp(command_word,"--pos")){
					sscanf(command_line, "%d:%d", &line, &position);
					char somestringhere[100];
					sprintf(somestringhere, "%d:%d", line, position);
					command_line += strlen(somestringhere) + 1;
					sscanf(command_line, "%50s", command_word);
					command_line += strlen(command_word) + 1;
					if(!strcmp(command_word,"-size")){
						sscanf(command_line, "%d", &size);
						sprintf(somestringhere, "%d", size);
						command_line += strlen(somestringhere) + 1;
						if(-1 != sscanf(command_line, "-%c", &bfward))
						{
							if(bfward == 'b'|| bfward == 'f'){
								command_line += 3;
								removestr(dir,line,position,size,bfward);
							}
							else{
								printf("%s\n", "sellect backward or forward using flags: -b -f");
								*command_line = '\0';
							}
						}
						else{
							printf("%s\n", "sellect backward or forward using flags: -b -f");
							*command_line = '\0';
						}
					}
					else{
						printf("%s\n", "put size after -size");
						*command_line = '\0';
					}
				}
				else{
					printf("%s\n", "put position after --pos");
					*command_line = '\0';
				}

			}
			else{
				printf("%s\n", "put address after --file");
				*command_line = '\0';
			}
		}
		else if(!strcmp(command_word,"copystr"))
		{
			sscanf(command_line, "%50s", command_word);
			command_line += strlen(command_word) + 1;
			if(!strcmp(command_word,"--file")){
				char dir[1000], bfward; int line, position, size;
				corr_read(command_line);
				strcpy(dir, corr_read_out);
				command_line += the_length + 1;
				sscanf(command_line, "%50s", command_word);
				command_line += strlen(command_word) + 1;
				if(!strcmp(command_word,"--pos")){
					sscanf(command_line, "%d:%d", &line, &position);
					char somestringhere[100];
					sprintf(somestringhere, "%d:%d", line, position);
					command_line += strlen(somestringhere) + 1;
					sscanf(command_line, "%50s", command_word);
					command_line += strlen(command_word) + 1;
					if(!strcmp(command_word,"-size")){
						sscanf(command_line, "%d", &size);
						sprintf(somestringhere, "%d", size);
						command_line += strlen(somestringhere) + 1;
						if(-1 != sscanf(command_line, "-%c", &bfward))
						{
							if(bfward == 'b'|| bfward == 'f'){
								command_line += 3;
								copystr(dir,line,position,size,bfward);
							}
							else{
								printf("%s\n", "sellect backward or forward using flags: -b -f");
								*command_line = '\0';
							}
						}
						else{
							printf("%s\n", "sellect backward or forward using flags: -b -f");
							*command_line = '\0';
						}
					}
					else{
						printf("%s\n", "put size after -size");
						*command_line = '\0';
					}
				}
				else{
					printf("%s\n", "put position after --pos");
					*command_line = '\0';
				}

			}
			else{
				printf("%s\n", "put address after --file");
				*command_line = '\0';
			}
		}
		else if(!strcmp(command_word,"cutstr"))
		{
			sscanf(command_line, "%50s", command_word);
			command_line += strlen(command_word) + 1;
			if(!strcmp(command_word,"--file")){
				char dir[1000], bfward; int line, position, size;
				corr_read(command_line);
				strcpy(dir, corr_read_out);
				command_line += the_length + 1;
				sscanf(command_line, "%50s", command_word);
				command_line += strlen(command_word) + 1;
				if(!strcmp(command_word,"--pos")){
					sscanf(command_line, "%d:%d", &line, &position);
					char somestringhere[100];
					sprintf(somestringhere, "%d:%d", line, position);
					command_line += strlen(somestringhere) + 1;
					sscanf(command_line, "%50s", command_word);
					command_line += strlen(command_word) + 1;
					if(!strcmp(command_word,"-size")){
						sscanf(command_line, "%d", &size);
						sprintf(somestringhere, "%d", size);
						command_line += strlen(somestringhere) + 1;
						if(-1 != sscanf(command_line, "-%c", &bfward))
						{
							if(bfward == 'b'|| bfward == 'f'){
								command_line += 3;
								copystr(dir,line,position,size,bfward);
								removestr(dir,line,position,size,bfward);
							}
							else{
								printf("%s\n", "sellect backward or forward using flags: -b -f");
								*command_line = '\0';
							}
						}
						else{
							printf("%s\n", "sellect backward or forward using flags: -b -f");
							*command_line = '\0';
						}
					}
					else{
						printf("%s\n", "put size after -size");
						*command_line = '\0';
					}
				}
				else{
					printf("%s\n", "put position after --pos");
					*command_line = '\0';
				}

			}
			else{
				printf("%s\n", "put address after --file");
				*command_line = '\0';
			}
		}
		else if(!strcmp(command_word,"pastestr"))
		{
			sscanf(command_line, "%50s", command_word);
			command_line += strlen(command_word) + 1;
			if(!strcmp(command_word,"--file")){
				char dir[1000]; int line, position;
				corr_read(command_line);
				strcpy(dir, corr_read_out);
				command_line += the_length + 1;

				sscanf(command_line, "%50s", command_word);
				if(!strcmp(command_word,"--pos")){
					command_line += strlen(command_word) + 1;
					sscanf(command_line, "%d:%d", &line, &position);
					char somestringhere[100];
					sprintf(somestringhere, "%d:%d", line, position);
					command_line += strlen(somestringhere) + 1;
					pastestr(dir,line,position);
				}
				else{
					printf("%s\n", "put position after --pos");
					*command_line = '\0';
				}
			}
			else{
				printf("%s\n", "put address after --file");
				*command_line = '\0';
			}
		}
		else{
			printf("%s\n", "Invalid command");
			*command_line = '\0';
		}

	}
	return 0;
}