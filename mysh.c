/*
 * [YOUR NAME HERE]
 *
 * CS441/541: Project 3
 *
 */
#include "mysh.h"

int main(int argc, char * argv[]) {
  int ret;
  history = NULL;
  //j//obs = NULL;
  his_index = 0;
  his_count = 0;
    /*
     * Parse Command line arguments to check if this is an interactive or batch
     * mode run.
     */
    if( 0 != (ret = parse_args_main(argc, argv)) ) {
        fprintf(stderr, "Error: Invalid command line!\n");
        return -1;
    }

    /*
     * If in batch mode then process all batch files
     */
    if( TRUE == is_batch) {
        if( TRUE == is_debug ) {
            printf("Batch Mode!\n");
        }

        if( 0 != (ret = batch_mode()) ) {
            fprintf(stderr, "Error: Batch mode returned a failure!\n");
        }
    }
    /*
     * Otherwise proceed in interactive mode
     */
    else if( FALSE == is_batch ) {
        if( TRUE == is_debug ) {
            printf("Interactive Mode!\n");
        }

        if( 0 != (ret = interactive_mode()) ) {
            fprintf(stderr, "Error: Interactive mode returned a failure!\n");
        }
    }
    /*
     * This should never happen, but otherwise unknown mode
     */
    else {
        fprintf(stderr, "Error: Unknown execution mode!\n");
        return -1;
    }

	//int i;
	//for(i = 0; i < his_index - 1; i++){
	//	printf("%d\n", jobs[i]);
	//}
    /*
     * Display counts
     */
    printf("-------------------------------\n");
    printf("Total number of jobs               = %d\n", total_jobs);
    printf("Total number of jobs in history    = %d\n", total_history);
    printf("Total number of jobs in background = %d\n", total_jobs_bg);

    /*
     * Cleanup
     */


    return 0;
}

int parse_args_main(int argc, char **argv)
{
    int i;
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    int count = 0;
    file_line_arr = (char**)malloc(sizeof(char*) * (count * 1));
    

    /*
     * If no command line arguments were passed then this is an interactive
     * mode run.
     */

    
     i = 1;
        /*if there is more then one arg passed in, its batch mode*/
     if(argc > 1){
            /*loop through each argv*/
            while(NULL != argv[i] && strstr(argv[i], ".txt") != NULL){
		is_batch = TRUE;
                /*open the file*/
                fp = fopen(argv[i], "r");
                if(fp == NULL){
                    exit(1);
                }
                /*read each line in the current file*/
                while((read = getline(&line, &len, fp)) != -1){
                    /*increase the size of the commands array and and the line from the file to it*/
                    file_line_arr[count] = malloc(sizeof(char) * ((strlen(line) + 1)));
                    strcpy(file_line_arr[count], line);
                    count++;
                    file_line_arr = (char**) realloc(file_line_arr, (count + 1) * sizeof(char*));      
                }
                i++;
                fclose(fp);
            }
     }

    /*
     * If command line arguments were supplied then this is batch mode.
     */

    return 0;
}

int batch_mode(void)
{
    int i;
    /*
     * For each file...
     */
   
    /*loop through each line of input*/
    i = 0;
    while(file_line_arr[i] != NULL){
        printf("%s%s\n", "command: ", file_line_arr[i]);
        i++;
    }

        /*
         * Open the batch file
         * If there was an error then print a message and move on to the next file.
         * Otherwise, 
         *   - Read one line at a time.
         *   - strip off new line
         *   - parse and execute
         * Close the file
         */

    /*
     * Cleanup
     */


    return 0;
}

