#include <sys/stat.h>
#include "lab3.h"

int main() {
    //获取信号灯
    int semId;
    semId = semget(semKey, 2, IPC_CREAT | 0666);

    //获取共享S的内存
    int shIDS;
    shIDS = shmget(shKeyS, sizeof(sharedBuf), IPC_CREAT | 0666);
    sharedBuf* sBuf = (sharedBuf*)shmat(shIDS, nullptr, SHM_R | SHM_W);
    printf("put first time.\n");

    FILE* fpw;
    if ((fpw = fopen("2.jpeg", "wb")) == nullptr) {
        printf("open ERROR");
        return -1;
    }
    int writeNum = 0;
    printf("point check");

    while (sBuf->over != 1||sBuf->length[writeNum] > 0) {
        P(semId, 1);
        printf("Puting a block to the file\n");
        fwrite(sBuf->data[writeNum], sizeof(char), sBuf->length[writeNum], fpw);
        sBuf->length[writeNum] = 0;
        V(semId, 0);
        writeNum = (writeNum + 1) % 10;  // Back to 0 when 9+1
    }
    fclose(fpw);
    printf("write successfully!\n");
    return 0;
}