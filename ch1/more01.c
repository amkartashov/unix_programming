/* more01.с - версия 0.1 программы more
 *  читает и выводит на экран 24 строки, затем следуют несколько
 * специальных команд */
#include <stdio.h>
#define PAGELEN 24
#define LINELEN 512

void do_more(FILE *);
int see_more();

int main(int ac, char *av[])
{
  FILE *fp;
  if (ac == 1)
    do_more(stdin);
  else
    while (--ac)
      if ((fp = fopen(*++av, "r")) != NULL)
      {
        do_more(fp);
        fclose(fp);
      }
      else
        exit(1);
  return 0;
}

void do_more(FILE *fp)
/*
 * читает PAGELEN строк, затем вызывает see more() для получения дальнейших инструкций
 */
{
  char line[LINELEN];
  int num_of_lines = 0;
  int see_more(), reply;
  while (fgets(line, LINELEN, fp))
  { /* ввод для more */
    if (num_of_lines == PAGELEN)
    {                     /* весь экран? */
      reply = see_more(); /* у: ответ пользователя */
      if (reply == 0)     /* n : завершить */
        break;
      num_of_lines -= reply; /* переустановка счетчика */
    }
    if (fputs(line, stdout) == EOF) /* показать строку */
      exit(1);                      /* или закончить */
    num_of_lines++;                 /* учесть очередную строку */
  }
}

int see_more()
/*
 *  выдать сообщение, ожидать ответа, возвратить значение числа строк
 *  q означает по, пробел означает yes, CR означает одну строку
 */
{
  int c;
  printf("\033[7m more? \033[m"); /* реверс изображения для vt100 */
  while ((c = getchar()) != EOF)  /* получение ответа */
  {
    if (c == 'q') /*q->N */
      return 0;
    if (c == ' ')     /* " => следующая страница */
      return PAGELEN; /* сколько показывать */
    if (c == '\n')    /* Требование на 1 строку */
      return 1;
  }
  return 0;
}