int interactive_mode(void)
{
	int * jobs;
    do {
        /*
         * Print the prompt
         */
	printf("mysh$ ");

        char * input = (char *)malloc(256 * sizeof(char));
	size_t len = 1024;
	getline(&input, &len, stdin);
	strtok(input, "\n");

	
	char * tmp = strdup(input);
	char * part_string = strdup(input);
	int i = 0; 
	int last_stop = 0; 
	while(tmp[i] != '\0'){
		char t = tmp[i];
		char * u = &t;
		char * job_name;	
		if(strcmp(u,"&") == 0){
			part_string = substr(strdup(tmp), last_stop, i);
			if(history == NULL){
				history = (char**)malloc(sizeof(char*));
				his_count = 1;
			} else {
				history = (char**)realloc(history, sizeof(history) * (his_count + 1));
			}
			if(jobs == NULL){
				jobs = (int*)malloc(sizeof(int));
			} else {
				jobs = (int*)realloc(jobs, sizeof(jobs) * (his_count) + 1);
				his_count++;
			}
			his_index++;
			//char * part_temp = strtok(strdup(part_string), " ");
			if(file_redir(strdup(part_string)) == 1){	
				total_history++;
				total_jobs_bg++;
				total_jobs++;
				char * part_tmp = strdup(part_string);
				char * file1 = strtok(part_tmp, ">");
				char * file2 = strtok(NULL, ">");
				job_creation(strdup(part_string), 1, file1, 1, file2, jobs); 	
			} else if(file_redir(strdup(part_string)) == 2){	
				total_history++;
				total_jobs_bg++;
				total_jobs++;
				char * part_tmp = strdup(part_string);
				char * file1 = strtok(part_tmp, "<");
				char * file2 = strtok(NULL, "<");
				job_creation(strdup(part_string), 1, file1, 1, file2, jobs); 	
			} else if(check_builtin(strtok(strdup(part_string), " ")) == 1){
				total_history++;
				if(strcmp("exit", strtok(strdup(part_string), " ")) == 0){
					return builtin_exit();
				} 
				add_history(strdup(part_string), 0, his_index-1);
				job_creation(strdup(part_string), 0, NULL, 0, NULL, jobs);
			} else {
				total_history++;
				total_jobs_bg++;
				total_jobs++;
				add_history(strdup(part_string), 1, his_index-1);
				job_creation(strdup(part_string), 1, strtok(strdup(part_string), " "), 0 , NULL, jobs);
			}
			last_stop = i + 1;		
		} else if (strcmp(u, ";") == 0){
			part_string = substr(strdup(tmp), last_stop, i);
			if(history == NULL){
				history = (char**)malloc(sizeof(char*));
				his_count = 1;
			} else {
				history = (char**)realloc(history, sizeof(history) * (his_count + 1));
			}
			if(jobs == NULL){
				jobs = (int*)malloc(sizeof(int));
			} else {
				jobs = (int*)realloc(jobs, sizeof(jobs) * (his_count) + 1);
				his_count++;
			}
			his_index++;
			add_history(strdup(part_string), 0, his_index-1);
			total_history++;
			if(file_redir(strdup(part_string)) == 1){	
				char * part_tmp = strdup(part_string);
				char * file1 = strtok(part_tmp, ">");
				char * file2 = strtok(NULL, ">");
				job_creation(strdup(part_string), 0, file1, 1, file2, jobs); 	
			} else if(file_redir(strdup(part_string)) == 2){	
				char * part_tmp = strdup(part_string);
				char * file1 = strtok(part_tmp, "<");
				char * file2 = strtok(NULL, "<");
				job_creation(strdup(part_string), 0, file1, 1, file2, jobs); 	
			} else if(check_builtin(strtok(strdup(part_string), " ")) == 1){
				if(strcmp("exit", strtok(strdup(part_string), " ")) == 0){
					return builtin_exit();
				}
				job_creation(strdup(part_string), 0, NULL, 0 , NULL, jobs);
			} else {
				total_jobs++;
				job_creation(strdup(part_string), 0, strtok(strdup(part_string), " "), 0, NULL, jobs);
			}
			last_stop = i + 1;
		}
		i++;
	}
	part_string = substr(strdup(tmp), last_stop, i);
	int leftover = get_length(strdup(part_string));
	if(leftover != 0){
		if(history == NULL){
			history = (char**)malloc(sizeof(char*));
			his_count = 1;
		} else {
			history = (char**)realloc(history, sizeof(history) * (his_count + 1));
		}
		if(jobs == NULL){
			jobs = (int*)malloc(sizeof(int));
		} else {				
			jobs = (int*)realloc(jobs, sizeof(jobs) * (his_count) + 1);
			his_count++;
		}
		his_index++;
		add_history(strdup(part_string), 0, his_index-1);
		total_history++;
		if(file_redir(strdup(part_string)) == 1){	
			char * part_tmp = strdup(part_string);
			char * file1 = strtok(strdup(part_tmp), ">");
			char * file2 = strtok(NULL, ">");
			job_creation(strdup(part_string), 0, file1, 1, file2, jobs); 	
		} else if(file_redir(strdup(part_string)) == 2){	
			char * part_tmp = strdup(part_string);
			char * file1 = strtok(part_tmp, "<");
			char * file2 = strtok(NULL, "<");
			job_creation(strdup(part_string), 0, file1, 1, file2, jobs); 	
		} else if(check_builtin(strtok(strdup(part_string), " ")) == 1){
			if(strcmp("exit", strtok(strdup(part_string), " ")) == 0){
				return builtin_exit();
			}
			job_creation(strdup(part_string), 0, NULL, 0, NULL, jobs);
		} else {
			total_jobs++;
			job_creation(strdup(part_string), 0, strtok(strdup(part_string), " "), 0 ,NULL, jobs);
		}
		printf("%d\n", jobs[his_index - 1]);
	}	 	
       
    } while( 1/* end condition */);

    /*
     * Cleanup
     */

    return 0;
}

