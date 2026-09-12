# Лабораторная работа №1

Linux/C++17 реализация триал-ограничения на 4 запуска.

## Сборка

```bash
cmake -S . -B build
cmake --build build
./build/SecureTrial
```

## Проверка что файлов не видно

После сборки:

```bash
strings build/SecureTrial
```

Команда не должна выводить m.dat и x.dat

Пути контрольных файлов находятся в `main.cpp` только в виде XOR-обфусцированных массивов байтов. Они восстанавливаются во время выполнения.

## Проверка количества запусков

Программа должна запуститься 4 раза. На пятом запуске:

```text
Trial period has ended or control data is invalid.
```

## Где хранятся данные

Фактические пути формируются относительно `$HOME`; три файла находятся в разных пользовательских каталогах. 

## Проверка размера и времени

Можно посмотреть:

```bash
stat ~/.config/.cache/.sysdata/m.dat
stat ~/.local/share/.appdata/x.dat
```


# Чтоб удалить и начать заново
```bash
rm -f "$HOME/.config/.cache/.sysdata/m.dat"
rm -f "$HOME/.local/share/.appdata/x.dat"
rm -f "$HOME/.cache/.desktop-data/desktop.dat"
```
