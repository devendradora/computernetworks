#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/pci.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <linux/device.h>



struct input_dev *mouse_input_dev;
static struct platform_device *mouse_platform_dev; 

static ssize_t write_to_mouse (struct device *dev,struct device_attribute *attr,const char *buffer, size_t count)
{
        int x, y, key;

        sscanf(buffer, "%d %d %d",&x,&y,&key);

   
        input_report_rel (mouse_input_dev, REL_X, x);
        input_report_rel (mouse_input_dev, REL_Y, y);

        printk ("mouse_event: X:%d Y:%d %d \n", x,y,key);

       
        if (key> 0) {
                if (key == 1)
                    input_report_key (mouse_input_dev,BTN_LEFT, 1);
                else if (key == 2)
                    input_report_key (mouse_input_dev,BTN_MIDDLE, 1);
                else
                    input_report_key (mouse_input_dev,BTN_RIGHT, 1);
        }

        input_sync (mouse_input_dev);

        return count;

}


DEVICE_ATTR (mouse_event,0777,NULL,write_to_mouse);


static struct attribute *mouse_attrs[] = { &dev_attr_mouse_event.attr,NULL };

static struct attribute_group mouse_attr_group = {
        .attrs = mouse_attrs,
};


static int __init mouse_init (void)
{
      
      mouse_platform_dev = platform_device_register_simple("mousesimulator",-1,NULL,0);
        
        if (IS_ERR (mouse_platform_dev)) {
                printk ("mouse simulator initailisatation failed\n");
          return PTR_ERR (mouse_platform_dev);
        }

      sysfs_create_group(&mouse_platform_dev->dev.kobj,&mouse_attr_group);
     
        
         mouse_input_dev = input_allocate_device();


        if (!mouse_input_dev) {
              printk ("Error input_allocate_device () \n");
                return -ENOMEM;
        }
         mouse_input_dev->name = "mousesimulator";
         mouse_input_dev->phys = "smousedriver/input0"; 
         mouse_input_dev->id.bustype = BUS_VIRTUAL;
         mouse_input_dev->id.vendor  = 0x0000;
         mouse_input_dev->id.product = 0x0000;
         mouse_input_dev->id.version = 0x0000;
       
        set_bit (EV_REL, mouse_input_dev->evbit);
        set_bit (REL_X, mouse_input_dev->relbit);
        set_bit (REL_Y, mouse_input_dev->relbit);
        set_bit (REL_WHEEL, mouse_input_dev->relbit);
 
        set_bit (EV_KEY, mouse_input_dev->evbit);
        set_bit (BTN_LEFT, mouse_input_dev->keybit);
        set_bit (BTN_MIDDLE, mouse_input_dev->keybit);
        set_bit (BTN_RIGHT, mouse_input_dev->keybit);

        input_register_device (mouse_input_dev);

        printk ("Mouse simulator Driver Initialized \n");

        return 0;
}

static void __exit mouse_exit(void)
{
      
        input_unregister_device (mouse_input_dev);   
        sysfs_remove_group (&mouse_platform_dev-> dev.kobj, &mouse_attr_group);
        platform_device_unregister(mouse_platform_dev);
        
        printk ("Mouse simulator Driver De Initialized \n");

        return;
}

module_init (mouse_init);
module_exit (mouse_exit);


MODULE_AUTHOR("Devendra Dora");
MODULE_DESCRIPTION("Mouse simulator Driver");
MODULE_LICENSE("GPL");
