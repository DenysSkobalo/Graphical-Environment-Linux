#!/bin/bash

# Визначає, чи підключений зовнішній монітор
external_monitor=$(xrandr | grep 'HDMI1 connected')

if [ -n "$external_monitor" ]; then
    # Якщо зовнішній монітор підключений, встановіть його як основний
    xrandr --output HDMI1 --primary --auto --output eDP1 --off
else
    # Якщо зовнішній монітор не підключений, встановіть монітор ноутбука як основний
    xrandr --output eDP1 --primary --auto --output HDMI1 --off
fi

