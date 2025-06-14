# CAuth

Сервер авторизации на C++17 с поддержкой ролей пользователей (Admin, User, Guest), хранением в SQLite3 и шифрованием паролей через SHA-256 (OpenSSL).

## Возможности

- Регистрация пользователей с ролью
- Аутентификация по логину/паролю
- Проверка доступа к ресурсу (в зависимости от роли)
- TCP-сервер на Boost.Asio
- SQLite для хранения данных
- Логирование действий в `log.txt`
- Защита от SQL-инъекций (prepared statements)