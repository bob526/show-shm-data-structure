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
**/
#define STR_MAX_LEN 200
#define SHM_SIZE 4096

int main()
{
    srand(time(NULL));
    struct shmid_ds share_mem_buffer;
    int shm_identifier=0,shm_key=0;
    int temp_file_descriptor;
    char filename_str[STR_MAX_LEN]="keystring_XXXXXX";
    unsigned short save_mode=0;

    //Create a temp file and use its pathname to get a shm_key.
    temp_file_descriptor = mkstemp(filename_str);
    if (temp_file_descriptor==-1) {
        perror("mkstemp error");
        return 0;
    }
    shm_key = ftok(filename_str,(rand()%250+1));


    //Create a shared memory with permission that owner can read and write.
    //IPC_CREAT = if shm exist -> return it, else -> create new one
    shm_identifier = shmget(shm_key,SHM_SIZE,IPC_CREAT/*|IPC_EXCL*/|S_IRUSR|S_IWUSR);
    if (shm_identifier==-1) {
        perror("shmget error");
        return 0;
    }

    //Read the share-memory segment. (Data will save in share_mem_buffer.)
    if (shmctl(shm_identifier,IPC_STAT,&share_mem_buffer)==-1) {
        perror("shmctl error");
        return 0;
    }

    //Output data
    printf("Segment ID: %d\nKey: %d\n",shm_identifier,share_mem_buffer.shm_perm.__key);
    printf("Mode: %hu\nOwner UID: %d\n",share_mem_buffer.shm_perm.mode,share_mem_buffer.shm_perm.uid);
    printf("Size: %lu\nNumber of attaches:%lu\n",share_mem_buffer.shm_segsz,share_mem_buffer.shm_nattch);

    save_mode = share_mem_buffer.shm_perm.mode;

    //Output readible mode text
    puts("\nThe permission of the share-memory segment is:");
    if (save_mode & 0400)
        printf("Read permission of owner.\n");
    if (save_mode & 0200)
        printf("Write permission of owner.\n");
    if (save_mode & 0040)
        printf("Read permission of group.\n");
    if (save_mode & 0020)
        printf("Write permission of group.\n");
    if (save_mode & 0004)
        printf("Read permission of world.\n");
    if (save_mode & 0002)
        printf("Write permission of world.\n");

    //Delete temp file
    remove(filename_str);
    return 0;
}