int file_redir(char * cmmd){
	if(strchr(strdup(cmmd), '>')) return 1;
	if(strchr(strdup(cmmd), '<')) return 2;
	return 0;
}
void add_history(char * cmmd, int background, int his_size){
	if(background == 1){
		strncat(cmmd, "&", 1);
	}
	history[his_size] = cmmd;
}

void add_job(int* tmp){
	//jobs[his_index - 1] = *tmp;
}

void job_creation(char * job_name, int background, char * binary, int redirection, char * filename, int * jobs){
	//printf("Job creation: %s\n", job_name);
	job_t * loc_job = (job_t *)malloc(sizeof(job_t));
	loc_job->full_command = strdup(job_name);
	loc_job->binary = binary;
	loc_job->is_background = background;
	loc_job->redirect = redirection;
	loc_job->file_redirect = filename;
	loc_job->done = 0;
	loc_job->pid = 0;
	//add_job(loc_job);	
	launch_job(loc_job, jobs);
}

char * substr(char *src, int start, int end){
	int len = end - start;
	char * dest = (char*)malloc(sizeof(char) * (len + 1));
	int i;
	for(i = start; i < end && ((*src + i) != '\0'); i++){
		*dest = *(src + i);
		dest++;
	}
	*dest = '\0';
	return dest - len;

}

int check_builtin(char * command){
	const char * exit_cmmd = "exit";
	const char * fg = "fg";
	const char * jobs = "jobs";
	const char * history = "history";
	const char * wait = "wait";
	
	if(strcmp(command, fg) == 0 || strcmp(command, jobs) == 0 || strcmp(command, history) == 0 || strcmp(command, wait) == 0 || strcmp(command, exit_cmmd) == 0){
		return 1;
	}
	
	return 0;
}

/*
 * You will want one or more helper functions for parsing the commands 
 * and then call the following functions to execute them
 */

