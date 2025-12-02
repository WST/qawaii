# Qawaii - Менеджер заметок

Приложение для управления заметками на Qt с поддержкой различных драйверов хранилища.

## Требования

- CMake 3.16 или выше
- Qt 5.15+ или Qt 6.x
- Компилятор с поддержкой C++17

## Сборка

```bash
mkdir build
cd build
cmake ..
make
```

Или с использованием Qt Creator:
1. Откройте проект через File -> Open File or Project
2. Выберите CMakeLists.txt
3. Нажмите Configure Project
4. Запустите сборку (Ctrl+B)

## Структура проекта

- `src/core/models/` - Модели данных (Note, Folder)
- `src/core/storage/` - Драйверы хранилища (StorageDriver, SQLiteDriver, MySQLDriver)
- `src/core/services/` - Бизнес-логика (NotesManager, SyncService, MigrationService)
- `src/ui/` - UI компоненты
- `resources/` - Ресурсы и файлы локализации

## Текущий статус

Проект находится на этапе начальной разработки. Базовая структура создана, основные компоненты определены.

Подробный план разработки см. в файле `plan.md`.

