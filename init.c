/*
 * Copyright (c) 1983, 1993
 *  The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *  This product includes software developed by the University of
 *  California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#define NULL ((void *)0)

#ifndef lint
static char sccsid[] = "@(#)init.c  8.1 (Berkeley) 5/31/93";
#endif /* not lint */

#define SIGINT 2

//#include <sys/types.h>
#include "externs.h"
#ifndef MSDOS /* BDS - Passwords in dos, sure! */
//#include <pwd.h>
#else /* Stuff I need for Borland C's "random" */
//#include <stdlib.h>
//#include <time.h>
#endif

initialize(startup)
    char startup;
{
    register struct objs *p;
    void die();

    puts("Version 4.2, fall 1984.");
    puts("First Adventure game written by His Lordship, the honorable");
    puts("Admiral D.W. Riggle\n");
#ifdef MSDOS
    puts("Lowered to MS-DOS Standards By Brian Douglas Smith, 31 May 1996\n");
    randomize(); /* Might as well put this here. */
#else /* unix */
    srand(getpid());
#endif
    getutmp(uname);
    if (startup)
        location = dayfile;
    wiz = wizard(uname);
    wordinit();
    if (startup) {
        direction = NORTH;
        game_time = 0;
        snooze = CYCLE * 1.5;
        position = 22;
        setbit(wear, PAJAMAS);
        fuel = TANKFULL;
        torps = TORPEDOES;
        for (p = dayobjs; p->room != 0; p++)
            setbit(location[p->room].objects, p->obj);
    } else
        restore();
    signal(SIGINT, die);
}

getutmp(uname)
    char *uname;
{
//#ifdef MSDOS
    strcpy( uname, "Player" );
/*#else
    struct passwd *ptr;

    ptr = getpwuid(getuid());
    strcpy(uname, ptr ? ptr->pw_name : "");
#endif*/
}

char *list[] = {    /* hereditary wizards */
    "riggle",
    "chris",
    "edward",
    "comay",
    "yee",
    "dmr",
    "ken",
    0
};

char *badguys[] = {
    "wnj",
    "root",
    "ted",
    0
};

wizard(uname)
    char *uname;
{
    char flag;

    if (flag = checkout(uname))
        printf("You are the Great wizard %s.\n", uname);
    return flag;
}

checkout(uname)
    register char *uname;
{
    register char **ptr;

    for (ptr = list; *ptr; ptr++)
        if (strcmp(*ptr, uname) == 0)
            return 1;
    for (ptr = badguys; *ptr; ptr++)
        if (strcmp(*ptr, uname) == 0) {
            printf("You are the Poor anti-wizard %s.  Good Luck!\n",
                uname);
            if (location != NULL) {
                CUMBER = 3;
                WEIGHT = 9;     /* that'll get him! */
                game_clock = 10;
                setbit(location[7].objects, WOODSMAN);  /* viper room */
                setbit(location[20].objects, WOODSMAN); /* laser " */
                setbit(location[13].objects, DARK);     /* amulet " */
                setbit(location[8].objects, ELF);       /* closet */
            }
            return 0;   /* anything else, Chris? */
        }
    return 0;
}
