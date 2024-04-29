#!/bin/bash

# Перевірка чи є користувач з правами адміністратора
if [ "$(id -u)" != "0" ]; then
   echo "This script requires root privileges to run" 1>&2
   exit 1
fi

# Перевірка чи підтримується "hibernate"
if ! grep -q "hibernate" /sys/power/state; then
    echo "System does not support hibernate mode" 1>&2
    exit 1
fi

# Виконання команди для переходу в режим "hibernate"
echo "Entering hibernate mode..."
systemctl hibernate

