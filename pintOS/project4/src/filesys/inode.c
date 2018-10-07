#include "filesys/inode.h"
#include <list.h>
#include <debug.h>
#include <round.h>
#include <string.h>
#include "filesys/filesys.h"
#include "filesys/free-map.h"
#include "threads/malloc.h"

/* Identifies an inode. */
#define INODE_MAGIC 0x494e4f44

/* On-disk inode.
   Must be exactly BLOCK_SECTOR_SIZE bytes long. */
struct inode_disk
  {
    block_sector_t index_num;               /* First data sector. */
    off_t length;                       /* File size in bytes. */
    unsigned magic;                     /* Magic number. */
    uint32_t index[62][2];               /* Not used.   0 = index of sector, 1 length of that index */ 
    uint32_t unused;
  };

/* Returns the number of sectors to allocate for an inode SIZE
   bytes long. */
static inline size_t
bytes_to_sectors (off_t size)
{
  return DIV_ROUND_UP (size, BLOCK_SECTOR_SIZE);
}

/* In-memory inode. */
struct inode 
  {
    struct list_elem elem;              /* Element in inode list. */
    block_sector_t sector;              /* Sector number of disk location. */
    int open_cnt;                       /* Number of openers. */
    bool removed;                       /* True if deleted, false otherwise. */
    int deny_write_cnt;                 /* 0: writes ok, >0: deny writes. */
    struct inode_disk data;             /* Inode content. */
  };

/* Returns the block device sector that contains byte offset POS
   within INODE.
   Returns -1 if INODE does not contain data for a byte at offset
   POS. */
static block_sector_t
byte_to_sector (const struct inode *inode, off_t pos) 
{
  ASSERT (inode != NULL);
  if (pos < inode->data.length) {
    int i = 0;
    int blocks = 0;
    int length = 0;
    for(i; i < inode->data.index_num; i++) {
       length += inode->data.index[i][1];
       if(pos <= length)
           return inode->data.index[i][0] + (pos - BLOCK_SECTOR_SIZE * blocks)/BLOCK_SECTOR_SIZE;
       blocks += bytes_to_sectors((off_t)(inode->data.index[i][1]));
    }
//    return inode->data.start + pos / BLOCK_SECTOR_SIZE;
  }
  else
    return -1;
}

/* List of open inodes, so that opening a single inode twice
   returns the same `struct inode'. */
static struct list open_inodes;

/* Initializes the inode module. */
void
inode_init (void) 
{
  list_init (&open_inodes);
//  printf("\n\nsize estimation, size of data_elem : %d, list size : %d, and uint32_t size : %d\n\n", sizeof(struct data_list), sizeof(struct list), sizeof(uint32_t));

}

/* Initializes an inode with LENGTH bytes of data and
   writes the new inode to sector SECTOR on the file system
   device.
   Returns true if successful.
   Returns false if memory or disk allocation fails. */
bool
inode_create (block_sector_t sector, off_t length)
{
  struct inode_disk *disk_inode = NULL;
  bool success = false;
//  printf("create is called\n");
//  printf("requested length : %d\n", length);
  ASSERT (length >= 0);

  /* If this assertion fails, the inode structure is not exactly
     one sector in size, and you should fix that. */
  ASSERT (sizeof *disk_inode == BLOCK_SECTOR_SIZE);

  disk_inode = calloc (1, sizeof *disk_inode);

  if (disk_inode != NULL)
    {
      size_t sectors = bytes_to_sectors (length); //get number of sectors we need
      disk_inode->length = length; //save the length value
      disk_inode->magic = INODE_MAGIC;
      static char zeros[BLOCK_SECTOR_SIZE]; //initial values for sector
        int remains = sectors;
        int possibles = sectors;
        int index_num = 0;
        int remaining_length = length;
        while(remains != 0) {
          while(!free_map_allocate (possibles, &disk_inode->index[index_num][0]))
              possibles--;
          disk_inode->index[index_num][1] = (remaining_length < possibles * BLOCK_SECTOR_SIZE ? remaining_length : possibles * BLOCK_SECTOR_SIZE);
          remaining_length -= disk_inode->index[index_num][1];
          remains -= possibles;
          possibles = remains;
          index_num++;
        }
        disk_inode->index_num = index_num;
        ASSERT(remaining_length == 0 && possibles == 0);
        block_write (fs_device, sector, disk_inode);
        if (sectors > 0) {
           static char zeros[BLOCK_SECTOR_SIZE];
           size_t i;
           for (i = 0; i < disk_inode->index_num; i++) {
              size_t j;
              for(j = 0; j < bytes_to_sectors(disk_inode->index[i][1]); j++)
                 block_write(fs_device, disk_inode->index[i][0] + j, zeros);
             
           }
        }
        success = true;
    }
/*

  disk_inode->index_num = 0;
  disk_inode->length = 0;
  disk_inode->magic = INODE_MAGIC;
  block_write(fs_device, sector, disk_inode);
  success = true;
*/
   return success;
}

