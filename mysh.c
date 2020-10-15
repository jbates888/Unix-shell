/*
 * AJ Thus, Jack Bates
 *
 * CS441/541: Project 3
 *
 */
#include "mysh.h"

//struct job_t * jobs;

int main(int argc, char * argv[]) {
  int ret;
  
    jobs = (job_t *)malloc(sizeof(job_t));
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
    do {
        /*
         * Print the prompt
         */
	printf("mysh$ ");
        /*create space for input string*/
        char * input = (char *)malloc(256 * sizeof(char));
	size_t len = 1024;
        /*get the next line from stdin*/
	getline(&input, &len, stdin);
        /*remove the next line char from the input*/
	strtok(input, "\n");
	if(strlen(strdup(input)) == 1){
		continue;
	}
	/*create a temp veriable for the input*/
	char * tmp = strdup(input);
	char * part_string = strdup(input);
	int i = 0; 
	int last_stop = 0; 
        /*loop until the and of the input is reached*/
	while(tmp[i] != '\0'){
		char t = tmp[i];
		char * u = &t;
		char * job_name;
                /*handle the backround case if & is found*/	
		if(strcmp(u,"&") == 0){
			part_string = substr(strdup(tmp), last_stop, i);
			his_index++;
			if(file_redir(strdup(part_string)) == 1){	
				total_history++;
				total_jobs_bg++;
				total_jobs++;
				char * part_tmp = strdup(part_string);
				char * file1 = strtok(part_tmp, ">");
				char * file2 = strtok(NULL, ">");
				job_creation(strdup(part_string), 1, file1, 1, file2); 	
			} else if(file_redir(strdup(part_string)) == 2){	
				total_history++;
				total_jobs_bg++;
				total_jobs++;
				char * part_tmp = strdup(part_string);
				char * file1 = strtok(part_tmp, "<");
				char * file2 = strtok(NULL, "<");
				job_creation(strdup(part_string), 1, file1, 1, file2); 	
			} else if(check_builtin(strtok(strdup(part_string), " ")) == 1){
				total_history++;
				if(strcmp("exit", strtok(strdup(part_string), " ")) == 0){
					return builtin_exit();
				} 
				job_creation(strdup(part_string), 0, " ", 0, " ");
			} else {
				total_history++;
				total_jobs_bg++;
				total_jobs++;
				job_creation(strdup(part_string), 1, strtok(strdup(part_string), " "), 0 , " ");
			}
			last_stop = i + 1;		
                        /*handle the case of more then one command on the same line*/
		} else if (strcmp(u, ";") == 0){
			part_string = substr(strdup(tmp), last_stop, i);
			his_index++;
			total_history++;
                        /*check for file redirection*/
			if(file_redir(strdup(part_string)) == 1){	
				char * part_tmp = strdup(part_string);
				char * file1 = strtok(part_tmp, ">");
				char * file2 = strtok(NULL, ">");
				job_creation(strdup(part_string), 0, file1, 1, file2); 	
			} else if(file_redir(strdup(part_string)) == 2){	
				char * part_tmp = strdup(part_string);
				char * file1 = strtok(part_tmp, "<");
				char * file2 = strtok(NULL, "<");
				job_creation(strdup(part_string), 0, file1, 1, file2); 
                                /*check if a built in command was entered*/	
			} else if(check_builtin(strtok(strdup(part_string), " ")) == 1){
				if(strcmp("exit", strtok(strdup(part_string), " ")) == 0){
					return builtin_exit();
				}
				job_creation(strdup(part_string), 0, " ", 0 , " ");
			} else {
				total_jobs++;
				job_creation(strdup(part_string), 0, strtok(strdup(part_string), " "), 0, " ");
			}
			last_stop = i + 1;
		}
		i++;
	}
	part_string = substr(strdup(tmp), last_stop, i);
	int leftover = get_length(strdup(part_string));
	if(leftover != 0){
		his_index++;
		total_history++;
		if(file_redir(strdup(part_string)) == 1){	
			char * part_tmp = strdup(part_string);
			char * file1 = strtok(strdup(part_tmp), ">");
			char * file2 = strtok(NULL, ">");
			job_creation(strdup(part_string), 0, file1, 1, file2); 	
		} else if(file_redir(strdup(part_string)) == 2){	
			char * part_tmp = strdup(part_string);
			char * file1 = strtok(part_tmp, "<");
			char * file2 = strtok(NULL, "<");
			job_creation(strdup(part_string), 0, file1, 1, file2); 	
		} else if(check_builtin(strtok(strdup(part_string), " ")) == 1){
			if(strcmp("exit", strtok(strdup(part_string), " ")) == 0){
				return builtin_exit();
			}
			job_creation(strdup(part_string), 0, " ", 0, " ");
		} else {
			total_jobs++;
			job_creation(strdup(part_string), 0, strtok(strdup(part_string), " "), 0 ," ");
		}
	}	 	
       
    } while( 1/* end condition */);

    /*
     * Cleanup
     */

    return 0;
}

