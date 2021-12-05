#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/time.h>

MODULE_AUTHOR("Ryuichi Ueda and Riko Bato");
MODULE_DESCRIPTION("driver for LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.1");
static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;
static volatile u32 *gpio_base = NULL; //アドレスをマッピングするための配列をグローバルで定義している
int gpio[] = {26, 19, 25};
void led_T(int gpio_num, int right_count)
{
    int i;
    for (i = 0; i < right_count; i++)
    {
        gpio_base[7] = 1 << gpio_num;
        msleep(25);
        gpio_base[10] = 1 << gpio_num;
        msleep(100);
    }
}
void hantei(int mode, int count_num)
{
    if (mode == 1)
        led_T(gpio[0], count_num);
    else if (mode == 2)
        led_T(gpio[1], count_num);
    else if (mode == 3)
    {
        int n = 2;
        led_T(gpio[2], n);
    }
}
static ssize_t led_write(struct file *filp, const char *buf, size_t count, loff_t *pos)
{
    char c;
    int i;
    int mode = 0;
    int count_num = 0;
    if (copy_from_user(&c, buf, sizeof(char)))
        return -EFAULT;

    printk(KERN_INFO "receive %c\n", c);

    if (c == 'a' || c == 'b' || c == 'c')
        mode = 1;
    else if (c == 'A' || c == 'B' || c == 'C')
        mode = 2;
    else if (c == 'Q')
        mode = 3;
    if (c == 'a' || c == 'A')
        count_num = 1;
    else if (c == 'b' || c == 'B')
        count_num = 2;
    else if (c == 'c' || c == 'C')
    {
        count_num = 3;
        led_T(gpio[2], 3);
    }

    hantei(mode, count_num);
    /*if (mode == 1)
        led_T(gpio[0], count_num);
    else if (mode == 2)
        led_T(gpio[1], count_num);
    else if (mode == 3)
    {
        int n = 2;
        led_T(gpio[2], n);
    }
    */
    msleep(50);
    for (i = 0; i < 3; i++)
        gpio_base[10] = 1 << gpio[i];
    return 1;
}

static ssize_t sushi_read(struct file *filp, char *buf, size_t count, loff_t *pos)
{
    int size = 0;
    char sushi[] = {'s', 'u', 's', 'h', 'i'};
    if (copy_to_user(buf + size, (const char *)sushi, sizeof(sushi)))
    {
        printk(KERN_ERR "sushi: copy_to_user failed\n");
        return -EFAULT;
    }
    size += sizeof(sushi);
    return size;
}
static struct file_operations led_fops = {
    .owner = THIS_MODULE,
    .write = led_write,
    .read = sushi_read};
static int __init init_mod(void)
{
    int retval;
    int i;
    retval = alloc_chrdev_region(&dev, 0, 1, "myled");
    if (retval < 0)
    {
        printk(KERN_ERR "alloc_chrdev_region failed\n");
        return retval;
    }
    printk(KERN_INFO "%s is loaded major:%d\n", __FILE__, MAJOR(dev));
    cdev_init(&cdv, &led_fops);
    retval = cdev_add(&cdv, dev, 1);
    if (retval < 0)
    {
        printk(KERN_ERR "cdev_add failed. major:%d minor %d\n", MAJOR(dev), MINOR(dev));
        return retval;
    }
    cls = class_create(THIS_MODULE, "myled");
    if (IS_ERR(cls))
    {
        printk(KERN_ERR "class_create failed.");
        return PTR_ERR(cls);
    }
    device_create(cls, NULL, dev, NULL, "myled%d", MINOR(dev));
    gpio_base = ioremap_nocache(0xfe200000, 0xA0);
    for (i = 0; i < 3; i++)
    {
        const u32 led = gpio[i];
        const u32 index = led / 10;       //gpiosel2
        const u32 shift = (led % 10) * 3; //15bit
        const u32 mask = ~(0x7 << shift);
        gpio_base[index] = (gpio_base[index] & mask) | (0x1 << shift);
    }
    return 0;
}
static void __exit cleanup_mod(void)
{
    cdev_del(&cdv);
    device_destroy(cls, dev);
    class_destroy(cls);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "%s is unloaded. major:%d\n", __FILE__, MAJOR(dev));
}

module_init(init_mod);    //マクロで関数を登録
module_exit(cleanup_mod); //同上
