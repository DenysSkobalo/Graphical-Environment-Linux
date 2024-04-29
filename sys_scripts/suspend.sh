#!/bin/bash

# Перевірка наявності прав адміністратора
if [ $(id -u) != "0" ]; then
    echo "Administrator privileges are required to run this script"
    exit 1
fi

# Виклик режиму сну
systemctl suspend