/*
 *helper function for checking the type of file redirection
*/
int file_redir(char * cmmd){
	if(strchr(strdup(cmmd), '>')) return 1;
	if(strchr(strdup(cmmd), '<')) return 2;
	return 0;
}

/*
 * Helper function for adding to the history array
 */
void add_history(job_t * loc_job){
	jobs = (job_t *)realloc(jobs, sizeof(jobs) * (his_count + 1) * sizeof(job_t));
	his_count++;
	job_t * tmp = (job_t*)malloc(sizeof(job_t));
	memcpy(tmp, loc_job, sizeof(job_t));
	jobs[his_count - 1].full_command = tmp->full_command;
	jobs[his_count - 1].binary = tmp->binary;
	jobs[his_count - 1].is_background = tmp->is_background;
	jobs[his_count - 1].redirect = tmp->redirect;
	jobs[his_count - 1].file_redirect = tmp->file_redirect;
	jobs[his_count - 1].done = tmp->done;
	jobs[his_count - 1].id = 0;
}


/*
 * Function to create a job using the parameters passed in
 */
void job_creation(char * job_name, int background, char * binary, int redirection, char * filename){
        /*set the job's attributes*/
	job_t * loc_job = (job_t *)malloc(sizeof(job_t));
	loc_job->full_command = strdup(job_name);
	loc_job->binary = binary;
	loc_job->is_background = background;
	loc_job->redirect = redirection;
	loc_job->file_redirect = filename;
	loc_job->done = 0;
	loc_job->pid = 0;
	if(strcmp(job_name, "") != 0){
		add_history(loc_job);	
	}	
	launch_job(loc_job);
}

/*
 * Helper funtion to return a peice of the string between the bounds passed in
 */
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

/*
 * Helper function to return if a command is a built in command
 */
int check_builtin(char * command){
	const char * exit_cmmd = "exit";
	const char * fg = "fg";
	const char * jobsc = "jobs";
	const char * history = "history";
	const char * wait = "wait";
	
	if(strcmp(command, fg) == 0 || strcmp(command, jobsc) == 0 || strcmp(command, history) == 0 || strcmp(command, wait) == 0 || strcmp(command, exit_cmmd) == 0){
		return 1;
	}
	
	return 0;
}

/*
 * You will want one or more helper functions for parsing the commands 
 * and then call the following functions to execute them
 */

int launch_job(job_t * loc_job)
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
	if(strcmp(loc_job->binary, " ") != 0){
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
		args = (char **)malloc(sizeof(char*) * (argc + 1));
		int i = 0;
		while(tmp != NULL){
			args[i] = remove_Spaces(strdup(tmp));
			tmp = strtok(NULL, " ");
			i++;
		}
		args[i] = NULL;	
		c_pid = fork();
		jobs[his_count - 1].pid = c_pid;
		if(c_pid < 0){
			return -1;
		} else if(c_pid == 0){
			if(loc_job->redirect == 1){
				char * nospace_file = remove_Spaces(strdup(loc_job->file_redirect));
				file_des = open(nospace_file, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
				if(file_des == -1){
					printf("ERROR WITH FILE\n");
					return 0;
				}
				dup2(file_des, STDOUT_FILENO);
				close(file_des);
				execvp(args[0], args);
			} else if (loc_job->redirect == 2){
				char * nospace_file = remove_Spaces(strdup(loc_job->file_redirect));
				file_des = open(nospace_file, O_RDONLY, S_IRUSR | S_IWUSR);
				if(file_des == -1){
					printf("ERROR WITH FILE");
					return 0;
				}
				dup2(file_des, STDIN_FILENO);
				close(file_des);
				execvp(args[0], args);
			} else {
				execvp(args[0], args);
			}
		}  else {
			if(loc_job->is_background == 0){
				waitpid(c_pid, &status, 0);
			}
		}
	} else {
		if(strcmp(history, strtok(strdup(loc_job->full_command), " ")) == 0){
			builtin_history();
		} else if (strcmp(exit_cmmd, strtok(strdup(loc_job->full_command), " ")) == 0){
			builtin_exit();
		} else if(strcmp("jobs", strtok(strdup(loc_job->full_command), " ")) == 0){
			builtin_jobs();
		} else if(strcmp(wait, strtok(strdup(loc_job->full_command), " ")) == 0){
			builtin_wait();
		} else if(strcmp(fg, strtok(strdup(loc_job->full_command), " ")) == 0){
			builtin_fg();
		}
	}
	

    return 0;
}

