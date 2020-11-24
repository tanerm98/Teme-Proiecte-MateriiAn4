/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "rpcdb.h"

users *userss = NULL;

void swap(float *p, float *q) {
   float t;
   t = *p;
   *p = *q;
   *q = t;
}

void sort(float a[], int n) {
	int i, j;

	for(i = 0; i < n-1; i++) {
		for(j = 0; j < n-i-1; j++) {
			if(a[j] > a[j+1]) {
				swap(&a[j], &a[j+1]);
			}
		}
	}
}

void free_memory_database(memory_database *list) {
	printf("[INFO] Freeing memory database for logged out user...\n");
	memory_database *pointer;

	while(list != NULL) {
		pointer = list;
		list = list->next;
		free(pointer);
	}
}

user_data* check_if_token_valid(u_quad_t token) {
	users *iterator;

	if (token != REJECTED_TOKEN) {
		iterator = userss;
    	while (iterator != NULL) {
            if (iterator->user.token == token) {
                return &(iterator->user);
            }
            iterator = iterator->next;
        }
	}

    return NULL;
}

void print_database_for_user(user_data *user) {
	printf("[INFO] Showing current database for user '%s'...\n", user->user_name);
	memory_database *iterator;

	iterator = user->mem_database;
    while (iterator != NULL) {

        printf("ID: %d; NO: %d;\n", iterator->data.data_id, iterator->data.no_values);

		if (iterator->data.array.array_len > 0) {
			printf("Values: ");
			for (int i = 0; i < iterator->data.array.array_len; i++) {
                printf("%.2f; ", iterator->data.array.array_val[i]);
            }
            printf("\n");
		}

        iterator = iterator->next;
    }

}

int add_to_database(user_data *user, int data_id, int no_values, float *values) {
	memory_database *new_data, *iterator, *previous;

	if (user->mem_database == NULL) {
		user->mem_database = (memory_database*) calloc (1, sizeof(memory_database));
        new_data = user->mem_database;

	} else {
		iterator = user->mem_database;
        while (iterator != NULL) {
            if (iterator->data.data_id == data_id) {
                return 0;
            }
            previous = iterator;
            iterator = previous->next;
        }

        new_data = (memory_database*) calloc (1, sizeof(memory_database));
        previous->next = new_data;
	}

	new_data->next = NULL;

	new_data->data.data_id = data_id;
	new_data->data.no_values = no_values;

	static float static_values[MAXBUF];
	for (int i = 0; i < no_values; i++) {
		static_values[i] = values[i];
	}
	new_data->data.array.array_len = no_values;
	new_data->data.array.array_val = static_values;

	return 1;
}

int update_database(user_data *user, int data_id, int no_values, float *values) {
	memory_database *iterator;

	if (user->mem_database != NULL) {
		iterator = user->mem_database;

        while (iterator != NULL) {
            if (iterator->data.data_id == data_id) {
                iterator->data.no_values = no_values;

                static float static_values[MAXBUF];
                for (int i = 0; i < no_values; i++) {
                    static_values[i] = values[i];
                }
                iterator->data.array.array_len = no_values;
                iterator->data.array.array_val = static_values;

                return 1;
            }

            iterator = iterator->next;
        }
	}

	return 0;
}

int delete_from_database(user_data *user, int data_id) {
	memory_database *iterator, *previous;

	if (user->mem_database != NULL) {
		iterator = user->mem_database;

        while (iterator != NULL) {
            if (iterator->data.data_id == data_id) {
                if (iterator == user->mem_database) {
                    user->mem_database = user->mem_database->next;
                } else {
                    previous->next = iterator->next;
                }

                free(iterator);

                return 1;
            }

			previous = iterator;
            iterator = iterator->next;
        }
	}

	return 0;
}

sensor_data* read_data(user_data *user, int data_id) {
	memory_database *iterator;

	if (user->mem_database != NULL) {
		iterator = user->mem_database;

        while (iterator != NULL) {
            if (iterator->data.data_id == data_id) {
                return &(iterator->data);
            }

            iterator = iterator->next;
        }
	}

	return NULL;
}

