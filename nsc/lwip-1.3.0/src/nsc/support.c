/*
  Network Simulation Cradle
  Copyright (C) 2003-2005 Sam Jansen

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc., 59
  Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#include <stdlib.h>

#include "lwip/raw.h"
#include "lwip/netbuf.h"

#include "lwip/memp.h"
#include "lwip/mem.h"

#include "lwip/api.h"
#include "lwip/api_msg.h"
#include "lwip/ip_frag.h"
#include "lwip/tcp.h"
#include "lwip/tcpip.h"
#include "lwip/udp.h"

#include "sim_errno.h"

/*extern void test();

int main()
{
    test();

    return 0;
}*/
static const u16_t memp_sizes[MEMP_MAX] = {
    sizeof(struct raw_pcb),
    sizeof(struct udp_pcb),
    sizeof(struct tcp_pcb),
    sizeof(struct tcp_pcb_listen),
    sizeof(struct tcp_seg),
    sizeof(struct ip_reassdata),
    sizeof(struct netbuf),
    sizeof(struct netconn),
    ~0, /* MEMP_ARP_QUEUE -- unused */
    sizeof(struct igmp_group),
    sizeof(struct pbuf),
    sizeof(struct pbuf),
//  sizeof(struct api_msg),
//  sizeof(struct tcpip_msg),
//  sizeof(struct sys_timeout)
};

void *
memp_malloc(memp_t type)
{
    return malloc(memp_sizes[type]);
}

void
memp_free(memp_t type, void *mem)
{
    free(mem);
}


int nsc_convert_syserr_to_nscerr(int syserr)
{
	if (syserr >= 0)
		return syserr;
	// like NSC_*, lwip errors are < 0

	switch (syserr) {
	case ERR_MEM: // used if there is not enough space in the xmit buffer
		return NSC_EAGAIN;
	case ERR_BUF:
		assert(0);
	case ERR_ABRT: // fallthrough
	case ERR_RST: return NSC_ECONNRESET;
	case ERR_CLSD: // fallthrough
	case ERR_CONN: return NSC_ENOTCONN;
	case ERR_VAL: // fallthrough
	case ERR_ARG: assert(0);
	case ERR_RTE: return NSC_EHOSTUNREACH;
	case ERR_USE: return NSC_EADDRINUSE;
	case ERR_IF: assert(0);
	case ERR_ISCONN: return NSC_EISCONN;
	}

	assert(0);
	return NSC_EUNKNOWN;
}
