 #include <fcntl.h>                 
  #include <unistd.h>                
  #include <sys/types.h>             
  #include <sys/poll.h>              
  #include "scmd_poll.h"             
 
  int main () {                                                          
    char buffer[128];                                                    
    int i = 0, result, nread;                                            
    struct pollfd pollfds[2];                                            
    int timeout = 2000;           /* Timeout in msec. */                 
 
    /* Initialize pollfds; we're looking at input from /dev/scmd*/       
 
    pollfds[0].fd = open("/dev/scmd",O_RDWR);                            
 
    if ( pollfds[0].fd  < 0 ) {                                          
      printf(" failed to open /dev/scmd \n");                            
      exit (1);                                                          
    }                                                                    
    pollfds[0].events = POLLIN;      /* Wait for input */                
 
    while (1) {                                                          
        result = poll (&pollfds, 1, timeout);                            
        switch (result) {                                                
          case 0:                                                        
            printf ("timeout\n");                                        
            break;                                                       
          case -1:                                                       
            printf ("poll error \n");                                    
            exit (1);                                                    
 
           default:                                                      
            if (pollfds[0].revents & POLLIN) {                           
                nread = read (pollfds[0].fd, buffer, SCMD_SIZE);         
                if (nread == 0) {                                        
                    printf ("/dev/scmd done\n");                         
                    exit (0);                                            
                } else {                                                 
                    buffer[nread] = 0;                                   
                    printf ("read %d from /dev/scmd: %s", nread, buffer);
                  }                                                      
              }                                                          
          }                                                              
      }                                                                  
    close(fd);                                                           
    return 0;                                                            
  }          