/* Reads an inode from SECTOR
   and returns a `struct inode' that contains it.
   Returns a null pointer if memory allocation fails. */
struct inode *
inode_open (block_sector_t sector)
{
  struct list_elem *e;
  struct inode *inode;

  /* Check whether this inode is already open. */
  for (e = list_begin (&open_inodes); e != list_end (&open_inodes);
       e = list_next (e)) 
    {
      inode = list_entry (e, struct inode, elem);
      if (inode->sector == sector) 
        {
          inode_reopen (inode);
          return inode; 
        }
    }

  /* Allocate memory. */
  inode = malloc (sizeof *inode);
  if (inode == NULL)
    return NULL;

  /* Initialize. */
  list_push_front (&open_inodes, &inode->elem);
  inode->sector = sector;
  inode->open_cnt = 1;
  inode->deny_write_cnt = 0;
  inode->removed = false;
  block_read (fs_device, inode->sector, &inode->data);
  return inode;
}

/* Reopens and returns INODE. */
struct inode *
inode_reopen (struct inode *inode)
{
  if (inode != NULL)
    inode->open_cnt++;
  return inode;
}

/* Returns INODE's inode number. */
block_sector_t
inode_get_inumber (const struct inode *inode)
{
  return inode->sector;
}

/* Closes INODE and writes it to disk.
   If this was the last reference to INODE, frees its memory.
   If INODE was also a removed inode, frees its blocks. */
void
inode_close (struct inode *inode) 
{
  /* Ignore null pointer. */
  if (inode == NULL)
    return;

  /* Release resources if this was the last opener. */
  if (--inode->open_cnt == 0)
    {
      /* Remove from inode list and release lock. */
      list_remove (&inode->elem);

//      printf("removing from list\n");
 
      /* Deallocate blocks if removed. */
      if (inode->removed) 
        {
          free_map_release (inode->sector, 1);
//          printf("inode->removed is true\n");
//          free_map_release (inode->data.start,
//                            bytes_to_sectors (inode->data.length)); 
          int i = 0;
          for(i; i < inode->data.index_num; i++) 
            free_map_release (inode->data.index[i][0], bytes_to_sectors (inode->data.index[i][1]));
        }

      free (inode); 
    }
}

/* Marks INODE to be deleted when it is closed by the last caller who
   has it open. */
void
inode_remove (struct inode *inode) 
{
  ASSERT (inode != NULL);
  inode->removed = true;
}

/* Reads SIZE bytes from INODE into BUFFER, starting at position OFFSET.
   Returns the number of bytes actually read, which may be less
   than SIZE if an error occurs or end of file is reached. */
