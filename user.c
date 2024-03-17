#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h> // for fork()
#include <sys/wait.h>
#include <errno.h>
#include <semaphore.h> // for semaphores
#include <sys/types.h>

#define DEVICE "/dev/kernal" // Corrected device name

int main() {
    int fd = 0;
    char ch, write_buf[100], read_buf[100];
    pid_t pid;
    sem_t sem; // Semaphore

    // Initialize semaphore
    if (sem_init(&sem, 0, 0) != 0) {
        perror("Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid > 0) { // Parent process
        // Print parent process ID
        printf("Parent process: PID = %d\n", getpid());
        pid_t pid2 = fork();
        if(pid2 > 0) {
            fd = open(DEVICE, O_RDWR);
            if (fd == -1) {
                perror("Failed to open the device file");
                exit(EXIT_FAILURE);
            }
            wait(NULL);

            // Prompt for user input
            printf("\nParent process: r = read from device, w = write to device\nEnter command: ");
            scanf(" %c", &ch);

            switch (ch) {
                case 'w':
                    printf("Enter data: ");
                    scanf(" %[^\n]", write_buf);
                    if (write(fd, write_buf, sizeof(write_buf)) == -1) {
                        perror("Write failed");
                    }
                    break;
                case 'r':
                    if (read(fd, read_buf, sizeof(read_buf)) == -1) {
                        if (errno == EAGAIN) {
                            printf("No data available in the device file\n");
                        } else {
                            perror("Read failed");
                        }
                    } else {
                        printf("Data read from device: %s\n", read_buf);
                    }
                    break;
                default:
                    printf("Command not recognized\n");
                    break;
            }

            // Close the device file in parent
            close(fd);

            // Wait for child process to complete
            wait(NULL);

            printf("Parent process terminated\n");

            // Destroy semaphore
            sem_destroy(&sem);
            exit(EXIT_SUCCESS);

        } else if (pid2 == 0) {
            sleep(1);
            exit(EXIT_SUCCESS);
        }
        // Open the device file
        fd = open(DEVICE, O_RDWR);
        if (fd == -1) {
            perror("Failed to open the device file");
            exit(EXIT_FAILURE);
        }

    } else { // Child process
        // Print child process ID
        pid_t p1=fork();
        if(p1<0){
            printf("fork failed for grandchild");
        }
        if(p1==0){
            printf("In grand  grand_child id:%d\n",getpid());
            sleep(10);
            printf("Grand child terminating...\n");
            sem_post(&sem); // Release semaphore
            exit(EXIT_SUCCESS);
        } else{
            printf("Child process: PID = %d\n", getpid());
            sleep(10);
            wait(NULL);

            printf("child process terminating...\n");
            exit(EXIT_SUCCESS);
        }
    }
    return 0;
}
