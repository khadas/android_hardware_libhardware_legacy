#define LOG_NDEBUG 0
#define LOG_TAG "MULTI_WIFI"

#ifndef SYSFS_PATH_MAX
#define SYSFS_PATH_MAX 256
#endif

#include<unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <asm/ioctl.h>

#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/socket.h>
#include <poll.h>
#include <sys/inotify.h>
#include <sys/select.h>
#include <sys/types.h>

#include "hardware_legacy/wifi.h"
#include "libwpa_client/wpa_ctrl.h"

#include "cutils/log.h"
#include "cutils/memory.h"
#include "cutils/properties.h"
#include "cutils/misc.h"
#include "netutils/ifc.h"
#include "private/android_filesystem_config.h"
#ifdef HAVE_LIBC_SYSTEM_PROPERTIES
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#endif


#include "../../../external/libusb/libusb-0.1.12/usb.h"

static const char SYSFS_CLASS_NET[]     = "/sys/class/net";
static const char SYS_MOD_NAME_DIR[]    = "device/driver/module";
static const char DRIVER_PROP_NAME[]    = "wlan.driver.status";



static const char BS8723_MODULE_NAME[]  = "8723bs";
static const char BS8723_MODULE_TAG[]   = "8723bs";
static const char BS8723_MODULE_PATH[]  = "/system/lib/8723bs.ko";
static const char BS8723_MODULE_ARG[]   = "ifname=wlan0 if2name=p2p0";

static const char BS8189_MODULE_NAME[]  = "8189es";
static const char BS8189_MODULE_TAG[]   = "8189es";
static const char BS8189_MODULE_PATH[]  = "/system/lib/8189es.ko";
static const char BS8189_MODULE_ARG[]   = "ifname=wlan0 if2name=p2p0";

static const char CFG80211_MODULE_TAG[]   = "cfg80211";
static const char QCN80211_MODULE_PATH[]  = "/system/lib/qcn80211.ko";
static const char CFG80211_MODULE_PATH[]  = "/system/lib/cfg80211.ko";
static const char CFG80211_MODULE_ARG[]   = "";
static const char COMPAT_MODULE_PATH[]  = "/system/lib/compat.ko";
static const char COMPAT_MODULE_ARG[]   = "";

static const char BS8822_MODULE_NAME[]  = "8822bs";
static const char BS8822_MODULE_TAG[]   = "8822bs";
static const char BS8822_MODULE_PATH[]  = "/system/lib/8822bs.ko";
static const char BS8822_MODULE_ARG[]   = "ifname=wlan0 if2name=p2p0";
#define USB_POWER_UP    _IO('m',1)
#define USB_POWER_DOWN  _IO('m',2)
#define SDIO_POWER_UP    _IO('m',3)
#define SDIO_POWER_DOWN  _IO('m',4)

static const char FS8189_MODULE_NAME[]  = "8189fs";
static const char FS8189_MODULE_TAG[]   = "8189fs";
static const char FS8189_MODULE_PATH[]  = "/system/lib/8189fs.ko";
static const char FS8189_MODULE_ARG[]   = "ifname=wlan0 if2name=p2p0";

static const char BCM40183_MODULE_NAME[]  = "dhd";
static const char BCM40183_MODULE_TAG[]   = "dhd";
static const char BCM40183_MODULE_PATH[]  = "/system/lib/dhd.ko";
static const char BCM40183_MODULE_ARG[]   ="firmware_path=/etc/wifi/AP6330/Wi-Fi/fw_bcm40183b2.bin nvram_path=/etc/wifi/AP6330/Wi-Fi/nvram_ap6330.txt";
static const char BCM6335_MODULE_ARG[]   ="firmware_path=/etc/wifi/6335/fw_bcm4339a0_ag.bin nvram_path=/etc/wifi/6335/nvram.txt";
static const char BCM6210_MODULE_ARG[]   ="firmware_path=/etc/wifi/40181/fw_bcm40181a2.bin nvram_path=/etc/wifi/40181/nvram.txt";
static const char BCM6234_MODULE_ARG[]   ="firmware_path=/etc/wifi/6234/fw_bcm43341b0_ag.bin nvram_path=/etc/wifi/6234/nvram.txt";
static const char BCM6354_MODULE_ARG[]   ="firmware_path=/etc/wifi/4354/fw_bcm4354a1_ag.bin nvram_path=/etc/wifi/4354/nvram_ap6354.txt";
static const char BCM62x2_MODULE_ARG[]   ="firmware_path=/etc/wifi/62x2/fw_bcm43241b4_ag.bin nvram_path=/etc/wifi/62x2/nvram.txt";
static const char BCM6255_MODULE_ARG[]   ="firmware_path=/etc/wifi/6255/fw_bcm43455c0_ag.bin nvram_path=/etc/wifi/6255/nvram.txt";
static const char BCM6212_MODULE_ARG[]   ="firmware_path=/etc/wifi/6212/fw_bcm43438a0.bin nvram_path=/etc/wifi/6212/nvram.txt";
static const char BCM6356_MODULE_ARG[]   ="firmware_path=/etc/wifi/4356/fw_bcm4356a2_ag.bin nvram_path=/etc/wifi/4356/nvram_ap6356.txt";

extern int init_module(void *, unsigned long, const char *);
extern int delete_module(const char *, unsigned int);
struct wifi_vid_pid {
    unsigned short int vid;
    unsigned short int pid;
};

