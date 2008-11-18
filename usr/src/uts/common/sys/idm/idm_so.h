/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*
 * Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef _IDM_SO_H
#define	_IDM_SO_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <sys/idm/idm_transport.h>

/*
 * Define TCP window size (send and receive buffer sizes)
 */

#define	IDM_RCVBUF_SIZE		(256 * 1024)
#define	IDM_SNDBUF_SIZE		(256 * 1024)

/* sockets-specific portion of idm_svc_t */
typedef struct idm_so_svc_s {
	struct sonode		*is_so;
	kthread_t		*is_thread;
	kt_did_t		is_thread_did;
	boolean_t		is_thread_running;
} idm_so_svc_t;

/* sockets-specific portion of idm_conn_t */
typedef struct idm_so_conn_s {
	struct sonode		*ic_so;

	kthread_t		*ic_tx_thread;
	kt_did_t		ic_tx_thread_did;
	boolean_t		ic_tx_thread_running;
	kmutex_t		ic_tx_mutex;
	kcondvar_t		ic_tx_cv;
	list_t			ic_tx_list;	/* List of PDUs for transmit */

	kthread_t		*ic_rx_thread;
	kt_did_t		ic_rx_thread_did;
	boolean_t		ic_rx_thread_running;
} idm_so_conn_t;

void idm_so_init(idm_transport_t *it);
void idm_so_fini();

/* Socket functions */

struct sonode *
idm_socreate(int domain, int type, int protocol);

void idm_soshutdown(struct sonode *so);

void idm_sodestroy(struct sonode *so);

int idm_get_ipaddr(idm_addr_list_t **);

int idm_sorecv(struct sonode *so, void *msg, size_t len);

int idm_sosendto(struct sonode *so, void *buff, size_t len,
    struct sockaddr *name, socklen_t namelen);

int idm_iov_sosend(struct sonode *so, iovec_t *iop, int iovlen,
    size_t total_len);

int idm_iov_sorecv(struct sonode *so, iovec_t *iop, int iovlen,
    size_t total_len);

void idm_sotx_thread(void *arg);
void idm_sorx_thread(void *arg);


int idm_sotx_pdu_constructor(void *hdl, void *arg, int flags);

void idm_sotx_pdu_destructor(void *pdu_void, void *arg);

int idm_sorx_pdu_constructor(void *hdl, void *arg, int flags);

void idm_sorx_pdu_destructor(void *pdu_void, void *arg);

void idm_so_svc_port_watcher(void *arg);


#ifdef	__cplusplus
}
#endif

#endif /* _IDM_SO_H */