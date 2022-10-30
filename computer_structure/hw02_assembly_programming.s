# Stack에 array를 사전 저장하는 방법은 아래 링크를 참조하였습니다.
# https://stackoverflow.com/questions/65965154/initialize-memory-for-array-in-risc-v


MAIN:
    addi    sp, sp, -40

    li  x18, 3
    sw  x18, 4(sp)
    li  x18, 6
    sw  x18, 8(sp)
    li  x18, 2
    sw  x18, 12(sp)
    li  x18, 7
    sw  x18, 16(sp)
    li  x18, 4
    sw  x18, 20(sp)
    li  x18, 1
    sw  x18, 24(sp)
    li  x18, 8
    sw  x18, 28(sp)
    li  x18, 5
    sw  x18, 32(sp)
    li  x18, 0
    sw  x18, 36(sp)
    li  x18, 9
    sw  x18, 40(sp)

    addi    x5, x0, 1               # x5 = i = 1
    addi    x31, x0, 10             # x31 = 10
    addi    x30, x0, -1             # x30 = -1
    addi    sp, sp, 4               # j 값을 확인할 sp, 현재 array 0번을 가리킴
    addi    s0, sp, 0               # i 값을 확인할 s0, sp의 위치 그대로 복사

SORT:
    bge     x5, x31, EXIT           # i가 10 이상인 경우 종료
    addi    x6, x5, -1              # x6 = j = i-1
    addi    s0, s0, 4               # s0에 4를 더하여 현재 i 값에 해당하는 array 주소 가리킴
    addi    sp, s0, -4              # sp에 s0 주소의 4를 제외하여 j=i-1의 array 주소 가리킴

    lw  x10, 0(s0)                  # cur = arr[i]
    beq x0, x0, WHILE               # WHILE문으로 이동

WHILE:
    beq x6, x30, FOR_END            # j == -1이면 while문 종료
    lw  x11, 0(sp)                  # x11= arr[j]
    bgt x10, x11, FOR_END           # arr[i]가 arr[j]보다 크면 while문 종료

    lw  x7, 0(sp)                   # arr[j] 값 로드
    sw  x7, 4(sp)                   # 로드한 arr[j]값을 arr[j+1]에 저장
    addi    x6, x6, -1              # j--
    addi    sp, sp, -4              # stack이 j--가 적용된 인덱스의 array를 가리키도록 설정
    beq x0, x0, WHILE               # while문 계속 반복

FOR_END:
    sw x10, 4(sp)                   # arr[j+1]에 arr[i]값 저장
    addi    x5, x5, 1               # i++
    beq x0, x0, SORT                # for 문 계속 반복

EXIT:
