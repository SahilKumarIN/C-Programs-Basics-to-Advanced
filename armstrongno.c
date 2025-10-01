#include <stdio.h>
#include <math.h>
int main()
{
  int num, a, sum = 0, d = 0, m;
  printf("Enter your number");
  scanf("%d", &num);
  a = num;
  while (a > 0)
  {
    a = a / 10;
    d++;
  }
  a = num;
  while (a > 0)
  {
    m = a % 10;
    sum = sum + pow(m, d);
    a = a / 10;
  }
  if (num == sum)
  {
    printf("%d is armstrong", num);
  }
  else
  {
    printf("%d is not armstrong", num);
  }
  return 0;
}