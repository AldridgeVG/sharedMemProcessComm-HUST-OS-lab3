#include <string.h>
#include "lab3.h"

int main() {

    //获取信号灯
    int semId;
    semId = semget(semKey, 2, IPC_CREAT | 0666);
    
    //获取共享S的内存
    int shIDS;
    shIDS = shmget(shKeyS, sizeof(sharedBuf), IPC_CREAT | 0666);
    sharedBuf* sBuf = (sharedBuf*)shmat(shIDS, nullptr, SHM_R | SHM_W);
    printf("Prepare to get\n");
    
    //读取到共享内存
    FILE* fp;
    if ((fp = fopen("1.jpeg", "rb")) == nullptr) {
        printf("open file ERROR");
        return -1;
    }
    int readNum = 0;
    int size;
    while (sBuf->over != 1) {
        P(semId, 0);
        size = fread(sBuf->data[readNum], sizeof(char), 100, fp);
        sBuf->length[readNum] = size;   //记录大小
        if (size < 100) {
            sBuf->over = 1;
        }
        printf("GETting a block\n");
        V(semId, 1);
        readNum = (readNum + 1) % 10;
    }
    fclose(fp);
    return 0;
}