off_t
inode_read_at (struct inode *inode, void *buffer_, off_t size, off_t offset) 
{
  uint8_t *buffer = buffer_;
  off_t bytes_read = 0;
  uint8_t *bounce = NULL;
  //off_t read_size;

//    printf("reading\n");
//  printf("reading inode %d, offset : %d\n",inode->data.index_num, offset);
//  printf("required length : %d, inode length : %d\n", size, inode->data.length);

    int total = 0;
    int j = 0;
    for(j; j < inode->data.index_num; j++) {
        total += inode->data.index[j][1];
    }
    ASSERT(total == inode->data.length);

    while (size > 0)
    {
       int i = 0;
       int length = 0;
       for(i; i < inode->data.index_num; i++) {
          if(length + inode->data.index[i][1] > offset) break;
          length += inode->data.index[i][1];
       }
       int inside_off = (offset - length)/BLOCK_SECTOR_SIZE;  //inside offset of index[i][j], which sector?

      // Disk sector to read, starting byte offset within sector. 
//      block_sector_t sector_idx = byte_to_sector (inode, offset);
      block_sector_t sector_idx = inode->data.index[i][0] + inside_off; //which sector of inode->data.index[i][0] ?
      int sector_ofs = offset % BLOCK_SECTOR_SIZE; //where to start at the block

      // Bytes left in inode, bytes left in sector, lesser of the two. 
      off_t inode_left = inode_length (inode) - offset;
      int sector_left = BLOCK_SECTOR_SIZE - sector_ofs;
      int min_left = inode_left < sector_left ? inode_left : sector_left;

      // Number of bytes to actually copy out of this sector. 
      int chunk_size = size < min_left ? size : min_left;
      if (chunk_size <= 0)
        break;

      if (sector_ofs == 0 && chunk_size == BLOCK_SECTOR_SIZE)
        {
          // Read full sector directly into caller's buffer. 
          block_read (fs_device, sector_idx, buffer + bytes_read);
        }
      else
        {
          // Read sector into bounce buffer, then partially copy
 //              into caller's buffer. 
          if (bounce == NULL)
            {
              bounce = malloc (BLOCK_SECTOR_SIZE);
              if (bounce == NULL)
                break;
            }
          block_read (fs_device, sector_idx, bounce);
          memcpy (buffer + bytes_read, bounce + sector_ofs, chunk_size);
        }

//       Advance. 
      size -= chunk_size;
      offset += chunk_size;
      bytes_read += chunk_size;
    }

  free (bounce);

  return bytes_read;
}

/* Writes SIZE bytes from BUFFER into INODE, starting at OFFSET.
   Returns the number of bytes actually written, which may be
   less than SIZE if end of file is reached or an error occurs.
   (Normally a write at end of file would extend the inode, but
   growth is not yet implemented.) */
