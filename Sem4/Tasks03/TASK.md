# Семестр 4 — Задание 3. Мини-СУБД: поиск по условию (`record`/`command`, v1)

Первая версия учебного проекта мини-СУБД, который развивается дальше в заданиях [4](../Tasks04/TASK.md)–[9](../Tasks09/TASK.md): контейнер записей `record` с полями `name`/`phone`/`group` и разбор текстовых команд поиска.

## Требования к программам

1. В программе должны быть реализованы следующие структуры данных:
   - Enum `condition` — условие сравнения поля: `none, eq, ne, lt, gt, le, ge, like`.
   - Контейнер `record` (move-only, `unique_ptr<char[]>` для `name`, целые `phone`, `group`):

     ```cpp
     enum class io_status { success, eof, format, memory, open, create };

     class record
     {
       private:
         std::unique_ptr<char []> name = nullptr;
         int phone = 0;
         int group = 0;
       public:
         record () = default;
         const char * get_name () const { return name.get (); }
         int get_phone () const { return phone; }
         int get_group () const { return group; }
         int init (const char *n, int p, int g);
         record (record &&x) = default;
         record& operator= (record&& x) = default;
         record (const record &x) = delete;
         record& operator= (const record&) = delete;
         // Проверить условие x для поля name/phone/group для this и y
         bool compare_name (condition x, const record& y) const;
         bool compare_phone (condition x, const record& y) const;
         bool compare_group (condition x, const record& y) const;
         void print (FILE * fp = stdout);
         io_status read (FILE *fp = stdin);
     };
     ```

     Функции `compare_*` сравнивают одно из полей `this` с соответствующим полем `y` согласно условию `x` (пример реализации `compare_phone` — простой `switch` по `condition`, для `like` на нестроковых полях возвращается `false`).

   - Класс `command : public record`, задающий разобранное условие поиска:

     ```cpp
     class command : public record
     {
       private:
         condition c_name = condition::none;
         condition c_phone = condition::none;
         condition c_group = condition::none;
       public:
         command () = default;
         // Разбор строки команды в структуру, например:
         // "phone = 1234567"  -> {command::phone=1234567, c_phone=condition::eq}
         // "name like St%"    -> {command::name="St%",    c_name=condition::like}
         bool parse (const char * string);
         void print (FILE *fp = stdout) const;
         bool apply (const record& x) const; // применить условие к записи x
     };
     ```

2. Задача программы:
   - Построить двунаправленный список объектов `record` и считать его из указанного файла (аргумент командной строки).
   - Считывать команды поиска по одной со стандартного ввода (`stdin`) до конца потока.
   - Применять команду к списку и выводить в `stdout` только найденные элементы; после каждой команды — пустая строка (даже если ничего не найдено).
3. Команда поиска задаёт одно условие на одно поле `record`:
   - `<поле> <оператор> <выражение>`, где `<поле>` — `name`/`phone`/`group`, `<оператор>` — `=`, `<>`, `<`, `>`, `<=`, `>=` (как в C).
   - `<поле> like <образец>` — только для `name`; образец поддерживает спецсимволы:
     - `_` — один любой символ (`\_`, `\\` — литералы);
     - `%` — 0 или более любых символов (`\%`, `\\` — литералы);
     - `[n-m]` — один символ с кодом в диапазоне `n...m` (`\[`, `\]`, `\\` — литералы);
     - `[bn-m]` — один символ с кодом **вне** диапазона `n...m` (`\[`, `\]`, `\b`, `\\` — литералы).

   Примеры: `phone >= 1234567`, `group = 209`, `name like St%`.
4. Программа должна получать все параметры в качестве аргументов командной строки и стандартного ввода: `filename` – имя файла со списком. Например, `cat commands.txt | ./a.out a.txt > result.txt`.
5. Класс "список" должен содержать функцию ввода списка из файла: строки вида `Слово-i Целое-число-1 Целое-число-2` (имя, телефон, группа); все записи различны. Концом ввода считается конец файла.
6. Вывод результата в `main`:

   ```cpp
   printf ("%s : Result = %d Elapsed = %.2f\n", argv[0], res, t);
   ```

   где `res` — общее количество найденных элементов по всем командам, `t` — время работы на все команды.

## Задачи

Написать программу, которая строит двунаправленный список `record`, читает команды поиска со стандартного ввода и выводит найденные элементы. Задание оценивается по качеству системы классов и по реализации:

1. Условие вида `<поле> <оператор> <выражение>`.
2. Условие `<поле> like <образец>` со спецсимволом `_`.
3. Условие `<поле> like <образец>` со спецсимволом `%`.
4. Условие `<поле> like <образец>` со спецсимволами `[n-m]`.
5. Условие `<поле> like <образец>` со спецсимволами `[bn-m]`.
