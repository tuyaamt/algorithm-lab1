/*
 * Лабораторная работа №1
 * Интроспективная сортировка (Introsort)
 * Чтение чисел из файла → статический или динамический массив
 *
 * Introsort = QuickSort + HeapSort + InsertionSort
 * Сложность: O(n log n) в худшем случае
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <windows.h>

#define STATIC_MAX_SIZE 100000   /* максимальный размер статического массива */
#define INSERTION_THRESHOLD 16   /* порог для перехода на InsertionSort       */

void insertion_sort(int *arr, int lo, int hi)
{
    for (int i = lo + 1; i <= hi; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= lo) {
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = key;
    }
}

static void sift_down(int *arr, int root, int end, int lo)
{
    while (1) {
        int largest = root;
        int left = lo + 2 * (root - lo) + 1;
        int right = lo + 2 * (root - lo) + 2;

        if (left <= end) {
            if (arr[left] > arr[largest]) {
                largest = left;
            }
        }
        if (right <= end) {
            if (arr[right] > arr[largest]) {
                largest = right;
            }
        }
        if (largest == root) {
            break;
        }

        int tmp = arr[root]; 
        arr[root] = arr[largest]; 
        arr[largest] = tmp;
        root = largest;
    }
}

void heap_sort(int *arr, int lo, int hi)
{
    int n = hi - lo + 1;

    /* Построение кучи */
    for (int i = lo + n / 2 - 1; i >= lo; i--)
    {
        sift_down(arr, i, hi, lo);
    }
        
    /* Извлечение элементов */
    for (int end = hi; end > lo; end--) {
        int tmp = arr[lo]; arr[lo] = arr[end]; arr[end] = tmp;
        sift_down(arr, lo, end - 1, lo);
    }
}

static int median_of_three(int *arr, int lo, int hi)
{
    int mid = lo + (hi - lo) / 2;
    if (arr[lo] > arr[mid]) { int t = arr[lo]; arr[lo] = arr[mid]; arr[mid] = t; }
    if (arr[lo] > arr[hi])  { int t = arr[lo]; arr[lo] = arr[hi];  arr[hi]  = t; }
    if (arr[mid] > arr[hi]) { int t = arr[mid]; arr[mid] = arr[hi]; arr[hi] = t; }

    int t = arr[mid]; arr[mid] = arr[hi - 1]; arr[hi - 1] = t;
    return arr[hi - 1];
}

static int partition(int *arr, int lo, int hi)
{
    int pivot = median_of_three(arr, lo, hi);
    int i = lo, j = hi - 1;
    while (1) {
        while (++i < hi)  { if (arr[i] >= pivot) break; }
        while (--j > lo)  { if (arr[j] <= pivot) break; }
        if (i >= j) break;
        int t = arr[i]; arr[i] = arr[j]; arr[j] = t;
    }
    int t = arr[i]; arr[i] = arr[hi - 1]; arr[hi - 1] = t;
    return i;
}

void introsort_impl(int *arr, int lo, int hi, int depth_limit)
{
    int size = hi - lo + 1;

    if (size <= 1) return;

    if (size <= INSERTION_THRESHOLD) {
        insertion_sort(arr, lo, hi);
        return;
    }

    if (depth_limit == 0) {
        heap_sort(arr, lo, hi);
        return;
    }

    /* Защита: для маленьких диапазонов median_of_three требует >= 3 эл. */
    if (size < 3) {
        insertion_sort(arr, lo, hi);
        return;
    }

    int p = partition(arr, lo, hi);
    introsort_impl(arr, lo,  p - 1, depth_limit - 1);
    introsort_impl(arr, p + 1, hi,  depth_limit - 1);
}

/* Публичная обёртка */
void introsort(int *arr, int n)
{
    if (n <= 1) {
        return;
    }
    int depth_limit = (int)(2.0 * log2((double)n));
    introsort_impl(arr, 0, n - 1, depth_limit);
}

int read_from_file(const char *filename, int **out_ptr, int *out_n, int use_dynamic)
{
    SetConsoleOutputCP(CP_UTF8);
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("Ошибка открытия файла");
        return -1;
    }

    /* Подсчёт количества чисел */
    int count = 0, tmp;
    while (fscanf(f, "%d", &tmp) == 1) {
        count++;
    }
    rewind(f);

    if (count == 0) {
        fprintf(stderr, "Файл пуст или не содержит чисел.\n");
        fclose(f);
        return -1;
    }

    int *arr = NULL;

    if (use_dynamic) {
        /* ── ДИНАМИЧЕСКИЙ массив ── */
        arr = (int *)malloc((size_t)count * sizeof(int));
        if (!arr) {
            fprintf(stderr, "Ошибка выделения памяти.\n");
            fclose(f);
            return -1;
        }
        printf("Режим: ДИНАМИЧЕСКИЙ массив\n");
    } else {
        /* ── СТАТИЧЕСКИЙ массив ── */
        if (count > STATIC_MAX_SIZE) {
            fprintf(stderr, "Слишком много чисел для статического массива "
                            "(max %d).\n", STATIC_MAX_SIZE);
            fclose(f);
            return -1;
        }
        static int static_buf[STATIC_MAX_SIZE];
        arr = static_buf;
        printf("Режим: СТАТИЧЕСКИЙ массив\n");
    }

    for (int i = 0; i < count; i++)
        fscanf(f, "%d", &arr[i]);

    fclose(f);

    *out_ptr = arr;
    *out_n   = count;
    return 0;
}

void print_array(const int *arr, int n, int max_show)
{
    SetConsoleOutputCP(CP_UTF8);
    int show = (n < max_show) ? n : max_show;
    for (int i = 0; i < show; i++) 
    {
        printf("%d ", arr[i]);
    }
    if (n > max_show) {
        printf("... (ещё %d эл.)", n - max_show);
    }
    printf("\n");
}

int is_sorted(const int *arr, int n)
{
    for (int i = 1; i < n; i++) {
        if (arr[i] < arr[i - 1]) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    char filename[256];
    int use_dynamic = -1;

    if (argc >= 2) {
        strncpy(filename, argv[1], 255);
    } else {
        printf("Введите имя файла с числами: ");
        if (scanf("%255s", filename) != 1) {
            fprintf(stderr, "Ошибка ввода.\n");
            return 1;
        }
    }

    if (argc >= 3) {
        use_dynamic = atoi(argv[2]);
    } else {
        printf("Тип массива:\n");
        printf("  1 -- статический  (стек, фиксированный размер %d)\n", STATIC_MAX_SIZE);
        printf("  2 -- динамический (куча, malloc)\n");
        printf("Ваш выбор: ");
        int ch;
        if (scanf("%d", &ch) != 1 || (ch != 1 && ch != 2)) {
            fprintf(stderr, "Неверный выбор.\n");
            return 1;
        }
        use_dynamic = (ch == 2);
    }

    /* ── Чтение данных ── */
    int *arr = NULL;
    int n = 0;

    if (read_from_file(filename, &arr, &n, use_dynamic) != 0) {
        return 1;
    }

    printf("Прочитано чисел: %d\n", n);
    printf("До сортировки: ");
    print_array(arr, n, 20);

    /* ── Сортировка ── */
    introsort(arr, n);

    printf("После сортировки: ");
    print_array(arr, n, 20);

    /* ── Проверка ── */
    printf("Проверка: %s\n", is_sorted(arr, n) ? "OK" : "ОШИБКА!");

    /* ── Освобождение (только динамический) ── */
    if (use_dynamic && arr) {
        free(arr);
    }

    return 0;
}
