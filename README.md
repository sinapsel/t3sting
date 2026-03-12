# Маленькая библиотека для домашнего тестирования на СИ

## Установка

Достаточно клонировать репозиторий себе в проект в качестве подмодуля

```sh
git submodule add https://github.com/sinapsel/t3sting.git t3sting
```

## Сборка

### 1. Описание сборки тестируемого кода как библиотеки

Для правильной работы фреймворка следует установить глобальную переменную `APP_LIB_TARGET`.

```cmake
set(APP_LIB_TARGET
    app_lib
    CACHE INTERNAL "TARGET LIBRARY"
)
```

По имени этой переменной указывается цель-библиотека, которую планируется тестировать.

```cmake
add_library(${APP_LIB_TARGET} STATIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/...
    ...
)
```

В случае необходимости указываются пути к заголовочным файлам и компоновка с прочими библиотеками.

```cmake
target_include_directories(${APP_LIB_TARGET} 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ...
)
target_link_libraries(${APP_LIB_TARGET}
    ...
)
```

### 2. Создание модульных тестов

Удобно будет разместить тесты в отдельной папке (для примера tests/) с соответствующим CMakeLists.txt.

Каждый модуль тестирования может быть добавлен вызовом функции `t3st_add(name sources...)`

```cmake
# tests/CMakeLists.txt
t3st_add(test_add test_add.c)
```


### 3. Подключение фреймворка тестов
В корневом CMakeLists.txt, где ранее устанавливалась переменная `APP_LIB_TARGET`:
```cmake
enable_testing()            # поддержка CTest
add_subdirectory(t3sting)   # подключение тестового фреймворка
add_subdirectory(tests)     # подключение тестов
```

### 4. Параметры запуска
Сборщику дополнительно можно сообщить аргументы:
- `TEST_VERBOSITY`: Насколько подробно выводить результат тестирования. 

При `-DTEST_VERBOSITY=0` отображаются только непройденные тесты
При `-DTEST_VERBOSITY=1` (по умолчанию) отображаются все результаты тестирования 
При `-DTEST_VERBOSITY=2` отображаются все результаты тестирования и выводится информация о перехвате `abort()` 

### 5. Запуск
Из папки сборки, содержащей `CMakeCache.txt` для тихой проверки следует выполнить:
```sh
ctest
```
или для подробной:
```sh
ctest -V
```
Также допустимо вызывать тестовые модули отдельно.

## Тесты

Тестовый модуль начинается с подключения общего заголовочного файла фреймворка:
```c
#include <t3sting.h>
```

### Структура

Для работы системы авторегистрации тестов следует каждый тестовый случай оборачивать в макрос `TEST` и придерживаться следующей структуры:
```c
TEST(name) {
    #pragma region arrange
        // Подготовка
    #pragma endregion arrange


    #pragma region act
        // Действие
    #pragma endregion act


    #pragma region assert
        // Предположения
    #pragma endregion assert
}
```

Функций `main()` писать не нужно, как и запускать тесты вручную, все произойдет автоматически при вызове `ctest`.

Пример вывода:
```
===TESTING CASE: rectangle_positive_values===

~/project/tests/add.c:34: sum == 20.0 PASSED

===1 of 1 tests passed 100.00% SUCCEEDED===
``` 

### Методы проверок

- `ASSERT(expr)` проверка выражения expr на истинность
`abort` в рамках тестового случая еще не была вызвана
- `ASSERT_EQ(expr, expected)` проверка равенства выражения `expr` ожидаемому `expected`
- `ASSERT_NEQ(expr,  expected)` проверка неравенства выражения `expr` ожидаемому `expected`
- `ASSERT_STR_EQ(left, right)` проверка равенства null-terminated строк
- `ASSERT_EQ_EPS(expr, expected, eps)`  проверка равенства выражения `expr` ожидаемому `expected` с погрешностью не более `eps`
- `EXPECT_ABORTED()` проверка факта (перехваченного) вызова функции `abort`
- `EXPECT_UNABORTED()` проверка того, что функция 


## Пример использования

Положим, тестируется некая функция `int is_even(int);`, от которой ожидается получить:
- 1, если входной параметр четный
- 0, если входной параметр нечетный
- экстренный выход, если входной параметр отрицательный

### Исодный код тестового модуля

```c
// test_is_even.c
#include <t3sting.h>
#include "numeric_func.h"

TEST(positive_even) {
    #pragma region arrange
        int input1 = 4;
        int input2 = 404;

        int expected = 1;
    #pragma endregion arrange


    #pragma region act
        bool actual1 = is_even(input1);
        bool actual2 = is_even(input2);
    #pragma endregion act


    #pragma region assert
        ASSERT_EQ(actual1, expected);
        ASSERT(actual2 == 1);
    #pragma endregion assert
}

TEST(positive_odd) {
    #pragma region arrange
        int input1 = 5;
        int input2 = 501;

        int expected = 0;
    #pragma endregion arrange


    #pragma region act
        bool actual1 = is_even(input1);
        bool actual2 = is_even(input2);
    #pragma endregion act


    #pragma region assert
        ASSERT_EQ(actual1, expected);
        ASSERT(actual2 == 0);
    #pragma endregion assert
}


TEST(negative_exit) {
    #pragma region arrange
        int input = -1;
    #pragma endregion arrange


    #pragma region act
        bool actual = is_even(input);
    #pragma endregion act


    #pragma region assert
        EXPECT_ABORTED();
    #pragma endregion assert
}
```

### Файл конфигуарции сборки

```cmake
# CMakeLists.txt


# Тестируемая библиотека
set(APP_LIB_TARGET
    app_lib
    CACHE INTERNAL "TARGET LIBRARY"
)

add_library(${APP_LIB_TARGET} STATIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/numeric_func.c
)
target_include_directories(${APP_LIB_TARGET} 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


# Тестирование
enable_testing()            # поддержка CTest
add_subdirectory(t3sting)   # подключение тестового фреймворка
t3st_add(is_even_test test_is_even.c) # Формирование цели тестового модуля

```

### Запуск

Подготовка проекта сборке
```sh
cmake -S . -B build
```

Сборка
```sh
cd build

cmake --build .
```

Запуск
```sh
ctest -V
```