int launch_job(job_t * loc_job, int * jobs)
{
    /*
     * Display the job
     */
	
	const char * exit_cmmd = "exit";
	const char * fg = "fg";
	//const char * jobs = "jobs";
	const char * history = "history";
	const char * wait = "wait";

    /*
     * Launch the job in either the foreground or background
     */

    /*
     * Some accounting
     */
	//printf("%s\n", loc_job->full_command);
	if(loc_job->binary != NULL){
		//printf("%s\n", loc_job->binary);
		//return 0;
		pid_t c_pid = 0;
		int status = 0;
		char **args;
		char * a;
		int file_des;
		int out_saved = dup(STDOUT_FILENO);
		if(loc_job->redirect != 0){
			a = strtok(strdup(loc_job->full_command), ">");
		} else {
			a = strdup(loc_job->full_command);
		}
		int argc = get_length(strdup(a));
		char * tmp = strtok(a, " ");
		//printf("a before: %s\n", a);	
		args = (char **)malloc(sizeof(char*) * (argc + 1));
		//args[0] = strdup(loc_job->binary);
		int i = 0;
		while(tmp != NULL){
			args[i] = remove_Spaces(strdup(tmp));
			//printf("before:%s\n", args[i]);
			tmp = strtok(NULL, " ");
			i++;
		}
		args[i] = NULL;
		//printf("output file: %s\n", loc_job->file_redirect);	
		c_pid = fork();		
		if(c_pid < 0){
			return -1;
		} else if(c_pid == 0){
			if(loc_job->redirect == 1){
				char * nospace_file = remove_Spaces(strdup(loc_job->file_redirect));
				file_des = open(nospace_file, O_WRONLY | O_APPEND);
				dup2(file_des, STDOUT_FILENO);
				close(file_des);
				execvp(args[0], args);
			} else if (loc_job->redirect == 2){
				char * nospace_file = remove_Spaces(strdup(loc_job->file_redirect));
				file_des = open(nospace_file, O_CREAT | O_RDONLY |  O_WRONLY);
				dup2(file_des, STDIN_FILENO);
				close(file_des);
				execvp(args[0], args);
			} else {
				//printf("%s\n", args[0]);	
				execvp(args[0], args);
			}
		}  else {
			if(loc_job->is_background == 0){
				//printf("%d\n", his_index);
				//job_t * tmp = (job_t*)jobs[his_index - 1];
				//if(tmp == NULL){
				//	printf("fuck");
				//	return 0;
				//}
				//printf("%s\n", tmp->full_command);
				//return 0;
				//job_t * tmp = &jobs[his_index - 1];
				//printf("%d\n", tmp.pid);
				jobs[(his_index-1)] = waitpid(c_pid, &status, 0);
			}
		}
	} else {
		if(strcmp(history, strtok(strdup(loc_job->full_command), " ")) == 0){
			builtin_history();
		} else if (strcmp(exit_cmmd, strtok(strdup(loc_job->full_command), " ")) == 0){
			builtin_exit();
		} else if(strcmp("jobs", strtok(strdup(loc_job->full_command), " ")) == 0){
			builtin_jobs();
		}
	}
	

    return 0;
}

char * remove_Spaces(char * tmp){
	//printf("Here\n");
	int count = 0;
	int i;
	for(i = 0; tmp[i]; i++){
		if(tmp[i] != ' '){
			tmp[count++] = tmp[i];
		}
	}
	tmp[count] = '\0';
	return tmp;
}


int get_length(char * tmp){
	int count = 0;
	char * token = strtok(tmp, " ");
	while(token != NULL){
		count++;
		token = strtok(NULL, " ");
	}
	//printf("%d\n", count);
	return count;
}

char * char_after_space(char * cmmd){
	char * starting = cmmd;
	while(*starting != ' '){
		starting++;
	}
	starting++;
	return starting;
}

int builtin_exit(void)
{
    return 0;
}

int builtin_jobs(void)
{

    return 0;
}

int builtin_history(void)
{
	int i;
	for(i = 0; i < his_index; i++){
		printf("[%d] %s\n", i + 1, history[i]);
	}
	return 0;
}

int builtin_wait(void)
{

    return 0;
}

int builtin_fg(void)
{

    return 0;
}

int builtin_fg_num(int job_num)
{

    return 0;
}
