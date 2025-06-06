/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * Copyright (C) 2005 Novell, Inc.
 *
 * Nemo is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * Nemo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
 * Boston, MA 02110-1335, USA.
 *
 * Author: Anders Carlsson <andersca@imendio.com>
 *
 */

#ifndef NEMO_QUERY_H
#define NEMO_QUERY_H

#include <glib-object.h>

#define NEMO_TYPE_QUERY		(nemo_query_get_type ())
#define NEMO_QUERY(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), NEMO_TYPE_QUERY, NemoQuery))
#define NEMO_QUERY_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), NEMO_TYPE_QUERY, NemoQueryClass))
#define NEMO_IS_QUERY(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), NEMO_TYPE_QUERY))
#define NEMO_IS_QUERY_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), NEMO_TYPE_QUERY))
#define NEMO_QUERY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), NEMO_TYPE_QUERY, NemoQueryClass))

typedef struct NemoQueryDetails NemoQueryDetails;

typedef struct NemoQuery {
	GObject parent;
	NemoQueryDetails *details;
} NemoQuery;

typedef struct {
	GObjectClass parent_class;
} NemoQueryClass;

GType          nemo_query_get_type (void);
gboolean       nemo_query_enabled  (void);

NemoQuery* nemo_query_new      (void);

char *         nemo_query_get_file_pattern (NemoQuery *query);
void           nemo_query_set_file_pattern (NemoQuery *query, const char *text);

char *         nemo_query_get_content_pattern (NemoQuery *query);
void           nemo_query_set_content_pattern (NemoQuery *query, const char *text);
gboolean       nemo_query_has_content_pattern (NemoQuery *query);

char *         nemo_query_get_location       (NemoQuery *query);
void           nemo_query_set_location       (NemoQuery *query, const char *uri);

GList *        nemo_query_get_mime_types     (NemoQuery *query);
void           nemo_query_set_mime_types     (NemoQuery *query, GList *mime_types);
void           nemo_query_add_mime_type      (NemoQuery *query, const char *mime_type);

void           nemo_query_set_show_hidden    (NemoQuery *query, gboolean hidden);
gboolean       nemo_query_get_show_hidden    (NemoQuery *query);

gboolean       nemo_query_get_file_case_sensitive (NemoQuery *query);
void           nemo_query_set_file_case_sensitive (NemoQuery *query, gboolean case_sensitive);

gboolean       nemo_query_get_content_case_sensitive (NemoQuery *query);
void           nemo_query_set_content_case_sensitive (NemoQuery *query, gboolean case_sensitive);

gboolean       nemo_query_get_use_file_regex      (NemoQuery *query);
void           nemo_query_set_use_file_regex      (NemoQuery *query, gboolean file_use_regex);

gboolean       nemo_query_get_use_content_regex      (NemoQuery *query);
void           nemo_query_set_use_content_regex      (NemoQuery *query, gboolean content_use_regex);

gboolean       nemo_query_get_recurse         (NemoQuery *query);
void           nemo_query_set_recurse         (NemoQuery *query, gboolean recurse);

char *         nemo_query_to_readable_string (NemoQuery *query);
NemoQuery *nemo_query_load               (char *file);
gboolean       nemo_query_save               (NemoQuery *query, char *file);

#endif /* NEMO_QUERY_H */
