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

    /*
     * If no command line arguments were passed then this is an interactive
     * mode run.
     */
	if(argc > 1){
		is_batch = TRUE;
	}

    /*
     * If command line arguments were supplied then this is batch mode.
     */

    return 0;
}

int batch_mode(void)
{

    /*
     * For each file...
     */

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
				his_count++;
			}
			his_index++;
			//char * part_temp = strtok(strdup(part_string), " ");
			if(check_builtin(strtok(strdup(part_string), " ")) == 1){
				add_history(strdup(part_string), 0, his_index-1);
				job_creation(strdup(part_string), 1, NULL);
			} else {
				add_history(strdup(part_string), 1, his_index-1);
				job_creation(strdup(part_string), 0, strtok(strdup(part_string), " "));
			}
			last_stop = i + 1;		
		} else if (strcmp(u, ";") == 0){
			part_string = substr(strdup(tmp), last_stop, i);
			if(history == NULL){
				history = (char**)malloc(sizeof(char*));
				his_count = 1;
			} else {
				history = (char**)realloc(history, sizeof(history) * (his_count + 1));
				his_count++;
			}
			his_index++;
			add_history(strdup(part_string), 0, his_index-1);
			if(check_builtin(strtok(strdup(part_string), " ")) == 1){
				job_creation(strdup(part_string), 0, NULL);
			} else {
				job_creation(strdup(part_string), 0, strtok(strdup(part_string), " "));
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
			his_count++;
		}
		his_index++;
		add_history(strdup(part_string), 0, his_index-1);
		if(check_builtin(strtok(strdup(part_string), " ")) == 1){
			job_creation(strdup(part_string), 0, NULL);
		} else {
			job_creation(strdup(part_string), 0, strtok(strdup(part_string), " "));
		}
	}
	total_jobs++;	 	
       
    } while( 1/* end condition */);

    /*
     * Cleanup
     */

    return 0;
}

void add_history(char * cmmd, int background, int his_size){
	if(background == 1){
		strncat(cmmd, "&", 1);
	}
	history[his_size] = cmmd;
}

void job_creation(char * job_name, int background, char * binary){
	//printf("Job creation: %s\n", job_name);
	job_t * loc_job = (job_t *)malloc(sizeof(job_t));
	loc_job->full_command = strdup(job_name);
	loc_job->binary = binary;
	loc_job->is_background = background;
	launch_job(loc_job);
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
	
	if(strcmp(command, fg) == 0 || strcmp(command, jobs) == 0 || strcmp(command, history) == 0 || strcmp(command, wait) == 0){
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
	const char * jobs = "jobs";
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

		pid_t c_pid = 0;
		int status = 0;
		char **args;

		char * a = strdup(loc_job->full_command);
		int argc = get_length(a);
		char * tmp = strtok(loc_job->full_command, " ");
	
		args = (char **)malloc(sizeof(char*) * (argc + 1));
		//args[0] = strdup(loc_job->binary);
		int i = 0;
		while(tmp != NULL){
			args[i] = strdup(tmp);
			tmp = strtok(NULL, " ");
			i++;
		}
		args[i] = NULL;
			
		c_pid = fork();
		if(c_pid < 0){
			return -1;
		} else if(c_pid == 0){
			execvp(loc_job->binary, args);
			exit(-1);
		} else {
			waitpid(c_pid, &status, 0);
		}
	} else {
		if(strcmp(history, strtok(strdup(loc_job->full_command), " ")) == 0){
			builtin_history();
		}
	}
	

    return 0;
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