struct usb_detail_table {
    unsigned  short int usb_port;
    unsigned  short int vid;
    unsigned  short int pid;
};

int verbose1 = 0;
int usb_sdio_wifi = 0;
extern struct usb_bus *usb_busses;

extern int cu8192_load_driver();
extern int search_cu(unsigned  int vid,unsigned  int pid);
extern int cu8192_unload_driver();

extern int du8192_load_driver();
extern int search_du(unsigned  int vid,unsigned  int pid);
extern int du8192_unload_driver();

extern int eu8188_load_driver();
extern int search_8188eu(unsigned  int vid,unsigned  int pid);
extern int eu8188_unload_driver();

extern int au8821_load_driver();
extern int search_8821au(unsigned  int vid,unsigned  int pid);
extern int au8821_unload_driver();
extern int bu8723_load_driver();
extern int search_8723bu(unsigned  int vid,unsigned  int pid);
extern int bu8723_unload_driver();
extern int ftv8188_load_driver();
extern int search_8188ftv(unsigned  int vid,unsigned  int pid);
extern int ftv8188_unload_driver();

extern int eu8192_load_driver();
extern int search_8192eu(unsigned  int vid,unsigned  int pid);
extern int eu8192_unload_driver();

extern int es8192_load_driver();
extern int search_es8192(int x, int y);
extern int es8192_unload_driver();

extern int qc9377_load_driver();
extern int search_qc9377(int x, int y);
extern int qc9377_unload_driver();

extern int qc6174_load_driver();
extern int search_qc6174(int x, int y);
extern int qc6174_unload_driver();

extern int mt7601_load_driver();
extern int search_mt7601(unsigned  int vid,unsigned  int pid);
extern int mt7601_unload_driver();

extern int mt7603_load_driver();
extern int search_mt7603(unsigned  int vid,unsigned  int pid);
extern int mt7603_unload_driver();
char dongle_id[] = "/data/misc/wifi/wid_fp";

int write_no(int *x)
{
    int fd,len;
    fd = open(dongle_id,O_CREAT|O_RDWR, S_IRWXU);
    if (fd == -1) {
        ALOGE("Open file failed !!!\n");
        return -1;
    }
    len = write(fd,x,1);
    if (len == -1) {
        ALOGE("Write file failed !!!\n");
        close(fd);
        return -1;
    }
    close(fd);
    if (chmod(dongle_id, 0664) < 0 || chown(dongle_id, AID_SYSTEM, AID_WIFI) < 0) {
       ALOGE("Error changing permissions of %s to 0664: %s",
             dongle_id, strerror(errno));
       return -1;
    }
    return 0;
}

