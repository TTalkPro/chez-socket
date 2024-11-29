//
// Created by david on 11/28/24.
//

#ifndef CZSOCKET_CHEZ_SOCKET_H
#define CZSOCKET_CHEZ_SOCKET_H

#define EV_READ		0x02 // 0010
#define EV_WRITE	0x04 // 0100
#define EV_IO	    0x06 // 0110
#define EV_TIMEOUT	0x08 // 1000
#define EV_CLOSED	0x80

#if defined(__linux__) || \
defined(__FreeBSD__) || \
defined(__OpenBSD__) || \
defined(__DragonFly__) || \
defined(__NetBSD__)
#define fd_t int
#endif


#endif //CZSOCKET_CHEZ_SOCKET_H
