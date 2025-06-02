Here's how the code in the relevant files might be modified according to your Phase 1 plan:

--- START OF MODIFIED FILE src/nemo-icon-private.h ---

```c
/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Nemo
 *
 * Copyright (C) 2000 Eazel, Inc.
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, MA 02110-1335, USA.
 *
 * Authors: Darin Adler <darin@bentspoon.com>
 *
 */

#ifndef NEMO_ICON_PRIVATE_H
#define NEMO_ICON_PRIVATE_H

#include <libnemo-private/nemo-icon.h>
#include <libnemo-private/nemo-icon-canvas-item.h>
#include <libnemo-private/nemo-icon-container.h>

struct NemoIcon {
	NemoIconData *data;
	NemoIconCanvasItem *item;
	double x;
	double y;
	double scale;
	gboolean selected;
	gboolean has_lazy_position;
	gboolean is_new_for_monitor;
	double saved_ltr_x; /* x position, ignoring RTL mode, for restoring after layout changes */

	/* For DnD */
	gboolean dnd_selected;
	gboolean dnd_sensitive;
	gboolean dnd_drop_target;

	/* For selection */
	gboolean band_selected;

	/* For rename */
	gboolean rename_active;

	/* For icon container */
	NemoIconContainer *container;
	GList *link;

	/* For desktop icon view */
	gboolean is_desktop_orphan;

	/* For filter */
	gboolean is_visible;        // NEW: visibility state (overall)
	gboolean is_filtered_out;   // NEW: filter state (specifically due to filtering)
};

#endif /* NEMO_ICON_PRIVATE_H */
```

--- END OF MODIFIED FILE src/nemo-icon-private.h ---

--- START OF MODIFIED FILE src/nemo-icon-container.h ---

