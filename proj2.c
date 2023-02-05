//      ~ In the name of GOD ~
//       <- Doctor Bandari ->
//          HARD MODE : ON

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<locale.h> // for utf8
#include<ncurses.h>
#include<time.h>
#include<math.h>
#include<sys/ioctl.h>

int createfileSilent = 0;
int upper_line = 0; int file_curser = 0, vis_curser = 0, curser_pos[2];
enum VIF_STATE {NORMAL, COMMAND, VISUAL, INSERT};
enum VIF_STATE vif_state = NORMAL;
char name[100]; int itssaved = 0;

int min(int x, int y)
{
	if(x < y) return x; return y;
}
int max(int x, int y)
{
	if(x > y) return x; return y;
}
int abs(int x)
{
	if(x > 0) return x; return -x;
}
void createfile(char *address)
{
	char scope[100];
	int i = 0;
	while(1){
		if(address[i] == '\0'){
			FILE *file = fopen(address, "r");
			if(file != NULL){
				fclose(file);
				if(!createfileSilent)
					printf("%s exists\n", address);
			}
			else{
				FILE *file = fopen(address, "w");
				fclose(file);
			}
			createfileSilent = 0;
			return;
		}
		else if(address[i] == '/'){
			struct stat st = {0};
			if(stat(scope, &st) == -1){
				mkdir(scope, 0700);
			}
		}
		scope[i] = address[i]; scope[i+1] = '\0'; i++;
	}
}
void backup(char *file_name)
{
	char backup_name[100]; backup_name[0] = '\0';
	strcat(backup_name, file_name); strcat(backup_name, ".bk");
	backup_name[0] = '.'; backup_name[1] = 'a'; backup_name[2] = 'r'; backup_name[3] = 'a';
	createfileSilent = 1; createfile(backup_name);
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
	int dirptr = 0;
	while(*address == ' '){address++; the_length++;}
	if(address[0] != '"'){
		int slash = 0;
		while(1){
			if(*address == '\\'){
				if(slash){
					corr_read_out[dirptr] = '\\';
					slash = 0;
					dirptr++;
				}
				else slash = 1;
			}
			else if(*address == ' ' || *address == '\n' || *address == '\0'){
				break;
			}
			else if(*address == 'n' && slash){
				corr_read_out[dirptr] = '\n';
				slash = 0;
				dirptr++;
			}
			else{
				corr_read_out[dirptr] = *address;
				dirptr++;
				slash = 0;
			}
			address++;
			the_length++;
		}
	}
	else{
		address++;
		the_length++;
		int slash = 0;
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
				else{
					the_length++;
					break;
				}
			}
			else if(*address == 'n' && slash){
				corr_read_out[dirptr] = '\n';
				slash = 0;
				dirptr++;
			}
			else{
				corr_read_out[dirptr] = *address;
				dirptr++;
				slash = 0;
			}
			address++;
			the_length++;
		}
	}
	corr_read_out[dirptr] = '\0';
}
void incorr_read(char *address)
{
	the_length = 0;
	FILE *str_file = fopen(".ara/str", "w");
	while(*address == ' '){address++; the_length++;}
	if(address[0] != '"'){
		char ch[2]; int slash = 0;
		while(sscanf(address, "%c", ch) != -1){
			if(*ch == ' ' || *ch == '\0' || *ch == EOF) break;
			the_length++;
			address++;
			if(*ch == '\\' && !slash){
				slash = 1;
			}
			else if((*ch == 'n') && slash){
				fprintf(str_file,"%c",'\n');
				slash = 0;
			}
			else if((*ch == '*') && slash){
				fprintf(str_file,"%c",'*');
				slash = 0;
			}
			else if(*ch == '*'){
				fprintf(str_file,"%c",27);
			}
			else if((*ch == '\\') && slash){
				fprintf(str_file,"%c",'\\');
				slash = 0;
			}
			else if((*ch == '"') && slash){
				fprintf(str_file,"%c",'"');
				slash = 0;
			}
			else{
				fprintf(str_file,"%c",*ch);
				slash = 0;
			}
		}
		the_length++;
	}
	else{
		address++;
		the_length++;
		int slash = 0;
		int dirptr = 0;
		while(1){
			if(*address == '\\'){
				if(slash){
					fprintf(str_file,"%c",'\\');
					slash = 0;
				}
				else slash = 1;
			}
			else if(*address == '"'){
				if(slash){
					fprintf(str_file,"%c",'"');
					slash = 0;
				}
				else { the_length++; break;}
			}
			else if(*address == '*'){
				if(slash){
					fprintf(str_file,"%c",'*');
					slash = 0;
				}
				else{
					fprintf(str_file,"%c",(char)(27));
				}
			}
			else if(*address == 'n' && slash){
				fprintf(str_file,"%c",'\n');
				slash = 0;
			}
			else{
				fprintf(str_file,"%c",*address);
				slash = 0;
			}
			address++;
			the_length++;
		}
	}
	fclose(str_file);
}
void insertstr(char *file_name, int line_no, int pos_in_line)
{
	if(!strcmp(file_name,".ara/.vif")) itssaved = 0;
	backup(file_name);
	FILE *file = fopen(file_name, "r");
	FILE *str_file = fopen(".ara/str", "r");
	FILE *place = fopen(".ara/place", "w");
	char one_line[1000];
	for(int i = 1; i < line_no && fgets(one_line,1000,file) != NULL; i++){
		fprintf(place,"%s",one_line);
	}
	char one_char[2];
	for(int i = 0; i < pos_in_line && fgets(one_char,2,file) != NULL; i++){
		fprintf(place, "%c", *one_char);
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
void insertstr_with_char(char *file_name, int pos, char *text)
{
	FILE *file = fopen(file_name, "r");
	FILE *place = fopen(".ara/place", "w");
	char one_line[1000];
	char one_char[2];
	for(int i = 0; i < pos && fgets(one_char,2,file) != NULL; i++){
		fprintf(place, "%c", *one_char);
	}
	fprintf(place,"%s",text);
	while(fgets(one_line,1000,file) != NULL){
		fprintf(place,"%s",one_line);
	}
	fclose(file);
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
		name[0] = '\0';
		if(!strcmp(file_name,".ara/.vif")) itssaved = 0;
		output_file = fopen(".ara/.vif", "w");
	}
	char one_line[1000];
	while(fgets(one_line, 1000, input_file) != NULL){
		fprintf(output_file, "%s", one_line);
	}
	fclose(output_file);
	fclose(input_file);
}
void removestr(char *file_name, int line_no, int pos_in_line, int rm_size, char ward)
{
	if(!strcmp(file_name,".ara/.vif")) itssaved = 0;
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
		fgets(one_line,pos_in_line + 1,file);
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
void removestr_with_char(char *file_name, int pos, int rm_size)
{
	FILE *file = fopen(file_name, "r");
	FILE *place = fopen(".ara/place", "w");
	char one_line[1000], one_char[2];

	for(int i = 0; i < pos && fgets(one_char,2,file) != NULL; i++){
		fprintf(place,"%c",*one_char);
	}
	fseek(file, rm_size, SEEK_CUR);
	while(fgets(one_line,1000,file) != NULL){
		fprintf(place,"%s",one_line);
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
	if(!strcmp(file_name,".ara/.vif")) itssaved = 0;
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
enum FinderType {ATR_COUNT, ATR_AT, ATR_ALL};
int char_id[10000], word_id[10000], lenght_of_word[10000], counter_of_find;
void findstr(char *file_name, enum FinderType atr, int at_num, int byword, int arman)
{
	counter_of_find = 0;
	FILE *file = fopen(file_name, "r");
	FILE *str_file = fopen(".ara/str", "r");
	char one_char[2]; int strPtr = 0, filePtr = 0, dynamicPtr = 0, filePtrByword = 0, lenghtInFile = 0, lastStarPtr = -1,lastStarStr, inStar = 0;
	int there_is_star = 0;
	while(fgets(one_char,2,str_file) != NULL){
		if(*one_char == 27){
			there_is_star = 1;
			break;
		}
	}
	fseek(str_file, 0, SEEK_END); int MAXsTRfILE = ftell(str_file); fseek(str_file, 0, SEEK_SET);
	fseek(file, 0, SEEK_END); int MAXfILE = ftell(file);
	for(filePtr = 0; filePtr < MAXfILE; filePtr++){
		fseek(file, filePtr, SEEK_SET);
		fseek(str_file, 0, SEEK_SET);
		char fch[2], sch[2];
		dynamicPtr = filePtr;
		inStar = 0;
		lenghtInFile = 0;
		for(strPtr = 0; strPtr < MAXsTRfILE; strPtr++){
			fgets(sch, 2, str_file);
			if(*sch == 27){
				inStar = 1;
				lastStarPtr = dynamicPtr - 1;
				lastStarStr = strPtr;
			}
			else{
				fgets(fch, 2, file);
				if(*sch == *fch){
					dynamicPtr++;
				}
				else{
					if(!inStar){
						strPtr = MAXsTRfILE;
						continue;
					}
					lastStarPtr++;
					strPtr = lastStarStr;
					dynamicPtr = lastStarPtr;
					if(lastStarPtr >= MAXfILE){
						strPtr = MAXsTRfILE;
						continue;
					}
					fseek(str_file, strPtr + 1, SEEK_SET);
					fseek(file, lastStarPtr, SEEK_SET);
					fgets(fch, 2, file);
					if(*fch == ' ' || *fch == '\0' || *fch == '\n' || *fch == EOF){
						inStar = 0;
					}
					fseek(file,-1,SEEK_CUR);
				}
			}
			if(strPtr == MAXsTRfILE - 1){
				lenghtInFile = dynamicPtr - filePtr;
				fseek(str_file, strPtr, SEEK_SET);
				fgets(sch, 2, str_file);
				if(*sch == 27){
					while(fgets(fch,2,file) != NULL){
						if(*fch == ' ' || *fch == '\0' || *fch == '\n' || *fch == EOF){
							break;
						}
						lenghtInFile++;
					}
					strPtr = MAXsTRfILE;
					continue;

				}
				else{
					if(!inStar){
						strPtr = MAXsTRfILE;
						continue;
					}
					lastStarPtr++;
					strPtr = lastStarStr;
					dynamicPtr = lastStarPtr;
					fseek(str_file, strPtr + 1, SEEK_SET);
					fseek(file, lastStarPtr, SEEK_SET);
					if(lastStarPtr >= MAXfILE){
						strPtr = MAXsTRfILE;
						continue;
					}
					fgets(fch, 2, file);
					if(*fch == ' ' || *fch == '\0' || *fch == '\n' || *fch == EOF){
						strPtr = MAXsTRfILE;
						continue;
					}
					fseek(file,-1,SEEK_CUR);
				}
			}
		}
		if(lenghtInFile > 0){
			char_id[counter_of_find] = filePtr;
			lenght_of_word[counter_of_find] = lenghtInFile;
			counter_of_find++;
			if(there_is_star){
				filePtr += lenghtInFile-1;
			}
		}
	}
	fclose(str_file);
	FILE *place = fopen(".ara/place", "w");
	int word_counter = 0;
	char last_char[2], the_char[2];
	fseek(file, 0, SEEK_SET);
	fgets(last_char, 2, file);
	int index = 0;
	if(char_id[0] == 0) index++;
	for(int i = 1; i < MAXfILE; i++){
		fseek(file,i,SEEK_SET);
		fgets(the_char, 2, file);
		if((*last_char==' '||*last_char=='\0'||*last_char=='\n'||*last_char==EOF)&&(*the_char!=' '&&*the_char!='\0'&&*the_char!='\n'&&*the_char!=EOF))
		{
			word_counter++;
		}
		if(char_id[index] == i){
			word_id[index] = word_counter;
			index++;
		}
		if(index >= counter_of_find) break;
		*last_char = * the_char;
	}
	if(atr == ATR_COUNT){
		fprintf(place, "%d", counter_of_find);
	}
	else if(atr == ATR_AT){
		if(at_num > counter_of_find){
			fprintf(place, "-1");
		}
		else{
			if(byword){
				fprintf(place, "%d", word_id[at_num-1]);
			}
			else{
				fprintf(place, "%d", char_id[at_num-1]);
			}
		}
	}
	else if(atr == ATR_ALL){
		for(int i = 0; i < counter_of_find; i++){
			if(byword){
				if(i == 0){
					fprintf(place, "%d", word_id[i]);
				}
				else{
					if(word_id[i] != word_id[i-1]){
						fprintf(place, ", %d", word_id[i]);
					}
				}
			}
			else{
				if(i == 0){
					fprintf(place, "%d", char_id[i]);
				}
				else{
					fprintf(place, ", %d", char_id[i]);
				}
			}
		}
	}
	fclose(file);
	fclose(place);
	if(arman < 2) cat(".ara/place", arman);
}
void replacestr(char *filename, char *str2, enum FinderType atr, int at_num)
{
	if(!strcmp(filename,".ara/.vif")) itssaved = 0;
	backup(filename);
	findstr(filename, at_num, at_num, 0, 1);
	FILE *str_file = fopen(".ara/str", "r");
	fseek(str_file, 0, SEEK_END); int MAXsTRfILE = ftell(str_file); fseek(str_file, 0, SEEK_SET);
	fclose(str_file);
	if(atr == ATR_AT && counter_of_find >= at_num){
		removestr_with_char(filename, char_id[at_num-1], lenght_of_word[at_num-1]);
		insertstr_with_char(filename, char_id[at_num-1], str2);
		if(!strcmp(filename,".ara/.vif")){
			file_curser = char_id[at_num-1];
		}
	}
	else{
		int li = 0, change = 0;
		for(int i = 0; i < counter_of_find; i++){
			if(i > 0) if(char_id[i] < char_id[li]+lenght_of_word[li]) continue;
			removestr_with_char(filename, char_id[i] + change, lenght_of_word[i]);
			insertstr_with_char(filename, char_id[i] + change, str2);
			change += strlen(str2) - lenght_of_word[i];
			li = i;
			if(!strcmp(filename,".ara/.vif")){
			file_curser = char_id[0];
			}
		}
	}
	return;
}
void grep(char (*files)[1000], int n, int cMode, int lMode, int arman)
{
	FILE *place = fopen(".ara/place2", "w");
	int count = 0;
	for(int i = 0; i < n; i++){
		int last_start = -1, line_start = 0, ix = 0, fix = 0;
		findstr(files[i], ATR_ALL, 1, 0, 2);
		FILE *file = fopen(files[i],"r");
		char ch[2];
		while(fgets(ch,2,file) != NULL){
			if(fix >= counter_of_find) break;
			if(*ch == '\n') {line_start = ix+1;}
			if(char_id[fix] == ix && last_start != line_start){
				fix++;
				if(lMode){
					if(cMode){
						count++;
					}
					else{
						fprintf(place, "%s\n", files[i]);
					}
					break;
				}
				else{
					if(cMode){
						count++;
					}
					else{
						fprintf(place, "%s: ", files[i]);
						char a_line[10000];
						fseek(file, line_start, SEEK_SET);
						fgets(a_line, 10000, file);
						fprintf(place, "%s", a_line);
						fseek(file, ix, SEEK_SET);
					}
				}
			}
			ix++;
		}
		fclose(file);
	}
	if(cMode) fprintf(place, "%d\n",count);
	fclose(place);
	cat(".ara/place2", arman);
}
void file_to_file(char *file1, char *file2)
{
	FILE *from = fopen(file1, "r");
	FILE *to = fopen(file2, "w");
	char line[10000];
	while(fgets(line,10000,from) != NULL){
		fprintf(to, "%s", line);
	}
	fclose(from);
	fclose(to);
}
void undo(char *file)
{
	if(!strcmp(file,".ara/.vif")) itssaved = 0;
	char bakfile[200];
	strcpy(bakfile, file);
	strcat(bakfile, ".bk"); bakfile[0]='.'; bakfile[1]='a'; bakfile[2]='r'; bakfile[3]='a';
	file_to_file(file, ".ara/place");
	file_to_file(bakfile, file);
	file_to_file(".ara/place", bakfile);
}
void auto_indent(char *file_name)
{
	if(!strcmp(file_name,".ara/.vif")) itssaved = 0;
	backup(file_name);
	FILE *file = fopen(file_name, "r");
	FILE *place = fopen(".ara/place", "w");
	char wtbuff[10000], ch[2]; wtbuff[0] = '\0';
	int level = 0;
	while(1){
		if(fgets(ch, 2, file) == NULL) goto out;
		if(*ch == ' ' || *ch == '\t'){
			strcat(wtbuff, ch);
		}
		else if(*ch == '{'){
			level++;
			wtbuff[0] = '\0';
			fprintf(place, "{");
			/* eat whites */ while(1){if(fgets(ch, 2, file) == NULL) goto out; if(*ch != ' ' && *ch != '\t') break;}
			while(*ch == '\n')
				/* eat whites */ while(1){if(fgets(ch, 2, file) == NULL) goto out; if(*ch != ' ' && *ch != '\t') break;}
			while(*ch == '}'){
				level--;
				fprintf(place,"\n"); for(int iii = 0; iii < level; iii++) fprintf(place,"\t"); fprintf(place,"}");
				/* eat whites */ while(1){if(fgets(ch, 2, file) == NULL) goto out; if(*ch != ' ' && *ch != '\t') break;}
				while(*ch == '\n')
					/* eat whites */ while(1){if(fgets(ch, 2, file) == NULL) goto out; if(*ch != ' ' && *ch != '\t') break;}
			}
			fprintf(place,"\n"); for(int iii = 0; iii < level; iii++) fprintf(place,"\t");
			fseek(file,-1,SEEK_CUR);
		}
		else if(*ch == '}'){
			wtbuff[0] = '\0';
			while(*ch == '}'){
				level--;
				fprintf(place,"\n"); for(int iii = 0; iii < level; iii++) fprintf(place,"\t"); fprintf(place,"}");
				/* eat whites */ while(1){if(fgets(ch, 2, file) == NULL) goto out; if(*ch != ' ' && *ch != '\t') break;}
				while(*ch == '\n')
					/* eat whites */ while(1){if(fgets(ch, 2, file) == NULL) goto out; if(*ch != ' ' && *ch != '\t') break;}
			}
			fprintf(place,"\n"); for(int iii = 0; iii < level; iii++) fprintf(place,"\t");
			fseek(file,-1,SEEK_CUR);
		}
		else if(*ch == '\n'){
			wtbuff[0] = '\0';
			/* eat whites */ while(1){if(fgets(ch, 2, file) == NULL) goto out; if(*ch != ' ' && *ch != '\t') break;}
			if(*ch != '}') {fprintf(place,"\n"); for(int iii = 0; iii < level; iii++) fprintf(place,"\t");}
			fseek(file,-1,SEEK_CUR);
		}
		else{
			fprintf(place, "%s%s", wtbuff, ch); wtbuff[0] = '\0';
			/* eat whites */ while(1){if(fgets(ch, 2, file) == NULL) goto out; if(*ch != ' ' && *ch != '\t') break; strcat(wtbuff, ch);}
			if(*ch == '{'){
				fprintf(place, " ");
				wtbuff[0] = '\0';
			}
			fseek(file,-1,SEEK_CUR);
		}
	}
	out:
	fclose(file);
	fclose(place);
	file_to_file(".ara/place", file_name);
}
void compare(char *file_1, char *file_2, int arman)
{
	FILE *file1 = fopen(file_1, "r");
	FILE *file2 = fopen(file_2, "r");
	FILE *place = fopen(".ara/place", "w");
	char line1[10000], line2[10000]; int line = 1;
	int number_of_lines1 = 0, number_of_lines2 = 0;
	while(fgets(line1,10000,file1) != NULL) number_of_lines1++;
	while(fgets(line2,10000,file2) != NULL) number_of_lines2++;
	fseek(file1,0,SEEK_SET); fseek(file2,0,SEEK_SET);
	while(1){
		char *try1 = fgets(line1,10000,file1);
		char *try2 = fgets(line2,10000,file2);
		if(try1 == NULL && try2 == NULL) break;
		if(try2 == NULL){
			fprintf(place, "<<<<<<<<<<<< #%d - #%d <<<<<<<<<<<<\n", line, number_of_lines1);
			while(fgets(line1,10000,file1) != NULL){
				fprintf(place, "%s", line1);
			}
			break;
		}
		if(try1 == NULL){
			fprintf(place, ">>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>\n", line, number_of_lines2);
			while(fgets(line2,10000,file2) != NULL){
				fprintf(place, "%s", line2);
			}
			break;
		}
		if(strcmp(line1,line2)){
			fprintf(place, "============ #%d ============\n%s%s", line, line1, line2);
		}
		line++;
	}
	fclose(file1);
	fclose(file2);
	fclose(place);
	cat(".ara/place", arman);
}
void tree_help(char *theAdd, int level, int maxlevel, FILE *place)
{
	if(level == maxlevel) return;
	char add[1000];
	struct dirent *ldir;
	DIR *dir = opendir(theAdd);
	if(!dir) return;
	while((ldir=readdir(dir)) != NULL)
	{
		if(ldir->d_name[0] == '.') continue;
		for(int i = 0; i < level; i++) fprintf(place, "│   ");
		fprintf(place, "├── %s\n", ldir->d_name);
		strcpy(add, theAdd);
		strcat(add, "/");
		strcat(add, ldir->d_name);
		tree_help(add, level+1, maxlevel, place);
	}
}
void tree(char *theAdd, int level, int maxlevel, int arman)
{
	FILE *place = fopen(".ara/place","w");
	tree_help(theAdd, level, maxlevel, place);
	fclose(place);
	cat(".ara/place", arman);
	itssaved = 0;
}
void antitab(char *file_name)
{
	FILE *file = fopen(file_name, "r");
	FILE *place = fopen(".ara/place", "w");
	int spacecounter = 4;
	char ch[2];
	while(fgets(ch,2,file) != NULL){
		if(*ch == '\t'){
			for(int i = 0; i < spacecounter; i++) fprintf(place," ");
			spacecounter = 4;
		}
		else{
			fprintf(place,"%c",*ch);
			if(*ch == '\n') spacecounter = 4;
			else spacecounter = (spacecounter + 2) % 4 + 1;
		}
	}
	fclose(place);
	fclose(file);
	file_to_file(".ara/place", file_name);
}
void show(int number_of_cols, int number_of_rows)
{
	char the_line[1000];
	FILE *file = fopen(".ara/.vif", "r");
	fseek(file,0,SEEK_END);
	file_curser = max(file_curser, 0);
	file_curser = min(file_curser, ftell(file));
	fseek(file,0,SEEK_SET);
	int line_counter = 0, real_line_counter = 1, newline = 1, how_many_char_we_seened = 0;
	while(fgets(the_line, number_of_cols - 4, file) != NULL){
		if(line_counter >= upper_line && line_counter < upper_line + number_of_rows - 2){
			if(newline){
				attron(COLOR_PAIR(2)); move(line_counter - upper_line, 0); printw("%4d", real_line_counter%10000);
			}
			attron(COLOR_PAIR(1)); move(line_counter - upper_line, 5); printw("%s", the_line);
		}
		if(file_curser >= how_many_char_we_seened && file_curser < how_many_char_we_seened + strlen(the_line)){
			curser_pos[0] = line_counter - upper_line;
			curser_pos[1] = file_curser - how_many_char_we_seened + 5;
		}
		how_many_char_we_seened += strlen(the_line);
		if(the_line[strlen(the_line)-1] == '\n') {newline = 1; real_line_counter++;}
		else{newline = 0;}
		line_counter++;
	}
	while(line_counter < upper_line + number_of_rows - 2){
		attron(COLOR_PAIR(2)); move(line_counter - upper_line, 0); printw("   ~");
		attron(COLOR_PAIR(1)); for(int i = 5; i < number_of_cols; i++) printw(" ");
		line_counter++;
	}
	fclose(file);
	if(vif_state == NORMAL){
		if(curser_pos[0] < 4 && upper_line != max(0,upper_line+curser_pos[0]-4)){
			upper_line = max(0,upper_line+curser_pos[0]-4);
			show(number_of_cols, number_of_rows);
		}
		else if(curser_pos[0] > number_of_rows - 6){
			upper_line += curser_pos[0] + 6 - number_of_rows;
			show(number_of_cols, number_of_rows);
		}
	}
}
void normalmoveh()
{
	if(file_curser == 0) return;
	FILE *file = fopen(".ara/.vif", "r");
	char ch[2];
	for(int i = 0; (i < file_curser) && (fgets(ch,2,file) != NULL); i++) {}
	if(*ch != '\n') file_curser--;
	fclose(file);
}
void normalmovel()
{
	FILE *file = fopen(".ara/.vif", "r");
	char ch[2];
	for(int i = -1; (i < file_curser) && (fgets(ch,2,file) != NULL); i++) {}
	if(*ch != '\n') file_curser++;
	fclose(file);
}
void normalmovek(int number_of_cols)
{
	char the_line[1000];
	FILE *file = fopen(".ara/.vif", "r");
	int startlinea = -1, lengthlinea = -1, startlineb = 0, lengthlineb = -1;
	while(fgets(the_line, number_of_cols - 4, file) != NULL){
		lengthlinea = lengthlineb;
		lengthlineb = strlen(the_line);
		if(file_curser >= startlineb && file_curser < startlineb + lengthlineb && startlinea != -1){
			file_curser = startlinea + min(file_curser - startlineb, lengthlinea - 1);
			if(curser_pos[0] < 5) upper_line = max(upper_line - 1, 0);
			break;
		}
		startlinea = startlineb;
		startlineb = (int)ftell(file);
	}
	fclose(file);
}
void normalmovej(int number_of_cols, int number_of_rows)
{
	char the_line[1000];
	FILE *file = fopen(".ara/.vif", "r");
	int startlinea = -1, lengthlinea = -1, startlineb = 0, lengthlineb = -1;
	while(fgets(the_line, number_of_cols - 4, file) != NULL){
		lengthlinea = lengthlineb;
		lengthlineb = strlen(the_line);
		if(file_curser >= startlinea && file_curser < startlinea + lengthlinea && startlinea != -1){
			file_curser = startlineb + min(file_curser - startlinea, lengthlineb - 1);
			if(curser_pos[0] == number_of_rows - 7) upper_line++;
			break;
		}
		startlinea = startlineb;
		startlineb = (int)ftell(file);
	}
	fclose(file);
}
int phase1(char * command_line, char * error_line, char *name)
{
	char command_word[50];
	int llll = 0;
	while(1){
		over:
		while(*command_line == ' ' || *command_line == '\n') command_line++;
		while(strlen(command_line) < 1){
			return 0;
		}
		llll++;
		sscanf(command_line, "%50s", command_word);
		command_line += strlen(command_word);
		if(!strcmp(command_word,"exit") || !strcmp(command_word,"x"))
		{
			return -13;
		}
		else if(!strcmp(command_word,"createfile"))
		{
			sscanf(command_line, "%50s", command_word);
			command_line += strlen(command_word) + 1;
			if(!strcmp(command_word,"--file")){
				corr_read(command_line);
				command_line += the_length + 1;
				char Address[1000];
				strcpy(Address, corr_read_out);
				createfile(Address);
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
			char dir[1000]; int line, position; dir[0] = '\0';
			if(!strcmp(command_word,"--file")){
				corr_read(command_line);
				strcpy(dir, corr_read_out);
				command_line += the_length + 1;
				sscanf(command_line, "%50s", command_word);
				command_line += strlen(command_word) + 1; while(*command_line == ' ') command_line++;
			}
			if(*dir == '\0') strcpy(dir,".ara/.vif");
			////
			FILE *testfile = fopen(dir, "r");
			if(testfile == NULL){
				error_line[0] = '\0'; strcat(error_line, dir); strcat(error_line," : there is no such file");
				return -1;
				*command_line = '\0';
			}
			fclose(testfile);
			if(!strcmp(command_word,"--str")){
				corr_read(command_line);
				FILE *file = fopen(".ara/str","w");
				fprintf(file, "%s", corr_read_out);
				fclose(file);
				command_line += the_length + 1; while(*command_line == ' ') command_line++;
			}
			sscanf(command_line, "%50s", command_word);
			if(!strcmp(command_word,"--pos")){
				command_line += strlen(command_word); while(*command_line == ' ') command_line++;
				sscanf(command_line, "%d:%d", &line, &position);
				char somestringhere[100];
				sprintf(somestringhere, "%d:%d", line, position);
				command_line += strlen(somestringhere);  while(*command_line == ' ') command_line++;
				insertstr(dir,line,position);
				if(!strcmp(dir,".ara/.vif")) antitab(dir);
			}
			else{
				error_line[0] = '\0'; strcat(error_line,"put position after --pos");
				*command_line = '\0';
			}
		}
		else if(!strcmp(command_word,"cat"))
		{
			while(*command_line == ' ') command_line++;
			sscanf(command_line, "%50s", command_word);
			command_line += strlen(command_word);  while(*command_line == ' ') command_line++;
			if(!strcmp(command_word,"--file")){
				char dir[1000];
				corr_read(command_line);
				strcpy(dir, corr_read_out);
				command_line += the_length;  while(*command_line == ' ') command_line++;
				sscanf(command_line, "%50s", command_word);
				////
				FILE *testfile = fopen(dir, "r");
				if(testfile == NULL){
					printf("%s : there is no such file \n", dir);
					*command_line = '\0';
				}
				else{
					fclose(testfile);
					if(!strcmp(command_word,"=D")){
						command_line += strlen(command_word);  while(*command_line == ' ') command_line++;
						FILE *file = fopen("kkkk", "w"); fprintf(file,"thes cmmndln is\"%s\"\n",command_line); fclose(file);
						cat(dir,1);
					}
					else{
						cat(dir,0);
					}
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
			char dir[1000], bfward = 6; int line, position, size; dir[0] = '\0';
			if(!strcmp(command_word,"--file")){
				corr_read(command_line);
				strcpy(dir, corr_read_out);
				command_line += the_length + 1;
				sscanf(command_line, "%50s", command_word);
				command_line += strlen(command_word) + 1;
			}
			if(*dir == '\0') strcpy(dir, ".ara/.vif");
			FILE *testfile = fopen(dir, "r");
			if(testfile == NULL){
				error_line[0] = '\0'; strcat(error_line, dir); strcat(error_line," : there is no such file");
				return -1;
				*command_line = '\0';
			}
			fclose(testfile);
			if(!strcmp(command_word,"--pos")){
				sscanf(command_line, "%d:%d", &line, &position);
				char somestringhere[100];
				sprintf(somestringhere, "%d:%d", line, position);
				command_line += strlen(somestringhere) + 1; while(*command_line == ' ') command_line++;
				sscanf(command_line, "%50s", command_word);
				command_line += strlen(command_word); while(*command_line == ' ') command_line++;
				if(!strcmp(command_word,"-size")){
					sscanf(command_line, "%d", &size);
					sprintf(somestringhere, "%d", size);
					command_line += strlen(somestringhere); while(*command_line == ' ') command_line++;
					if(-1 != sscanf(command_line, "-%c", &bfward))
					{
						if(bfward == 'b'|| bfward == 'f'){
							command_line += 2; while(*command_line == ' ') command_line++;
							removestr(dir,line,position,size,bfward);
						}
						else{
							error_line[0] = '\0'; strcat(error_line,"sellect backward or forward using flags: -b -f");
							*command_line = '\0';
						}
					}
					else{
						error_line[0] = '\0'; strcat(error_line,"sellect backward or forward using flags: -b -f");
						*command_line = '\0';
					}
				}
				else{
					error_line[0] = '\0'; strcat(error_line,"put size after -size");
					*command_line = '\0';
				}
			}
			else{
				error_line[0] = '\0'; strcat(error_line,"put position after --pos");
				*command_line = '\0';
			}
		}
		else if(!strcmp(command_word,"copystr"))
		{
			sscanf(command_line, "%50s", command_word);
			command_line += strlen(command_word) + 1; while(*command_line == ' ') command_line++;
			char dir[1000], bfward; int line, position, size; dir[0] = '\0';
			if(!strcmp(command_word,"--file")){
				corr_read(command_line);
				strcpy(dir, corr_read_out);
				command_line += the_length + 1; while(*command_line == ' ') command_line++;
				sscanf(command_line, "%50s", command_word);
				command_line += strlen(command_word) + 1; while(*command_line == ' ') command_line++;
			}
			if(*dir == '\0') strcpy(dir, ".ara/.vif");
			FILE *testfile = fopen(dir, "r");
			if(testfile == NULL){
				error_line[0] = '\0'; strcat(error_line, dir); strcat(error_line," : there is no such file");
				*command_line = '\0';
			}
			else{
				fclose(testfile);
				if(!strcmp(command_word,"--pos")){
					sscanf(command_line, "%d:%d", &line, &position);
					char somestringhere[100];
					sprintf(somestringhere, "%d:%d", line, position);
					command_line += strlen(somestringhere) + 1; while(*command_line == ' ') command_line++;
					sscanf(command_line, "%50s", command_word);
					command_line += strlen(command_word) + 1; while(*command_line == ' ') command_line++;
					if(!strcmp(command_word,"-size")){
						sscanf(command_line, "%d", &size);
						sprintf(somestringhere, "%d", size);
						command_line += strlen(somestringhere) + 1; while(*command_line == ' ') command_line++;
						if(-1 != sscanf(command_line, "-%c", &bfward))
						{
							if(bfward == 'b'|| bfward == 'f'){
								command_line += 3;
								copystr(dir,line,position,size,bfward);
							}
							else{
								error_line[0] = '\0'; strcat(error_line,"sellect backward or forward using flags: -b -f");
								*command_line = '\0';
							}
						}
						else{
							error_line[0] = '\0'; strcat(error_line,"sellect backward or forward using flags: -b -f");
							*command_line = '\0';
						}
					}
					else{
						error_line[0] = '\0'; strcat(error_line,"put size after -size");
						*command_line = '\0';
					}
				}
				else{
					error_line[0] = '\0'; strcat(error_line,"put position after --pos");
					*command_line = '\0';
				}
			}
		}
		else if(!strcmp(command_word,"cutstr"))
		{
			sscanf(command_line, "%50s", command_word);
			command_line += strlen(command_word) + 1; while(*command_line == ' ') command_line++;
			char dir[1000], bfward; int line, position, size; dir[0] = '\0';
			if(!strcmp(command_word,"--file")){
				corr_read(command_line);
				strcpy(dir, corr_read_out);
				command_line += the_length + 1; while(*command_line == ' ') command_line++;
				sscanf(command_line, "%50s", command_word);
				command_line += strlen(command_word) + 1; while(*command_line == ' ') command_line++;
			}
			if(*dir == '\0') strcpy(dir, ".ara/.vif");
			FILE *testfile = fopen(dir, "r");
			if(testfile == NULL){
				error_line[0] = '\0'; strcat(error_line, dir); strcat(error_line," : there is no such file");
				*command_line = '\0';
			}
			else{
				fclose(testfile);
				if(!strcmp(command_word,"--pos")){
					sscanf(command_line, "%d:%d", &line, &position);
					char somestringhere[100];
					sprintf(somestringhere, "%d:%d", line, position);
					command_line += strlen(somestringhere) + 1; while(*command_line == ' ') command_line++;
					sscanf(command_line, "%50s", command_word);
					command_line += strlen(command_word) + 1; while(*command_line == ' ') command_line++;
					if(!strcmp(command_word,"-size")){
						sscanf(command_line, "%d", &size);
						sprintf(somestringhere, "%d", size);
						command_line += strlen(somestringhere) + 1; while(*command_line == ' ') command_line++;
						if(-1 != sscanf(command_line, "-%c", &bfward))
						{
							if(bfward == 'b'|| bfward == 'f'){
								command_line += 3;
								copystr(dir,line,position,size,bfward);
								removestr(dir,line,position,size,bfward);
							}
							else{
								error_line[0] = '\0'; strcat(error_line,"sellect backward or forward using flags: -b -f");
								*command_line = '\0';
							}
						}
						else{
							error_line[0] = '\0'; strcat(error_line,"sellect backward or forward using flags: -b -f");
							*command_line = '\0';
						}
					}
					else{
						error_line[0] = '\0'; strcat(error_line,"put size after -size");
						*command_line = '\0';
					}
				}
				else{
					error_line[0] = '\0'; strcat(error_line,"put position after --pos");
					*command_line = '\0';
				}
			}
		}
		else if(!strcmp(command_word,"pastestr"))
		{
			sscanf(command_line, "%50s", command_word);
			command_line += strlen(command_word) + 1;  while(*command_line == ' ') command_line++;
			char dir[1000]; int line, position; dir[0] = '\0';
			if(!strcmp(command_word,"--file")){
				corr_read(command_line);
				strcpy(dir, corr_read_out);
				command_line += the_length + 1; while(*command_line == ' ') command_line++;
			}
			if(*dir == '\0') strcpy(dir, ".ara/.vif");
			FILE *testfile = fopen(dir, "r");
			if(testfile == NULL){
				error_line[0] = '\0'; strcat(error_line, dir); strcat(error_line," : there is no such file");
				*command_line = '\0';
			}
			else{
				fclose(testfile);
				FILE *file = fopen("kkk", "w"); fprintf(file,"the command line was \"%s\"\n", command_line); fclose(file);
				if(!strcmp(command_word,"--pos")){
					sscanf(command_line, "%d:%d", &line, &position);
					char somestringhere[100];
					sprintf(somestringhere, "%d:%d", line, position);
					command_line += strlen(somestringhere); while(*command_line == ' ') command_line++;
					pastestr(dir,line,position);
				}
				else{
					error_line[0] = '\0'; strcat(error_line,"put position after --pos");
					*command_line = '\0';
				}
			}
		}
		else if(!strcmp(command_word,"find"))
		{
			int theOut;
			while(*command_line == ' ') command_line++;
			sscanf(command_line, "%50s", command_word);
			command_line += strlen(command_word); while(*command_line == ' ') command_line++;
			if(!strcmp(command_word,"--str")){
				incorr_read(command_line);
				command_line += the_length; while(*command_line == ' ') command_line++;
				sscanf(command_line, "%50s", command_word);
				command_line += strlen(command_word) + 1; while(*command_line == ' ') command_line++;
			}
			char dir[1000]; int atr_count = 0, atr_at = 0, at_num = 1, atr_byword = 0, atr_all = 0, arman = 0;
			dir[0] = '\0';
			if(!strcmp(command_word,"--file")){
				corr_read(command_line);
				strcpy(dir, corr_read_out);
				command_line += the_length; while(*command_line == ' ') command_line++;
				theOut = sscanf(command_line, "%50s", command_word);
				command_line += strlen(command_word); while(*command_line == ' ') command_line++;
			}
			if(*dir == '\0') strcpy(dir, ".ara/.vif");
			FILE *testfile = fopen(dir, "r");
			if(testfile == NULL){
				error_line[0] = '\0'; strcat(error_line, dir); strcat(error_line," : there is no such file");
				*command_line = '\0';
			}
			else{
				fclose(testfile);
				while(1){
					if(theOut == -1) break;
					if(!strcmp(command_word,"-count")){
						atr_count = 1;
					}
					else if(command_word[0]=='-' && command_word[1]=='a' && command_word[2]=='t'){
						atr_at = 1;
						sscanf(command_word,"-at%d",&at_num);
						while(*command_line == ' ') command_line++;
					}
					else if(!strcmp(command_word,"-byword")){
						atr_byword = 1;
					}
					else if(!strcmp(command_word,"-all")){
						atr_all = 1;
						FILE *file = fopen("kkk", "w"); fprintf(file,"the cmmndwd ffffffffff is\"%s\"\n",command_word); fclose(file);
					}
					else if(!strcmp(command_word,"=D")){
						arman = 1;
						break;
					}
					else break;
					theOut = sscanf(command_line, "%50s", command_word);
					if(theOut == -1) break;
					command_line += strlen(command_word); while(*command_line == ' ') command_line++;
				}
				if(atr_all + atr_at + atr_count > 1){
					error_line[0] = '\0'; strcat(error_line,"you can use at most one of -count , -at or -all");
					*command_line = '\0';
				}
				else{
					enum FinderType findertype = ATR_AT;
					if(atr_count) findertype = ATR_COUNT;
					else if(atr_all) findertype = ATR_ALL;
					findstr(dir,findertype,at_num,atr_byword,arman);
				}
			}
		}
		else if(!strcmp(command_word,"replace"))
		{
			while(*command_line == ' ') command_line++;
			sscanf(command_line, "%50s", command_word);
			command_line += strlen(command_word); while(*command_line == ' ') command_line++;
			if(!strcmp(command_word,"--str1")){
				incorr_read(command_line);
				command_line += the_length; while(*command_line == ' ') command_line++;
				sscanf(command_line, "%50s", command_word);
				command_line += strlen(command_word); while(*command_line == ' ') command_line++;
			}
			if(!strcmp(command_word,"--str2")){
				char str2[1000];
				corr_read(command_line);
				strcpy(str2, corr_read_out);
				command_line += the_length; while(*command_line == ' ') command_line++;
				while(*command_line == ' ') command_line++;
				sscanf(command_line, "%50s", command_word);
				command_line += strlen(command_word); while(*command_line == ' ') command_line++;
				char dir[1000]; int atr_at = 0, at_num = 1, atr_all = 0; dir[0] = '\0';
				if(!strcmp(command_word, "--file")){
					corr_read(command_line);
					strcpy(dir, corr_read_out);
					command_line += the_length; while(*command_line == ' ') command_line++;
				}
				if(*dir == '\0') strcpy(dir, ".ara/.vif");
				FILE *testfile = fopen(dir, "r");
				if(testfile == NULL){
					error_line[0] = '\0'; strcat(error_line, dir); strcat(error_line," : there is no such file");
					*command_line = '\0';
				}
				else{
					fclose(testfile);
					while(1){
						if(command_word[0]=='-' && command_word[1]=='a' && command_word[2]=='t'){
							atr_at = 1;
							sscanf(command_word,"-at%d",&at_num);
							while(*command_line == ' ') command_line++;
						}
						else if(!strcmp(command_word,"-all")){
							atr_all = 1;
						}
						else break;
						int theOut = sscanf(command_line, "%50s", command_word);
						if(theOut == -1) break;
						command_line += strlen(command_word); while(*command_line == ' ') command_line++;
					}
					if(atr_all + atr_at > 1){
						error_line[0] = '\0'; strcat(error_line, "you can use at most one of -at or -all");
						*command_line = '\0';
					}
					else{
						enum FinderType findertype = ATR_AT;
						if(atr_all) findertype = ATR_ALL;
						replacestr(dir, str2, findertype, at_num);
					}
				}
			}
			else{
				error_line[0] = '\0'; strcat(error_line, "please put an string after str2");
				*command_line = '\0';
			}
		}
		else if(!strcmp(command_word,"grep"))
		{
			while(*command_line == ' ') command_line++;
			sscanf(command_line, "%50s", command_word);
			command_line += strlen(command_word) + 1;
			while(*command_line == ' ') command_line++;
			int cOption = 0, lOption = 0;
			if(!strcmp(command_word,"-l")){
				lOption = 1;
				while(*command_line == ' ') command_line++;
				sscanf(command_line, "%50s", command_word);
				command_line += strlen(command_word) + 1;
				while(*command_line == ' ') command_line++;
			}
			if(!strcmp(command_word,"-c")){
				cOption = 1;
				while(*command_line == ' ') command_line++;
				sscanf(command_line, "%50s", command_word);
				command_line += strlen(command_word) + 1;
				while(*command_line == ' ') command_line++;
			}
			if(!strcmp(command_word,"-l")){
				lOption = 1;
				while(*command_line == ' ') command_line++;
				sscanf(command_line, "%50s", command_word);
				command_line += strlen(command_word) + 1;
				while(*command_line == ' ') command_line++;
			}
			if(!strcmp(command_word,"--str")){
				incorr_read(command_line);
				command_line += the_length;
				while(*command_line == ' ') command_line++;
				sscanf(command_line, "%50s", command_word);
				command_line += strlen(command_word) + 1;
				while(*command_line == ' ') command_line++;
			}
			if(!strcmp(command_word,"--files")){
				char dir[50][1000]; int file_counter = 0, arman = 0;
				while(*command_line != '\n' && *command_line != '\0'){
					corr_read(command_line);
					command_line += the_length + 1; while(*command_line == ' ') command_line++;
					if(!strcmp(corr_read_out,"=D")){
						arman = 1;
						break;
					}
					else{
						strcpy(dir[file_counter], corr_read_out);
						file_counter++;
						////
						FILE *testfile = fopen(dir[file_counter], "r");
						if(testfile == NULL){
							error_line[0] = '\0'; strcat(error_line, dir[file_counter]); strcat(error_line," : there is no such file");
							*command_line = '\0';
							goto over;
						}
						else{
							fclose(testfile);
						}
					}
				}
				grep(dir, file_counter, cOption, lOption, arman);
			}
			else{
				printf("%s\n", "put address after --files");
				*command_line = '\0';
			}
		}
		else if(!strcmp(command_word,"undo"))
		{
			sscanf(command_line, "%50s", command_word);
			command_line += strlen(command_word) + 1;
			char Address[1000]; Address[0] = '\0';
			if(!strcmp(command_word,"--file")){
				corr_read(command_line);
				command_line += the_length + 1;
				strcpy(Address, corr_read_out);
			}
			if(*Address == '\0') strcpy(Address, ".ara/.vif");
			FILE *testfile = fopen(Address, "r");
			if(testfile == NULL){
				error_line[0] = '\0'; strcat(error_line, Address); strcat(error_line," : there is no such file");
				*command_line = '\0';
			}
			else{
				fclose(testfile);
				undo(Address);
			}
		}
		else if(!strcmp(command_word,"auto-indent"))
		{
			sscanf(command_line, "%50s", command_word);
			command_line += strlen(command_word) + 1;
			char Address[1000]; Address[0] = '\0';
			if(!strcmp(command_word,"--file")){
				corr_read(command_line);
				command_line += the_length + 1;
				strcpy(Address, corr_read_out);
			}
			if(*Address == '\0') strcpy(Address, ".ara/.vif");
			FILE *testfile = fopen(Address, "r");
			if(testfile == NULL){
				error_line[0] = '\0'; strcat(error_line, Address); strcat(error_line," : there is no such file");
				*command_line = '\0';
			}
			else{
				fclose(testfile);
				auto_indent(Address);
			}
			if(!strcmp(Address,".ara/.vif")) antitab(Address);
		}
		else if(!strcmp(command_word,"compare"))
		{
			char dir1[1000], dir2[1000]; int arman = 0;
			corr_read(command_line);
			strcpy(dir1, corr_read_out);
			command_line += the_length + 1; while(*command_line == ' ') command_line++;
			corr_read(command_line);
			strcpy(dir2, corr_read_out);
			command_line += the_length + 1; while(*command_line == ' ') command_line++;
			sscanf(command_line, "%50s", command_word);
			command_line += strlen(command_word) + 1; while(*command_line == ' ') command_line++;
			if(!strcmp(command_word,"=D")) arman = 1;
			////
			FILE *testfile = fopen(dir1, "r");
			if(testfile == NULL){
				error_line[0] = '\0'; strcat(error_line, dir1); strcat(error_line," : there is no such file");
				*command_line = '\0';
			}
			else{
				fclose(testfile);
				////
				FILE *testfile = fopen(dir2, "r");
				if(testfile == NULL){
					error_line[0] = '\0'; strcat(error_line, dir2); strcat(error_line," : there is no such file");
					*command_line = '\0';
				}
				else{
					fclose(testfile);
					compare(dir1, dir2, arman);
				}
			}
		}
		else if(!strcmp(command_word,"tree"))
		{
			while(*command_line == ' ') command_line++;
			int depth, arman = 0;
			sscanf(command_line,"%d", &depth);
			char somestringhere[100]; sprintf(somestringhere, "%d", depth); command_line += strlen(somestringhere);
			while(*command_line == ' ') command_line++;
			int yes = sscanf(command_line, "%50s", command_word);
			if(yes != -1){
				command_line += strlen(command_word); while(*command_line == ' ' || *command_line == '\n') command_line++;
				if(!strcmp(command_word,"=D")) arman = 1;
			}
			tree("root",0,depth,arman);
		}
		else if(!strcmp(command_word,"save"))
		{
			if(*name == '\0'){
				error_line[0] = '\0'; strcat(error_line,"No name! You can use \"saveas\" to choosing name and saving.");
				*command_line = '\0';
			}
			else{
				char dir[1000]; strcpy(dir, "root/"); strcat(dir,name);
				file_to_file(".ara/.vif", dir);
				itssaved = 1;
			}
		}
		else if(!strcmp(command_word,"saveas"))
		{
			while(*command_line == ' ') command_line++;
			char dir[1000]; strcpy(dir, "root/");
			corr_read(command_line);
			strcpy(name, corr_read_out);
			strcat(dir, name);
			createfileSilent = 1; createfile(dir);
			file_to_file(".ara/.vif", dir);
			itssaved = 1;
			command_line += the_length; while(*command_line == ' ') command_line++;
		}
		else if(!strcmp(command_word,"open"))
		{
			if(*name != '\0'){
				char dir[1000]; strcpy(dir, "root/"); strcat(dir,name);
				file_to_file(".ara/.vif", dir);
			}
			while(*command_line == ' ') command_line++;
			char dir[1000]; strcpy(dir, "root/");
			corr_read(command_line);
			strcpy(name, corr_read_out);
			strcat(dir, name);
			file_to_file(dir, ".ara/.vif");
			command_line += the_length; while(*command_line == ' ') command_line++;
			antitab(".ara/.vif");
			itssaved = 1;
		}
		else{
			error_line[0] = '\0'; strcat(error_line, " Invalid command : "); strcat(error_line, command_word);
			*command_line = '\0';
		}
	}
	return 0;
}
int tellmewruwillshowitout[2];
void tellmewruwillshowit(int fcurser, int number_of_cols, int number_of_rows)
{
	char the_line[1000];
	FILE *file = fopen(".ara/.vif", "r");
	int line_counter = 0, real_line_counter = 1, newline = 1, how_many_char_we_seened = 0;
	while(fgets(the_line, number_of_cols - 4, file) != NULL){
		if(fcurser >= how_many_char_we_seened && fcurser < how_many_char_we_seened + strlen(the_line)){
			tellmewruwillshowitout[0] = line_counter - upper_line;
			tellmewruwillshowitout[1] = fcurser - how_many_char_we_seened + 5;
		}
		how_many_char_we_seened += strlen(the_line);
		line_counter++;
	}
	fclose(file);
}
void highlighter(int c1, int c2, int number_of_cols, int number_of_rows)
{
	int a = min(c1, c2), b = max(c1, c2);
	tellmewruwillshowit(a, number_of_cols, number_of_rows); int ya = tellmewruwillshowitout[0], xa = tellmewruwillshowitout[1];
	tellmewruwillshowit(b, number_of_cols, number_of_rows); int yb = tellmewruwillshowitout[0], xb = tellmewruwillshowitout[1];
	for(int i = max(ya,0); i <= min(yb, number_of_rows - 3); i++){
		for(int j = 5; j < number_of_cols; j++){
			if((i>ya || j>=xa) && (i<yb || j<xb)){
				char aChar = mvinch(i,j);
				move(i,j); attron(COLOR_PAIR(6)); printw("%c",aChar);
			}
		}
	}
}

int main(){
	name[0] = '\0';
	FILE *file = fopen(".ara/.vif", "w"); fprintf(file,"\n"); fclose(file);
	char COMMAND_LINE[1000]; COMMAND_LINE[0] = '\0';
	char error_line[1000]; error_line[0] = '\0';
	char command_char;
	struct winsize win_size;
	setlocale(LC_ALL, "");
	
	initscr();
	start_color();
	// nodelay(stdscr, 1);
	noecho();
	
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_WHITE, COLOR_BLUE);
	init_pair(4, COLOR_GREEN, COLOR_BLACK);
	init_pair(5, COLOR_RED, COLOR_BLACK);
	init_pair(6, COLOR_BLACK, COLOR_GREEN);

	char c; int a_delay_for_getting_char = 0;
	while(1){
		if(a_delay_for_getting_char){
			c = getch();
			a_delay_for_getting_char = 0;
		}
		else{
			a_delay_for_getting_char = 1;
			c = '\0';
		}
		clear();
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &win_size);
		show(win_size.ws_col, win_size.ws_row);
		move(win_size.ws_row - 2, 0);
		attron(COLOR_PAIR(3)); for(int i = 0; i < win_size.ws_col; i++) printw(" ");
		if(vif_state == NORMAL){
			move(win_size.ws_row - 2, 0); attron(COLOR_PAIR(3)); printw(" NORMAL %s %c", name, itssaved?'+':' ');
			move(win_size.ws_row - 1, 0); attron(COLOR_PAIR(5)); printw(error_line);
			if(c == 'h' || c == 68){
				normalmoveh();
				error_line[0] = '\0';
			}
			else if(c == 'l' || c == 67){
				normalmovel();
				error_line[0] = '\0';
			}
			else if(c == 'k' || c == 65){
				normalmovek(win_size.ws_col);
				error_line[0] = '\0';
			}
			else if(c == 'j' || c == 66){
				normalmovej(win_size.ws_col, win_size.ws_row);
				error_line[0] = '\0';
			}
			else if(c == 'i'){
				vif_state = INSERT;
				error_line[0] = '\0';
			}
			else if(c == 'p'){
				pastestr(".ara/.vif", curser_pos[0]+1, curser_pos[1]-5);
				error_line[0] = '\0';
			}
			else if(c == 'u'){
				char cmmndln[100]; strcpy(cmmndln,"undo ");
				phase1(cmmndln,error_line,name);
				error_line[0] = '\0';
			}
			else if(c == '='){
				char cmmndln[100]; strcpy(cmmndln,"auto-indent ");
				phase1(cmmndln,error_line,name);
				error_line[0] = '\0';
			}
			else if(c == 'v'){
				vif_state = VISUAL;
				vis_curser = file_curser;
				show(win_size.ws_col, win_size.ws_row);
				error_line[0] = '\0';
			}
			else if(c == ':' || c == '/')
			{
				command_char = c;
				vif_state = COMMAND;
			}
		}
		else if(vif_state == COMMAND){
			move(win_size.ws_row - 2, 0); attron(COLOR_PAIR(3)); printw(" NORMAL %s", name);
			move(win_size.ws_row - 1, 0); attron(COLOR_PAIR(4)); printw(" %c%s", command_char, COMMAND_LINE);
			if(c == 10){
				if(command_char == ':'){
					error_line[0] = '\0';
					int output = phase1(COMMAND_LINE, error_line, name);
					if(output == -13) break;
					COMMAND_LINE[0] = '\0';
					vif_state = NORMAL;
				}
				else if(command_char == '/'){
					error_line[0] = '\0';
					vif_state = NORMAL;
					incorr_read(COMMAND_LINE);
					findstr(".ara/.vif", ATR_AT, 0, 0, 1);
					int hlights[10000][2]; int j = counter_of_find - 1;
					clear();
					show(win_size.ws_col,win_size.ws_row);
					for(int i = 0; i < counter_of_find; i++){
						hlights[i][0] = char_id[i];
						hlights[i][1] = char_id[i] + lenght_of_word[i];
						if(i > 0 && file_curser >= char_id[i-1] && file_curser < char_id[i]){
							j = i - 1;
						}
						highlighter(hlights[i][0],hlights[i][1],win_size.ws_col,win_size.ws_row);
					}
					move(win_size.ws_row - 2, 0); attron(COLOR_PAIR(3)); for(int i = 0; i < win_size.ws_col; i++) printw(" ");
					move(win_size.ws_row - 2, 0); attron(COLOR_PAIR(3)); printw(" NORMAL %s", name);
					move(curser_pos[0], curser_pos[1]);
					refresh();
					while('n' == getch()){
						j = (j+1) % counter_of_find;
						file_curser = hlights[j][0];
						clear();
						show(win_size.ws_col,win_size.ws_row);
						for(int i = 0; i < counter_of_find; i++) highlighter(hlights[i][0],hlights[i][1],win_size.ws_col,win_size.ws_row);
						move(win_size.ws_row - 2, 0); attron(COLOR_PAIR(3)); for(int i = 0; i < win_size.ws_col; i++) printw(" ");
						move(win_size.ws_row - 2, 0); attron(COLOR_PAIR(3)); printw(" NORMAL %s", name);
						move(curser_pos[0], curser_pos[1]);
						refresh();
					}
					show(win_size.ws_col,win_size.ws_row);
					COMMAND_LINE[0] = '\0';
				}
			}
			else if(c == 27){
				vif_state = NORMAL;
			}
			else if(c > 31 && c < 127){
				int sz = strlen(COMMAND_LINE); COMMAND_LINE[sz] = c; COMMAND_LINE[sz+1] = '\0';
			}
			else if(c == 127){
				int sz = strlen(COMMAND_LINE); if(sz > 0) COMMAND_LINE[sz - 1] = '\0';
			}
			curser_pos[0] = win_size.ws_row - 1; curser_pos[1] = strlen(COMMAND_LINE) + 2;
		}
		else if(vif_state == VISUAL){
			move(win_size.ws_row - 2, 0);
			printw(" VISUAL", name);
			if(c == 'h' || c == 68){
				normalmoveh();
			}
			else if(c == 'l' || c == 67){
				normalmovel();
			}
			else if(c == 'k' || c == 65){
				normalmovek(win_size.ws_col);
			}
			else if(c == 'j' || c == 66){
				normalmovej(win_size.ws_col, win_size.ws_row);
			}
			else if(c == 'd'){ // cut
				int aaa = min(file_curser, vis_curser);
				tellmewruwillshowit(aaa, win_size.ws_col, win_size.ws_row);
				copystr(".ara/.vif",tellmewruwillshowitout[0]+1,tellmewruwillshowitout[1]-5,abs(file_curser-vis_curser),'f');
				removestr(".ara/.vif",tellmewruwillshowitout[0]+1,tellmewruwillshowitout[1]-5,abs(file_curser-vis_curser),'f');
				file_curser = aaa;
				vif_state = NORMAL;
			}
			else if(c == 'y'){ // copy
				int aaa = min(file_curser, vis_curser);
				tellmewruwillshowit(aaa, win_size.ws_col, win_size.ws_row);
				copystr(".ara/.vif",tellmewruwillshowitout[0]+1,tellmewruwillshowitout[1]-5,abs(file_curser-vis_curser),'f');
				vif_state = NORMAL;
			}
			else if(c == 27){
				vif_state = NORMAL;
			}
			highlighter(vis_curser, file_curser, win_size.ws_col, win_size.ws_row);
		}
		else if(vif_state == INSERT){
			move(win_size.ws_row - 2, 0); attron(COLOR_PAIR(3)); printw(" INSERT %s", name);
			if(c > 31 && c < 127 || c == 10){
				char string[2]; string[0] = c; string[1] = '\0';
				insertstr_with_char(".ara/.vif", file_curser, string);
				file_curser++;
			}
			else if(c == 127){
				if(file_curser > 0){
					removestr_with_char(".ara/.vif", file_curser-1, 1);
					file_curser--;
				}
			}
			else if(c == 27){
				vif_state = NORMAL;
			}
		}
		// set curser in corrent pos
		move(curser_pos[0], curser_pos[1]);
		refresh();
	}
	endwin();
	
	return 0;
}