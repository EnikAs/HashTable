# Теперь поговорим про оптимизацию

---

## До оптимизации

Воспользуемся программой callgrind, она выдает суммарную *стоимость* программы - refs

В качестве теста будем заполнять таблицу базой на 200 тыс слов, а потом искать каждое слово 10 раз.

Время посмотреть на первый скрин

![Screenshot from 2022-04-19 15-34-49](https://user-images.githubusercontent.com/89395114/164006341-f610771e-f9e5-4d46-838f-a92b7395d9e7.png)

Это отчет данной программы для не оптимизированной версии хеш-таблицы.

![Screenshot from 2022-04-19 15-34-52](https://user-images.githubusercontent.com/89395114/164006524-bae902a4-f044-4efe-90bc-c78350e8b0ae.png)

Refs = 450 млн

Как мы можем заметить, наибольший вклад вносит функция поиска элементов, далее идет хеш функция и strncmp. 

Применив всю свою силу воли я решил не переписывать strncmp, а переписать таблицу под работу с m256i переменными, так как это даст возможность использовать интринсики, которые заменят strncmp, и использовать crc_hash, который будет явно быстрее murmurHash-а !

В результате программа станет требовательнее по памяти, но зато значительно быстрее.

---

## После оптимизации

![Screenshot from 2022-04-19 15-59-05](https://user-images.githubusercontent.com/89395114/1conveer64009512-60b473e8-5470-400b-9fef-370ff9880711.png)

Refs = 272 млн

Как мы можем заметить, ситуация кардинально изменилась, теперь наибольший вклад вносит функция поиска (что логично, учитывая специфику теста)

На самом деле до этой финальной версии были и другие, так что справедливости ради стоит сказать, что такой уровень быстродействия был достигнут также за счет некоторых манипуляций с логикой программы, но промежуточных результатов нет, так как я забыл их сохранить.

Смысла оптимизтировать функции, кроме функции поиска нет, так как это даст незначительный вклад в общюю картину.

А если говорить о функции поиска - то тут проще уже некуда, там и так используется минимум инструкций, достаточный для корректной работы программы.

Так что я решил остановиться на этой отметке. 

__Итого : программа стала работать на 40% быстрее, тоесть примерно в 1,7 раза.__

КПД = 1000 * /frac{10}{10}








