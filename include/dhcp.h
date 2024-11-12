/*******************************************************************************
*
* FILENAME : dhcp_test.c
*
* DESCRIPTION : DHCP (Dynamic Host Configuration Protocol) is a network protocol
* commonly used in IP networks to automatically assign IP addresses and other
* network configuration parameters to devices. DHCP enables devices to join a
* network and obtain the necessary network settings without manual
* configuration.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 17.06.2023
* 
*******************************************************************************/

#ifndef __NSRD_DHCP_H__
#define __NSRD_DHCP_H__

#include <stddef.h> /* size_t */

enum ip_status
{
	IP_SUCCESS = 0,
	IP_ALLOC_FAIL,
	IP_SUBSEQUENT_IS_OCCUPIED
};

typedef struct dhcp dhcp_t;
typedef unsigned int ip_t;

/*
DESCRIPTION
    Creates new DHCP. The function takes the network ID, which is a portion
    of an IP address that uniquely identifies the network, and the number
    of network bits that determine the network's size.
    DHCP use IPv4 (Internet Protocol version 4).
    DHCP will preallocate 3 addresses: ...000, ...110, ...111 (binary format).
    You can count range of possible addresses using formula:
    	(2^(32 - network_bits)) - 3.
    Creation may fail, due to memory allocation fail.
    User is responsible for memory deallocation.
    Passing network_bits >= 31 may cause undefined behavior.
RETURN
    Pointer to the created DHCP on success.
    NULL if allocation failed.
INPUT
    network_id: IP address in decimal format (DHCP will save only network ID).
    network_bits: number of bits that determine network ID.
TIME_COMPLEXITY
    O(log n)
*/
dhcp_t *DHCPCreate(ip_t network_id, size_t network_bits);

/*
DESCRIPTION
    Frees the memory allocated for each IP address of a DHCP
    and the DHCP by itself.
    All data inside the DHCP will be lost.
    User is responsible for handling dangling pointers after destroy.
RETURN
    Doesn't return anything.
INPUT
    dhcp: pointer to the DHCP.
TIME_COMPLEXITY
    O(n)
*/
void DHCPDestroy(dhcp_t *dhcp);

/*
DESCRIPTION
	Allocates a new IP address to a user within the DHCP instance.
	It searches for the smallest available IP address that is bigger than
	or equal to the requested IP (prefered ip).
	The user may pass a requested IP of 0 if they want to receive the first
	available IP in the DHCP pool.
	Creation may fail, due to memory allocation fail.
RETURN
    0 (IP_SUCCESS): If the requested IP or a bigger one is found and
    successfully allocated.
    1 (IP_ALLOC_ERROR): If the allocation of a new IP address fails
    due to memory allocation failure.
    2 (IP_SUBSEQUENT_EXHAUSTED): If no free IPs are available from the requested
    IP to the maximum available IP in the DHCP pool.
INPUT
    dhcp: pointer to the DHCP.
    requested_ip: the IP address that the user prefers, represented in
    decimal format. If the user wants any available IP, they can pass 0.
    result_ip: pointer to the variable where the resulting IP address will
    be saved if allocation is successful.
    The allocated IP will be in decimal format.
TIME_COMPLEXITY
    O(log n)
*/
int DHCPAllocIP(dhcp_t *dhcp, ip_t requested_ip, ip_t *result_ip);

/*
DESCRIPTION
	Searches for the IP address within the DHCP instance.
	It looks for the IP address that matches the host ID part of the provided
	IP address in decimal format. If the matching IP address is found,
	it frees it, making it available for future assignments.
	If no matching IP address is found, the function does nothing.
RETURN
    Doesn't return anything.
INPUT
    dhcp: pointer to the DHCP.
    ip: IP to be searched and potentially freed represented in decimal format.
TIME_COMPLEXITY
    O(log n)
*/
void DHCPFreeIP(dhcp_t *dhcp, ip_t ip);

/*
DESCRIPTION
    Calculates the number of free IPs available for allocation.
RETURN
    Number of free available IPs in DHCP.
INPUT
    dhcp: pointer to the DHCP.
TIME_COMPLEXITY
    O(n)
*/
size_t DHCPCountFree(const dhcp_t *dhcp);

/*
DESCRIPTION
	Function takes the IP address represented as a 32-bit decimal
	number and converts it to a string format "XXX.XXX.XXX.XXX".
	The converted IP address is then stored in the provided buffer.
	Buffer must be big enough to fit string with NULL terminated symbol.
RETURN
    Pointer to users buffer containing the string representation
    of the IP address.
INPUT
    ip: IP represented in decimal format.
    buffer: destination where IP in string format will be saved
    ("XXX.XXX.XXX.XXX").
TIME_COMPLEXITY
    O(1)
*/
char *DHCPIPToStr(ip_t ip, char *buffer);

/*
DESCRIPTION
	Takes the string representation of an IP address in the
	format "XXX.XXX.XXX.XXX" and converts it to a 32-bit binary number.
	The function returns the converted IP address.
	Passing string different format may cause undefined behavior.
RETURN
    Representation of IP in decimal format
INPUT
    string: pointer to the string format "XXX.XXX.XXX.XXX".
TIME_COMPLEXITY
    O(1)
*/
ip_t DHCPStrToIP(const char *string);

#endif /* __NSRD_DHCP_H__ */