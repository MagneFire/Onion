#ifndef SYSTEM_KCX_H__
#define SYSTEM_KCX_H__

#include <errno.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define AXPDEV "/dev/i2c-1"
#define KCXID (0x17) // 0x68 >> 1
//#define	KCXID	(0x3D)	// test for miyoomini i2c-1

int kcx_write(unsigned char address, unsigned char val)
{
    struct i2c_msg msg[1];
    struct i2c_rdwr_ioctl_data packets;
    unsigned char buf[2];
    int ret;
    int fd = open(AXPDEV, O_RDWR);
    ioctl(fd, I2C_TIMEOUT, 5);
    ioctl(fd, I2C_RETRIES, 1);

    buf[0] = address;
    buf[1] = val;
    msg[0].addr = KCXID;
    msg[0].flags = 0;
    msg[0].len = 2;
    msg[0].buf = buf;

    packets.nmsgs = 1;
    packets.msgs = &msg[0];
    ret = ioctl(fd, I2C_RDWR, &packets);

    close(fd);
    if (ret < 0)
        return -1;
    return 0;
}

int kcx_read(unsigned char address)
{
    struct i2c_msg msg[2];
    struct i2c_rdwr_ioctl_data packets;
    unsigned char val;
    int ret;
    int fd = open(AXPDEV, O_RDWR);
    ioctl(fd, I2C_TIMEOUT, 5);
    ioctl(fd, I2C_RETRIES, 1);

    msg[0].addr = KCXID;
    msg[0].flags = 0;
    msg[0].len = 1;
    msg[0].buf = &address;
    msg[1].addr = KCXID;
    msg[1].flags = I2C_M_RD;
    msg[1].len = 1;
    msg[1].buf = &val;

    packets.nmsgs = 2;
    packets.msgs = &msg[0];
    ret = ioctl(fd, I2C_RDWR, &packets);

    close(fd);
    if (ret < 0)
        return -1;
    return val;
}

#endif // SYSTEM_KCX_H__