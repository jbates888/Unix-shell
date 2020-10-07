/*
 * [YOUR NAME HERE]
 *
 * CS441/541: Project 3
 *
 */
#include "mysh.h"

int main(int argc, char * argv[]) {
  int ret;

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
	const char * exit_cmmd = "exit";
	const char * fg = "fg";
	const char * jobs = "jobs";
	const char * history = "history";
	const char * wait = "wait";
	getline(&input, &len, stdin);
	strtok(input, "\n");
	//printf("first input: %s\n", input);

	//if(strcmp(input, exit_cmmd) == 0){
	//	return 0;	
	//}

	
	char * tmp = strdup(input);
	int i = 0; 
	int last_stop = 0; 
	while(tmp[i] != '\0'){
		char t = tmp[i];
		char * u = &t;
		char * job_name;
		printf("%s\n", u);
		if(strcmp(u,"&") == 0){
			job_name = substr(tmp, last_stop, i);
			printf("job_name : %s\n", job_name);
			last_stop = i;
			return 0;
		} else if (strcmp(u, ";") == 0){
			job_name = substr(tmp, last_stop, i);
			last_stop = i;
		}
	//	printf("%s\n", u);
		i++;
	}
	return 0;
	job_t loc_job = {.full_command = input};
	loc_job.binary = strtok(tmp, " ");
	launch_job(&loc_job);


	total_jobs++;	 
        /*
         * Read stdin, break out of loop if Ctrl-D
      	*/


        /* Strip off the newline */
       

        /*
         * Parse and execute the command
         */
	
       
    } while( 1/* end condition */);

    /*
     * Cleanup
     */

    return 0;
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

/*
 * You will want one or more helper functions for parsing the commands 
 * and then call the following functions to execute them
 */

int launch_job(job_t * loc_job)
{
    /*
     * Display the job
     */
	

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
			//printf("%s\n", args[i]);
			tmp = strtok(NULL, " ");
			i++;
		}
		args[i] = NULL;
			
		c_pid = fork();
		if(c_pid < 0){
			//printf("Fork Failed");
			return -1;
		} else if(c_pid == 0){
			execvp(loc_job->binary, args);
			//printf("Exec failed");
			exit(-1);
		} else {
			waitpid(c_pid, &status, 0);
			//printf("Child finished\n");
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

char * get_second(char * cmmd){
	const char delim[] = " ";
	char input[255];
	strcpy(input, cmmd);
	char * ptr = strtok(input, delim);
	return strtok(NULL, delim);
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