statistics* get_stat_data(user_data *user, int data_id) {
	memory_database *iterator;
	statistics* stat = (statistics*) calloc(1, sizeof(statistics));

	stat->min = INF;
	stat->max = -INF;
	stat->avg = 0;

    if (user->mem_database != NULL) {
        iterator = user->mem_database;

        while (iterator != NULL) {
            if (iterator->data.data_id == data_id) {
				for (int i = 0; i < iterator->data.array.array_len; i++) {
					float x = iterator->data.array.array_val[i];
					if (x < stat->min) {stat->min = x;}
					if (x > stat->max) {stat->max = x;}
					stat->avg += x;
				}
				stat->avg /= iterator->data.array.array_len;

				// computing median
				int n = iterator->data.array.array_len;
				sort(iterator->data.array.array_val, n);
				n = (n + 1) / 2 - 1;

				stat->med = iterator->data.array.array_val[n];

                return stat;
            }

            iterator = iterator->next;
        }
    }

    return NULL;
}

void login(package *argp, char *command, package *result) {
	users *iterator, *previous, *new_user;

    printf("[INFO] Interpreting `LOGIN` command.\n");

	char *user_name = strtok(command, " ");
	if (user_name == NULL) {
		result->message = "[ERROR] Could not parse user name for login command!";
		printf("%s\n", result->message);
		return;
	}
	user_name = strtok(NULL, " \n");
	if (user_name == NULL) {
		result->message = "[ERROR] Could not parse user name for login command!";
        printf("%s\n", result->message);
        return;
    }

    printf("[INFO] Logging in user '%s'...\n", user_name);
    if (userss == NULL) {
        userss = (users*) calloc (1, sizeof(users));
        new_user = userss;

    } else {
        iterator = userss;
        while (iterator != NULL) {
            if (strcmp(iterator->user.user_name, user_name) == 0) {
                if ((iterator->user.token == REJECTED_TOKEN) || (iterator->user.token == argp->token)) {
                    iterator->user.mem_database = NULL;

                    iterator->user.token = rand();
                    iterator->user.token = (iterator->user.token << 32) | rand();

                    result->message = "[SUCCESSFUL] Welcome back! (Re)Login succesful!";
                    result->token = iterator->user.token;

                } else {
                    result->message = "[ERROR] Login error! User already authenticated!";
                    result->token = REJECTED_TOKEN;
                }

                printf("%s\n", result->message);
                return;
            }

            previous = iterator;
			iterator = previous->next;
        }

		new_user = (users*) calloc (1, sizeof(users));
		previous->next = new_user;
    }

    new_user->next = NULL;

    new_user->user.disk_database = NULL;
    new_user->user.mem_database = NULL;

    new_user->user.user_name = (char*) calloc (MAXBUF, sizeof(char));
    strcpy(new_user->user.user_name, user_name);

    new_user->user.token = rand();
    new_user->user.token = (new_user->user.token << 32) | rand();

    result->message = "[SUCCESSFUL] Login succesful!";
    result->token = new_user->user.token;

    printf("%s\n", result->message);

}

void logout(package *argp, package *result) {
	users *iterator;

    printf("[INFO] Interpreting `LOGOUT` command.\n");

    user_data *user = check_if_token_valid(argp->token);

    if (user == NULL) {
        result->message = "[ERROR] Logout failed! No users logged in or the client is not logged in!";
        printf("%s\n", result->message);
        return;

    } else {
        printf("[INFO] Logging out user '%s'...\n", user->user_name);
        user->token = REJECTED_TOKEN;

		free_memory_database(user->mem_database);
        user->mem_database = NULL;

        result->message = "[SUCCESSFUL] Logout successful!";
        printf("%s\n", result->message);
        return;
    }
}

