# Семестр 1 — Задание 10. Реализация строковых функций из `<string.h>`

## Требования к программам

1. Программа должна получать все параметры в качестве аргументов командной строки.
2. Аргументы командной строки для задач 1–2:
   1. `n` – задает число повторений вызова функции и используется для измерения времени, имеет тип `size_t`,
   2. `s1` – строка.

   Например, `./a01.out 10000000 "abcd"` — измерить время 10000000 вызовов функции для строки `"abcd"`.

3. Аргументы командной строки для задач 3–4: `n`, `s1` – строка, `ch` – символ. Например, `./a03.out 10000000 "abcd" 'b'`.
4. Аргументы командной строки для задач 5–10: `n`, `s1` – строка, `s2` – строка. Например, `./a05.out 10000000 "abcd" "efgh"`.
5. Программа должна содержать функцию измерения времени работы `n` вызовов указанной функции. Функция тестирования получает `n`, указатель на тестируемую функцию, её аргументы, указатель на место для результата, и возвращает время работы в секундах. Прототипы:

   ```c
   // задача 1
   double test_1 (size_t n, size_t (*p) (const char *),
                  const char *s, size_t *result);
   // задачи 2 и 5
   double test_2_5 (size_t n, char* (*p) (char*, const char *),
                    char *s1, const char *s2, char **result);
   // задачи 3, 4
   double test_3_4 (size_t n, char* (*p) (const char *, int),
                    const char *s, int ch, char **result);
   // задача 6
   double test_6 (size_t n, int (*p) (const char*, const char *),
                  const char *s1, const char *s2, int *result);
   // задачи 7, 8
   double test_7_8 (size_t n, size_t (*p) (const char*, const char *),
                    const char *s1, const char *s2, size_t *result);
   // задача 9
   double test_9 (size_t n, char* (*p) (const char*, const char *),
                  char *s1, const char *s2, char **result);
   // задача 10
   double test_10 (size_t n, char* (*p) (char*, const char *, char**),
                   char *str, const char *delim, char **saveptr);
   ```

   В задаче 5 после каждого вызова тестируемой функции надо приводить строку `s1` в исходное состояние (проставляя символ с кодом 0 на прежнее место); в задаче 2 можно делать то же самое. В задаче 10 после каждого вызова надо восстанавливать символ на месте первого найденного разделителя.

6. Пример функции тестирования:

   ```c
   double test_1 (size_t n, size_t (*p) (const char *),
                  const char *s, size_t *result)
   {
     double t; size_t i, r;
     t = clock ();
     for (i = 0; i < n; i++)
       r = (*p) (s);
     t = (clock () - t) / CLOCKS_PER_SEC;
     *result = r;
     return t;
   }
   ```

7. Функция тестирования вызывается из `main` сначала для реализованной функции, затем для стандартной библиотечной функции с тем же прототипом (без символа `_` в конце, прототипы в `<string.h>`), и результаты обеих выводятся.
8. Вывод результата для задач 1, 7, 8:

   ```c
   printf ("%s : Task = %d Res = %zd Elapsed = %.2f\n",
           argv[0], task, res, t);
   ```

9. Вывод результата для задачи 6:

   ```c
   printf ("%s : Task = %d Res = %d Elapsed = %.2f\n",
           argv[0], task, res, t);
   ```

10. Вывод результата для задач 2, 5:

    ```c
    printf ("%s : Task = %d Res = %s Elapsed = %.2f\n",
            argv[0], task, res, t);
    ```

11. Вывод результата для задач 3, 4, 9: тот же формат, что в п. 10, где `res` — строка результата, если возвращаемое значение не 0, иначе `"Not found"`.
12. Вывод результата для задачи 10:

    ```c
    printf ("%s : Task = %d Res = %s Saveptr = %s Elapsed = %.2f\n",
            argv[0], task, res, next, t);
    ```

    `res`/`next` — результат/остаток строки, либо `"Not found"`.

## Задачи

Реализовать аналоги стандартных функций `<string.h>` (с суффиксом `_` в имени), сравнить их производительность со стандартной библиотечной реализацией:

1. `size_t strlen_ (const char *string);` — длина строки в байтах, без учёта завершающего нулевого символа.
2. `char *strcpy_ (char *string1, const char *string2);` — копирует `string2` (включая `\0`) в `string1`, возвращает `string1`.
3. `char *strchr_ (const char *string, int ch);` — указатель на первое вхождение символа `ch` в строку (0, если не найден); завершающий `\0` включается в поиск.
4. `char *strrchr_ (const char *string, int ch);` — то же самое, но последнее вхождение.
5. `char *strcat_ (char *string1, const char *string2);` — добавляет `string2` (включая `\0`) в конец `string1`, возвращает `string1`.
6. `int strcmp_ (const char *string1, const char *string2);` — лексикографическое сравнение строк (`<0`, `=0`, `>0`).
7. `size_t strcspn_ (const char *string1, const char *string2);` — индекс первого символа `string1`, принадлежащего набору символов `string2`.
8. `size_t strspn_ (const char *string1, const char *string2);` — индекс первого символа `string1`, **не** принадлежащего набору символов `string2`.
9. `char *strstr_ (const char *string1, const char *string2);` — указатель на первое вхождение `string2` в `string1` (0, если не найдено).
10. `char *strtok_r_ (char *str, const char *delim, char **saveptr);` — указатель на первое слово в `str` (слово не содержит символов из `delim`); записывает `\0` на место первого разделителя после слова, адрес следующего символа сохраняет в `saveptr`; возвращает 0, если слово не найдено.
