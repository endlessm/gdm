/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2018 Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "config.h"

#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include <glib.h>

/* Return TRUE if we want to disable Wayland */
gboolean
parse_disable_wayland(char *argv)
{
        if (argv[0] == "0" ||
            !strncmp(argv, "FALSE\0", 6) ||
            !strncmp(argv, "false\0", 6))
                return FALSE;
        else
                return TRUE;
}

int
main (int argc, char *argv[])
{
        g_autoptr(GKeyFile) key_file = NULL;
        g_autoptr(GError) error = NULL;
        gboolean wayland_flag = FALSE;
        gboolean saved_okay;

        if (argc > 1)
                wayland_flag = !parse_disable_wayland(argv[1]);

        setlocale (LC_ALL, "");

        key_file = g_key_file_new ();

        g_key_file_set_boolean (key_file, "daemon", "WaylandEnable", wayland_flag);

        g_mkdir_with_parents (GDM_RUN_DIR, 0711);

        saved_okay = g_key_file_save_to_file (key_file, GDM_RUNTIME_CONF, &error);

        if (!saved_okay) {
                g_printerr ("gdm-disable-wayland: unable to %s wayland: %s",
                            wayland_flag ? "enable" : "disable", error->message);
                return EX_CANTCREAT;
        }

        return EX_OK;
}
