#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>

static struct kobject *my_kobj;
static int my_sys;

static int a;
static int b;
static int c[5];
static int arr_argc;

module_param(a, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
module_param(b, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
module_param_array(c, int, &arr_argc, 0000);

static ssize_t my_sys_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", my_sys);
}

static ssize_t my_sys_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	int ret = kstrtoint(buf, 10, &my_sys);
	if (ret < 0) return ret;
	return count;
}

static struct kobj_attribute my_sys_attribute = 
__ATTR(my_sys, 0755, my_sys_show, my_sys_store);

static int __init init_chrdev(void)
{
	my_kobj = kobject_create_and_add("my_kobject", kernel_kobj);
	int retval = sysfs_create_file(my_kobj, &my_sys_attribute.attr);
	int sum = a + b;
	int i;
	for (i = 0; i < 5; i++) sum += c[i];
	my_sys = sum;
	if (retval) kobject_put(my_kobj);
	return retval;
}

static void __exit cleanup_chrdev(void)
{
	kobject_put(my_kobj);
}

module_init (init_chrdev);
module_exit (cleanup_chrdev);

MODULE_LICENSE("GPL");
