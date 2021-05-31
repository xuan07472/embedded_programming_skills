/**
 * GPIO test driver, test the GPIO8_15, GPIO pin number 144, linux number 144-1,
 * the gpio is SD1_D0 pin.
 * ZhangJing 2018-03-20
 */

#include <linux/gpio.h>
#include <linux/platform_data/gpio-davinci.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/init.h> 
#include <linux/module.h> 
#include <linux/kernel.h> 
#include <linux/types.h>
#include <mach/mux.h>
#include <linux/interrupt.h>
#include <linux/irq.h>

#define GPIO_MAJOR 199                  // Major device NO.
#define GPIO_MINOR 0                    // Minor device NO.
#define DEVICE_NAME "omapl138_gpios"    // Driver and device node name.

#define GPIO_TEST_IOC_MAGIC          0x92
#define SET_OUTPUT_LOW _IO(GPIO_TEST_IOC_MAGIC, 0)
#define SET_OUTPUT_HIGH _IO(GPIO_TEST_IOC_MAGIC, 1)
#define GET_VALUE _IO(GPIO_TEST_IOC_MAGIC, 2)
#define SET_INPUT _IO(GPIO_TEST_IOC_MAGIC, 3)

//#define DEBUG 1

static struct class *gpio_class;
static struct gpio gpio_array[] =
{
    //{ GPIO_TO_PIN(0, 1), GPIOF_OUT_INIT_HIGH, "RTU_PLC_BAK_IO1"}, 
    //{ GPIO_TO_PIN(0, 2), GPIOF_OUT_INIT_LOW,     "RTU_CHG_EN" }, 
    //{ GPIO_TO_PIN(0, 3), GPIOF_IN,         "RTU_CHG_PG" }, 
    { GPIO_TO_PIN(8, 15), GPIOF_IN,         "DA850_GPIO8_15" }, 
};

static int gpio_open(struct inode *inode,struct file *file)
{
    printk(KERN_WARNING"gpio open success!\n");
    return 0;
}

static int gpio_release(struct inode *inode, struct file *filp)
{
  printk (KERN_ALERT "Device gpio released\n");
  return 0;
}

static int gpio_read(struct file*f,char *dst,size_t size,loff_t*offset)
{
    unsigned char num;
    unsigned long ret;

    ret = __copy_to_user(&num,dst,1);
#ifdef DEBUG
     printk("__copy_to_user:%d\n",num);
#endif

    return ret;
}

static int gpio_write(struct file*f,const char *src,size_t size,loff_t *offset)
{
     unsigned char num;
     unsigned long ret;

     ret = __copy_from_user(&num,src,1);
#ifdef DEBUG
     printk("__copy_from_user:%d\n",num);
#endif
     return ret;

}

static long gpio_ioctl(struct file *file,unsigned int cmd,unsigned long gpio)
{
    //int i;
    //unsigned long gpio_num = (gpio/100)*16+gpio%100; // gpio can set to 815, means gpio8_15, gpio num is 144-1.
    unsigned long gpio_num = GPIO_TO_PIN(8, 15);

    printk("====gpio_ioctl, cmd:%d, gpio:%ld.\n", cmd, gpio);
    
    printk(KERN_WARNING"gpio_ioctl.\n");
    //for (i = 0; i < ARRAY_SIZE(gpio_array); i++) {
    //    if(gpio_array[i].gpio == gpio_num)
    //        goto valid_gpio;
    //}
    //return -1;
        
//valid_gpio:
    switch(cmd) // The GPIO control.
    {
        case SET_OUTPUT_LOW: // 0
        {
            gpio_direction_output(gpio_num, 0);
            break;
        }
        case SET_OUTPUT_HIGH: // 1 
        {
            gpio_direction_output(gpio_num, 1);
            break;
        }
        case GET_VALUE: // 2 
        {
            printk("====GET_VALUE: %d.\n", gpio_get_value(gpio_num));
            return gpio_get_value(gpio_num);    
        } 
        case SET_INPUT: // 3
        {
            gpio_direction_input(gpio_num);
            break;
        }
        default:
        {
            printk(KERN_EMERG "GPIO command mistake!!!\n");
            break;
        }
    }
    return 0;
}
        
static const struct file_operations gpio_fops =
{
  .owner = THIS_MODULE,
  .open = gpio_open,
  .release = gpio_release,
  .read = gpio_read,
  .write = gpio_write,
  .unlocked_ioctl = gpio_ioctl,
};

static irqreturn_t resume_irq(int irq, void *devid)
{
	printk("====Get resume_irq!\n");
	return IRQ_HANDLED;
}

static int __init gpio_init(void) /* Module init. */
{
    int ret;
    dev_t my_dev_no;
    struct cdev *gpio_cdev;
    int result;

    /* Request the GPIO. */
    ret = gpio_request_array(gpio_array, ARRAY_SIZE(gpio_array));
    if (ret < 0) 
    {
        printk(KERN_EMERG "GPIO request failed\n");
        goto request_failed; 
    }

    /* Register char driver. */
    gpio_cdev = cdev_alloc();
    if(gpio_cdev == NULL)
    {
        printk(KERN_EMERG "Cannot alloc cdev\n");
        goto request_failed;
    }
    cdev_init(gpio_cdev,&gpio_fops);
    gpio_cdev->owner=THIS_MODULE;
    result=alloc_chrdev_region(&my_dev_no,0,1,DEVICE_NAME); 
    if(result < 0)
    {
        printk(KERN_EMERG "alloc_chrdev_region failed\n");
        goto request_failed;
    }
    ret=cdev_add(gpio_cdev,my_dev_no,1); 
    
    ret = register_chrdev(GPIO_MAJOR, DEVICE_NAME, &gpio_fops);
    if(ret < 0)
    {
        printk(KERN_EMERG "GPIO register failed\n");
        goto request_failed;
    }

    /* Register gpio irq. */
    ret = gpio_direction_input(GPIO_TO_PIN(8, 15));
    if (ret)
        goto request_failed;
    irq_set_irq_type(gpio_to_irq(GPIO_TO_PIN(8, 15)),
             IRQ_TYPE_EDGE_FALLING);
    ret = request_irq(gpio_to_irq(GPIO_TO_PIN(8, 15)),
            resume_irq, 0,
            "Resume Detect", NULL);
    if (ret)
        goto request_failed2;    
    
    /* Create /sysfs class file. */
    gpio_class = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(gpio_class, NULL, MKDEV(GPIO_MAJOR, GPIO_MINOR), NULL, DEVICE_NAME);
    return ret;

request_failed2:
	free_irq(gpio_to_irq(GPIO_TO_PIN(8, 15)), NULL);

request_failed:
    gpio_free_array(gpio_array, ARRAY_SIZE(gpio_array)); 
    return ret;
}

static void __exit gpio_exit(void)
{
    device_destroy(gpio_class, MKDEV(GPIO_MAJOR, GPIO_MINOR));
    class_unregister(gpio_class);
    class_destroy(gpio_class);
    unregister_chrdev(GPIO_MAJOR, DEVICE_NAME);
    //gpio_free_array(gpio_array, ARRAY_SIZE(gpio_array));
}

module_init(gpio_init);
module_exit(gpio_exit);
MODULE_LICENSE("GPL");
MODULE_VERSION ("v2.0");
MODULE_AUTHOR("ZhangJing <jing_zhang@xxx.com>");
MODULE_DESCRIPTION("OMAPL138 GPIO driver");
