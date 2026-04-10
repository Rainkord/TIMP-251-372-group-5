# ТМП — Подгруппа 5

**Тема:** Графическое отображение ветвящейся функции в рамках клиент-серверного проекта

**Функция №9** (с параметрами a, b, c):

```
        ⎧ |x·a| − 2       , x < −2
f(x) =  ⎨ b·(x²) + x + 1  , −2 ≤ x < 2
        ⎩ |x − 2| + 1·c    , x ≥ 2
```

## Участники

| ФИО | Роль |
|-----|------|
| Орлов Руслан (Админ) | Сервер, GitHub |
| Карелин Кирилл | Численная задача, Doxygen, Отчёты |
| Серёгина Елизавета | БД, Docker |
| Воробьёва Елизавета | Тестирование, Отчёты |
| Сарафанов Алексей | UI (Qt), Авторизация/Регистрация |

## Структура проекта

```
├── server/          — TCP-сервер (Qt5, SQLite, SMTP)
│   ├── email.txt    — учётные данные SMTP (не коммитить с ключом!)
│   └── ...
├── client/          — GUI-клиент (Qt5 Widgets)
├── docker/          — Dockerfile для сервера
├── docs/            — Doxyfile + сгенерированная документация
├── tests/           — Тесты калькулятора
└── README.md
```

## Возможности

- Регистрация с подтверждением по email (6-значный код)
- Авторизация с двухфакторной аутентификацией (код на email)
- Пароли хранятся в виде SHA-256 хэша
- Блокировка при неудачных попытках входа (30с → 5мин → 10мин → 9999мин)
- Интерактивный график с параметрами a, b, c (ползунки)
- Три цветных ветви функции на графике
- Таблица значений x / F(x)
- Окно с заданием и блок-схемой вычислительного процесса
- Docker-контейнер для сервера

---

## Первый запуск (клонирование)

```bash
git clone https://github.com/Rainkord/Test
cd Test

# Запретить git следить за изменениями в файле с ключом
# (выполнить один раз после клонирования, ДО того как заполнять email.txt)
git update-index --assume-unchanged server/email.txt
```

Затем открой `server/email.txt` и заполни:

```
# Ключ можно получить: https://myaccount.google.com/apppasswords
email=your@gmail.com
key=xxxx xxxx xxxx xxxx
```

Пробелы в ключе убираются автоматически при чтении.

---

## Зависимости

### Arch Linux / CachyOS

```bash
sudo pacman -S qt5-base openssl sqlite
```

### Docker (для сборки образа)

```bash
sudo pacman -S docker
sudo systemctl enable --now docker
sudo usermod -aG docker $USER
# Перелогиниться или выполнить: newgrp docker
```

Если `docker build` не может скачать пакеты (ошибка DNS) — добавь в `/etc/docker/daemon.json`:

```json
{
  "dns": ["8.8.8.8", "1.1.1.1"]
}
```

```bash
sudo systemctl restart docker
```

---

## Сборка и запуск

### Сервер (нативно)

```bash
cd server
qmake echoServer.pro
make -j$(nproc)
./echoServer
```

### Клиент

```bash
cd client
qmake client.pro
make -j$(nproc)
./client
```

### Сервер через Docker

> **Важно:** перед сборкой образа `server/email.txt` должен быть заполнен (email и ключ).  
> Файл не попадёт в git — он игнорируется `.gitignore`, но включается в Docker-образ через `COPY`.

```bash
# Сборка образа (запускать из корня репозитория)
docker build -t timp-group5 -f docker/Dockerfile .

# Запуск контейнера
docker run -d -p 33333:33333 --name timp-server timp-group5

# Логи сервера
docker logs -f timp-server

# Остановка
docker stop timp-server

# Полная очистка (контейнер + образ)
docker rm timp-server
docker rmi timp-group5
```

После запуска сервер слушает порт **33333**. Клиент по умолчанию подключается к `127.0.0.1:33333`.

### Документация (Doxygen)

```bash
sudo pacman -S doxygen graphviz
cd docs
doxygen Doxyfile
# Открыть в браузере
xdg-open output/html/index.html
```

### Тесты

```bash
cd tests
g++ -o test_calculator test_calculator.cpp -std=c++17
./test_calculator
```

---

## Порты

| Сервис | Порт |
|--------|------|
| TCP-сервер | 33333 |
