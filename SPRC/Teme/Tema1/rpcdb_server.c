/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "rpcdb.h"
users *userss = NULL;
users *iterator, *previous, *temp;

void free_memory_database(memory_database *list) {
	printf("Freeing memory database for logged out user...\n");
	memory_database *pointer;

	while(list != NULL) {
		pointer = list;
		list = list->next;
		free(pointer);
	}
}

void login(package *argp, char *command, package *result) {
	char *user_name = strtok(command, " ");
	if (user_name == NULL) {
		result->message = "[ERROR-1] Could not parse user name for login command!";
		printf("%s\n", result->message);
		return;
	}

	user_name = strtok(NULL, " \n");
	if (user_name == NULL) {
		result->message = "[ERROR-2] Could not parse user name for login command!";
        printf("%s\n", result->message);
        return;
    }

    printf("Logging in user '%s'...\n", user_name);
    if (userss == NULL) {
        userss = (users*) calloc (1, sizeof(users));

        userss->next = NULL;

        userss->user.disk_database = NULL;
        userss->user.mem_database = NULL;

        userss->user.user_name = (char*) calloc (MAXBUF, sizeof(char));
        strcpy(userss->user.user_name, user_name);

		userss->user.token = rand();
        userss->user.token = (userss->user.token << 32) | rand();

        result->message = "[SUCCESSFUL-1] Login succesful!";
        result->token = userss->user.token;

        printf("%s\n", result->message);

    } else {
        iterator = userss;
        while (iterator != NULL) {
            if (strcmp(iterator->user.user_name, user_name) == 0) {
                if ((iterator->user.token == REJECTED_TOKEN) || (iterator->user.token == argp->token)) {
                    iterator->user.mem_database = NULL;

                    iterator->user.token = rand();
                    iterator->user.token = (iterator->user.token << 32) | rand();

                    result->message = "[SUCCESSFUL-2] Welcome back! (Re)Login succesful!";
                    result->token = iterator->user.token;

                } else {
                    result->message = "[ERROR-3] Login error! User already authenticated!";
                    result->token = REJECTED_TOKEN;
                }

                printf("%s\n", result->message);
                return;
            }

            previous = iterator;
			iterator = previous->next;
        }

		temp = (users*) calloc (1, sizeof(users));
		previous->next = temp;

        temp->next = NULL;

        temp->user.disk_database = NULL;
        temp->user.mem_database = NULL;

        temp->user.user_name = (char*) calloc (MAXBUF, sizeof(char));
        strcpy(temp->user.user_name, user_name);

        temp->user.token = rand();
        temp->user.token = (temp->user.token << 32) | rand();

        result->message = "[SUCCESSFUL-3] Login succesful!";
        printf("%s\n", result->message);
        result->token = temp->user.token;
    }
}

void logout(package *argp, package *result) {
    printf("Received logout command.\n");

    if ((argp->token == REJECTED_TOKEN) || (userss == NULL)) {
        result->message = "[ERROR-4] Logout failed! No users logged in or the client is not logged in!";
        result->token = REJECTED_TOKEN;
        printf("%s\n", result->message);

    } else {
        iterator = userss;
        while (iterator != NULL) {
            if (iterator->user.token == argp->token) {
                printf("Logging out user '%s'...\n", iterator->user.user_name);
                iterator->user.token = REJECTED_TOKEN;

				free_memory_database(iterator->user.mem_database);
                iterator->user.mem_database = NULL;

	            result->message = "[SUCCESSFUL-4] Logout successful!";
	            result->token = REJECTED_TOKEN;
                printf("%s\n", result->message);
                return;
            }

			iterator = iterator->next;
        }

		result->message = "[ERROR-5] Logout failed! Client is not logged in!";
        result->token = REJECTED_TOKEN;
        printf("%s\n", result->message);
    }
}

package* command_1_svc(package *argp, struct svc_req *rqstp) {
	static package result;

	result.command = "";
    result.message = "";
    static float values[0];
    result.data.array.array_len = 0;
    result.data.array.array_val = values;

    printf("Received command '%s'.\n", argp->command);

	char *command = argp->command;
	if (strstr(command, LOGIN_COMMAND) == command) {
		login(argp, command, &result);
    } else if (strstr(command, LOGOUT_COMMAND) == command) {
        logout(argp, &result);
    } else if (strstr(command, ADD_COMMAND) == command) {
        printf("%s\n", ADD_COMMAND);
    } else if (strstr(command, DEL_COMMAND) == command) {
        printf("%s\n", DEL_COMMAND);
    } else if (strstr(command, UPDATE_COMMAND) == command) {
        printf("%s\n", UPDATE_COMMAND);
    } else if (strstr(command, READ_COMMAND) == command) {
        printf("%s\n", READ_COMMAND);
    } else if (strstr(command, GET_STAT_COMMAND) == command) {
        printf("%s\n", GET_STAT_COMMAND);
    } else if (strstr(command, GET_STAT_ALL_COMMAND) == command) {
        printf("%s\n", GET_STAT_ALL_COMMAND);
    } else if (strstr(command, STORE_COMMAND) == command) {
        printf("%s\n", STORE_COMMAND);
    } else if (strstr(command, LOAD_COMMAND) == command) {
        printf("%s\n", LOAD_COMMAND);
    } else {
        result.message = "[ERROR] Could not interpret command!";
        printf("%s\n", result.message);
    }

	return &result;
}