void add(package *argp, package *result) {
	users *iterator;

    printf("[INFO] Interpreting 'ADD' command.\n");

    user_data *user = check_if_token_valid(argp->token);

    if (user == NULL) {
        result->message = "[ERROR] Adding data failed! Invalid token!";
        printf("%s\n", result->message);
        return;

    } else {
        printf("[INFO] Adding data for user '%s'...\n", user->user_name);

		int data_id = BLANK;
        int no_values = BLANK;

        char *command = (char*) calloc (strlen(argp->command) + 1, sizeof(char));
        strcpy(command, argp->command);

        char *string_value = strtok(command, " ");
        if (string_value == NULL) {
            result->message = "[ERROR] Could not parse 'ADD' command!";
            printf("%s\n", result->message);
            return;
        }

		for (int i = 0; i < 2; i++) {
			string_value = strtok(NULL, " ");
			if (string_value == NULL) {
                result->message = "[ERROR] Could not parse data ID and/or number of values!";
                printf("%s\n", result->message);
                return;

            } else {
                if (i == 0) {
                    data_id = atoi(string_value);
                } else {
                    no_values = atoi(string_value);
                    if (no_values < 0) {
                        result->message = "[ERROR] Number of values must be greater than 0!";
                        printf("%s\n", result->message);
                        return;
                    }
                }
            }
		}
		printf("[INFO] Data ID: %d; Number of values: %d; Values found: ", data_id, no_values);

		float values[no_values];
		for (int i = 0; i < no_values; i++) {
            string_value = strtok(NULL, " ");
            if (string_value == NULL) {
                result->message = "[ERROR] Could not parse all values!";
                printf("%s\n", result->message);
                return;

            } else {
                values[i] = atof(string_value);
                printf("%.2f; ", values[i]);
            }
        }
        printf("\n");

        free(command);

        int successful = add_to_database(user, data_id, no_values, values);
        if (!successful) {
            result->message = "[ERROR] Data ID already exists in database. Try 'UPDATE' command or different ID!";
        } else {
            result->message = "[SUCCESSFUL] Adding data successful!";
        }

        printf("%s\n", result->message);
        print_database_for_user(user);

        return;
    }
}

void update(package *argp, package *result) {
	users *iterator;

    printf("[INFO] Interpreting 'UPDATE' command.\n");

    user_data *user = check_if_token_valid(argp->token);

    if (user == NULL) {
        result->message = "[ERROR] Updating data failed! Invalid token!";
        printf("%s\n", result->message);
        return;

    } else {
        printf("[INFO] Updating data for user '%s'...\n", user->user_name);

		int data_id = BLANK;
        int no_values = BLANK;

        char *command = (char*) calloc (strlen(argp->command) + 1, sizeof(char));
        strcpy(command, argp->command);

        char *string_value = strtok(command, " ");
        if (string_value == NULL) {
            result->message = "[ERROR] Could not parse 'UPDATE' command!";
            printf("%s\n", result->message);
            return;
        }

		for (int i = 0; i < 2; i++) {
			string_value = strtok(NULL, " ");
			if (string_value == NULL) {
                result->message = "[ERROR] Could not parse data ID and/or number of values!";
                printf("%s\n", result->message);
                return;

            } else {
                if (i == 0) {
                    data_id = atoi(string_value);
                } else {
                    no_values = atoi(string_value);
                    if (no_values < 0) {
                        result->message = "[ERROR] Number of values must be greater than 0!";
                        printf("%s\n", result->message);
                        return;
                    }
                }
            }
		}
		printf("[INFO] Data ID: %d; Number of values: %d; Values found: ", data_id, no_values);

		float values[no_values];
		for (int i = 0; i < no_values; i++) {
            string_value = strtok(NULL, " ");
            if (string_value == NULL) {
                result->message = "[ERROR] Could not parse all values!";
                printf("%s\n", result->message);
                return;

            } else {
                values[i] = atof(string_value);
                printf("%.2f; ", values[i]);
            }
        }
        printf("\n");

        free(command);

        int successful = update_database(user, data_id, no_values, values);
        if (!successful) {
            result->message = "[ERROR] Data ID does not exist in database. Try 'ADD' command!";
        } else {
            result->message = "[SUCCESSFUL] Updating data successful!";
        }

        printf("%s\n", result->message);
        print_database_for_user(user);

        return;
    }
}

void del(package *argp, package *result) {
	users *iterator;

    printf("[INFO] Interpreting 'DEL' command.\n");

    user_data *user = check_if_token_valid(argp->token);

    if (user == NULL) {
        result->message = "[ERROR] Deleting data failed! Invalid token!";
        printf("%s\n", result->message);
        return;

    } else {
        printf("[INFO] Deleting data for user '%s'...\n", user->user_name);

		int data_id = BLANK;

        char *command = (char*) calloc (strlen(argp->command) + 1, sizeof(char));
        strcpy(command, argp->command);

        char *string_value = strtok(command, " ");
        if (string_value == NULL) {
            result->message = "[ERROR] Could not parse 'DEL' command!";
            printf("%s\n", result->message);
            return;
        }

		string_value = strtok(NULL, " ");
		if (string_value == NULL) {
            result->message = "[ERROR] Could not parse data ID !";
            printf("%s\n", result->message);
            return;
        } else {
            data_id = atoi(string_value);
        }
		printf("[INFO] Data ID: %d;\n", data_id);

        free(command);

        int successful = delete_from_database(user, data_id);
        if (!successful) {
            result->message = "[ERROR] Data ID does not exist in database.";
        } else {
            result->message = "[SUCCESSFUL] Deleting data successful!";
        }

        printf("%s\n", result->message);
        print_database_for_user(user);

        return;
    }
}

