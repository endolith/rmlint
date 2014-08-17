/**
*  This file is part of rmlint.
*
*  rmlint is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  rmlint is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with rmlint.  If not, see <http://www.gnu.org/licenses/>.
*
** Authors:
 *
 *  - Christopher <sahib> Pahl 2010-2014 (https://github.com/sahib)
 *  - Daniel <SeeSpotRun> T.   2014-2014 (https://github.com/SeeSpotRun)
 *
** Hosted on http://github.com/sahib/rmlint
*
**/

#ifndef RM_LIST_H
#define RM_LIST_H

#define __need_timespec
#include <sys/types.h>
#include <sys/stat.h>
#include <glib.h>

#include "defs.h"


typedef int (* RmFileListSortFunc)(RmFile *a, RmFile *b, gpointer);



/**
 * @brief Set the checksum.
 *
 * @param list the list the file belongs to.
 * @param file the file to update
 * @param digest The rm digest to update.
 */
void rm_file_set_checksum(RmFileList *list, RmFile *file, RmDigest *digest);

/* TODO: To-be-removed API */
void rm_file_set_fingerprint(RmFileList *list, RmFile *file, guint index, RmDigest *digest);
void rm_file_set_middle_bytes(RmFileList *list, RmFile *file, const char *bytes, gsize len);


/**
 * @brief Get a subgroup (or isle) of the list.
 *
 * If rm_file_list_group() was not called yet, there is only one group at 0.
 *
 * @param child
 *
 * @return a GQueue, containting all children in the group.
 */
GSequenceIter *rm_file_list_get_iter(RmFileList *list);


/**
 * @brief Clear a subgroup.
 *
 * @param child  The index of the group to remove.
 */
void rm_file_list_clear(RmFileList *list, GSequenceIter *iter);



/**
 * @brief Sort a single group after a user-defined criteria.
 */
void rm_file_list_sort_group(RmFileList *list, GSequenceIter *group, GCompareDataFunc func, gpointer user_data);

/**
 * @brief Remove a single file, possibly adjusting groups.
 *
 * @param file The file to remove, will be freed.
 */
void rm_file_list_remove(RmFileList *list, RmFile *file);

/**
 * @brief Sort and clean all groups.
 *
 * The following actions are done:
 * - Subgroups with only one element are deleted.
 * - If the files in the group lie in a original path and
 *   --must-match-original is set, the group is removed.
 * - If the files in the group lie in a non-original path and
 *   --keep-all-originals is set, the group is removed.
 *
 * All surviving groups are sorted by inode and device id.
 *
 * @returns: The number of filtered files.
 */
gsize rm_file_list_sort_groups(RmFileList *list, RmSession *session);

/**
 * @brief: Return the number of groups in the list.
 */
gsize rm_file_list_len(RmFileList *list);

/**
 * @brief: Return the number of bytes the files in this group have together.
 */
gulong rm_file_list_byte_size(RmFileList *list, GQueue *group);

/**
 * @brief Print the list on stdout for debugging purpose.
 */
void rm_file_list_print(RmFileList *list);

/**
 * @brief Iterate over all files in all groups easily.
 *
 * @param list The list to iterate.
 * @param previous The previously returned file or NULL on start.
 *
 *
 * Iteration example:
 *
 * RmFile *iter = NULL;
 * while((iter = rm_file_list_iter_all(list, iter))) {
 *     ...
 * }
 *
 * @return The next file or NULL if none left.
 */
RmFile *rm_file_list_iter_all(RmFileList *list, RmFile *previous);


GHashTable *rm_file_list_create_devlist_table(RmFileList *list);
void rm_file_list_resort_device_offsets(GQueue *dev_list, bool forward, bool force_update);

#endif /* RM_LIST_H */