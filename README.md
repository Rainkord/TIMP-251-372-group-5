# ТМП — Подгруппа 5
---

## Структура проекта

```
├── client/          # Qt-приложение (виджеты, GUI)
├── server/          # Серверная часть (обработка запросов, БД)
├── docker/          # Вспомогательные скрипты для Docker
└── README.md
```

---

## Сборка клиента

```bash
cd client
qmake client.pro
make -j$(nproc)
./client
```
## Сборка сервера

```bash
cd server
cmake -B build && cmake --build build -j$(nproc)
./build/server
```

---

Если при сборке Docker-образа `docker build` падает с ошибками:

```bash
chmod +x docker/fix-dns.sh
./docker/fix-dns.sh
```

## Компоненты клиента

| Файл | Назначение |
|---|---|
| `authwidget.cpp/h` | Экран авторизации |
| `regwidget.cpp/h` | Экран регистрации + подтверждение email |
| `resetwidget.cpp/h` | Восстановление пароля (3 шага) |
| `verifywidget.cpp/h` | Подтверждение кода при входе (2FA) |
| `clientsingleton.cpp/h` | TCP-соединение с сервером (Singleton) |
| `mainwindow.cpp/h` | Главное окно, переключение экранов |
| `graphwidget.cpp/h` | Граф / блок-схема (основная функция приложения) |

---
