#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/ioctl.h>
#include<linux/cdev.h>
#include<linux/slab.h>


#define WR_VALUE  _IOW('a','a',int32_t);
#define RD_VALUE  _IOR('a','b',int32_t);

int32_t value =0;
dev_t dev =0;

static struct class *desd_class;
static struct cdev *desd_cdev;

static int __init desd_driver_init(void);
static void __exit desd_driver_exit(void);

int desd_open(struct inode *inode , struct file *file);
int desd_release(struct inode *inode , struct file *file);
ssize_t desd_read(struct file *file,char__user *buff,size_t len,loff_t *off);
ssize_t desd_write(struct file *file,const char *buff,size_t len,loff_t *off);
static long desd_ioctl(struct file *file,unsigned int cmd ,long arg);


static struct file_operation fops =
{
    .owner         =THIS_MODULE,
    .read          =desd_read,
    .write         =desd_write,
    .release       =desd_release,
    .unlock_ioctl  =desd_ioctl,
}

static int desd_open(struct inode *inode , struct file *file)
{
    pr_info("device file open now\n");
}

static int desd_release(struct inode *inode , struct file *file)
{
    pr_info("device file release\n");
}

static int desd_read(struct file *file,char__user *buff,size_t len,loff_t *off)
{
    pr_info("now reading is active\n");
}

static int desd_write(struct file *file,char__user *buff,size_t len,loff_t *off)
{
    pr_info("now writing is active\n");
}

static long desd_ioctl(struct file *file,unsigned int cmd ,long arg)
{
    switch(cmd)
    {
        case WR_VALUE:
                    if(copy_from_user(&value,(int32_t*)arg,sizeof(value)))
                    {
                        pr_err("data writing err\n");
                    }
                    pr_info("value = %d",value);
                    break;

        case RD_VALUE:
                    if(copy_to_user(&value,(int32_t*)arg,sizeof(value)))     
                     {
                        pr_err("data writing err\n");
                    }
                    pr_info("value = %d",value);
                    break;

        default :
        pr_info("default error\n");
        break;
    }
    return 0;
}

static int __init hello_desd_init(void)
{
    if((alloc_chrdev_region(&dev,0,1,"desd_lab_xam"))<0)
    {
        pr_err("ERROR\n");
    }
    pr_info("major = %d \n minor = %d\n",MAJOR(dev),MINOR(dev));

    cdev_init(&desd_cdev,&fops);
    if((cdev_add(&desd_cdev,dev,1))<0)
    {
        pr_err("fail to create cdev\n");
        goto r_class;
    }

    if(IS_ERR(dev_class = class_create(THIS_MODULE,"desd_class")))
    {
        pr_err("error in device \n");
        goto r_device;
    }
     
     pr_info("module is inserted in kernel\n");

    r_device:
    class_destory(cdev_class);

    r_class:
    unregister_chrdev_region(dev,1);
    return -1;
}

static void __exit_driver_exit(void)
{
    device_destroy(dev_class,dev);
    class_destroy(dev_class);
    cdev_del(desd_cdev);
    unregister_chrdev_region(dev,1);
    pr_info("removed from kernel\n");
}

module_init(hello_desd);
module_exit(driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("PRANJAL");
MODULE_DESCRIPTION("IOCTL CONTROL");