```c
/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* fm-icon-container.h - the container widget for file manager icons

   Copyright (C) 2002 Sun Microsystems, Inc.

   The Gnome Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the Gnome Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
   Boston, MA 02110-1335, USA.

   Author: Michael Meeks <michael@ximian.com>
*/

#ifndef NEMO_ICON_CONTAINER_H
#define NEMO_ICON_CONTAINER_H

#include <eel/eel-canvas.h>
#include <libnemo-private/nemo-icon.h>
#include <libnemo-private/nemo-file.h>
#include <libnemo-private/nemo-view-layout-constants.h>
#include <libnemo-private/nemo-placement-grid.h>
#include <libnemo-private/nemo-centered-placement-grid.h>

#define NEMO_TYPE_ICON_CONTAINER nemo_icon_container_get_type()
#define NEMO_ICON_CONTAINER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), NEMO_TYPE_ICON_CONTAINER, NemoIconContainer))
#define NEMO_ICON_CONTAINER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), NEMO_TYPE_ICON_CONTAINER, NemoIconContainerClass))
#define NEMO_IS_ICON_CONTAINER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), NEMO_TYPE_ICON_CONTAINER))
#define NEMO_IS_ICON_CONTAINER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), NEMO_TYPE_ICON_CONTAINER))
#define NEMO_ICON_CONTAINER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), NEMO_TYPE_ICON_CONTAINER, NemoIconContainerClass))

typedef struct NemoIconContainerDetails NemoIconContainerDetails;

struct NemoIconContainer {
	EelCanvas parent;
	NemoIconContainerDetails *details;
};

struct NemoIconContainerClass {
	EelCanvasClass parent_class;

    gboolean is_grid_container;

	/* virtual methods */
	NemoIconInfo * (* get_icon_images) (NemoIconContainer *container,
					    NemoIconData      *data,
					    int                    size,
					    gboolean               for_drag_accept,
					    gboolean              *has_window_open,
                        gboolean               visible);
	char *         (* get_icon_description) (NemoIconContainer *container,
						 NemoIconData      *data);
	void           (* get_icon_text) (NemoIconContainer *container,
					  NemoIconData      *data,
					  char                 **editable_text,
					  char                 **additional_text,
                      gboolean              *pinned,
                      gboolean              *fav_unavailable,
					  gboolean               include_invisible);
	int            (* compare_icons) (NemoIconContainer *container,
					  NemoIconData      *icon_a,
					  NemoIconData      *icon_b);
	void           (* freeze_updates) (NemoIconContainer *container);
	void           (* unfreeze_updates) (NemoIconContainer *container);
	void           (* lay_down_icons) (NemoIconContainer *container,
					   GList               *icons,
					   double               start_y);
	void           (* icon_set_position) (NemoIconContainer *container,
					      NemoIcon          *icon,
					      double             x,
					      double             y);
	void           (* move_icon) (NemoIconContainer *container,
				      NemoIcon          *icon,
				      int                  x,
				      int                  y,
				      double               scale,
				      gboolean             raise,
				      gboolean             snap,
				      gboolean             update_position);
	void           (* update_icon) (NemoIconContainer *container,
					NemoIcon          *icon,
                    gboolean           visible);
	void           (* align_icons) (NemoIconContainer *container);
	void           (* reload_icon_positions) (NemoIconContainer *container);
	void           (* finish_adding_new_icons) (NemoIconContainer *container);
	void           (* icon_get_bounding_box) (NemoIcon          *icon,
						  int                 *x1_return,
						  int                 *y1_return,
						  int                 *x2_return,
						  int                 *y2_return,
						  NemoIconCanvasItemBoundsUsage usage);
	void           (* set_zoom_level) (NemoIconContainer *container,
					   gint                 new_level);
    gint           (* get_max_layout_lines_for_pango) (NemoIconContainer *container);
    gint           (* get_max_layout_lines) (NemoIconContainer *container);
    gint           (* get_additional_text_line_count) (NemoIconContainer *container);

	/* signals */
	void (* selection_changed) (NemoIconContainer *container);
	void (* activate) (NemoIconContainer *container,
			   GList *file_list);
	void (* activate_alternate) (NemoIconContainer *container,
				     GList *file_list);
	void (* activate_previewer) (NemoIconContainer *container,
				     GList *file_list,
				     GArray *locations);
	void (* middle_click) (NemoIconContainer *container,
			       GdkEventButton *event);
	void (* band_select_started) (NemoIconContainer *container);
	void (* band_select_ended) (NemoIconContainer *container);
	void (* context_click_selection) (NemoIconContainer *container,
					  GdkEventButton *event);
	void (* context_click_background) (NemoIconContainer *container,
					   GdkEventButton *event);
	void (* icon_position_changed) (NemoIconContainer *container,
					NemoIconData *icon_data,
					const NemoIconPosition *position);
	void (* icon_rename_started) (NemoIconContainer *container,
				      GtkWidget *editable,
				      gpointer callback_data);
	void (* icon_rename_ended) (NemoIconContainer *container,
				    NemoIconData *icon_data,
				    const char *new_name);
	void (* icon_stretch_started) (NemoIconContainer *container);
	void (* icon_stretch_ended) (NemoIconContainer *container);
	void (* layout_changed) (NemoIconContainer *container);
	gboolean (* get_stored_layout_timestamp) (NemoIconContainer *container,
						  NemoIconData *icon_data,
						  time_t *timestamp);
	gboolean (* store_layout_timestamp) (NemoIconContainer *container,
					     NemoIconData *icon_data,
					     const time_t *timestamp);
	char * (* get_icon_uri) (NemoIconContainer *container,
				 NemoIconData *icon_data);
	char * (* get_icon_drop_target_uri) (NemoIconContainer *container,
					     NemoIconData *icon_data);
	void (* move_copy_items) (NemoIconContainer *container,
				  const GList *item_uris,
				  GArray *relative_item_points,
				  const char *target_dir,
				  int copy_action,
				  int x, int y);
	char * (* get_container_uri) (NemoIconContainer *container);
	gboolean (* can_accept_item) (NemoIconContainer *container,
				      NemoFile *target_item,
				      const char *item_uri);
	void (* handle_netscape_url) (NemoIconContainer *container,
				      const char *encoded_url,
				      const char *target_uri,
				      GdkDragAction action,
				      int x, int y);
	void (* handle_uri_list) (NemoIconContainer *container,
				  const char *item_uris,
				  const char *target_uri,
				  GdkDragAction action,
				  int x, int y);
	void (* handle_text) (NemoIconContainer *container,
			      const char *text,
			      const char *target_uri,
			      GdkDragAction action,
			      int x, int y);
	void (* handle_raw) (NemoIconContainer *container,
			     const char *raw_data,
			     int length,
			     const char *target_uri,
			     const char *direct_save_uri,
			     GdkDragAction action,
			     int x, int y);
    gchar * (* get_tooltip_text) (NemoIconContainer *container,
                                  NemoFile          *file);
    void (* icon_added) (NemoIconContainer *container,
                         NemoIconData      *icon_data);
    void (* icon_removed) (NemoIconContainer *container,
                           NemoIconData      *icon_data);
};

/* GObject support */
GType nemo_icon_container_get_type (void);

/* Public methods */
NemoIconContainer *nemo_icon_container_new (void);
void nemo_icon_container_set_use_drop_shadows (NemoIconContainer *container,
						   gboolean use_drop_shadows);
void nemo_icon_container_set_font (NemoIconContainer *container,
				       const char *font_name);
void nemo_icon_container_set_zoom_level (NemoIconContainer *container,
					     NemoZoomLevel zoom_level);
NemoZoomLevel nemo_icon_container_get_zoom_level (NemoIconContainer *container);
void nemo_icon_container_set_label_position (NemoIconContainer *container,
						 NemoIconLabelPosition label_position);
NemoIconLabelPosition nemo_icon_container_get_label_position (NemoIconContainer *container);
void nemo_icon_container_set_all_columns_same_width (NemoIconContainer *container,
							 gboolean all_columns_same_width);
gboolean nemo_icon_container_get_all_columns_same_width (NemoIconContainer *container);
void nemo_icon_container_set_layout_mode (NemoIconContainer *container,
					      NemoIconLayoutMode layout_mode);
NemoIconLayoutMode nemo_icon_container_get_layout_mode (NemoIconContainer *container);
gboolean nemo_icon_container_is_layout_rtl (NemoIconContainer *container);
void nemo_icon_container_set_rtl_positions (NemoIconContainer *container);
double nemo_icon_container_get_mirror_x_position (NemoIconContainer *container,
						      NemoIcon *icon,
						      double x);
void nemo_icon_container_set_is_fixed_size (NemoIconContainer *container,
						gboolean is_fixed_size);
gboolean nemo_icon_container_get_is_fixed_size (NemoIconContainer *container);
void nemo_icon_container_set_is_desktop (NemoIconContainer *container,
					     gboolean is_desktop);
gboolean nemo_icon_container_get_is_desktop (NemoIconContainer *container);
void nemo_icon_container_set_margins (NemoIconContainer *container,
					  gint left,
					  gint right,
					  gint top,
					  gint bottom);
void nemo_icon_container_set_ok_to_load_deferred_attrs (NemoIconContainer *container,
							    gboolean ok);
void nemo_icon_container_set_store_layout_timestamps (NemoIconContainer *container,
							  gboolean store_layout_timestamps);
void nemo_icon_container_store_layout_timestamps_now (NemoIconContainer *container);
void nemo_icon_container_set_forced_icon_size (NemoIconContainer *container,
						   gint forced_icon_size);
void nemo_icon_container_set_single_click_mode (NemoIconContainer *container,
						    gboolean single_click_mode);
void nemo_icon_container_set_click_to_rename_enabled (NemoIconContainer *container,
                                                          gboolean enabled);
void nemo_icon_container_set_allow_moves (NemoIconContainer *container,
					      gboolean allow_moves);
void nemo_icon_container_set_auto_layout (NemoIconContainer *container,
					      gboolean auto_layout);
gboolean nemo_icon_container_is_auto_layout (NemoIconContainer *container);
void nemo_icon_container_set_keep_aligned (NemoIconContainer *container,
					       gboolean keep_aligned);
gboolean nemo_icon_container_is_keep_aligned (NemoIconContainer *container);
void nemo_icon_container_set_grid_adjusts (NemoIconContainer *container,
                                           gint               h_adjust,
                                           gint               v_adjust);
void nemo_icon_container_set_horizontal_layout (NemoIconContainer *container,
                                                gboolean           horizontal);
void nemo_icon_container_set_highlighted_for_clipboard (NemoIconContainer *container,
							    GList *file_list);
void nemo_icon_container_begin_loading (NemoIconContainer *container);
void nemo_icon_container_end_loading (NemoIconContainer *container,
					  gboolean all_files_seen);
gboolean nemo_icon_container_add (NemoIconContainer *container,
				      NemoIconData *icon_data);
gboolean nemo_icon_container_remove (NemoIconContainer *container,
					 NemoIconData *icon_data);
void nemo_icon_container_clear (NemoIconContainer *container);
void nemo_icon_container_request_update (NemoIconContainer *container,
					     NemoIconData *icon_data);
void nemo_icon_container_request_update_all (NemoIconContainer *container);
void nemo_icon_container_invalidate_labels (NemoIconContainer *container);
void nemo_icon_container_for_each (NemoIconContainer *container,
				       NemoIconContainerForeachFunc callback,
				       gpointer callback_data);
GList *nemo_icon_container_get_selection (NemoIconContainer *container);
GList *nemo_icon_container_peek_selection (NemoIconContainer *container);
gint nemo_icon_container_get_selection_count (NemoIconContainer *container);
void nemo_icon_container_set_selection (NemoIconContainer *container,
					    GList *selection);
void nemo_icon_container_select_all (NemoIconContainer *container);
void nemo_icon_container_unselect_all (NemoIconContainer *container);
void nemo_icon_container_invert_selection (NemoIconContainer *container);
void nemo_icon_container_reveal (NemoIconContainer *container,
				     NemoIconData *icon_data);
void nemo_icon_container_scroll_to_icon (NemoIconContainer *container,
					     NemoIconData *icon_data);
NemoIconData *nemo_icon_container_get_first_visible_icon (NemoIconContainer *container);
void nemo_icon_container_start_renaming_selected_item (NemoIconContainer *container,
							   gboolean select_all);
void nemo_icon_container_end_renaming_mode (NemoIconContainer *container,
						gboolean accept_changes);
NemoIconData *nemo_icon_container_get_icon_being_renamed (NemoIconContainer *container);
void nemo_icon_container_redo_layout (NemoIconContainer *container);
void nemo_icon_container_sort (NemoIconContainer *container);
void nemo_icon_container_resort (NemoIconContainer *container);
void nemo_icon_container_freeze_icon_positions (NemoIconContainer *container);
void nemo_icon_container_show_stretch_handles (NemoIconContainer *container);
void nemo_icon_container_unstretch (NemoIconContainer *container);
gboolean nemo_icon_container_has_stretch_handles (NemoIconContainer *container);
gboolean nemo_icon_container_is_stretched (NemoIconContainer *container);
gboolean nemo_icon_container_is_empty (NemoIconContainer *container);
GArray *nemo_icon_container_get_selected_icon_locations (NemoIconContainer *container);
NemoIcon *nemo_icon_container_lookup_icon_by_file (NemoIconContainer *container, NemoFile *file); // NEW
void nemo_icon_container_set_icon_visibility(NemoIconContainer *container, NemoIconData *data, gboolean visible); // NEW
void nemo_icon_container_set_all_icons_visibility(NemoIconContainer *container, gboolean visible); // NEW

#endif /* NEMO_ICON_CONTAINER_H */
```