off_t
inode_write_at (struct inode *inode, const void *buffer_, off_t size,
                off_t offset) 
{
// printf("inode write at called, inode's sector number : %d, required size : %d, requested offset : %d\n", inode->sector, size, offset);
  const uint8_t *buffer = buffer_;
  off_t bytes_written = 0;
  uint8_t *bounce = NULL;

  if (inode->deny_write_cnt)
    return 0;

//need to zero out, when offset > inode->data.length
  uint8_t *zeros = malloc(BLOCK_SECTOR_SIZE);
  memset(zeros, 0, BLOCK_SECTOR_SIZE);
/*
  if(offset > inode->data.length) {
//    printf("offset > inode->data.length\n");
    int expand_size = offset - inode->data.length;
    expand_size -= (BLOCK_SECTOR_SIZE - inode->data.index[inode->data.index_num-1][1]%BLOCK_SECTOR_SIZE);
    printf("expand size : %d\n", expand_size);
  }
*/

  if(offset + size > inode->data.length) {
   //we need to expand it
   int required = (offset + size) - inode->data.length;
   //in case of
   if(required < BLOCK_SECTOR_SIZE && inode->data.length%BLOCK_SECTOR_SIZE != 0 &&
      (BLOCK_SECTOR_SIZE - inode->data.length%BLOCK_SECTOR_SIZE) > required ) { //it will fit in current sector, we don't need to expand
       //if inode->data.length%BLOCK_SECTOR_SIZE == 0 it will be BLOCK_SECTOR_SIZE, but it have to be extended
//         printf("\nlength extended without new sector, required length : %d\n", required);
         if(inode->data.index_num == 0 && inode->data.length == 0) { //possibility of having no sector at all
             int remains = bytes_to_sectors(required); //printf("remains : %d\n", remains);
             int possibles = remains;
             int index_num = 0;
             int remaining_length = required;
//             printf("remains : %d, possibles : %d, index_num : %d, remaining_length : %d\n", remains, possibles, index_num, remaining_length);
             while(remains != 0) {
                 while(!free_map_allocate (possibles, &inode->data.index[index_num][0]))
                      possibles--;
//                 printf("possibles : %d, index's length : %d\n", possibles, inode->data.index[index_num][0]);
                 int i = 0;
                 for(i; i < possibles; i++)
                    block_write(fs_device, inode->data.index[index_num][0] + i, zeros);
                 inode->data.index[index_num][1] = (remaining_length < possibles * BLOCK_SECTOR_SIZE ? remaining_length : possibles * BLOCK_SECTOR_SIZE);
                 inode->data.length += inode->data.index[index_num][1];
//                 printf("length set to %d\n", inode->data.index[index_num][1]);
                 remaining_length -= inode->data.index[index_num][1];
                 remains -= possibles;
                 possibles = remains;
                 index_num++;
             }
//             printf("remains : %d, possibles : %d, index_num : %d, remaining_length : %d\n",remains, possibles, index_num, remaining_length);
             inode->data.index_num = index_num;
             ASSERT(remaining_length == 0 && possibles == 0);
//             inode->data.length = required;
         }
         else { //possibility that it has at least one block {
//             printf("expand by %d\n", required);
             ASSERT(inode->data.index_num != 0);
             inode->data.index[inode->data.index_num -1][1] += required;
             inode->data.length += required;
         }
//         block_write(fs_device, inode->sector, &inode->data);
   }
   else {
//         printf("\nneed to expand file by adding sector\n") ; 
     int extend_size;
//     printf("a\n");
     if(inode->data.length%BLOCK_SECTOR_SIZE != 0)
       extend_size = required - (BLOCK_SECTOR_SIZE - inode->data.length%BLOCK_SECTOR_SIZE);
     else
       extend_size = required;
//     printf("offset : %d, size : %d, required : %d, extend_size : %d\n", offset, size, required, extend_size);
//     inode->data.length += (offset + size) - inode->data.length;//(BLOCK_SECTOR_SIZE - inode->data.length%BLOCK_SECTOR_SIZE);
     if(inode->data.length%BLOCK_SECTOR_SIZE != 0) {
        inode->data.index[inode->data.index_num-1][1] += 
                                      (BLOCK_SECTOR_SIZE - inode->data.length%BLOCK_SECTOR_SIZE);
        inode->data.length += (BLOCK_SECTOR_SIZE - inode->data.length%BLOCK_SECTOR_SIZE);
     }
//     printf("b\n");
//     printf("extend size : %d\n", extend_size);
     int sectors = bytes_to_sectors(extend_size);
     int possibles = sectors;
     int remaining_length = extend_size;
     int index_num = inode->data.index_num;
//     printf("sectors : %d, possibles : %d, remaining_length : %d, index_num : %d\n", sectors, possibles, remaining_length, index_num);
     while(sectors != 0) {
          while(!free_map_allocate(possibles, &inode->data.index[index_num][0]))
             possibles--;
//           printf("possibles : %d\n", possibles);
          int i = 0;
          for(i; i < possibles; i++)
             block_write(fs_device, inode->data.index[index_num][0] + i, zeros);
          inode->data.index[index_num][1] = (remaining_length < possibles * BLOCK_SECTOR_SIZE ?
                                             remaining_length : possibles * BLOCK_SECTOR_SIZE);
          inode->data.length += inode->data.index[index_num][1];
//          printf("data.index's length : %d\n", inode->data.index[index_num][1]);
          remaining_length -= inode->data.index[index_num][1];
          sectors -= possibles;
          possibles = sectors;
          index_num++;
//          printf("index_num : %d\n", index_num);
     }
//     printf("c");
     inode->data.index_num = index_num;
     ASSERT(remaining_length == 0 && possibles == 0);
//     block_write(fs_device, inode->sector, &inode->data);
   }
   block_write(fs_device, inode->sector, &inode->data);
  }
//  block_write(fs_device, inode->sector, &inode->data);
//     printf("d\n");
//  if(inode->data.index_num > 61) printf("index is too large ? : %d\n", inode->data.index_num);

  while (size > 0)
    {
      int i = 0;
      int length = 0;
      for(i; i < inode->data.index_num; i++) {
           if(length + inode->data.index[i][1] > offset) break;
           length += inode->data.index[i][1];
      }
//      printf("a\n");
//      ASSERT(length%BLOCK_SECTOR_SIZE == 0);
      int inside_off = (offset - length)/BLOCK_SECTOR_SIZE;
//      printf("length : %d, inside offset : %d\n", length, inside_off);
      /* Sector to write, starting byte offset within sector. */
      block_sector_t sector_idx = inode->data.index[i][0] + inside_off;//byte_to_sector (inode, offset);
      int sector_ofs = offset % BLOCK_SECTOR_SIZE;
//      printf("sector_idx : %d, sector_ofs : %d\n", sector_idx, sector_ofs);
      /* Bytes left in inode, bytes left in sector, lesser of the two. */
      off_t inode_left = inode_length (inode) - offset;
      int sector_left = BLOCK_SECTOR_SIZE - sector_ofs;
      int min_left = inode_left < sector_left ? inode_left : sector_left;
//      printf("inode_left : %d, sector_left : %d, min_left : %d\n", inode_left, sector_left, min_left);
      /* Number of bytes to actually write into this sector. */
//      printf("b\n");
      int chunk_size = size < min_left ? size : min_left;
      if (chunk_size <= 0)
        break;

      if (sector_ofs == 0 && chunk_size == BLOCK_SECTOR_SIZE)
        {
          /* Write full sector directly to disk. */
          block_write (fs_device, sector_idx, buffer + bytes_written);
        }
      else
        {
          /* We need a bounce buffer. */
          if (bounce == NULL)
            {
              bounce = malloc (BLOCK_SECTOR_SIZE);
              if (bounce == NULL)
                break;
            }

          /* If the sector contains data before or after the chunk
 *              we're writing, then we need to read in the sector
 *                           first.  Otherwise we start with a sector of all zeros. */
          if (sector_ofs > 0 || chunk_size < sector_left)
            block_read (fs_device, sector_idx, bounce);
          else
            memset (bounce, 0, BLOCK_SECTOR_SIZE);
          memcpy (bounce + sector_ofs, buffer + bytes_written, chunk_size);
          block_write (fs_device, sector_idx, bounce);
        }
//      printf("c\n");
      /* Advance. */
      size -= chunk_size;
      offset += chunk_size;
      bytes_written += chunk_size;
    }

  free (bounce);
  free (zeros);

  return bytes_written;
}

/* Disables writes to INODE.
   May be called at most once per inode opener. */
void
inode_deny_write (struct inode *inode) 
{
  inode->deny_write_cnt++;
  ASSERT (inode->deny_write_cnt <= inode->open_cnt);
}

/* Re-enables writes to INODE.
   Must be called once by each inode opener who has called
   inode_deny_write() on the inode, before closing the inode. */
void
inode_allow_write (struct inode *inode) 
{
  ASSERT (inode->deny_write_cnt > 0);
  ASSERT (inode->deny_write_cnt <= inode->open_cnt);
  inode->deny_write_cnt--;
}

/* Returns the length, in bytes, of INODE's data. */
off_t
inode_length (const struct inode *inode)
{
  return inode->data.length;
}
