/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "rpcdb.h"

package* login_1_svc(package *argp, struct svc_req *rqstp) {
	package *result;

	result = (package*) calloc (1, sizeof(package));

	printf("Received login request from user '%s'\n", argp -> message);
	result->token = 19;

	/*
	 * insert server code here
	 */

	return result;
}

package* logout_1_svc(package *argp, struct svc_req *rqstp) {
	static package  result;

	/*
	 * insert server code here
	 */

	return &result;
}

package* add_1_svc(package *argp, struct svc_req *rqstp) {
	static package  result;

	/*
	 * insert server code here
	 */

	return &result;
}

package* update_1_svc(package *argp, struct svc_req *rqstp) {
	static package  result;

	/*
	 * insert server code here
	 */

	return &result;
}

package* del_1_svc(package *argp, struct svc_req *rqstp) {
	static package  result;

	/*
	 * insert server code here
	 */

	return &result;
}

package* read_1_svc(package *argp, struct svc_req *rqstp) {
	static package  result;

	/*
	 * insert server code here
	 */

	return &result;
}

package* get_stat_1_svc(package *argp, struct svc_req *rqstp) {
	static package  result;

	/*
	 * insert server code here
	 */

	return &result;
}

package* get_stat_all_1_svc(package *argp, struct svc_req *rqstp) {
	static package  result;

	/*
	 * insert server code here
	 */

	return &result;
}

package* load_1_svc(void *argp, struct svc_req *rqstp) {
	static package  result;

	/*
	 * insert server code here
	 */

	return &result;
}

package* store_1_svc(void *argp, struct svc_req *rqstp) {
	static package  result;

	/*
	 * insert server code here
	 */

	return &result;
}