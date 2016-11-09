#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
/**
    Current Problem:
    The definition of the problem is unclear -> Where does the identifier that will
        be passed come from?
    shmget error: Maybe I need to use ftok() to generate a suitable key value.
    shmctl error: Invalid argument????
**/

int main()
{
    struct shmid_ds share_mem_buffer;
    int shm_identifier=0;
    int shm_key;

    //puts("Please input the identifier for a share-memory segment");
    //scanf("%d", &shm_identifier);
    puts("Please input the key you want to use for the share-memory segment.");
    scanf("%d",&shm_key);

    //IPC_CREAT = if shm exist -> return it, else -> create new one

    shm_identifier = shmget(shm_key,40960,IPC_CREAT&IPC_EXCL);
    if (shm_identifier==-1) {
        perror("shmget error");
        return 0;
    }

    if (shmctl(shm_identifier,IPC_STAT,&share_mem_buffer)==-1) {
        perror("shmctl error");
        return 0;
    }

    printf("Segment ID: %d\nKey: %d\n",shm_identifier,share_mem_buffer.shm_perm.__key);

    return 0;
}
