#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>

int main() {

struct pollfd fds[1];
int timeout_msecs = 4000;
int ret;

fds[0].fd = 0;                   /*Adding fd(to be watched ) to FDLIST, here we are adding fd=0, i.e. STDIN to be monitored*/
fds[0].events = POLLIN;         /*Adding an event to be watched for this added fd, here we are adding POLLIN event that means this fd will be monitored if it receive any data*/

ret = poll(fds, 1,timeout_msecs);
if (fds[0].revents && POLLIN) {        /*for testing if any event occurred */
printf("\n revents in fd[0] = %d ", fds[0].revents);

}

if (ret > 0) {
printf("\nData is available now.\n");
}

else if (ret == -1)
perror("poll()");

else
printf("No data within %dseconds.Time out.......\nreturn value : %d\n",timeout_msecs/1000,ret);

return 0;

}