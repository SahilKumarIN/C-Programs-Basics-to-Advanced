#include <stdio.h>

int pivotIndex(int* nums, int numsSize) {
    int totalSum = 0, leftSum = 0;
    for (int i = 0; i < numsSize; i++) {
        totalSum += nums[i];
    }
    for (int i = 0; i < numsSize; i++) {
        if (leftSum == totalSum - leftSum - nums[i]) {
            return i;
        }
        leftSum += nums[i];
    }
    return -1;
}

int main() {
    int nums[] = {1, 7, 3, 6, 5, 6};
    int size = sizeof(nums) / sizeof(nums[0]);
    int result = pivotIndex(nums, size);
    printf("Pivot Index: %d\n", result);
    return 0;
}