--- END OF MODIFIED FILE src/nemo-icon-container.h ---

--- START OF MODIFIED FILE src/nemo-icon-container.c ---

```c
/* ... (existing includes) ... */
#include "nemo-icon-private.h" // Ensure this is included for NemoIcon struct access

/* ... (existing static function declarations) ... */

// NEW function implementation
NemoIcon *
nemo_icon_container_lookup_icon_by_file (NemoIconContainer *container, NemoFile *file)
{
    g_return_val_if_fail (NEMO_IS_ICON_CONTAINER (container), NULL);
    g_return_val_if_fail (NEMO_IS_FILE (file), NULL);

    // Assuming details->icon_set is the GHashTable mapping NemoFile* to NemoIcon*
    // If not, adjust this to how icons are actually stored/indexed.
    // This was an assumption in the user's plan. If icon_set is a GList,
    // then a g_list_find_custom would be needed.
    if (container->details->icon_set) { // Check if icon_set is initialized
         return g_hash_table_lookup (container->details->icon_set, file);
    }
    return NULL;
}

// NEW function implementation
void
nemo_icon_container_set_icon_visibility(NemoIconContainer *container, NemoIconData *data, gboolean visible)
{
    NemoIcon *icon;

    g_return_if_fail(NEMO_IS_ICON_CONTAINER(container));
    g_return_if_fail(data != NULL);

    icon = nemo_icon_container_lookup_icon(container, data);
    if (icon && icon->item) {
        if (icon->is_visible != visible) { // Only act if state changes
            icon->is_visible = visible;
            if (visible) {
                eel_canvas_item_show(EEL_CANVAS_ITEM(icon->item));
            } else {
                eel_canvas_item_hide(EEL_CANVAS_ITEM(icon->item));
            }
            // Requesting a redraw of the specific item might be more efficient
            // than a full container relayout if only visibility changes.
            // However, if filtering implies relayout, then a full update is needed.
            // For now, assume a full update is desired after visibility changes.
            // eel_canvas_item_request_redraw(EEL_CANVAS_ITEM(icon->item));
        }
    }
}

// NEW function implementation
void
nemo_icon_container_set_all_icons_visibility(NemoIconContainer *container, gboolean visible)
{
    GList *l;
    g_return_if_fail(NEMO_IS_ICON_CONTAINER(container));

    for (l = container->details->icons; l != NULL; l = l->next) {
        NemoIcon *icon = l->data;
        if (icon && icon->item) {
            if (icon->is_visible != visible) { // Only act if state changes
                 icon->is_visible = visible;
                if (visible) {
                    eel_canvas_item_show(EEL_CANVAS_ITEM(icon->item));
                } else {
                    eel_canvas_item_hide(EEL_CANVAS_ITEM(icon->item));
                }
            }
        }
    }
    // After changing visibility of multiple icons, a relayout and full redraw is likely necessary.
    // nemo_icon_container_request_update_all(container); // Or nemo_icon_container_redo_layout(container);
}


/* Modify existing functions like nemo_icon_container_redo_layout */
static void
nemo_icon_container_redo_layout (NemoIconContainer *container)
{
	GList *layout_icons = NULL; // NEW: List of icons to actually layout
	GList *p;

	/* ... (existing code) ... */

	if (container->details->auto_layout) {
		nemo_icon_container_sort (container);
		// NEW: Filter icons for layout based on visibility
		for (p = container->details->icons; p != NULL; p = p->next) {
			NemoIcon *icon = p->data;
			if (icon->is_visible && !icon->is_filtered_out) { // Consider both general visibility and filter status
				layout_icons = g_list_prepend(layout_icons, icon);
			}
		}
		layout_icons = g_list_reverse(layout_icons); // Maintain original order among visible icons
		NEMO_ICON_CONTAINER_GET_CLASS (container)->lay_down_icons (container, layout_icons, 0.0);
		g_list_free(layout_icons); // Free the temporary list
	} else if (container->details->keep_aligned) {
		NEMO_ICON_CONTAINER_GET_CLASS (container)->align_icons (container);
	}

	/* ... (existing code) ... */
}

/* ... (other existing functions in nemo-icon-container.c) ... */
```

