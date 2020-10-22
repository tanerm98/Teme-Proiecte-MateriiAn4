/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "add.h"

bool_t
xdr_prime_request (XDR *xdrs, prime_request *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->min))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->max))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_prime_result (XDR *xdrs, prime_result *objp)
{
	register int32_t *buf;

	 if (!xdr_array (xdrs, (char **)&objp->array.array_val, (u_int *) &objp->array.array_len, MAXPRIMES,
		sizeof (int), (xdrproc_t) xdr_int))
		 return FALSE;
	return TRUE;
}