/*
 * Helper function to remove teh spaces of a command
 */
char * remove_Spaces(char * tmp){
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

/*
 *Helper function to get the length 
 */
int get_length(char * tmp){
	int count = 0;
	char * token = strtok(tmp, " ");
	while(token != NULL){
		count++;
		token = strtok(NULL, " ");
	}
	return count;
}

/*
 * Helper function to ingnore the spaces before a command
 */
char * char_after_space(char * cmmd){
	char * starting = cmmd;
	while(*starting != ' '){
		starting++;
	}
	starting++;
	return starting;
}

/*
 * function for built in commman exit
 */
int builtin_exit(void){
    /*wait until all the jobs are done*/
    builtin_wait();
    return 0;
}

/*
 *
 */
int builtin_jobs(void)
{
    	int i;
	int status = 0;
	for(i = 0; i < his_index; i++){
		jobs[i].id = i;
		if(jobs[i].is_background == 1){
			if(jobs[i].done == 0){
				if(waitpid((pid_t)jobs[i].pid, &status, WNOHANG) == 0){
					jobs[i].done = 0;
					printf("[%d] Running %s\n", jobs[i].id, jobs[i].full_command);
				} else {
					jobs[i].done = 1;
					printf("[%d] Done %s\n", jobs[i].id, jobs[i].full_command);
				}
			} 
		}
	}
    return 0;
}

/*
 * Function for built in command history
 */
int builtin_history(void)
{
	int i;
        /*loop through each job*/
	for(i = 0; i < his_index; i++){
		char * tmp = strdup(jobs[i].full_command);
		if(jobs[i].is_background == 1){
			strncat(tmp, "&", 1);
		}
		printf("[%d] %s\n", i + 1, tmp);
	}
	return 0;
}

/*
 *Function for built in wait command
 */
int builtin_wait(void){
	int i;
	int status = 0;
        /*loop though each job and call wait until all jobs are done*/
	for(i = 0; i < his_index; i++){
		if(jobs[i].is_background == 1){
			if(jobs[i].done == 0){
				waitpid((pid_t)jobs[i].pid, &status, 0);
			}
		}
	}
    return 0;
}

/*
 *
 */
int getVal(char * cmmd){
	char * tmp = strtok(cmmd, " ");
	char * val = strtok(NULL, " ");
	char vals[1]; 
	strcpy(vals, val);
	return vals[0] - '0';	
}

/*
 * Funtion for built in FG command
 */
int builtin_fg(void)
{
	int status = 0;
	if(get_length(strdup(jobs[his_count - 1].full_command)) == 2){ 
		int val = getVal(strdup(jobs[his_count - 1].full_command));
		if(jobs[val].done == 0){
			waitpid((pid_t)jobs[his_count - 1].pid, &status, 0);
		} else {
			printf("Job has completed\n");
		}
	} else {
		int i;
		int index = 0;
		int found_background = 0;
		for(i = 0; i < his_index; i++){
			if(jobs[i].is_background == 1){
				if(jobs[i].done == 0){
					found_background = 1;
					index = i;
				}
			}
		}
		if(found_background == 1){
			waitpid((pid_t)jobs[index].pid, &status, 0);
		} else {
			printf("Job has completed\n");
		}


	}
    return 0;
}

int builtin_fg_num(int job_num)
{
    return 0;
}
