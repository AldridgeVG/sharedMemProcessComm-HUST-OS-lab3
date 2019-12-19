#include "lab3.h"
#include <sys/wait.h>

union semun {
    int val;
} arg;

int main() {
    //共享内存的设置
    int shIDS;
    shIDS = shmget(shKeyS, sizeof(sharedBuf), IPC_CREAT | 0666);
    sharedBuf* sBuf = (sharedBuf*)shmat(shIDS, nullptr, SHM_R | SHM_W);
    sBuf->over = 0;

    //创建2个信号灯
    int semId;
    semId = semget(semKey, 2, IPC_CREAT | 0666);

    arg.val = 1;
    semctl(semId, 0, SETVAL, arg);  // SEmpty
    arg.val = 0;
    semctl(semId, 1, SETVAL, arg);  // SFull

    int p1 = -1, p2 = -1;
    char* args[] = {(char*)0};
    if ((p1 = fork()) == -1) {
        printf("Fail to create subprocess 1 ");
    } else if (p1 == 0) {
        printf("in p1\n");
        execv("./get", args);
    } else if (p1 > 0) {
        if ((p2 = fork()) == -1) {
            printf("Fail to create subprocess 2");
        } else if (p2 == 0) {
            printf("in p2\n");
            execv("./put", args);
        } else {
            //等待退出
            wait(&p1);
            wait(&p2);
            //删除信号灯&共享内存
            semctl(semId, 1, IPC_RMID, 0);
            shmctl(shIDS, IPC_RMID, 0);
            return 0;
        }
    }
}