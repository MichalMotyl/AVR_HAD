void initSerialPort();
void debugInit();

#if 0
        #define DEBUG(fmt, args...)   printf("[DEBUG][%s]: "fmt, __FUNCTION__, ##args)                /*info function */
#else
        #define DEBUG(fmt, args...)
#endif


#if 0
        #define DHCP_DEBUG(fmt, args...)   printf("[DEBUG][%s]: "fmt, __FUNCTION__, ##args)                /*info function */
#else
        #define DHCP_DEBUG(fmt, args...)
#endif

#if 0
        #define IP_DEBUG(fmt, args...)   printf("[DEBUG][%s]: "fmt, __FUNCTION__, ##args)                /*info function */
#else
        #define IP_DEBUG(fmt, args...)
#endif

#if 1
        #define NETSTACK_DEBUG(fmt, args...)   printf("[DEBUG][%s]: "fmt, __FUNCTION__, ##args)                /*info function */
#else
        #define NETSTACK_DEBUG(fmt, args...)
#endif
