// gen.c — генератор тестового файла с числами 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> // нужен для SetConsoleOutputCP 


int main(int argc, char *argv[])
{
    // Устанавливаем кодировку консоли UTF-8, чтобы русский текст отображался корректно 
    SetConsoleOutputCP(CP_UTF8);

    /*
     Считываем аргументы командной строки:
        argv[1] — количество чисел (n), если не передан — по умолчанию 20
        argv[2] — режим генерации  (mode), если не передан — по умолчанию 0
    */
    int n = (argc >= 2) ? atoi(argv[1]) : 20;
    int mode = (argc >= 3) ? atoi(argv[2]) : 0;
    // mode: 0=random, 1=reversed, 2=sorted, 3=all equal 


 /*
 Режимы генерации (mode):
   0 — случайные числа (random)
   1 — обратный порядок: n, n-1, ..., 1  (reversed) — худший случай для QuickSort
   2 — возрастающий порядок: 0, 1, ..., n-1 (sorted)
   3 — все числа одинаковы (all equal = 42)
 */

    // Открываем файл input.txt для записи 
    FILE *f = fopen("input.txt", "w");
    if (!f) { perror("fopen"); return 1; }  //если не удалось — выводим ошибку и выходим 
    
    //Инициализируем генератор случайных чисел текущим временем
    srand((unsigned)time(NULL));
  
     // Генерируем n чисел и записываем каждое на отдельную строку 
    for (int i = 0; i < n; i++) {
        int val;
        if (mode == 1) val = n - i;   // убывающая последовательность 
        else if (mode == 2) val = i;  // возрастающая последовательность 
        else if (mode == 3) val = 42; // константа 
        else val = rand() % (n * 10); // случайное число в диапазоне [0, n*10) 
        fprintf(f, "%d\n", val);  // запись числа в файл 
    }

    fclose(f);
    printf("Сгенерировано %d чисел в input.txt (mode=%d)\n", n, mode);
    return 0;
}
