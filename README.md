# Coursework(Status: ✅)

![preview](https://github.com/Yrti-cpu/Voenmeh_enjoer/assets/113043293/eb606aef-15c1-4d6c-b7aa-ef45458dcb15)

## ТРЕБОВАНИЯ К ВЫПОЛНЕНИЮ КУРСОВОЙ РАБОТЫ
+ 1 Требования к возможностям программы и процессу ее выполнения:
  + 1.1. Приложение реализуется согласно варианту задания
  + 1.2. Программа при выполнении должна иметь графический интерфейс,включающий заставку, меню, справку и средства для управленияпрограммой
    + 1.2.1. Заставка должна выводится на экран при запуске программы идавать представление о ней. На заставке должны быть отраженыназвание приложения, фамилия и номер учебной группы ее автора.Для игр после заставки должен быть реализован ввод имени игрока.
    + 1.2.2. Главное меню должно в обязательном порядке включать в себяпункты «Запустить», «Справка», «Выход». Для игр должны такжеприсутствовать пункты «Смена игрока» и «Рекорды». Названия пунктов меню можно менять для лучшего соответствия варианту. Посогласованию с преподавателем можно добавлять или заменять пункты.
      + 1.2.2.1. При выборе пункта “Запустить” запускается игровойпроцесс, создаётся файл и т.д. в зависимости от варианта.
      + 1.2.2.2. При выборе пункта “Справка” выводится информация овозможностях программы/правилах игры, а также описываютсядействия, которые нужно выполнить для достижения определённых целей
      + 1.2.2.3. При выборе пункта “Выход” приложение закрывается
      + 1.2.2.4. При выборе пункта “Смена игрока” предлагается ввестиновое имя игрока.
      + 1.2.2.5. При выборе пункта меню «Просмотр результатов» наэкран должно в табличном виде выводиться содержимое двоичногофайла результатов (см. п.1.6), все результаты игрока, полученные втечение текущего сеанса игры, и подменю, позволяющее выполнить следующие действия: «очистить список» (при выборе данногопункта очищается файл результатов), «подготовить файл дляпечати» (при выборе данного пункта создается текстовый файл сданными для печати), «вернуться в главное меню».
    + 1.2.3. Для игры: В процессе игры на экране должна присутствовать справочная строка с актуальной для текущего момента информацией («горячие» клавиши, подсказки и т.п.)
    + 1.2.4. Для приложения: Должны быть реализованы горячие клавиши. Какие конкретно и в каком объёме - зависит от варианта и согласовывается с преподавателем.
  + 1.3. Взаимодействия пользователя с приложением осуществляется с помощью мыши и/или клавиатуры.
  + 1.4. Необходимо обеспечить достаточную скорость перерисовки динамически перемещаемых элементов.
  + 1.5. Для вариантов-игр: во время игры должна быть возможность поставить её на паузу или завершить. Если реализована пауза, то из неё или идет продолжение, или выход в главное меню с завершением.
  + 1.6. Для вариантов-игр: десять лучших результатов (очки, минуты и т.п.) сохраняются в файле. Для каждого результата должно быть сохранено имя пользователя и очки.
+ 2 Требования к реализации программы:
  + 2.1. Выполнение курсовой работы должно быть основано на принципах объектно-ориентированного программирования. В работе должна быть построена и обоснована иерархия классов, описывающих предметную область конкретной задачи.
  + 2.2. Программа должна являться графическим приложением, написанным на языке C++ с использованием библиотеки SDL2.
  + 2.3. Все отображаемые на экране элементы пользовательского интерфейса принадлежат к одной из двух иерархий классов: «меню и диалоговые окна» или «графические элементы». Предполагается возможность использования объектов одной иерархии как полиморфных.
    + 2.3.1. В иерархии «меню и диалоговые окна» используется базовый класс «непрозрачное окно с текстовой строкой, выводимое в определенном месте экрана», от которой наследуется 1-2 уровня классов;
    + 2.3.2. В иерархии «графические элементы» все они наследуются от абстрактного базового класса «статический графический элемент», при этом элементы, которые могут перемещаться, образуются путем множественного наследования класса некоторой конкретной геометрической фигуры (многоугольника, прямоугольника, круга и т.п.) и абстрактного базового класса «управляемо перемещаемый по экрану элемент»
  + 2.4. Описание классов одной иерархии должно располагаться в отдельном файле.
  + 2.5. Разнотипные объекты одной иерархии должны быть объединены в общий массив или список. В простых случаях допускается использовать одну простую или ссылочную переменную для работы с разными объектами.
  + 2.6. Для организации корректной перерисовки все отображаемые на экране объекты игрового процесса (перемещаемые и не перемещаемые) должны храниться в списке, отсортированном в требуемом порядке перерисовки (как правило, первым будет объект на заднем плане – фон, последним – объект на переднем плане, чаще всего – управляемый пользователем).
## Игра "Военмех enjoer"
### Правила игры
В разных точках экрана на некоторое время появляются предметы, как полезные, так опасные. Игрок, управляя персонажем, собирает предметы. За каждый вид дается определенное количество очков. По завершению игры на экран выводится набранное число очков. 10 лучших результатов хранятся в файле и выводятся на экран по запросу пользователя. Количество очков в зависимости от предмета: оценка 5 – 12, 4 – 8,  деньги – 7, Витамикс – 6, кофе – 5, чикенбургер – 2, оценка 3 – -2, пачка сигарет – -10. При попадании в инвентарь 3 опасных предметов (оценка 3 или пачка сигарет) игра заканчивается, а набранные очки обнуляются. С течением времени продолжительность жизни предметов уменьшается, но при этом они начинают давать больше очков (x2 или x3). На всю игру отводиться 1,5 минуты. 