void read(package *argp, package *result) {
	users *iterator;

    printf("[INFO] Interpreting 'READ' command.\n");

    user_data *user = check_if_token_valid(argp->token);

    if (user == NULL) {
        result->message = "[ERROR] Reading data failed! Invalid token!";
        printf("%s\n", result->message);
        return;

    } else {
        printf("[INFO] Reading data for user '%s'...\n", user->user_name);

		int data_id = BLANK;

        char *command = (char*) calloc (strlen(argp->command) + 1, sizeof(char));
        strcpy(command, argp->command);

        char *string_value = strtok(command, " ");
        if (string_value == NULL) {
            result->message = "[ERROR] Could not parse 'READ' command!";
            printf("%s\n", result->message);
            return;
        }

		string_value = strtok(NULL, " ");
		if (string_value == NULL) {
            result->message = "[ERROR] Could not parse data ID !";
            printf("%s\n", result->message);
            return;
        } else {
            data_id = atoi(string_value);
        }
		printf("[INFO] Data ID: %d;\n", data_id);

        free(command);

        sensor_data *data_from_database = read_data(user, data_id);
        if (data_from_database == NULL) {
            result->message = "[ERROR] Data ID does not exist in database.";
        } else {
            result->message = "[SUCCESSFUL] Reading data successful!";
            result->data = (*data_from_database);
        }

        printf("%s\n", result->message);

        return;
    }
}

void get_stat(package *argp, package *result) {
	users *iterator;

    printf("[INFO] Interpreting 'GET_STAT' command.\n");

    user_data *user = check_if_token_valid(argp->token);

    if (user == NULL) {
        result->message = "[ERROR] Getting statistic for data failed! Invalid token!";
        printf("%s\n", result->message);
        return;

    } else {
        printf("[INFO] Getting statistic for data for user '%s'...\n", user->user_name);

		int data_id = BLANK;

        char *command = (char*) calloc (strlen(argp->command) + 1, sizeof(char));
        strcpy(command, argp->command);

        char *string_value = strtok(command, " ");
        if (string_value == NULL) {
            result->message = "[ERROR] Could not parse 'GET_STAT' command!";
            printf("%s\n", result->message);
            return;
        }

		string_value = strtok(NULL, " ");
		if (string_value == NULL) {
            result->message = "[ERROR] Could not parse data ID !";
            printf("%s\n", result->message);
            return;
        } else {
            data_id = atoi(string_value);
        }
		printf("[INFO] Data ID: %d;\n", data_id);

        free(command);

        statistics *stat = get_stat_data(user, data_id);
        if (stat == NULL) {
            result->message = "[ERROR] Data ID does not exist in database.";
        } else {
            result->message = "[SUCCESSFUL] Getting statistic for data successful!";
            result->stats = (*stat);
        }

        printf("%s\n", result->message);
		free(stat);

        return;
    }
}

package* command_1_svc(package *argp, struct svc_req *rqstp) {
	static package result;

	result.command = EMPTY;
    result.message = EMPTY;

    result.data.data_id = BLANK;
    result.data.no_values = BLANK;

    static float values[0];
    result.data.array.array_len = 0;
    result.data.array.array_val = values;

    printf("[INFO] Received command '%s'.\n", argp->command);

	char *command = argp->command;
	if (strstr(command, LOGIN_COMMAND) == command) {
		login(argp, command, &result);
    } else if (strstr(command, LOGOUT_COMMAND) == command) {
        logout(argp, &result);
    } else if (strstr(command, ADD_COMMAND) == command) {
        add(argp, &result);
    } else if (strstr(command, DEL_COMMAND) == command) {
        del(argp, &result);
    } else if (strstr(command, UPDATE_COMMAND) == command) {
        update(argp, &result);
    } else if (strstr(command, READ_COMMAND) == command) {
        read(argp, &result);
    } else if (strstr(command, GET_STAT_COMMAND) == command) {
        get_stat(argp, &result);
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