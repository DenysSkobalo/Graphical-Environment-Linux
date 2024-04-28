/* See LICENSE file for copyright and license details. */

/* interval between updates (in ms) */
const unsigned int interval = 1000;

/* text to show if no value can be retrieved */
static const char unknown_str[] = "n/a";

/* maximum output string length */
#define MAXLEN 2048

/*
 * function            description                     argument (example)
 *
 * battery_perc        battery percentage              battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * battery_state       battery charging state          battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * battery_remaining   battery remaining HH:MM         battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * cpu_perc            cpu usage in percent            NULL
 * cpu_freq            cpu frequency in MHz            NULL
 * datetime            date and time                   format string (%F %T)
 * disk_free           free disk space in GB           mountpoint path (/)
 * disk_perc           disk usage in percent           mountpoint path (/)
 * disk_total          total disk space in GB          mountpoint path (/")
 * disk_used           used disk space in GB           mountpoint path (/)
 * entropy             available entropy               NULL
 * gid                 GID of current user             NULL
 * hostname            hostname                        NULL
 * ipv4                IPv4 address                    interface name (eth0)
 * ipv6                IPv6 address                    interface name (eth0)
 * kernel_release      `uname -r`                      NULL
 * keyboard_indicators caps/num lock indicators        format string (c?n?)
 *                                                     see keyboard_indicators.c
 * keymap              layout (variant) of current     NULL
 *                     keymap
 * load_avg            load average                    NULL
 * netspeed_rx         receive network speed           interface name (wlan0)
 * netspeed_tx         transfer network speed          interface name (wlan0)
 * num_files           number of files in a directory  path
 *                                                     (/home/foo/Inbox/cur)
 * ram_free            free memory in GB               NULL
 * ram_perc            memory usage in percent         NULL
 * ram_total           total memory size in GB         NULL
 * ram_used            used memory in GB               NULL
 * run_command         custom shell command            command (echo foo)
 * swap_free           free swap in GB                 NULL
 * swap_perc           swap usage in percent           NULL
 * swap_total          total swap size in GB           NULL
 * swap_used           used swap in GB                 NULL
 * temp                temperature in degree celsius   sensor file
 *                                                     (/sys/class/thermal/...)
 *                                                     NULL on OpenBSD
 *                                                     thermal zone on FreeBSD
 *                                                     (tz0, tz1, etc.)
 * uid                 UID of current user             NULL
 * uptime              system uptime                   NULL
 * username            username of current user        NULL
 * vol_perc            OSS/ALSA volume in percent      mixer file (/dev/mixer)
 * wifi_perc           WiFi signal in percent          interface name (wlan0)
 * wifi_essid          WiFi ESSID                      interface name (wlan0)
 */
/*static const struct arg args[] = {
//function format          argument
{ netspeed_rx, " ^c#585858^| ^c#B799FF^ %s%%^c#585858^󰇙","enp9s0" },
{ netspeed_tx, "^c#F29727^ %s%% B/s ^c#585858^|", "enp9s0"  },
{ cpu_perc, " ^c#FF78C4^󰧑 Cpu %s%% ^c#585858^|",NULL  },
{ ram_perc, " ^c#F24C3D^⚡ Ram %s%% ^c#585858^|",NULL },
{ disk_free," ^c#FF0077^ Hdd %sB ^c#585858^|", "/"  },
//{ run_command," ^c#1da4d1^  %s%% | %32d ","pactl get-sink-volume @DEFAULT_SINK@ | grep -Po '\\d+(?=%)' | head -n 1" },
{ datetime, " ^c#20bda5^󰙹 %s","%d/%m/%y %b ^c#585858^| ^c#35b072^ %I:%M %p ^c#35b072^|"},
//{ run_command," ^c#1da4d1^   %s |", "amixer -D pulse sget Master | grep 'Right:' | awk -F'[][]' '{ print $2 }'" },
//{ datetime, " ^#20bda5^ 📆 %s "," %d/%m/%y %b ^c#585858^ | ^c#35b072^ %I:%M %p ^c#35b072^ |"},
//{ cpu_perc, "[CPU %s%%]  ",   NULL  },
//{ ram_perc, "[RAM %s%%]  ", NULL },
//{ disk_free,"[hdd %sB]  | ",      "/" },
//{ datetime, "%s","%a %b %d %R" }
}; */

const char *
volume_perc(void) {
    FILE *fp;
    int volume, mute;

    // Отримуємо рівень гучності за допомогою amixer
    if (!(fp = popen("amixer sget Master | grep 'Right:' | awk -F'[][]' '{ print $2 }' | sed 's/%//'", "r"))) {
        return NULL;
    }
    if (fscanf(fp, "%d", &volume) != 1) {
        return NULL;
    }
    pclose(fp);

    // Отримуємо стан вимкнення звуку за допомогою amixer
    if (!(fp = popen("amixer sget Master | grep 'Right:' | awk -F'[][]' '{ print $4 }'", "r"))) {
        return NULL;
    }
    char mute_status[8];
    if (fgets(mute_status, sizeof(mute_status), fp) == NULL) {
        return NULL;
    }
    pclose(fp);

    // Перевіряємо, чи звук вимкнений
    mute = (strcmp(mute_status, "off\n") == 0);

    // Повертаємо відсоткове значення гучності або 0% при вимкненні звуку
    if (mute) {
        return bprintf("0%%");
    } else {
        return bprintf("%d%%", volume);
    }
}



const char *
brightness_perc(void) {
    FILE *fp;
    int brightness, max_brightness;
    double brightness_percent;

    if (!(fp = popen("brightnessctl g", "r"))) {
        return NULL;
    }
    if (fscanf(fp, "%d", &brightness) != 1) {
        return NULL;
    }
    pclose(fp);

    if (!(fp = popen("brightnessctl max", "r"))) {
        return NULL;
    }
    if (fscanf(fp, "%d", &max_brightness) != 1) {
        return NULL;
    }
    pclose(fp);

    // Нормалізуємо значення яскравості до відсотків
    brightness_percent = ((double)brightness / max_brightness) * 100;

    return bprintf("%d%%", (int)brightness_percent);
}

const char *
bluetooth_status(void) {
    FILE *fp;
    char status[32];

    // Отримуємо інформацію про стан Bluetooth
    if (!(fp = popen("systemctl is-active bluetooth", "r"))) {
        return NULL;
    }

    // Читаємо стан Bluetooth
    if (fgets(status, sizeof(status), fp) == NULL) {
        return NULL;
    }
    pclose(fp);

    // Перевіряємо, чи Bluetooth увімкнено
    if (strstr(status, "active") != NULL) {
        return bprintf("BT: On");
    } else {
        return bprintf("BT: Off");
    }
}

static const struct arg args[] = {
	// { netspeed_rx, "| 📥 Down %s%%  |","wlo1" },
	// { netspeed_tx, " 📤 up %s%%  |", "wlo1"  },
	{ volume_perc, "| Volume: %s", NULL },
	{ brightness_perc, "| Brightness: %s", NULL },
	{ cpu_perc, "| CPU: %s%%",   NULL  },
	{ ram_perc, "| RAM: %s%%", NULL    },
	// { disk_free,"[hdd %sB]| ",      "/" },
	{ wifi_essid, "| Wifi: %s", "wlp0s20f3" },
	{ bluetooth_status, "| %s",  NULL },
	{ kernel_release, "| V: %s", NULL },
	{ battery_perc, "| BAT: %s%% |", "BAT0"   },
	{ datetime, " %s |","%a %b %d %R" }
};



