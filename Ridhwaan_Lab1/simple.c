#include <linux/int.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>

struct birthday
{
int month;
int day;
int year;
char *name;
struct list_head list;
}

static LIST_HEAD(birthday_list);

int simple_init(void)
{
int i=0
int day= 2, month= 8, year= 1995;
char*name[]= {"linux", "unix", "Mac", "Windows", "Solaris"};
struct birthday *temp= NULL;

while (i<5){

temp = kmalloc(sizeof(*temp),GFP_KERNEL);
temp->day= day++;
temp->month= month++;
temp->year=year++;
temp->name=name[i];


INT_HEAD_LIST(&(temp->list));
list_add_tail(&(temp->list), &(birthday_list));

i++;
}

temp = NULL;
list_for_each_entry(temp, &birthday_list,list){

print(KERN_INFO "Name: <%s>\n,"temp->name);
print(KERN_INFO "Birthday day: <%d/%d/%d>\n,"temp->month,temp->day,temp->year);
}

return 0;
}

int simple_exit(void)
{

struct birthday* ptr=NULL, *next= *NULL;
int i=1;

list_for_each_entry_safe(ptr,next,&birthday_list,list){

print(KERN_INFO "Remove Element <%d> from list \n", i++);
list_del(&(ptr->list));
kfree(ptr);
}

return 0;
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SIMPLE MODULE");
MODULE_AUTHOR("RIDHWAAN");




