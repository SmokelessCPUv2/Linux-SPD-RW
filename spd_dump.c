#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Run as %s i2c-num device address out-file\n",argv[0]);
        printf("Run as %s 1 0x50 spd_dump.bin\n",argv[0]);
    }
    else
    {
        int i2c_num = strtol(argv[1],NULL,0);
        int addr = strtol(argv[2],NULL,16);
        int file;
        system("modprobe i2c-dev");
        char buf[512] = {0};
        char filename[64];
        sprintf(filename,"/dev/i2c-%d",i2c_num);
        if ((file = open(filename, O_RDWR)) < 0)
        {
            perror("Failed to open the i2c bus");
            exit(1);
        }

        //Set Page 0 (SPA0 command)
        ioctl(file, I2C_SLAVE, 0x36);
        i2c_smbus_write_byte_data(file,0,0);

        //Go back to regular to perform reading
        ioctl(file, I2C_SLAVE, addr);

        //Read the entire page 0
        for (int i = 0; i < 256; i++)
        {
            buf[i] = i2c_smbus_read_byte_data(file, i);
        }

        //Set Page 1 (SPA1 command)
        ioctl(file, I2C_SLAVE, 0x37);
        i2c_smbus_write_byte_data(file,0,0);
        ioctl(file, I2C_SLAVE, addr);

        //Read the entire page 1
        for (int i = 0; i < 256; i++)
        {
            buf[i+256] = i2c_smbus_read_byte_data(file, i);
        }


        //Save Dump to file
        int spd = open(argv[3], O_RDWR | O_TRUNC | O_CREAT);
        if (spd < 1)
        {
            perror("Failed to open file");
            exit(1);
        }
        write(spd, buf, 512);
        close(spd);
        chmod(argv[3], 0666);
        chown(argv[3], 1000,1000);
    }
}
