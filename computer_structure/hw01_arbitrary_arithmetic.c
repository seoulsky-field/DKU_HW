/**
 * cs_hw01_revised.c    : Arbitrary precision integer arithmetic implementation file
 * @author              : Kyoungmin Jeon
 * @email               : yeon2002.kj@dankook.ac.kr
 * @version             : 0.7.8
 * @date                : 2022. 10. 02. SUN.
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#define MAX_LENGTH 1000

int result[2 * MAX_LENGTH];
int result_size;

int absBigger(int* num1, int* num2, int num1_size, int num2_size);
void addition(int* num1, int* num2, int num1_size, int num2_size);
void subtraction(int* num1, int* num2, int num1_size, int num2_size);
void multiplication(int* num1, int* num2, int num1_size, int num2_size);
void division(int* num1, int* num2, int num1_size, int num2_size);

int main() {
    char temp1[MAX_LENGTH], temp2[MAX_LENGTH];
    int num1[MAX_LENGTH] = {0}, num2[MAX_LENGTH] = {0};
    int num1_size, num2_size, zero_flag = 0;
    long unsigned temp_size;
    char operator;

    // 숫자와 연산자를 각각 문자열, 문자로 입력 받는다.
    scanf("%s %c %s", temp1, &operator, temp2);

    // 입력받은 숫자의 자릿수를 저장하고, 4자리로 끊을 시 int 배열의 총 크기도 구한다.
    temp_size = strlen(temp1);
    num1_size = temp_size / 4;
    if (temp_size % 4 != 0) {
        num1_size++;
    }

    // 입력받은 숫자를 4자리로 끊어 저장하되, 제일 작은 자리부터 끊어서 앞에 오게 저장한다.
    for (int i = temp_size-1; i >= 0; i--) {
        if (isdigit(temp1[i]) != 0) {
            num1[(temp_size-1-i) / 4] += pow(10, (temp_size-1-i) % 4) * (int)(temp1[i] - '0');
        }else {
            if ((temp_size-1-i) % 4 == 0) {
                num1[(temp_size-1-i) / 4 - 1] = -num1[(temp_size-1-i) / 4 - 1];
                num1_size--;
            }else {
                num1[(temp_size-1-i) / 4] = -num1[(temp_size-1-i) / 4];
            }
        }
    }

    // 입력받은 숫자의 자릿수를 저장하고, 4자리로 끊을 시 int 배열의 총 크기도 구한다.
    temp_size = strlen(temp2);
    num2_size = temp_size / 4;
    if (temp_size % 4 != 0) {
        num2_size++;
    }

    // 입력받은 숫자를 4자리로 끊어 저장하되, 제일 작은 자리부터 끊어서 앞에 오게 저장한다.
    for (int i = temp_size-1; i >= 0; i--) {
        if (isdigit(temp2[i]) != 0) {
            num2[(temp_size-1-i) / 4] += pow(10, (temp_size-1-i) % 4) * (int)(temp2[i] - '0');
        }else {
            if ((temp_size-1-i) % 4 == 0) {
                num2[(temp_size-1-i) / 4 - 1] = -num2[(temp_size-1-i) / 4 - 1];
                num2_size--;
            }else {
                num2[(temp_size-1-i) / 4] = -num2[(temp_size-1-i) / 4];
            }
        }
    }

    if (operator == '+') {
        if (num1[num1_size-1] * num2[num2_size-1] >= 0) {
            addition(num1, num2, num1_size, num2_size);  // 양수 + 양수 = 양수, 음수 + 음수 = -(절댓값끼리의 합)
        }else {
            subtraction(num1, num2, num1_size, num2_size); // 음수 + 양수 = 양수 - 절댓값, 양수 + 음수 = 양수 - 절댓값
        }
    }else if (operator == '-') {
        if (num1[num1_size-1] * num2[num2_size-1] >= 0) {
            subtraction(num1, num2, num1_size, num2_size); // 양수 - 양수, 음수1 - 음수2 = 음수2의 절댓값 - 음수1의 절댓값
        }else {
            addition(num1, num2, num1_size, num2_size); // 음수 + 양수 = 양수 - 절댓값, 양수 + 음수 = 양수 - 절댓값
        }
    }else if (operator == '*') {
        multiplication(num1, num2, num1_size, num2_size);
    }else if (operator == '/') {
        if((num2[0] == 0) && (num2_size == 1)) {
            printf("ERROR: CANNOT DIVIDE BY 0.\n");
        }else {
            division(num1, num2, num1_size, num2_size);
        }
    }

    // for (int i = 0; i < num1_size; i++) {
    //     printf("%04d ", num1[i]);
    // }
    // printf("\n");

    // for (int i = 0; i < num2_size; i++) {
    //     printf("%04d ", num2[i]);
    // }
    // printf("\n");

    for (int i = result_size - 1; i >= 0; i--) {
        if (result[i] == 0 && zero_flag != 1) {
            continue;
        }else if (result[i] != 0 && zero_flag != 1) {
            printf("%d", result[i]);
            zero_flag = 1;
        }else {
            printf("%04d", result[i]);
        }
    }
    if (zero_flag != 1) {
        printf("0");
    }

    // printf("RESULT_SIZE = %d, END!\n", result_size);
    printf("\n");

    return 0;
}

// num1이 크다면 1을 반환, num2가 크다면 -1을 반환, 같다면 0을 반환
int absBigger(int* num1, int* num2, int num1_size, int num2_size) {
    if (num1_size > num2_size) {
        // printf("<<<ABSBIGGER: NUM1 SIZE BIG>>>\n");
        return 1;
    }else if (num2_size > num1_size) {
        return -1;
    }else {
        for (int i = 0; i < num1_size; i++) {
            if (abs(num1[num1_size - 1 - i]) > abs(num2[num1_size - 1 - i])) {
                // printf("<<<ABSBIGGER: NUM1 IS BIG>>>\n");
                return 1;
            }else if (abs(num2[num1_size - 1 - i]) > abs(num1[num1_size - 1 - i])) {
                // printf("<<<ABSBIGGER: NUM2 IS BIG>>>\n");
                return -1;
            }else {
                continue;
            }
        }
    }
    return 0;  // for loop가 끝까지 돌았다면 두 수가 같으므로 0 반환
}

// 양수 + 양수, 음수 + 음수, 양수 - 음수, 음수 - 양수에서 사용
void addition(int* num1, int* num2, int num1_size, int num2_size) {   
    int count_size;
    int next = 0, temp;
    int pos_flag = 1;

    if (num1[num1_size-1] < 0) {
        pos_flag = 0;
    }

    if (num1_size > num2_size) {
        count_size = num1_size;
    }else {
        count_size = num2_size;
    }
    result_size = count_size;

    for (int i = 0; i < count_size; i++) {
        temp = 0;

        if (i < num1_size) {
            temp += abs(num1[i]);
        }
        if (i < num2_size){
            temp += abs(num2[i]);
        }

        temp += next;
        result[i] = abs(temp % 10000);
        next = temp / 10000;

        if ((i == count_size - 1) && (next != 0)) {
            result[count_size] = next;
            result_size++;
        }
    }

    if (pos_flag == 0) {
        result[result_size-1] = -result[result_size-1];
    }
}

// 양수 - 양수, 음수 - 음수, 양수 + 음수, 음수 + 양수에서 사용
void subtraction(int* num1, int* num2, int num1_size, int num2_size) {
    int num1_pos_flag = 1, num2_pos_flag = 1;
    int bigger_flag = absBigger(num1, num2, num1_size, num2_size);  // 1 = num1이 크다. 0 = 같다. -1 = num2이 크다.
    int temp = 0, next = 0;

    // printf("SUBTRACTION: %d\n", bigger_flag);

    if (num1[num1_size-1] < 0) {
        num1_pos_flag = 0;
    }

    if (num2[num2_size-1] < 0) {
        num2_pos_flag = 0;
    }

    if (bigger_flag == 0) {
        result[0] = 0;
        result_size = 1;
    }else if (bigger_flag == 1) {
        for (int i = 0; i < num1_size; i++) {
            temp = abs(num1[i]) - next;

            if (i < num2_size) {
                temp -= abs(num2[i]);
            }

            if (temp < 0) {
                temp += 10000;
                next = 1;
            }
            result[i] = temp;
            result_size++;
        }
        if (num1_pos_flag == 0) {
            int zero_flag = 0;
            for (int i = result_size - 1; i >= 0; i--) {
                if (result[i] == 0) {
                    result_size--;
                    continue;
                }else if (result[i] != 0 && zero_flag != 1) {
                    result[i] = -result[i];
                    break;
                }
            }
        }else {
            int zero_flag = 0;
            for (int i = result_size - 1; i >= 0; i--) {
                if (result[i] == 0) {
                    result_size--;
                    continue;
                }else if (result[i] != 0 && zero_flag != 1) {
                    break;
                }
            }
        }
    }else {
        for (int i = 0; i < num2_size; i++) {
            temp = abs(num2[i]) - next;

            if (i < num1_size) {
                temp -= abs(num1[i]);
            }

            if (temp < 0) {
                temp += 10000;
                next = 1;
            }
            result[i] = temp;
            result_size++;
        }
        if (num2_pos_flag == 1) {
            int zero_flag = 0;
            for (int i = result_size - 1; i >= 0; i--) {
                if (result[i] == 0) {
                    result_size--;
                    continue;
                }else if (result[i] != 0 && zero_flag != 1) {
                    result[i] = -result[i];
                    break;
                }
            }
        }else {
            int zero_flag = 0;
            for (int i = result_size - 1; i >= 0; i--) {
                if (result[i] == 0) {
                    result_size--;
                    continue;
                }else if (result[i] != 0 && zero_flag != 1) {
                    break;
                }
            }
        }
    }
}

// 모든 조합의 수에서 사용
void multiplication(int* num1, int* num2, int num1_size, int num2_size) {
    int pos_flag = 1;  // 1: 모두 양수거나 모두 음수, 0: 하나만 음수인 경우
    int count_size;
    // printf("ENTER METHOD\n");

    if (num1_size >= num2_size) {
        // printf("ENTER MULTIPLICATION\n");
        for (int i = 0; i < num2_size; i++) {
            int temp = 0, next = 0;
            int temp_num[2*MAX_LENGTH];
            int temp_size = 0;
            for (int j = 0; j < i; j++) {
                temp_num[j] = 0;
                temp_size++;
            }
            for (int j = 0; j < num1_size; j++) {
                temp = abs(num1[j]) * abs(num2[i]) + next;
                temp_num[i+j] = temp % 10000;
                next = temp / 10000;
                temp_size++;
            }
            if (next != 0) {
                temp_num[temp_size] = next;
                temp_size++;
            }

            addition(temp_num, result, temp_size, result_size);
        }
    }else {
        // printf("ENTER MULTIPLICATION\n");
        for (int i = 0; i < num1_size; i++) {
            int temp = 0, next = 0;
            int temp_num[2*MAX_LENGTH];
            int temp_size = 0;
            for (int j = 0; j < i; j++) {
                temp_num[j] = 0;
                temp_size++;
            }
            for (int j = 0; j < num2_size; j++) {
                temp = abs(num2[j]) * abs(num1[i]) + next;
                temp_num[i+j] = temp % 10000;
                next = temp / 10000;
                temp_size++;
            }
            if (next != 0) {
                temp_num[temp_size] = next;
                temp_size++;
            }
            addition(temp_num, result, temp_size, result_size);
        }
    }

    if (num1[num1_size-1] * num2[num2_size-1] < 0) {
        result[result_size-1] = -result[result_size-1];
    }
}

void division(int* num1, int* num2, int num1_size, int num2_size) {
    int bigger_flag = absBigger(num1, num2, num1_size, num2_size);
    int temp_num[2 * MAX_LENGTH] = {0};
    int temp_result[2*MAX_LENGTH] = {0};
    int num1_pos_flag = 1;
    int count_size = 0, temp_size = 1;
    int ten[1] = {9};  // while에서 이미 한 번 num2로 초기화 했기 때문

    if ((bigger_flag == -1) || ((num1[0] == 0) && num1_size == 1)) {
        result[0] = 0;
        result_size = 1;
    }else if (bigger_flag == 0) {
        if (num1[num1_size-1] * num2[num2_size-1] > 0) {
            result[0] = 1;
            result_size = 1;
        }else {
            result[0] = -1;
            result_size = 1;
        }
    }else {
        count_size = num1_size - num2_size;

        if (num1[num1_size-1] * num2[num2_size-1] < 0) {
            num1_pos_flag = 0;
        }

        num1[num1_size-1] = abs(num1[num1_size-1]);
        num2[num2_size-1] = abs(num2[num2_size-1]);

        int zero[1] = {0};
        int count = (count_size+1) * 4 - 1;
        int for_loop_first_flag = 1;
        int check_count;

        for (int i = 0; i < num2_size; i++) {
            temp_num[i] = num2[i];
        }
        temp_size = num2_size;

        for (int i = 0; i < num1_size; i++) {
            result[i] = num1[i];
        }
        result_size = num1_size;

        division_result_size = (result_size - temp_size) * 4 - 1;

        while (1) {
            if (num1_size != temp_size) {
                for (int i = temp_size; i > 0; i--) {
                    temp_num[i] = temp_num[i-1];
                }
                temp_num[0] = 0;
                temp_size++;
            }else {
                // // 상위 index에 num1과 temp의 자릿 수 및 값 비교를 위함.
                // check_count = num1[num1_size-1] / temp_num[temp_size-1];
                break;
            }
        }

        // if (check_count == 0) {
        //     // 자릿 수 하나 빼기
        //     int next = 0;
        //     for (int i = result_size-1; i >= 0; i--) {
        //         int temp = (result[i]+next*10000) / 10;
        //         next = temp % 10;
        //         division_result_size--;

        //         if ((temp == 0) && (i == result_size-1)) {
        //             result_size--;
        //         }else {
        //             result[i] = temp;
        //         }
        //     }
        // }else if (check_count >= 1000) {
        //     temp_num[temp_size-1] *= 1000;
        //     division_result_size += 3;
        // }else if (check_count >= 100) {
        //     temp_num[temp_size-1] *= 100;
        //     division_result_size += 2;
        // }else if (check_count >= 10) {
        //     temp_num[temp_size-1] *= 10;
        //     division_result_size += 1;
        // }
        
        int while_count = division_result_size;
        while(while_count > 0) {
            if (absBigger(result, temp_num, result_size, temp_size) != -1) {
                subtraction(result, temp_num, result_size, temp_size);
                temp_result[while_count-1]++;
            }else {
                int next = 0;
                for (int i = temp_size-1; i >= 0; i--) {
                    int temp = (temp_num[i]+next*10000) / 10;
                    next = temp % 10;

                    if ((temp == 0) && (i == temp_size-1)) {
                        temp_size--;
                    }else {
                        temp_num[i] = temp;
                    }
                }
                while_count--;
            }
            printf("RESULT: ");
            for (int i = 0; i < result_size; i++) {
                printf("%04d ", result[i]);
            }
            printf("\n");

            printf("TEMP: ");
            for (int i = 0; i < temp_size; i++) {
                printf("%04d ", temp_num[i]);
            }
            printf("\n");
        }
        printf("PLEASE %d\n", division_result_size);
        for (int i = 0; i < division_result_size+1; i++) {
            printf("%d ", temp_result[i]);
        }
        printf("\n");
    }
}
