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
        printf("Run as %s i2c-num device address in-file\n",argv[0]);
        printf("Run as %s 1 0x50 spd_dump.bin\n",argv[0]);
    }
    else
    {
        char buf[512] = {0};
        int spd = open(argv[3], O_RDONLY);
        if (spd < 1)
        {
            perror("Failed to open file");
            exit(1);
        }
        if(read(spd, buf, 512)!=512)
        {
            perror("Failed read file");
            exit(1);
        }
        close(spd);

        int i2c_num = strtol(argv[1],NULL,0);
        int addr = strtol(argv[2],NULL,16);
        int file;
        system("modprobe i2c-dev");

        char filename[64];
        sprintf(filename,"/dev/i2c-%d",i2c_num);
        if ((file = open(filename, O_RDWR)) < 0)
        {
            perror("Failed to open the i2c bus");
            exit(1);
        }s

        //Set Page 0 (SPA0 command)
        ioctl(file, I2C_SLAVE, 0x36);
        i2c_smbus_write_byte_data(file,0,0);

        //Go back to regular to perform reading
        ioctl(file, I2C_SLAVE, addr);

        //Read the entire page 0
        for (int i = 0; i < 256; i++)
        {
            if(i2c_smbus_write_byte_data(file, i, buf[i])<0)
           {
            printf("Failded to write address Page 0 Add %02x",i);
           }

           usleep(500);
        }

        //Set Page 1 (SPA1 command)
        ioctl(file, I2C_SLAVE, 0x37);
        i2c_smbus_write_byte_data(file,0,0);
        ioctl(file, I2C_SLAVE, addr);

        //Read the entire page 1
        for (int i = 0; i < 256; i++)
        {
            if(i2c_smbus_write_byte_data(file, i, buf[i+256])<0)
           {
            printf("Failded to write address Page 1 Add %02x",i);
           }

           usleep(500);
        }

    }
}
