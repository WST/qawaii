cmake_minimum_required(VERSION 3.16)

project(Qawaii VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Поиск Qt6, если не найден - ищем Qt5
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /usr/lib/x86_64-linux-gnu/cmake)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Sql LinguistTools)

# Включение автоматической обработки MOC, UIC, RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Установка кодировки UTF-8
if(MSVC)
    add_compile_options(/utf-8)
else()
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

# Исходные файлы
set(SOURCES
    src/main.cpp
    src/core/models/Folder.cpp
    src/core/models/Note.cpp
    src/core/storage/StorageDriver.cpp
    src/core/storage/SQLiteDriver.cpp
    src/core/storage/MySQLDriver.cpp
    src/core/services/NotesManager.cpp
    src/core/services/SyncService.cpp
    src/core/services/MigrationService.cpp
    src/ui/MainWindow.cpp
    src/ui/widgets/NotesTreeWidget.cpp
    src/ui/widgets/NoteEditorWidget.cpp
    src/ui/widgets/MarkdownViewer.cpp
    src/ui/dialogs/SettingsDialog.cpp
    src/ui/dialogs/DriverSettingsWidget.cpp
    src/ui/dialogs/IconSelectionDialog.cpp
    src/utils/MarkdownParser.cpp
)

# Заголовочные файлы
set(HEADERS
    src/core/models/Folder.h
    src/core/models/Note.h
    src/core/storage/StorageDriver.h
    src/core/storage/SQLiteDriver.h
    src/core/storage/MySQLDriver.h
    src/core/services/NotesManager.h
    src/core/services/SyncService.h
    src/core/services/MigrationService.h
    src/ui/MainWindow.h
    src/ui/widgets/NotesTreeWidget.h
    src/ui/widgets/NoteEditorWidget.h
    src/ui/widgets/MarkdownViewer.h
    src/ui/dialogs/SettingsDialog.h
    src/ui/dialogs/DriverSettingsWidget.h
    src/ui/dialogs/IconSelectionDialog.h
    src/utils/MarkdownParser.h
)

# UI файлы
set(UI_FILES
    src/ui/MainWindow.ui
    src/ui/dialogs/SettingsDialog.ui
    src/ui/dialogs/IconSelectionDialog.ui
)

# Ресурсы
set(RESOURCES
    resources/resources.qrc
)

# Создание исполняемого файла
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
    ${RESOURCES}
)

# Подключение Qt библиотек
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

# Файлы переводов
set(TS_FILES
    resources/translations/qawaii_ru.ts
    resources/translations/qawaii_en.ts
)

# Обновление переводов
qt_add_translations(${PROJECT_NAME} TS_FILES ${TS_FILES})

# Установка переводов
install(FILES ${TS_FILES} DESTINATION translations)

# Установка исполняемого файла
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Настройки для отладки
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE QT_QML_DEBUG)
endif()

# ============================================
# Тесты
# ============================================
enable_testing()

# Поиск Qt Test
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

# Тесты для SQLiteDriver
add_executable(TestSQLiteDriver
    tests/TestSQLiteDriver.cpp
    src/core/storage/SQLiteDriver.cpp
    src/core/storage/StorageDriver.cpp
    src/core/models/Folder.cpp
    src/core/models/Note.cpp
)

target_link_libraries(TestSQLiteDriver PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Test
)

# Добавляем тест в CTest
add_test(NAME SQLiteDriverTests COMMAND TestSQLiteDriver)

