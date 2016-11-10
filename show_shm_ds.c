#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <fcntl.h>
/**
    Current Problem:
    The definition of the problem is unclear -> Where does the identifier that will
        be passed come from?
    shmget error: Maybe I need to use ftok() to generate a suitable key value.
    shmctl error: Invalid argument????
**/
#define STR_MAX_LEN 200


int main()
{
    srand(time(NULL));
    struct shmid_ds share_mem_buffer;
    int shm_identifier=0,shm_key=0;
    int temp_file_descriptor;
    char filename_str[STR_MAX_LEN]="keystring_XXXXXX";

    temp_file_descriptor = mkstemp(filename_str);
    if (temp_file_descriptor==-1) {
        perror("mkstemp error");
        return 0;
    }
    shm_key = ftok(filename_str,(rand()%250+1));


    //Create a shared memory with permission that owner can read and write.
    //IPC_CREAT = if shm exist -> return it, else -> create new one
    shm_identifier = shmget(shm_key,40960,IPC_CREAT|IPC_EXCL|S_IRUSR|S_IWUSR);
    if (shm_identifier==-1) {
        perror("shmget error");
        return 0;
    }

    if (shmctl(shm_identifier,IPC_STAT,&share_mem_buffer)==-1) {
        perror("shmctl error");
        return 0;
    }

    printf("Segment ID: %d\nKey: %d\n",shm_identifier,share_mem_buffer.shm_perm.__key);

    remove(filename_str);
    return 0;
}
