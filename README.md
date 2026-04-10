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

---

## Настройка server/email.txt

Файл `server/email.txt` хранит учётные данные почты, от имени которой сервер отправляет письма с кодами подтверждения (регистрация, сброс пароля).

**Формат файла:**
```
# Ключ можно получить: https://myaccount.google.com/apppasswords
email=
key=
```

**Заполнение:**

1. Откройте файл `server/email.txt`
2. В поле `email=` укажите адрес Gmail-почты, от которой будут рассылаться письма:
   ```
   email=your_address@gmail.com
   ```
3. В поле `key=` укажите **пароль приложения** (не пароль от аккаунта Google):
   ```
   key=xxxx xxxx xxxx xxxx
   ```

**Как получить пароль приложения Google:**

1. Перейдите на [https://myaccount.google.com/apppasswords](https://myaccount.google.com/apppasswords)
2. Войдите в аккаунт Google, от которого будет работать рассылка
3. В поле «Название приложения» введите любое имя (например, `TIMP Server`)
4. Нажмите **Создать** — Google выдаст 16-символьный пароль вида `xxxx xxxx xxxx xxxx`
5. Скопируйте его в поле `key=` файла `email.txt`

> ⚠️ **Важно:** файл `email.txt` добавлен в `.gitignore` — не коммитьте его в репозиторий, чтобы не раскрыть учётные данные.

**Готовый пример:**
```
# Ключ можно получить: https://myaccount.google.com/apppasswords
email=example@gmail.com
key=abcd efgh ijkl mnop
```

---

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
