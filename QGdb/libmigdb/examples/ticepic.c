/**[txh]********************************************************************

  Copyright (c) 2004 by Salvador E. Tropea.
  Covered by the GPL license.

  Comment:
  X11 example/test of the libmigdb.
  Run it from an X11 terminal (xterm, Eterm, etc.).
  
***************************************************************************/

#include <stdio.h>
#include <unistd.h> //usleep
#include "mi_gdb.h"

void cb_console(const char *str, void *data)
{
 printf("CONSOLE> %s\n",str);
}

/* Note that unlike what's documented in gdb docs it isn't usable. */
void cb_target(const char *str, void *data)
{
 printf("TARGET> %s\n",str);
}

void cb_log(const char *str, void *data)
{
 printf("LOG> %s\n",str);
}

void cb_to(const char *str, void *data)
{
 printf(">> %s",str);
}

void cb_from(const char *str, void *data)
{
 printf("<< %s\n",str);
}

volatile int async_c=0;

void cb_async(mi_output *o, void *data)
{
 printf("ASYNC\n");
 async_c++;
}

void print_frames(mi_frames *f, int free_f)
{
 mi_frames *ff=f;

 if (!f)
   {
    printf("Error! empty frames info\n");
    return;
   }
 while (f)
   {
    printf("Level %d, addr %p, func %s, where: %s:%d args? %c\n",f->level,f->addr,
           f->func,f->file,f->line,f->args ? 'y' : 'n');
    f=f->next;
   }
 if (free_f)
    mi_free_frames(ff);
}

int wait_for_stop(mi_h *h)
{
 int res=1;
 mi_stop *sr;

 while (!mi_get_response(h))
    usleep(1000);
 /* The end of the async. */
 sr=mi_res_stop(h);
 if (sr)
   {
    printf("Stopped, reason: %s\n",mi_reason_enum_to_str(sr->reason));
    print_frames(sr->frame,0);
    mi_free_stop(sr);
   }
 else
   {
    printf("Error while waiting\n");
    res=0;
   }
 return res;
}

void print_gvar(mi_gvar *v)
{
 if (!v)
   {
    printf("Error! failed to define variable\n");
    return;
   }
 printf("Variable name: '%s', type: '%s', number of children: %d format: %s expression: %s lang: %s editable: %c\n",
        v->name,v->type,v->numchild,mi_format_enum_to_str(v->format),
        v->exp,mi_lang_enum_to_str(v->lang),v->attr & MI_ATTR_EDITABLE ? 'y' : 'n');
}

void print_update(mi_gvar_chg *changed)
{
 printf("List of changed variables:\n");
 while (changed)
   {
    printf("Name: %s\nIn scope: %c\n",changed->name,changed->in_scope ? 'y' : 'n');
    if (changed->in_scope && changed->new_type)
      {
       printf("New type: %s\nNew num children: %d\n",changed->new_type,changed->new_num_children);
      }
    changed=changed->next;
    printf("\n");
   }
}

void print_children(mi_gvar *ch)
{
 int i;
 mi_gvar *s;

 if (!ch->child)
   {
    printf("Error! getting children list\n");
    return;
   }
 printf("\nChildren List (%d):\n",ch->numchild);
 for (i=0, s=ch->child; i<ch->numchild; s++, i++)
    {
     printf("Name: %s Exp: %s Children: %d",s->name,s->exp,s->numchild);
     if (s->type)
        printf(" Type: %s",s->type);
     if (s->value)
        printf(" Value: %s",s->value);
     printf("\n");
    }
 printf("\n");
}


int main(int argc, char *argv[])
{
 mi_aux_term *xterm_tty=NULL;
 mi_bkpt *bk;
 mi_frames *fr;
 /* This is like a file-handle for fopen.
    Here we have all the state of gdb "connection". */
 mi_h *h;
 mi_gvar *gv, *gv2;
 mi_gvar_chg *changed;
 char *value;
 int r_assign;

 /* You can use any gdb you want: */
 mi_set_gdb_exe("./icepic");
 /* You can use a terminal different than xterm: */
 /*mi_set_xterm_exe("/usr/bin/Eterm");*/

 /* Connect to gdb child. */
 h=mi_connect_local();
 if (!h)
   {
    printf("Connect failed\n");
    return 1;
   }
 printf("Connected to gdb!\n");

 /* Set all callbacks. */
 mi_set_console_cb(h,cb_console,NULL);
 mi_set_target_cb(h,cb_target,NULL);
 mi_set_log_cb(h,cb_log,NULL);
 mi_set_async_cb(h,cb_async,NULL);
 mi_set_to_gdb_cb(h,cb_to,NULL);
 mi_set_from_gdb_cb(h,cb_from,NULL);

 /* Set the name of the child and the command line aguments. */
 if (!gmi_set_exec(h,"./test_bcd.cod",""))
   {
    printf("Error setting exec y args\n");
    mi_disconnect(h);
    return 1;
   }

 /* Exit from gdb. */
 gmi_gdb_exit(h);
 /* Close the connection. */
 mi_disconnect(h);
 /* Wait 5 seconds and close the auxiliar terminal. */
 printf("Waiting 5 seconds\n");
 sleep(5);
 gmi_end_aux_term(xterm_tty);

 return 0;
}
