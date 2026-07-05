# Семестр 3 — Задание 11. B-дерево объектов `student`

## Требования к программам

1. В программе реализованы: контейнер `student` (как в предыдущих заданиях: `read`, `print`, `operator<`) и **шаблонное B-дерево** порядка `m` (каждый узел хранит до `2m` значений и до `2m+1` указателей на потомков; переполнение узла при вставке приводит к расщеплению и подъёму среднего значения — стандартный алгоритм вставки в B-дерево):

   ```cpp
   template <class T> class b_tree_node;
   template <class T> class b_tree;

   template <class T>
   class b_tree_node
     {
       T* values = nullptr;              // массив из 2*m элементов
       int size = 0;                     // текущее число значений
       b_tree_node** children = nullptr; // массив из 2*m+1 указателей
     public:
       ...
       void print (int p = 0);
       int bin_search (const T& x) const; // позиция для вставки
       friend class b_tree<T>;
     private:
       void erase_links ();
       void delete_node ();
       io_status init (int m);
       void add_value (T& x, b_tree_node<T>* down, int index);
     };

   template <class T>
   class b_tree
     {
     private:
       int m = 0;                 // порядок B-дерева
       b_tree_node<T> * root = nullptr;
     public:
       b_tree (int i = 0) { m = i; }
       ...
       io_status read (FILE * fp = stdin);
       void print (int r, FILE * fp = stdout) const;
       ~b_tree ();
     private:
       static void delete_subtree (b_tree_node<T> * curr);
       static void print_subtree (b_tree_node<T> * curr, int level, int r, FILE * fp = stdout) const;
       // Вставка x в дерево; при переполнении узла - расщепление
       // и возврат нового узла через io_status::create
       io_status add_value (T& x);
       static io_status add_value_subtree (b_tree_node<T>*& curr, b_tree_node<T>*& down, T& x, int m);
     };
   ```

   Все функции задания — методы класса `b_tree<T>`, вызываются для `T = student`.

2. Программа должна получать все параметры в качестве аргументов командной строки: `m` – порядок B-дерева, `r` – макс. количество выводимых уровней, `k` – параметр задачи, `filename`. Например, `./a.out 2 4 3 a.txt`.
3. Класс `b_tree` должен содержать функцию ввода дерева из файла: строки вида `Слово-i Целое-число-i` (все записи различны). Дерево заполняется как упорядоченное B-дерево (вставка по стандартному алгоритму с расщеплением узлов при переполнении). Никакие другие функции, кроме чтения, не используют упорядоченность.
4. Класс `b_tree` должен содержать подпрограмму вывода на экран не более `r` уровней дерева, с понятной структурой.
5. Дерево не изменяется функциями задач — поэтому вся работа идёт в одной `main` (один исполняемый файл `a.out`):

   ```cpp
   int main (int argc, char *argv[])
   {
     // ... ввод аргументов ...
     b_tree<student> *a = new b_tree<student>;
     // чтение, вывод, вызов задач 1-6, вывод результатов и времени
     delete a;
     return 0;
   }
   ```

6. Вывод результата в `main`:

   ```cpp
   printf ("%s : Task = %d M = %d K = %d Result = %d Elapsed = %.2f\n",
           argv[0], task, m, k, res, t);
   ```

## Задачи

Функция — член класса `b_tree`, получающая целое число `k`:

1. Возвращает количество элементов `student` в узлах, имеющих ровно `k` потомков.
2. Возвращает количество элементов `student` в поддеревьях, имеющих не более `k` вершин.
3. Возвращает количество элементов `student` в поддеревьях, имеющих не более `k` уровней.
4. Возвращает количество элементов `student` в поддеревьях, имеющих не более `k` узлов в любом уровне.
5. Возвращает количество элементов `student` в `k`-м уровне дерева.
6. Возвращает количество элементов `student` во всех ветвях длины `k`, начиная с корня.
