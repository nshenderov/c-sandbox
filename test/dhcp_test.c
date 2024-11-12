/*******************************************************************************
*
* FILENAME : dhcp_test.c
*
* DESCRIPTION : DHCP unit tests.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 17.06.2023
* 
*******************************************************************************/

#include <stdlib.h> /* malloc, free*/
#include <string.h> /* strcmp */

#include "dhcp.h"
#include "testing.h"

static void TestDHCPCreate(void);
static void TestDHCPIPToStr(void);
static void TestDHCPStrToIP(void);
static void TestDHCPCountFree(void);
static void TestDHCPAllocIP(void);
static void TestDHCPFreeIP(void);

int main()
{
	TH_TEST_T tests[] = {
		{"Create", TestDHCPCreate},
		{"IPToStr", TestDHCPIPToStr},
		{"StrToIP", TestDHCPStrToIP},
		{"AllocIP", TestDHCPAllocIP},
		{"FreeIP", TestDHCPFreeIP},
		{"CountFree", TestDHCPCountFree},
		TH_TESTS_ARRAY_END
	};

	TH_RUN_TESTS(tests);

	return (0);
}

static void TestDHCPCreate(void)
{
    dhcp_t *dhcp = DHCPCreate(0xFFFFFF00, 24);

    TH_ASSERT(NULL != dhcp);

    DHCPDestroy(dhcp);
}


static void TestDHCPIPToStr(void)
{
    dhcp_t *dhcp = DHCPCreate(0xFFFFFF00, 24);

    char buffer[16] = {0};

    DHCPIPToStr(0xC016017B, buffer);
    
    TH_ASSERT(0 == strcmp("192.22.1.123", buffer));
    
    DHCPDestroy(dhcp);
}

static void TestDHCPStrToIP(void)
{
    dhcp_t *dhcp = DHCPCreate(0xFFFFFF00, 24);

    TH_ASSERT(0xC016017B == DHCPStrToIP("192.22.1.123"));
    
    DHCPDestroy(dhcp);
}

static void TestDHCPCountFree(void)
{
    dhcp_t *dhcp = DHCPCreate(0xFFFFFF00, 24);

    TH_ASSERT(253 == DHCPCountFree(dhcp));

    DHCPDestroy(dhcp);
}

static void TestDHCPAllocIP(void)
{
    dhcp_t *dhcp = DHCPCreate(0xFFFFFF00, 24);

    ip_t ip_res = 0;

    int i = 0;

    for (; i < 253; ++i)
    {
        DHCPAllocIP(dhcp, 0x0, &ip_res);
    }

    TH_ASSERT(0 == DHCPCountFree(dhcp));
    
    DHCPDestroy(dhcp);
}

static void TestDHCPFreeIP(void)
{
    dhcp_t *dhcp = DHCPCreate(0xFFFFFF00, 24);

    ip_t ip_res = 0;

    int i = 0;

    for (; i < 253; ++i)
    {
        DHCPAllocIP(dhcp, 0x0, &ip_res);
    }

    DHCPFreeIP(dhcp, ip_res);

    TH_ASSERT(1 == DHCPCountFree(dhcp));

    DHCPFreeIP(dhcp, 0x0);

    TH_ASSERT(2 == DHCPCountFree(dhcp));

    DHCPFreeIP(dhcp, 0xFFFFFFFF);

    TH_ASSERT(3 == DHCPCountFree(dhcp));
    
    DHCPDestroy(dhcp);
}