--- END OF MODIFIED FILE src/nemo-icon-container.c ---

--- START OF MODIFIED FILE src/nemo-window.c ---

```c
/* ... (existing includes) ... */
#include "nemo-icon-container.h" // Make sure this is included
#include "nemo-icon-private.h"   // For NemoIcon struct access
#include "nemo-icon-view.h"      // For NEMO_IS_ICON_VIEW and nemo_icon_view_get_icon_container
#include "nemo-list-view.h"      // For NEMO_IS_LIST_VIEW
#include "nemo-list-model.h"     // For NemoListModel functions

/* ... (existing static function declarations & definitions) ... */

// NEW: Helper function from your plan (or similar logic)
// This function determines if an icon should be visible based on the filter.
// It's used by apply_filter_to_icon_container_optimized.
static gboolean
should_file_be_visible_in_filter (NemoWindow *window, NemoFile *file)
{
    const gchar *filter = window->details->filter_text; // Assuming filter_text is already lowercase

    if (!filter || filter[0] == '\0') {
        return TRUE; // No filter, always visible
    }

    gboolean visible = FALSE;
    if (file) {
        gchar *display_name = nemo_file_get_display_name (file);
        if (display_name) {
            gchar *name_lower = g_utf8_strdown (display_name, -1);
            if (g_strstr_len (name_lower, -1, filter) != NULL) {
                visible = TRUE;
            }
            g_free (name_lower);
            g_free (display_name);
        }
    }
    return visible;
}

// NEW: Optimized filter application function from your plan
// This function now toggles visibility instead of clearing and recreating.
static int
apply_filter_to_icon_container_optimized(NemoIconContainer *container,
                                         NemoWindow *window,
                                         GList *all_files, // All files in the current directory
                                         guint *out_visible_folders,
                                         guint *out_visible_files)
{
    GList *l;
    int visible_item_count = 0;
    gboolean needs_relayout = FALSE; // Track if relayout is needed

    g_return_val_if_fail(NEMO_IS_ICON_CONTAINER(container), 0);
    g_return_val_if_fail(NEMO_IS_WINDOW(window), 0);

    if (out_visible_folders) *out_visible_folders = 0;
    if (out_visible_files) *out_visible_files = 0;

    if (all_files == NULL && g_list_length(container->details->icons) == 0) {
        // No files in directory and no icons in container, nothing to do.
        return 0;
    }

    // Iterate through all icons currently in the container
    for (l = container->details->icons; l != NULL; l = l->next) {
        NemoIcon *icon = NEMO_ICON(l->data);
        NemoFile *file = NEMO_FILE(icon->data); // Assuming icon->data is NemoFile*

        gboolean should_be_visible_now = should_file_be_visible_in_filter(window, file);

        // Update the icon's filtered_out state and actual visibility
        if (icon->is_filtered_out == should_be_visible_now) { // is_filtered_out is true if it *should not* be visible
            icon->is_filtered_out = !should_be_visible_now;
            needs_relayout = TRUE; // Visibility change might require relayout
            if (should_be_visible_now) {
                eel_canvas_item_show(EEL_CANVAS_ITEM(icon->item));
            } else {
                eel_canvas_item_hide(EEL_CANVAS_ITEM(icon->item));
            }
        }

        if (should_be_visible_now) {
            visible_item_count++;
            if (out_visible_folders && out_visible_files) {
                if (nemo_file_is_directory(file)) {
                    (*out_visible_folders)++;
                } else {
                    (*out_visible_files)++;
                }
            }
        }
    }

    if (needs_relayout) {
        // A full relayout might be needed if icon visibility changes affect grid
        nemo_icon_container_request_update_all(container);
    }

    return visible_item_count;
}


// MODIFIED: on_filter_entry_changed
static void
on_filter_entry_changed (GtkEntry *entry, gpointer user_data)
{
    NemoWindow *window = NEMO_WINDOW (user_data);
    const gchar *text = gtk_entry_get_text (entry);
    NemoWindowSlot *slot;
    NemoView *active_view;
    guint visible_files_count = 0;
    guint visible_folders_count = 0;

    DEBUG ("Filter: Text changed to: '%s'", text ? text : "(null)");

    g_clear_pointer (&window->details->filter_text, g_free);
    if (text && text[0] != '\0') {
        window->details->filter_text = g_utf8_strdown (text, -1);
    } else {
        window->details->filter_text = NULL; // Cleared
    }

    slot = nemo_window_get_active_slot (window);
    if (!slot) {
        DEBUG("Filter: No active slot!");
        return;
    }
    active_view = nemo_window_slot_get_current_view (slot);
    if (!active_view) {
        DEBUG("Filter: No current view in active slot!");
        return;
    }

    NemoDirectory *directory = nemo_view_get_model(active_view);
    if (!directory) {
        DEBUG("Filter: Could not get directory model for the active view.");
        return;
    }
    GList *all_files_in_dir = nemo_directory_get_file_list(directory);


    if (NEMO_IS_LIST_VIEW (active_view)) {
        DEBUG("Filter: Handling List View.");
        NemoListView *list_view = NEMO_LIST_VIEW(active_view);
        GtkTreeView *tree_view = nemo_list_view_get_tree_view(list_view);
        NemoListModel *list_model = NEMO_LIST_MODEL(gtk_tree_view_get_model(tree_view));

        if (!NEMO_IS_LIST_MODEL(list_model)) {
            DEBUG("Filter: List View model is not NemoListModel.");
            if (all_files_in_dir) nemo_file_list_free(all_files_in_dir);
            return;
        }

        // For list view, we still need to repopulate the model based on the filter
        // as GtkTreeView doesn't have a per-row visibility toggle in the same way
        // an icon canvas does.
        nemo_list_model_clear(list_model);

        if (all_files_in_dir) {
            GList *iter_list;
            for (iter_list = all_files_in_dir; iter_list != NULL; iter_list = iter_list->next) {
                NemoFile *file = NEMO_FILE(iter_list->data);
                if (should_file_be_visible_in_filter(window, file)) {
                    // Assuming 'FALSE' for the third param of add_file means don't treat as subdirectory
                    nemo_list_model_add_file(list_model, file, FALSE);
                    if (nemo_file_is_directory(file)) {
                        visible_folders_count++;
                    } else {
                        visible_files_count++;
                    }
                }
            }
        }
        visible_files_count = nemo_list_model_get_length(list_model); // More accurate for list view after filtering
    } else if (NEMO_IS_ICON_VIEW (active_view)) {
        DEBUG("Filter: Handling Icon View with optimized filter.");
        NemoIconView *icon_view_instance = NEMO_ICON_VIEW(active_view);
        NemoIconContainer *icon_container = nemo_icon_view_get_icon_container(icon_view_instance);

        if (NEMO_IS_ICON_CONTAINER(icon_container)) {
            // Use the new optimized function
            apply_filter_to_icon_container_optimized(icon_container, window, all_files_in_dir,
                                                       &visible_folders_count, &visible_files_count);
            // The visible_item_count is returned by apply_filter_to_icon_container_optimized
            // but we are using separate folder/file counts here.
            visible_files_count = visible_folders_count + visible_files_count;

        } else {
            DEBUG("Filter: IconContainer is NULL for IconView.");
        }
    } else {
        DEBUG("Filter: Active view type %s is not handled.", G_OBJECT_TYPE_NAME(active_view));
    }

    if (all_files_in_dir) {
        nemo_file_list_free(all_files_in_dir);
    }

    // Update results label
    if (window->details->filter_results_label) {
        if (window->details->filter_text && window->details->filter_text[0] != '\0') {
            char *results_text = g_strdup_printf(_("%u folders, %u files match"),
                                               visible_folders_count, visible_files_count - visible_folders_count);
            gtk_label_set_text(GTK_LABEL(window->details->filter_results_label), results_text);
            g_free(results_text);
            gtk_widget_show(window->details->filter_results_label);
        } else {
            gtk_label_set_text(GTK_LABEL(window->details->filter_results_label), "");
            gtk_widget_hide(window->details->filter_results_label);
        }
    }
    ensure_selection_visible_and_focused(active_view);
}

/* ... (other existing functions in nemo-window.c) ... */
```

--- END OF MODIFIED FILE src/nemo-window.c ---