int read_no(int *x)
{
    int fd,len;
    fd = open(dongle_id,O_RDONLY, S_IRWXU);
    if (fd == -1) {
        ALOGE("Open file failed !!!\n");
        return -1;
    }
    len = read(fd,x,1);
    if (len == -1) {
        ALOGE("Read file failed !!!\n");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

static int wifi_insmod(const char *filename, const char *args)
{
    void *module;
    unsigned int size;
    int ret;

    module = (void*)load_file(filename, &size);
    if (!module)
        return -1;

    ret = init_module(module, size, args);

    free(module);

    return ret;
}

static int wifi_rmmod(const char *modname)
{
    int ret = -1;
    int maxtry = 10;

    while (maxtry-- > 0) {
        ret = delete_module(modname, O_NONBLOCK | O_EXCL);
        if (ret < 0 && errno == EAGAIN)
            usleep(500000);
        else
            break;
    }

    if (ret != 0)
        ALOGD("Unable to unload driver module \"%s\": %s\n",
             modname, strerror(errno));
    return ret;
}

int bcm6335_load_driver()
{
    if (wifi_insmod(BCM40183_MODULE_PATH, BCM6335_MODULE_ARG) !=0) {
        ALOGE("Failed to insmod dhd ! \n");
        return -1;
    }

    ALOGD("Success to insmod bcm6335 driver! \n");
    return 0;
}

int bcm6335_unload_driver()
{
    if (wifi_rmmod(BCM40183_MODULE_NAME) != 0) {
        ALOGE("Failed to rmmod bcm6335 driver ! \n");
        return -1;
    }
    ALOGD("Success to rmmod bcm6335 driver ! \n");
    return 0;
}

int search_bcm6335(unsigned int x,unsigned int y)
{
    int fd,len;
    char sdio_buf[128];
    char file_name[] = "/sys/bus/mmc/devices/sdio:0001/sdio:0001:1/device";
    FILE *fp = fopen(file_name,"r");
    if (!fp) {
        ALOGE("Open sdio wifi file failed !!! \n");
        return 2;
    }
    memset(sdio_buf,0,sizeof(sdio_buf));
    if (fread(sdio_buf, 1,128,fp) < 1) {
        ALOGE("Read error for %m\n", errno);
        fclose(fp);
        return -1;
    }
    fclose(fp);
    if (strstr(sdio_buf,"4335")) {
        ALOGE("Found 6335 !!!\n");
        usb_sdio_wifi=0;
        return 1;
    }
    return 0;
}


int bcm40183_load_driver()
{
    if (wifi_insmod(BCM40183_MODULE_PATH, BCM40183_MODULE_ARG) !=0) {
    ALOGE("Failed to insmod dhd ! \n");
    return -1;
    }
    ALOGD("Success to insmod bcm40183 driver! \n");
    return 0;
}

int bcm40183_unload_driver()
{
    if (wifi_rmmod(BCM40183_MODULE_NAME) != 0) {
    ALOGE("Failed to rmmod bcm40183 driver ! \n");
    return -1;
    }
    ALOGD("Success to rmmod bcm40183 driver ! \n");
    return 0;
}

int search_bcm40183(unsigned int x,unsigned int y)
{
    int fd,len;
    char sdio_buf[128];
    char file_name[] = "/sys/bus/mmc/devices/sdio:0001/sdio:0001:1/device";
    FILE *fp = fopen(file_name,"r");
    if (!fp) {
        ALOGE("Open sdio wifi file failed 40183 !!! \n");
        return 2;
    }
    memset(sdio_buf,0,sizeof(sdio_buf));
    if (fread(sdio_buf, 1,128,fp) < 1) {
        ALOGE("Read error for %m\n", errno);
        fclose(fp);
        return -1;
    }
    ALOGE("sdio_buf:%s\n",sdio_buf);
    fclose(fp);
    if (strstr(sdio_buf,"4330")) {
        ALOGE("Found 40183 !!!\n");
        usb_sdio_wifi=0;
        return 1;
    }
    return 0;
}

int bcm6210_load_driver()
{
    if (wifi_insmod(BCM40183_MODULE_PATH, BCM6210_MODULE_ARG) !=0) {
        ALOGE("Failed to insmod dhd ! \n");
        return -1;
    }

    ALOGD("Success to insmod bcm6210 driver! \n");
    return 0;
}

int bcm6210_unload_driver()
{
    if (wifi_rmmod(BCM40183_MODULE_NAME) != 0) {
        ALOGE("Failed to rmmod bcm6210 driver ! \n");
        return -1;
    }
    ALOGD("Success to rmmod bcm6210 driver ! \n");
    return 0;
}

int search_bcm6210(unsigned int x,unsigned int y)
{
    int fd,len;
    char sdio_buf[128];
    char file_name[] = "/sys/bus/mmc/devices/sdio:0001/sdio:0001:1/device";
    FILE *fp = fopen(file_name,"r");
    if (!fp) {
        ALOGE("Open sdio wifi file failed !!! \n");
        return 2;
    }
    memset(sdio_buf,0,sizeof(sdio_buf));
    if (fread(sdio_buf, 1,128,fp) < 1) {
        ALOGE("Read error for %m\n", errno);
        fclose(fp);
        return -1;
    }
    fclose(fp);
    if (strstr(sdio_buf,"a962")) {
        ALOGE("Found 6210 !!!\n");
        usb_sdio_wifi=0;
        return 1;
    }
    return 0;
}


int bcm6234_load_driver()
{
    if (wifi_insmod(BCM40183_MODULE_PATH, BCM6234_MODULE_ARG) !=0) {
        ALOGE("Failed to insmod dhd ! \n");
        return -1;
    }
    ALOGD("Success to insmod bcm6234 driver! \n");
    return 0;
}

int bcm6234_unload_driver()
{
    if (wifi_rmmod(BCM40183_MODULE_NAME) != 0) {
        ALOGE("Failed to rmmod bcm6234 driver ! \n");
        return -1;
    }
    ALOGD("Success to rmmod bcm6234 driver ! \n");
    return 0;
}

int search_bcm6234(unsigned int x,unsigned int y)
{
    int fd,len;
    char sdio_buf[128];
    char file_name[] = "/sys/bus/mmc/devices/sdio:0001/sdio:0001:1/device";
    FILE *fp = fopen(file_name,"r");
    if (!fp) {
        ALOGE("Open sdio wifi file failed !!! \n");
        return 2;
    }
    memset(sdio_buf,0,sizeof(sdio_buf));
    if (fread(sdio_buf, 1,128,fp) < 1) {
        ALOGE("Read error for %m\n", errno);
        fclose(fp);
        return -1;
    }
    fclose(fp);
    if (strstr(sdio_buf,"a94d")) {
        ALOGE("Found 6234 !!!\n");
        usb_sdio_wifi=0;
        return 1;
    }
    return 0;
}

int bcm6354_load_driver()
{
    if (wifi_insmod(BCM40183_MODULE_PATH, BCM6354_MODULE_ARG) !=0) {
        ALOGE("Failed to insmod dhd ! \n");
        return -1;
    }
    ALOGD("Success to insmod bcm6354 driver! \n");
    return 0;
}

int bcm6354_unload_driver()
{
    if (wifi_rmmod(BCM40183_MODULE_NAME) != 0) {
        ALOGE("Failed to rmmod bcm6354 driver ! \n");
        return -1;
    }
    ALOGD("Success to rmmod bcm6354 driver ! \n");
    return 0;
}

int search_bcm6354(unsigned int x,unsigned int y)
{
    int fd,len;
    char sdio_buf[128];
    char file_name[] = "/sys/bus/mmc/devices/sdio:0001/sdio:0001:1/device";
    FILE *fp = fopen(file_name,"r");
    if (!fp) {
        ALOGE("Open sdio wifi file failed !!! \n");
        return 2;
    }
    memset(sdio_buf,0,sizeof(sdio_buf));
    if (fread(sdio_buf, 1,128,fp) < 1) {
        ALOGE("Read error for %m\n", errno);
        fclose(fp);
        return -1;
    }
    fclose(fp);
    if (strstr(sdio_buf,"4354")) {
        ALOGE("Found 6354 !!!\n");
        usb_sdio_wifi=0;
        return 1;
    }
    return 0;
}

int bcm6356_load_driver()
{
    if (wifi_insmod(BCM40183_MODULE_PATH, BCM6356_MODULE_ARG) !=0) {
        ALOGE("Failed to insmod dhd ! \n");
        return -1;
    }
    ALOGD("Success to insmod bcm6356 driver! \n");
    return 0;
}

int bcm6356_unload_driver()
{
    if (wifi_rmmod(BCM40183_MODULE_NAME) != 0) {
        ALOGE("Failed to rmmod bcm6356 driver ! \n");
        return -1;
    }
    ALOGD("Success to rmmod bcm6356 driver ! \n");
    return 0;
}

int search_bcm6356(unsigned int x,unsigned int y)
{
    int fd,len;
    char sdio_buf[128];
    char file_name[] = "/sys/bus/mmc/devices/sdio:0001/sdio:0001:1/device";
    FILE *fp = fopen(file_name,"r");
    if (!fp) {
        ALOGE("Open sdio wifi file failed !!! \n");
        return 2;
    }
    memset(sdio_buf,0,sizeof(sdio_buf));
    if (fread(sdio_buf, 1,128,fp) < 1) {
        ALOGE("Read error for %m\n", errno);
        fclose(fp);
        return -1;
    }
    fclose(fp);
    if (strstr(sdio_buf,"4356")) {
        ALOGE("Found 6356 !!!\n");
        usb_sdio_wifi=0;
        return 1;
    }
    return 0;
}


int bcm62x2_load_driver()
{
    if (wifi_insmod(BCM40183_MODULE_PATH, BCM62x2_MODULE_ARG) !=0) {
        ALOGE("Failed to insmod dhd ! \n");
        return -1;
    }
    ALOGD("Success to insmod bcm62x2 driver! \n");
    return 0;
}

int bcm62x2_unload_driver()
{
    if (wifi_rmmod(BCM40183_MODULE_NAME) != 0) {
        ALOGE("Failed to rmmod bcm62x2 driver ! \n");
        return -1;
    }
    ALOGD("Success to rmmod bcm62x2 driver ! \n");
    return 0;
}

int search_bcm62x2(unsigned int x,unsigned int y)
{
    int fd,len;
    char sdio_buf[128];
    char file_name[] = "/sys/bus/mmc/devices/sdio:0001/sdio:0001:1/device";
    FILE *fp = fopen(file_name,"r");
    if (!fp) {
        ALOGE("Open sdio wifi file failed !!! \n");
        return 2;
    }

    memset(sdio_buf,0,sizeof(sdio_buf));
    if (fread(sdio_buf, 1,128,fp) < 1) {
        ALOGE("Read error for %m\n", errno);
        fclose(fp);
        return -1;
    }
    fclose(fp);
    if (strstr(sdio_buf,"4324")) {
        ALOGE("Found 62x2 !!!\n");
        usb_sdio_wifi=0;
        return 1;
    }
    return 0;
}


int bcm6255_load_driver()
{
    if (wifi_insmod(BCM40183_MODULE_PATH, BCM6255_MODULE_ARG) !=0) {
        ALOGE("Failed to insmod dhd ! \n");
        return -1;
    }
    ALOGD("Success to insmod bcm6255 driver! \n");
    return 0;
}

int bcm6255_unload_driver()
{
    if (wifi_rmmod(BCM40183_MODULE_NAME) != 0) {
        ALOGE("Failed to rmmod bcm6255 driver ! \n");
        return -1;
    }
    ALOGD("Success to rmmod bcm6255 driver ! \n");
    return 0;
}

int search_bcm6255(unsigned int x,unsigned int y)
{
    int fd,len;
    char sdio_buf[128];
    char file_name[] = "/sys/bus/mmc/devices/sdio:0001/sdio:0001:1/device";
    FILE *fp = fopen(file_name,"r");
    if (!fp) {
        ALOGE("Open sdio wifi file failed !!! \n");
        return 2;
    }

    memset(sdio_buf,0,sizeof(sdio_buf));
    if (fread(sdio_buf, 1,128,fp) < 1) {
        ALOGE("Read error for %m\n", errno);
        fclose(fp);
        return -1;
    }
    fclose(fp);
    if (strstr(sdio_buf,"a9bf")) {
        ALOGE("Found 6255 !!!\n");
        usb_sdio_wifi=0;
        return 1;
    }
    return 0;
}


int bcm6212_load_driver()
{
    if (wifi_insmod(BCM40183_MODULE_PATH, BCM6212_MODULE_ARG) !=0) {
        ALOGE("Failed to insmod dhd ! \n");
        return -1;
    }
    ALOGD("Success to insmod bcm6212 driver! \n");
    return 0;
}

int bcm6212_unload_driver()
{
    if (wifi_rmmod(BCM40183_MODULE_NAME) != 0) {
        ALOGE("Failed to rmmod bcm6212 driver ! \n");
        return -1;
    }
    ALOGD("Success to rmmod bcm6212 driver ! \n");
    return 0;
}

int search_bcm6212(unsigned int x,unsigned int y)
{
    int fd,len;
    char sdio_buf[128];
    char file_name[] = "/sys/bus/mmc/devices/sdio:0001/sdio:0001:1/device";
    FILE *fp = fopen(file_name,"r");
    if (!fp) {
        ALOGE("Open sdio wifi file failed !!! \n");
        return 2;
    }
    memset(sdio_buf,0,sizeof(sdio_buf));
    if (fread(sdio_buf, 1,128,fp) < 1) {
        ALOGE("Read error for %m\n", errno);
        fclose(fp);
        return -1;
    }
    fclose(fp);
    if (strstr(sdio_buf,"a9a6")) {
        ALOGE("Found 6212 !!!\n");
        usb_sdio_wifi=0;
        return 1;
    }
    return 0;
}


int bs8723_load_driver()
{
    if (wifi_insmod(BS8723_MODULE_PATH, BS8723_MODULE_ARG) !=0) {
        ALOGE("Failed to insmod 8723 ! \n");
        return -1;
    }
    ALOGD("Success to insmod bs8723 driver! \n");
    return 0;
}

int bs8723_unload_driver()
{
    if (wifi_rmmod(BS8723_MODULE_NAME) != 0) {
        ALOGE("Failed to rmmod bs8723 driver ! \n");
        return -1;
    }
    ALOGD("Success to rmmod bs8723 driver ! \n");
    return 0;
}

int search_bs8723(unsigned int x,unsigned int y)
{
    int fd,len;
    char sdio_buf[128];
    char file_name[] = "/sys/bus/mmc/devices/sdio:0001/sdio:0001:1/device";
    FILE *fp = fopen(file_name,"r");
    if (!fp) {
        ALOGE("Open sdio wifi file failed !!! \n");
        return 2;
    }
    memset(sdio_buf,0,sizeof(sdio_buf));
    if (fread(sdio_buf, 1,128,fp) < 1) {
        ALOGE("Read error for %m\n", errno);
        fclose(fp);
        return -1;
    }
    fclose(fp);
    if (strstr(sdio_buf,"b723")) {
        ALOGE("Found 8723bs !!!\n");
        usb_sdio_wifi=1;
        return 1;
    }
    return 0;
}


int es8189_load_driver()
{
    if (wifi_insmod(BS8189_MODULE_PATH, BS8189_MODULE_ARG) !=0) {
        ALOGE("Failed to insmod 8189 ! \n");
        return -1;
    }
    ALOGD("Success to insmod bs8189 driver! \n");
    return 0;
}

int es8189_unload_driver()
{
    if (wifi_rmmod(BS8189_MODULE_NAME) != 0) {
        ALOGE("Failed to rmmod bs8189 driver ! \n");
        return -1;
    }
    ALOGD("Success to rmmod bs8189 driver ! \n");
    return 0;
}

int search_es8189(unsigned int x,unsigned int y)
{
    int fd,len;
    char sdio_buf[128];
    char file_name[] = "/sys/bus/mmc/devices/sdio:0001/sdio:0001:1/device";
    FILE *fp = fopen(file_name,"r");
    if (!fp) {
        ALOGE("Open sdio wifi file failed !!! \n");
        return 2;
    }
    memset(sdio_buf,0,sizeof(sdio_buf));
    if (fread(sdio_buf, 1,128,fp) < 1) {
        ALOGE("Read error for %m\n", errno);
        fclose(fp);
        return -1;
    }
    fclose(fp);
    if (strstr(sdio_buf,"8179")) {
        ALOGE("Found 8189bs !!!\n");
        usb_sdio_wifi=1;
        return 1;
    }
    return 0;
}



int bs8822_load_driver()
{
    if (wifi_insmod(BS8822_MODULE_PATH, BS8822_MODULE_ARG) !=0) {
        ALOGE("Failed to insmod 8822bs ! \n");
        return -1;
    }
    ALOGD("Success to insmod bs8822 driver! \n");
    return 0;
}

int bs8822_unload_driver()
{
    if (wifi_rmmod(BS8822_MODULE_NAME) != 0) {
        ALOGE("Failed to rmmod bs8822 driver ! \n");
        return -1;
    }
    ALOGD("Success to rmmod bs8822 driver ! \n");
    return 0;
}

int search_bs8822(unsigned int x,unsigned int y)
{
    int fd,len;
    char sdio_buf[128];
    char file_name[] = "/sys/bus/mmc/devices/sdio:0001/sdio:0001:1/device";
    FILE *fp = fopen(file_name,"r");
    if (!fp) {
        ALOGE("Open sdio wifi file failed !!! \n");
        return 2;
    }
    memset(sdio_buf,0,sizeof(sdio_buf));
    if (fread(sdio_buf, 1,128,fp) < 1) {
        ALOGE("Read error for %m\n", errno);
        fclose(fp);
        return -1;
    }
    fclose(fp);
    if (strstr(sdio_buf,"b822")) {
        ALOGE("Found 8822bs !!!\n");
        usb_sdio_wifi=1;
        return 1;
    }
    return 0;
}

int fs8189_load_driver()
{
    if (wifi_insmod(FS8189_MODULE_PATH, FS8189_MODULE_ARG) !=0) {
        ALOGE("Failed to insmod 8189 ! \n");
        return -1;
    }
    ALOGD("Success to insmod bs8189 driver! \n");
    return 0;
}

int fs8189_unload_driver()
{
    if (wifi_rmmod(FS8189_MODULE_NAME) != 0) {
        ALOGE("Failed to rmmod bs8189 driver ! \n");
        return -1;
    }
    ALOGD("Success to rmmod bs8189 driver ! \n");
    return 0;
}

int search_fs8189(unsigned int x,unsigned int y)
{
    int fd,len;
    char sdio_buf[128];
    char file_name[] = "/sys/bus/mmc/devices/sdio:0001/sdio:0001:1/device";
    FILE *fp = fopen(file_name,"r");
    if (!fp) {
        ALOGE("Open sdio wifi file failed !!! \n");
        return 2;
    }
    memset(sdio_buf,0,sizeof(sdio_buf));
    if (fread(sdio_buf, 1,128,fp) < 1) {
        ALOGE("Read error for %m\n", errno);
        fclose(fp);
        return -1;
    }
    fclose(fp);
    if (strstr(sdio_buf,"f179")) {
        ALOGE("Found 8189fs !!!\n");
        usb_sdio_wifi=1;
        return 1;
    }
    return 0;
}

static int cur_vid = 0;
static int cur_pid = 0;
typedef struct load_info{
	int (*load)();
	int (*unload)();
	int (*search)(unsigned int,unsigned int);
}dongle_info;

#define SDIO_WIFI_COUNT 2
static const dongle_info dongle_registerd[]={\
	{bcm40183_load_driver,bcm40183_unload_driver,search_bcm40183},\
	{bcm6210_load_driver,bcm6210_unload_driver,search_bcm6210},\
	{bcm6335_load_driver,bcm6335_unload_driver,search_bcm6335},\
	{bcm6234_load_driver,bcm6234_unload_driver,search_bcm6234},\
	{bcm6354_load_driver,bcm6354_unload_driver,search_bcm6354},\
	{bcm62x2_load_driver,bcm62x2_unload_driver,search_bcm62x2},\
	{bcm6255_load_driver,bcm6255_unload_driver,search_bcm6255},\
	{bcm6212_load_driver,bcm6212_unload_driver,search_bcm6212},\
	{bcm6356_load_driver,bcm6356_unload_driver,search_bcm6356},\
	{qc9377_load_driver,qc9377_unload_driver,search_qc9377},\
	{qc6174_load_driver,qc6174_unload_driver,search_qc6174},\
	{bs8723_load_driver,bs8723_unload_driver,search_bs8723},\
	{es8189_load_driver,es8189_unload_driver,search_es8189},\
	{bs8822_load_driver,bs8822_unload_driver,search_bs8822},\
	{fs8189_load_driver,fs8189_unload_driver,search_fs8189},\
	{es8192_load_driver,es8192_unload_driver,search_es8192},\
	{eu8188_load_driver,eu8188_unload_driver,search_8188eu},\
	{bu8723_load_driver,bu8723_unload_driver,search_8723bu},\
	{au8821_load_driver,au8821_unload_driver,search_8821au},\
	{ftv8188_load_driver,ftv8188_unload_driver,search_8188ftv},\
	{cu8192_load_driver,cu8192_unload_driver,search_cu},\
	{du8192_load_driver,du8192_unload_driver,search_du},\
	{eu8192_load_driver,eu8192_unload_driver,search_8192eu},\
	{mt7601_load_driver,mt7601_unload_driver,search_mt7601},\
	{mt7603_load_driver,mt7603_unload_driver,search_mt7603}};

int DONT_SUPPORT_P2P = 5;
static int load_dongle_index = -1;

static int indent_usb_table = 0;
static struct usb_detail_table usb_table[10] = {};

static void print_endpoint(struct usb_endpoint_descriptor *endpoint)
{
  ALOGD("      bEndpointAddress: %02xh\n", endpoint->bEndpointAddress);
  ALOGD("      bmAttributes:     %02xh\n", endpoint->bmAttributes);
  ALOGD("      wMaxPacketSize:   %d\n", endpoint->wMaxPacketSize);
  ALOGD("      bInterval:        %d\n", endpoint->bInterval);
  ALOGD("      bRefresh:         %d\n", endpoint->bRefresh);
  ALOGD("      bSynchAddress:    %d\n", endpoint->bSynchAddress);
}

static void print_altsetting(struct usb_interface_descriptor *interface)
{
  int i;

  ALOGD("    bInterfaceNumber:   %d\n", interface->bInterfaceNumber);
  ALOGD("    bAlternateSetting:  %d\n", interface->bAlternateSetting);
  ALOGD("    bNumEndpoints:      %d\n", interface->bNumEndpoints);
  ALOGD("    bInterfaceClass:    %d\n", interface->bInterfaceClass);
  ALOGD("    bInterfaceSubClass: %d\n", interface->bInterfaceSubClass);
  ALOGD("    bInterfaceProtocol: %d\n", interface->bInterfaceProtocol);
  ALOGD("    iInterface:         %d\n", interface->iInterface);

  for (i = 0; i < interface->bNumEndpoints; i++)
    print_endpoint(&interface->endpoint[i]);
}

static void print_interface(struct usb_interface *interface)
{
  int i;

  for (i = 0; i < interface->num_altsetting; i++)
    print_altsetting(&interface->altsetting[i]);
}

static void print_configuration(struct usb_config_descriptor *config)
{
  int i;

  printf("  wTotalLength:         %d\n", config->wTotalLength);
  printf("  bNumInterfaces:       %d\n", config->bNumInterfaces);
  printf("  bConfigurationValue:  %d\n", config->bConfigurationValue);
  printf("  iConfiguration:       %d\n", config->iConfiguration);
  printf("  bmAttributes:         %02xh\n", config->bmAttributes);
  printf("  MaxPower:             %d\n", config->MaxPower);

  for (i = 0; i < config->bNumInterfaces; i++)
    print_interface(&config->interface[i]);
}

static int print_device(struct usb_device *dev, int level)
{
  usb_dev_handle *udev;
  char description[256];
  char string[256];
  int ret, i;

  if (dev == NULL)
	return 0;
  udev = usb_open(dev);
  if (udev) {
    if (dev->descriptor.iManufacturer) {
      ret = usb_get_string_simple(udev, dev->descriptor.iManufacturer, string, sizeof(string));
      if (ret > 0)
        snprintf(description, sizeof(description), "%s - ", string);
      else
        snprintf(description, sizeof(description), "%04X - ",
                 dev->descriptor.idVendor);
    } else
      snprintf(description, sizeof(description), "%04X - ",
               dev->descriptor.idVendor);
		usb_table[indent_usb_table].vid = dev->descriptor.idVendor;

    if (dev->descriptor.iProduct) {
      ret = usb_get_string_simple(udev, dev->descriptor.iProduct, string, sizeof(string));
      if (ret > 0)
        snprintf(description + strlen(description), sizeof(description) -
                 strlen(description), "%s", string);
      else
        snprintf(description + strlen(description), sizeof(description) -
                 strlen(description), "%04X", dev->descriptor.idProduct);
    } else
      snprintf(description + strlen(description), sizeof(description) -
               strlen(description), "%04X", dev->descriptor.idProduct);
		usb_table[indent_usb_table].pid = dev->descriptor.idProduct;
    indent_usb_table ++;
  } else{
        ALOGE("Open failed! \n");
        snprintf(description, sizeof(description), "%04X - %04X",
             dev->descriptor.idVendor, dev->descriptor.idProduct);
    ALOGE("The VID:PID is 0x%04X : 0x%04X\n",dev->descriptor.idVendor,dev->descriptor.idProduct);
}


  printf("%.*sDev #%d: %s\n", level * 2, "                    ", dev->devnum,
         description);

  if (udev && verbose1) {
    if (dev->descriptor.iSerialNumber) {
      ret = usb_get_string_simple(udev, dev->descriptor.iSerialNumber, string, sizeof(string));
      if (ret > 0)
        printf("%.*s  - Serial Number: %s\n", level * 2,
               "                    ", string);
    }
  }

  if (udev)
    usb_close(udev);

  if (verbose1) {
    if (!dev->config) {
      printf("  Couldn't retrieve descriptors\n");
      return 0;
    }

    for (i = 0; i < dev->descriptor.bNumConfigurations; i++)
      print_configuration(&dev->config[i]);
  } else {
    for (i = 0; i < dev->num_children; i++)
      print_device(dev->children[i], level + 1);
  }

  return 0;
}

static int get_driver_info()
{
    DIR  *netdir;
    struct dirent *de;
    char path[SYSFS_PATH_MAX];
    char link[SYSFS_PATH_MAX];
    int ret = 0;
    if ((netdir = opendir(SYSFS_CLASS_NET)) != NULL) {
        while ((de = readdir(netdir)) != NULL) {
            struct dirent **namelist = NULL;
            int cnt;
            if ((!strcmp(de->d_name,".")) || (!strcmp(de->d_name,"..")))
                continue;
            snprintf(path, SYSFS_PATH_MAX, "%s/%s/wireless", SYSFS_CLASS_NET, de->d_name);
            if (access(path, F_OK)) {
                snprintf(path, SYSFS_PATH_MAX, "%s/%s/phy80211", SYSFS_CLASS_NET, de->d_name);
                if (access(path, F_OK))
                    continue;
            } else
                ret = 1;

        }
    }
    closedir(netdir);
    return ret;
}

int is_driver_loaded()
{
    if (!get_driver_info()) {
        property_set(DRIVER_PROP_NAME,"unloaded");
        return 0;
    } else {
        property_set(DRIVER_PROP_NAME,"ok");
        return 1;
    }
}


static int is_cfg80211_loaded() {
    FILE *proc;
    char line[sizeof(CFG80211_MODULE_TAG)+10];

    /*
     * If the property says the driver is loaded, check to
     * make sure that the property setting isn't just left
     * over from a previous manual shutdown or a runtime
     * crash.
     */
    if ((proc = fopen("/proc/modules", "r")) == NULL) {
        ALOGW("Could not open %s", strerror(errno));
        return 0;
    }
    while ((fgets(line, sizeof(line), proc)) != NULL) {
        if (strncmp(line, CFG80211_MODULE_TAG, strlen(CFG80211_MODULE_TAG)) == 0) {
            fclose(proc);
            return 1;
        }
    }
    fclose(proc);
    return 0;
}

int qcn_cfg80211_load_driver()
{
	if (wifi_insmod(COMPAT_MODULE_PATH, COMPAT_MODULE_ARG) !=0) {
        ALOGE("Failed to insmod qcn compat ko ! \n");
        return -1;
    }
	if (wifi_insmod(QCN80211_MODULE_PATH, CFG80211_MODULE_ARG) !=0) {
        ALOGE("Failed to insmod qcn cfg80211 ! \n");
        return -1;
    }
    ALOGD("Success to insmod qcn cfg80211 driver! \n");
    return 0;
}

int cfg80211_load_driver()
{
    if (wifi_insmod(CFG80211_MODULE_PATH, CFG80211_MODULE_ARG) !=0) {
        ALOGE("Failed to insmod  cfg80211 ! \n");
        return -1;
    }
    ALOGD("Success to insmod cfg80211 driver! \n");
    return 0;
}
int usb_wifi_load_driver()
{
    int i,j;
    int usb_vidpid_count=0;
    int count = 100;
    struct usb_bus *bus;
    usb_init();
    usb_find_busses();
    usb_find_devices();
    if (is_driver_loaded()) {
        ALOGD("Wi-Fi driver has loaded !");
        return 0;
    }
    for (bus = usb_busses; bus; bus = bus->next) {
        if (bus->root_dev && !verbose1) {
            print_device(bus->root_dev, 0);
        }else {
            struct usb_device *dev;
            for (dev = bus->devices; dev; dev = dev->next)
                print_device(dev, 0);
        }
    }
    usb_vidpid_count = indent_usb_table;
    indent_usb_table = 0;
    load_dongle_index = -1;
    for (j = 16;j < sizeof(dongle_registerd)/sizeof(dongle_info);j ++) {
        for (i = 0;i < usb_vidpid_count; i ++) {
            if (dongle_registerd[j].search(usb_table[i].vid,usb_table[i].pid) == 1) {
                usb_sdio_wifi=1;
                write_no(&j);
                if (!is_cfg80211_loaded())
                    cfg80211_load_driver();
                cur_vid = usb_table[i].vid;
                cur_pid = usb_table[i].pid;
                load_dongle_index = j;
                ALOGD("The matched dongle no. is %d\n",j);
                if (dongle_registerd[j].load() != 0) {
                    ALOGD("Load Wi-Fi driver error !!!\n");
                    return -1;
                }
                return 0;
            }
        }
    }

    if (load_dongle_index == -1) {
        ALOGD("Not find matched usb wifi dongle!!!\n");
        return -1;
    }
    return 0;
}

int sdio_wifi_load_driver()
{
    int i;
    int ret =0;
    load_dongle_index = -1;
    for (i=0;i < 16; i++) {
        ret=dongle_registerd[i].search(0,0);
        if (ret ==1) {
            load_dongle_index = i;
            write_no(&i);
            if (!is_cfg80211_loaded()) {
                if (strcmp(get_wifi_vendor_name(), "qcn") == 0)
                    qcn_cfg80211_load_driver();
                else
                    cfg80211_load_driver();
        }
            ALOGD("The matched dongle no. is %d\n", i);
            if (dongle_registerd[i].load() != 0) {
                ALOGD("Load Wi-Fi driver error !!!\n");
                return -1;
            }
        return 0;
        }else if(ret ==2){
            return -1;
            ALOGD("NO sdio device!!!\n");
        }else if(ret ==0){
            continue;
        }
    }
    if (load_dongle_index == -1) {
        ALOGD("Didn't find matched sdio wifi dongle!!!\n");
        return -1;
    }
    return 0;
}

const char *get_wifi_vendor_name()
{
    int dgle_no= 0;
    read_no(&dgle_no);
    if (dgle_no < 9) {
        return "bcm";
    }
    else if (8 <dgle_no && dgle_no < 11) {
        return "qcn";
    }
    else if (10 <dgle_no && dgle_no < 23) {
        return "realtek";
    }
    else if (dgle_no > 22) {
        return "mtk";
    }
    ALOGE("get_wifi_vendor_name failed, return defalut value: bcm");
    return "bcm";
}

int multi_wifi_load_driver()
{
    int wait_time=0;
    int dgle_no=0;
    if (!read_no(&dgle_no)) {
        if (!is_cfg80211_loaded()) {
            if (strcmp(get_wifi_vendor_name(), "qcn") == 0)
                qcn_cfg80211_load_driver();
            else
                cfg80211_load_driver();
        }
        if (dgle_no > 15)
            set_wifi_power(USB_POWER_UP);
        ALOGD("The matched dgle_no. is %d\n", dgle_no);
        if (dongle_registerd[dgle_no].load() != 0) {
                ALOGD("Load Wi-Fi driver error !!!\n");
                return -1;
        }
        ALOGD("wifi driver load ok\n");
        return 0;
    }
    set_wifi_power(SDIO_POWER_UP);
    if (!sdio_wifi_load_driver()) {
        return 0;
    }
    do {
        if (!usb_wifi_load_driver()) {
            return 0;
        }
        wait_time++;
        usleep(50000);
        ALOGD("wait usb ok\n");
    }while(wait_time<300);

    ALOGE("Wifi load fail\n");
    return -1;
}

int multi_wifi_unload_driver()
{
    int dgle_no=0;
    usleep(200000);
    read_no(&dgle_no);
    ALOGE("Start to unload driver ...\n");
    if (strcmp(get_wifi_vendor_name(), "mtk") == 0) {
        ifc_init();
        ifc_down("wlan0");
    }
    if (dongle_registerd[dgle_no].unload() != 0) {
        ALOGE("Failed to unload driver !\n");
        return -1;
    }
    ALOGE("Unload driver OK !\n");
    return 0;
}
